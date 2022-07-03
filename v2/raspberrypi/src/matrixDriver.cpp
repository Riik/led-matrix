#include <system_error>
#include <algorithm>
#include <chrono>
#include <functional>
#include <sstream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "matrixDriver.hpp"


MatrixDriver::MatrixDriver(const std::string &spiDevName, const MatrixScreen &exampleScreen, const uint32_t framesPerSecond,
        const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation) :
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

    // Disable decode mode
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x09;
        buf[i+1] = 0x00;
    }

    ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) {
        close(this->spifd);
        throw std::system_error(errno, std::generic_category(), spiDevName);
    }

    // Disable test mode
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0F;
        buf[i+1] = 0x00;
    }

    ret = ioctl(this->spifd, SPI_IOC_MESSAGE(1), &tr);
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

void MatrixDriver::setScreen(MatrixScreen screen)
{
#if defined(DEBUG)
    if (screen.getMatrixCountWidth() != this->screen.getMatrixCountWidth()) {
        std::stringstream ss;
        ss << "Matrix count width is incorrect. Expected: " << this->screen.getMatrixCountWidth() << ", got: " << screen.getMatrixCountWidth();
        throw std::out_of_range(ss.str());
    }

    if (screen.getMatrixCountHeight() != this->screen.getMatrixCountHeight()) {
        std::stringstream ss;
        ss << "Matrix count height is incorrect. Expected: " << this->screen.getMatrixCountHeight() << ", got: " << screen.getMatrixCountHeight();
        throw std::out_of_range(ss.str());
    }
#endif //defined(DEBUG)

    this->screenMutex.lock();
    this->screen = std::move(screen);
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
    tr[9].cs_change = 0;
    for (std::size_t i = 1; i < 9; ++i) {
        tr[i].tx_buf = (unsigned long) matrixContentBuf[i - 1].data();
    }

    std::stop_callback cb(stopToken, [&sem = this->newDataAvailable] {
        sem.release();
    });

    while(true) {
        this->newDataAvailable.acquire();
        if (stopToken.stop_requested()) {
            return;
        }
        this->screenMutex.lock();
        // Now we convert the screen to our buffer.
        for (std::size_t matCol = 0; matCol < this->matrixCountWidth; ++matCol) {
            for (std::size_t matRow = 0; matRow < this->matrixCountHeight; ++matRow) {
                // Isolate the pixelcoordinates that belong to this matrix
                std::size_t pixelRowMin;
                if (this->physicalConnectionLocation == MatrixDriver::PhysicalConnectionLocation::topLeft) {
                    pixelRowMin = this->screen.getPixelCountHeight() - (matRow + 1)*MatrixScreen::matrixPixelCountHeight;
                } else {
                    pixelRowMin = matRow*MatrixScreen::matrixPixelCountHeight;
                }
                std::size_t pixelColMin = matCol*MatrixScreen::matrixPixelCountWidth;
                // The matrix index in the total list of matrices. Note that the first byte is shoved all the way through, pointing to
                // the last matrix.
                std::size_t curIndex = matrixCount - 1 - (matRow * this->matrixCountWidth + matCol);
                // Finally, we set the relevant bytes.
                // A buffer is about a column.
                for (std::size_t i = 0; i < MatrixScreen::matrixPixelCountWidth; ++i) {
                    uint8_t &ref = matrixContentBuf[i][2*curIndex + 1];
                    ref = 0;
                    for (std::size_t j = 0; j < MatrixScreen::matrixPixelCountHeight; ++j) {
                        if (this->screen(pixelColMin + i, pixelRowMin + j) == PixelColor::on) {
                            // -1%8 to compensate for hardware error where bit 8 maps to led 0, bit 0 to led 1, etc.
                            ref |= 1 << ((j-1)%8);
                        }
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
