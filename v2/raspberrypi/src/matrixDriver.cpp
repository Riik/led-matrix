#include <system_error>
#include <algorithm>
#include <chrono>
#include <functional>
#include <sstream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>

#include "matrixDriver.hpp"


MatrixDriver::MatrixDriver(const std::string &spiDevName, const MatrixScreen &exampleScreen,
        const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation) :
    screen{exampleScreen}, matrixCountWidth{exampleScreen.getMatrixCountWidth()}, matrixCountHeight{exampleScreen.getMatrixCountHeight()},
    physicalConnectionLocation{physicalConnectionLocation}, newDataAvailable{0}
{

    std::cout << "Matridriver started" << std::endl;

    this->outputWorker = std::jthread(std::bind_front(&MatrixDriver::screenToSpi, this));
}

MatrixDriver::~MatrixDriver()
{
    // close(this->spifd);
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
}

void MatrixDriver::screenToSpi(std::stop_token stopToken)
{
        this->screenMutex.unlock();
        this->spiFdMutex.lock();
        // ioctl(this->spifd, SPI_IOC_MESSAGE(10), &tr[0]);
        this->spiFdMutex.unlock();
    // }
}
