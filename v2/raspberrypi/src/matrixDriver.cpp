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
    curs_set(0);           // set the cursor to invisible
#if defined(DEBUG)
    std::cout << "Matridriver started" << std::endl;
    std::cout << "Width: " << std::to_string(this->screen.getPixelCountWidth()) << std::endl << "Height: " << std::to_string(this->screen.getPixelCountHeight()) << std::endl;
#endif // defined(DEBUG)
}

MatrixDriver::~MatrixDriver()
{
    endwin();
}

void MatrixDriver::setScreen(MatrixScreen screen)
{

  for (std::size_t y = 0; y < this->screen.getPixelCountHeight(); ++y) {
      move(y,0);
      for (std::size_t x = 0; x < this->screen.getPixelCountWidth(); ++x) {

        if (this->screen(x, y) == PixelColor::on) {
            addch('.' | A_REVERSE);
            addch('.' | A_REVERSE);
        } else {
            addch('.');
      			addch('.');
        }
      }
  }
  refresh();
}
