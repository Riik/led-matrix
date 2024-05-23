#include <vector>

#include "matrixDriver.hpp"
#include "gpio.h"
#include "spi.h"

static void doTransaction(const std::vector<uint8_t>& buffer) {
    gpio_setPin(GPIO_PIN_SPI_SS, false);
    spi_transaction(buffer.data(), buffer.size(), nullptr, 0);
    gpio_setPin(GPIO_PIN_SPI_SS, true);
}

void MatrixDriver::turnScreenOff() const {
    // Shutdown all matrices
    std::vector<uint8_t> buf(this->matrixCount * 2, 0);
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0C;
        buf[i+1] = 0x00;
    }
    doTransaction(buf);
}

void MatrixDriver::turnScreenOn() const {
    // Shutdown all matrices
    std::vector<uint8_t> buf(this->matrixCount * 2, 0);
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0C;
        buf[i+1] = 0x01;
    }
    doTransaction(buf);
}

void MatrixDriver::transmitSetOfCols(const std::vector<uint8_t>& buffer, size_t colIndex) const {
    std::vector<uint8_t> transmitData(this->matrixCount*2, 0);
    for (size_t i = 0; i < transmitData.size(); i += 2) {
        transmitData[i] = colIndex + 1;
        transmitData[i+1] = buffer[i/2];
    }
    doTransaction(transmitData);
}

MatrixDriver::MatrixDriver(size_t matrixCount) {
    // Configure slave select/chip select pin to be out
    gpio_setPinInoutType(GPIO_PIN_SPI_SS, GPIO_INOUT_TYPE_OUT);
    // Pull the pin high to deselect the matrices
    gpio_setPin(GPIO_PIN_SPI_SS, true);
    // Initialize the spi device to 10 MHz
    spi_init(10000000, false, false);

    this->matrixCount = matrixCount;
    this->turnScreenOff();

    std::vector<uint8_t> buf(matrixCount * 2, 0);

    // Disable decode mode
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x09;
        buf[i+1] = 0x00;
    }
    doTransaction(buf);

    // Disable test mode
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0F;
        buf[i+1] = 0x00;
    }
    doTransaction(buf);

    // Transmit "scan all lines" to every matrix
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0B;
        buf[i+1] = 0x07;
    }
    doTransaction(buf);
}

void MatrixDriver::setScreen(MatrixScreen screen) const {
    // We expect to be connected on bottom-right
    this->turnScreenOff();
    std::vector<uint8_t> transmissionData(this->matrixCount, 0);

    for (int colIndex = 0; colIndex < 8; ++colIndex) {
        size_t matrixIndex = 0;
        for (int matrixYCoord = screen.getMatrixCountHeight() - 1; matrixYCoord >= 0; --matrixYCoord) {
            for (size_t matrixXCoord = 0; matrixXCoord < screen.getMatrixCountWidth(); ++matrixXCoord) {
                // 7 - colIndex, since highest index is closest to male connector
                size_t pixelXCoord = matrixXCoord*8 + (7 - colIndex);
                size_t pixelYCoord = matrixYCoord*8;
                uint8_t &ref = transmissionData[matrixIndex];
                ref = 0;
                for (size_t pixelCount = 0; pixelCount < 8; ++pixelCount) {
                    if (screen(pixelXCoord, pixelYCoord + pixelCount) !=  PixelColor::on) continue;
                    // -1%8 to compensate for hardware error where bit 8 maps to led 0, bit 0 to led 1, etc.
                    ref |= 1 << ((7 - pixelCount - 1)%8);
                }
                matrixIndex++;
            }
        }
        transmitSetOfCols(transmissionData, colIndex);
    }
    this->turnScreenOn();
}

void MatrixDriver::setBrightness(uint_fast8_t brightness) const {
    if (brightness > 15) {
        brightness = 15;
    }
    std::vector<uint8_t> buf(this->matrixCount * 2, 0);
    for (std::size_t i = 0; i < buf.size(); i += 2) {
        buf[i] = 0x0A;
        buf[i+1] = brightness;
    }
    doTransaction(buf);
}
