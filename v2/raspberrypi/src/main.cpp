#include <atomic>
#include <csignal>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

int main(void) {
    signal(SIGINT, sigintHandler);

    MatrixScreen screen(1,1);
    MatrixDriver matrixDriver(spidev, screen, 4);
    while(!halt) {
        screen.resetScreen(MatrixScreen::PixelColor::off);
        screen(0,0) = MatrixScreen::PixelColor::on;
        screen(1,0) = MatrixScreen::PixelColor::on;
        screen(2,0) = MatrixScreen::PixelColor::on;
        screen(3,0) = MatrixScreen::PixelColor::on;
        screen(4,0) = MatrixScreen::PixelColor::on;
        screen(5,0) = MatrixScreen::PixelColor::on;
        screen(6,0) = MatrixScreen::PixelColor::on;
        screen(7,0) = MatrixScreen::PixelColor::on;
        matrixDriver.setScreen(screen);
    }
    return EXIT_SUCCESS;
}
