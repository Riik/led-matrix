#include <ncurses.h>

int main()
{
	initscr();             // initialize curses
	cbreak();              // pass key presses to program, but not signals
	noecho();              // don't echo key presses to screen
	keypad(stdscr, TRUE);  // allow arrow keys
	// timeout(0);            // no blocking on getch()
	curs_set(0);           // set the cursor to invisible
	// printw("Hello World !!!");	/* Print Hello World		  */
	for(int j = 0; j < 32; j ++){
		move(j,2*(j%2));
		for(int i =0; i < 32; i++){
			addch(' ' | A_REVERSE);
			addch(' ' | A_REVERSE);
			addch(' ');
			addch(' ');
		}
	}
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}
