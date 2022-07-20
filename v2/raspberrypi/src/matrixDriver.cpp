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
        // terminal characters have 5:2 aspect ratio.
        // Printing two next eachother results in pixels with 5:4 aspect ratio
        if (this->screen(x, y) == PixelColor::on) {
            addch('.' | A_REVERSE); // A_REVERSE flips fg / bg color.
            addch('.' | A_REVERSE); // character can be a space or dot or whatever, dot is clearer for debugging
        } else {
            addch('.');
      			addch('.');
        }
      }
  }
  refresh();
}
