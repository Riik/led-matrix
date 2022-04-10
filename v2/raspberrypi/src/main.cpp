#include <atomic>
#include <csignal>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"
#include "gameOfLife.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

int main(void) {
    signal(SIGINT, sigintHandler);

    MatrixScreen screen(2,1);
    MatrixDriver matrixDriver(spidev, screen, 1);
    screen.resetScreen(MatrixScreen::PixelColor::off);
    screen(1,1) = MatrixScreen::PixelColor::on;
    screen(2,1) = MatrixScreen::PixelColor::on;
    screen(3,1) = MatrixScreen::PixelColor::on;
    screen(4,1) = MatrixScreen::PixelColor::on;
    screen(5,1) = MatrixScreen::PixelColor::on;

    GameOfLife life(screen);
    matrixDriver.setScreen(screen);

    while(!halt) {
        matrixDriver.setScreen(life.nextframe());
    }
    return EXIT_SUCCESS;
}
