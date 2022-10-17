#pragma once

#include <thread>
#include <semaphore>
#include <mutex>
#include <string>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"
#include "frameLimiter.hpp"

class MatrixDriverSpi : public MatrixDriver {
    public:

        /* You have to connect the matrices row-based, but you can either go up or down. */
        enum class PhysicalConnectionLocation {
            topLeft,
            bottomLeft
        };

        MatrixDriverSpi(const std::string &spiDevName, MatrixScreen exampleScreen, uint_fast8_t brightness = 15,
                MatrixDriverSpi::PhysicalConnectionLocation physicalConnectionLocation = MatrixDriverSpi::PhysicalConnectionLocation::topLeft);

        ~MatrixDriverSpi();
        // Because this object holds a filedescriptor, which cannot trivially be copied, we need to think about the
        // rule of three. dup and dup2 exist, but for now this is the easiest solution.
        MatrixDriverSpi& operator=(const MatrixDriverSpi&) = delete;
        MatrixDriverSpi(const MatrixDriverSpi&) = delete;

        void setScreen(MatrixScreen screen) final;

    private:
        MatrixScreen screen;
        int spifd;

        const std::size_t matrixCountWidth;
        const std::size_t matrixCountHeight;
        const MatrixDriverSpi::PhysicalConnectionLocation physicalConnectionLocation;

        std::jthread outputWorker;
        std::binary_semaphore newDataAvailable;
        std::mutex spiFdMutex;
        std::mutex screenMutex;

        void screenToSpi(std::stop_token stopToken);

};
