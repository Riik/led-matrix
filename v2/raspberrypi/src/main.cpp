#include <atomic>
#include <csignal>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <memory>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"
#include "matrixDriverSpi.hpp"
#include "matrixDriverNcurses.hpp"
#include "frameLimiter.hpp"
#include "argumentParser.hpp"
#include "textScroller.hpp"
#include "diceGenerator.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

int main(int argc, char * const argv[]) {
    signal(SIGINT, sigintHandler);

    ParsedArguments pArgs;
    try {
        pArgs = parseArguments(argc, argv);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    MatrixScreen referenceScreen(pArgs.ledMatrixWidth, pArgs.ledMatrixHeight);
    FrameLimiter frameLimiter(pArgs.maxFramesPerSecond);
    std::unique_ptr<MatrixDriver> matrixDriver;
#if !defined(__APPLE__)
    if (pArgs.matrixDriver == SelectedMatrixDriver::spi) {
        matrixDriver = std::make_unique<MatrixDriverSpi>(spidev, referenceScreen, pArgs.brightness);
    } else {
#endif //!defined(__APPLE__)
        matrixDriver = std::make_unique<MatrixDriverNcurses>();
#if !defined(__APPLE__)
    }
#endif //!defined(__APPLE__)
    std::unique_ptr<SubProgram> subProgram;
    if (pArgs.subProgram == SelectedSubProgram::diceGenerator) {
        subProgram = std::make_unique<DiceGenerator>(referenceScreen, pArgs);
    } else {
        subProgram = std::make_unique<TextScroller>(referenceScreen, pArgs);
    }
    while(!halt) {
        matrixDriver->setScreen(subProgram->getScreen());
        frameLimiter.waitForNextFrame();
    }
    return EXIT_SUCCESS;
}
