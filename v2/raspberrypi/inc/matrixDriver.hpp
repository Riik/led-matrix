#ifndef MATRIXDRIVER_HPP
#define MATRIXDRIVER_HPP
#include <thread>
#include <semaphore>
#include <mutex>
#include <string>

#include "matrixScreen.hpp"
#include "frameLimiter.hpp"

class MatrixDriver {
    public:

        /* You have to connect the matrices row-based, but you can either go up or down. */
        enum class PhysicalConnectionLocation {
            topLeft,
            bottomLeft
        };

        MatrixDriver(const std::string &spiDevName, const MatrixScreen &exampleScreen,
                const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation = MatrixDriver::PhysicalConnectionLocation::topLeft,
                const uint32_t framesPerSecond = 0);

        // Forbid copy because of the spidev object and thread safety.
        MatrixDriver(const MatrixDriver&) = delete;

        ~MatrixDriver();

        void setScreen(const MatrixScreen &screen);

    private:
        MatrixScreen screen;
        int spifd;

        const std::size_t matrixCountWidth;
        const std::size_t matrixCountHeight;
        const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation;

        std::jthread outputWorker;
        std::binary_semaphore newDataAvailable;
        std::mutex spiFdMutex;
        std::mutex screenMutex;

        FrameLimiter frameLimiter;

        void screenToSpi(std::stop_token stopToken);

};
#endif //MATRIXDRIVER_HPP
