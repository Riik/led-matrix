#include <system_error>
#include <algorithm>
#include <chrono>
#include <functional>
#include <sstream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>

#include "matrixDriver.hpp"
#include <ncurses.h>


MatrixDriver::MatrixDriver(const std::string &spiDevName, const MatrixScreen &exampleScreen,
        const MatrixDriver::PhysicalConnectionLocation physicalConnectionLocation) :
    screen{exampleScreen}, matrixCountWidth{exampleScreen.getMatrixCountWidth()}, matrixCountHeight{exampleScreen.getMatrixCountHeight()},
    physicalConnectionLocation{physicalConnectionLocation}, newDataAvailable{0}
{
    initscr();             // initialize curses
    cbreak();              // pass key presses to program, but not signals
    noecho();              // don't echo key presses to screen
    keypad(stdscr, TRUE);  // allow arrow keys
    // timeout(0);            // no blocking on getch()
    curs_set(0);           // set the cursor to invisible
    // std::cout << "Matridriver started" << std::endl;
    this->outputWorker = std::jthread(std::bind_front(&MatrixDriver::screenToSpi, this));
}

MatrixDriver::~MatrixDriver()
{
    // close(this->spifd);
    endwin();			/* End curses mode		  */
}

void MatrixDriver::setScreen(MatrixScreen screen)
{
  const std::size_t matrixCount = this->matrixCountWidth * this->matrixCountHeight;

  std::array<std::vector<uint8_t>, 8> matrixContentBuf;
  for (std::size_t i = 0; i < 8; ++i) {
      matrixContentBuf[i] = std::vector<uint8_t>(matrixCount*2, 0);
      for (std::size_t j = 0; j < matrixContentBuf[i].size(); j += 2) {
          matrixContentBuf[i][j] = i+1;
      }
  }

  // std::cout << "setting screen" << std::endl;
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

    for (std::size_t matCol = 0; matCol < this->screen.getMatrixCountWidth(); ++matCol) {
        for (std::size_t matRow = 0; matRow < this->screen.getMatrixCountHeight(); ++matRow) {
            // Isolate the pixelcoordinates that belong to this matrix
            std::size_t pixelRowMin;
            if (this->physicalConnectionLocation == MatrixDriver::PhysicalConnectionLocation::topLeft) {
                pixelRowMin = this->screen.getPixelCountHeight() - (matRow + 1)*MatrixScreen::matrixPixelCountHeight;
            } else {
                pixelRowMin = matRow*MatrixScreen::matrixPixelCountHeight;
            }
            std::size_t pixelColMin = matCol*MatrixScreen::matrixPixelCountWidth;

            std::size_t curIndex = matrixCount - 1 - (matRow * this->matrixCountWidth + matCol);
            // Finally, we set the relevant bytes.
            // A buffer is about a column.
            for (std::size_t i = 0; i < MatrixScreen::matrixPixelCountWidth; ++i) {
                uint8_t &ref = matrixContentBuf[i][2*curIndex + 1];
                ref = 0;
                move(i,0);
                for (std::size_t j = 0; j < MatrixScreen::matrixPixelCountHeight; ++j) {
                    // std::cout << "..";
                    if (this->screen(pixelColMin + i, pixelRowMin + j) == PixelColor::on) {
                        addch('.' | A_REVERSE);
                        addch('.' | A_REVERSE);
                    } else {
                        addch('.');
                  			addch('.');
                    }
                }
            }
        }
    }
    refresh();

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
