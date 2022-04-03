#include <system_error>
#include <algorithm>
#include <chrono>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "matrixDriver.hpp"


MatrixDriver::MatrixDriver(const std::string &spiDevName, const MatrixScreen &exampleScreen,
        const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation, const uint32_t framesPerSecond) :
    screen{exampleScreen}, matrixCountWidth{exampleScreen.getMatrixCountWidth()}, matrixCountHeight{exampleScreen.getMatrixCountHeight()},
    physicalConnectionLocation{physicalConnectionLocation}, newDataAvailable{0}, frameLimiter{framesPerSecond}
{
    // All spidev stuff will be done c style, since I cant seem to figure out how to do it c++ style.
    this->spifd = open(spiDevName.c_str(), O_RDWR);
    if (this->spifd < 0) {
        throw std::system_error(errno, std::generic_category(), spiDevName);
    }

    // Now we need to bring the ledmatrices into a default state.
    const std::size_t matrixCount = this->matrixCountWidth * this->matrixCountHeight;
    std::vector<uint8_t> buf(matrixCount*2, 0);
    struct spi_ioc_transfer tr = {0};
    tr.bits_per_word = 8;
    tr.speed_hz = 10000000;
    tr.len = buf.size();
    tr.tx_buf = (unsigned long)buf.data();
    tr.cs_change = 1;

    // First we transmit a shutdown to every matrix
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0C;
        buf[i+1] = 0x00;
    }

    int ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        close(this->spifd);
        throw std::system_error(errno, std::generic_category(), spiDevName);
    }

    // Next, transmit scan all lines to every matrix
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0B;
        buf[i+1] = 0x07;
    }

    ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        close(this->spifd);
        throw std::system_error(errno, std::generic_category(), spiDevName);
    }

    // Finally, we transfer max brightness to every matrix
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0A;
        buf[i+1] = 0x0F;
    }

    ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        close(this->spifd);
        throw std::system_error(errno, std::generic_category(), spiDevName);
    }
    // Create and start the thread
    this->outputWorker = std::jthread(std::bind_front(&MatrixDriver::screenToSpi, this));
}

MatrixDriver::~MatrixDriver()
{
    close(this->spifd);
}

void MatrixDriver::setScreen(const MatrixScreen &screen)
{
    this->screenMutex.lock();
    this->screen = screen;
    this->screenMutex.unlock();
    this->newDataAvailable.release();
    this->frameLimiter.waitForNextFrame();
}

void MatrixDriver::screenToSpi(std::stop_token stopToken)
{
    const std::size_t matrixCount = this->matrixCountWidth * this->matrixCountHeight;

    std::vector<uint8_t> turnOffBuf(matrixCount*2, 0);
    std::vector<uint8_t> turnOnBuf(matrixCount*2, 0);
    // Opcode for turning the matrix off, one opcode per matrix.
    for (std::size_t i = 0; i < turnOffBuf.size(); i += 2) {
        turnOffBuf[i] = 0x0C;
        turnOffBuf[i+1] = 0x00;
    }
    // Opcode for turning the matrix on, one opcode per matrix
    for (std::size_t i = 0; i < turnOnBuf.size(); i += 2) {
        turnOnBuf[i] = 0x0C;
        turnOnBuf[i+1] = 0x01;
    }
    // Opcodes for setting the data. Every first byte is well know, it refers to a row (1 to 8).
    // Every second byte needs to be filled in from this->screen
    std::array<std::vector<uint8_t>, 8> matrixContentBuf;
    for (std::size_t i = 0; i < 8; ++i) {
        matrixContentBuf[i] = std::vector<uint8_t>(matrixCount*2, 0);
        for (std::size_t j = 0; j < matrixContentBuf[i].size(); j += 2) {
            matrixContentBuf[i][j] = i+1;
        }
    }

    // We know we need exactly 10 spi objects: off, 8 rows, on
    std::array<struct spi_ioc_transfer, 10> tr = {{0}};
    for (std::size_t i = 0; i < 10; ++i) {
        tr[i].bits_per_word = 8;
        tr[i].speed_hz = 10000000;
        tr[i].len = 2*matrixCount;
        tr[i].tx_buf = 0;
        tr[i].cs_change = 1;
    }
    tr[0].tx_buf = (unsigned long)turnOffBuf.data();
    tr[9].tx_buf = (unsigned long)turnOnBuf.data();
    for (std::size_t i = 1; i < 9; ++i) {
        tr[i].tx_buf = (unsigned long) matrixContentBuf[i - 1].data();
    }

    while(true) {
        bool success = this->newDataAvailable.try_acquire_for(std::chrono::milliseconds(1));
        if (stopToken.stop_requested()) {
            return;
        }
        if (!success) {
            continue;
        }
        this->screenMutex.lock();
        // Now we convert the screen to our buffer.
        for (std::size_t matCol = 0; matCol < this->matrixCountWidth; ++matCol) {
            for (std::size_t matRow = 0; matRow < this->matrixCountHeight; ++matRow) {
                // Isolate the pixelcoordinates that belong to this matrix
                std::size_t pixelRowMin;
                if (this->physicalConnectionLocation == MatrixDriver::PhysicalConnectionLocation::topLeft) {
                    pixelRowMin = this->screen.getPixelCountHeight() - matRow*MatrixScreen::matrixPixelCountHeight -
                        (MatrixScreen::matrixPixelCountHeight);
                } else {
                    pixelRowMin = matRow*MatrixScreen::matrixPixelCountHeight;
                }
                std::size_t pixelRowMax = pixelRowMin + (MatrixScreen::matrixPixelCountHeight - 1);
                std::size_t pixelColMin = matCol*MatrixScreen::matrixPixelCountWidth;
                // The matrix index in the total list of matrices. Note that the first byte is shoved all the way through, pointing to
                // the last matrix.
                std::size_t curIndex = matrixCount - 1 - (matRow * this->matrixCountWidth + matCol);
                // Finally, we set the relevant bytes.
                for (std::size_t i = 0; i < 8; ++i) {
                    uint8_t &ref = matrixContentBuf[i][2*curIndex + 1];
                    ref = 0;
                    if (this->screen(pixelRowMax - i, pixelColMin) == MatrixScreen::PixelColor::on) {
                        ref |= 0x80;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 1) == MatrixScreen::PixelColor::on) {
                        ref |= 0x01;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 2) == MatrixScreen::PixelColor::on) {
                        ref |= 0x02;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 3) == MatrixScreen::PixelColor::on) {
                        ref |= 0x04;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 4) == MatrixScreen::PixelColor::on) {
                        ref |= 0x08;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 5) == MatrixScreen::PixelColor::on) {
                        ref |= 0x10;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 6) == MatrixScreen::PixelColor::on) {
                        ref |= 0x20;
                    }
                    if (this->screen(pixelRowMax - i, pixelColMin + 7) == MatrixScreen::PixelColor::on) {
                        ref |= 0x40;
                    }
                }
            }
        }
        this->screenMutex.unlock();
        this->spiFdMutex.lock();
        ioctl(this->spifd, SPI_IOC_MESSAGE(10), &tr[0]);
        this->spiFdMutex.unlock();
    }
}
