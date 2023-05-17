#include <ncurses.h>

#include "matrixDriverNcurses.hpp"

MatrixDriverNcurses::MatrixDriverNcurses()
{
    initscr();             // initialize curses
    cbreak();              // pass key presses to program, but not signals
    noecho();              // don't echo key presses to screen
    keypad(stdscr, TRUE);  // allow arrow keys
    curs_set(0);           // set the cursor to invisible
}

MatrixDriverNcurses::~MatrixDriverNcurses()
{
    endwin();
}

void MatrixDriverNcurses::setScreen(MatrixScreen screen)
{
  for (std::size_t y = 0; y < screen.getPixelCountHeight(); ++y) {
      move(screen.getPixelCountHeight() - y, 0);
      for (std::size_t x = 0; x < screen.getPixelCountWidth(); ++x) {
        // terminal characters have 5:2 aspect ratio.
        // Printing two next eachother results in pixels with 5:4 aspect ratio
        if (screen(x, y) == PixelColor::on) {
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
