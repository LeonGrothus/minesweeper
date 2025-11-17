#include "keyboard_controller.hpp"

#include <ncurses.h>
#include <cstdlib>

KeyboardController::KeyboardController() {
	if (!getenv("TERM")) {
		setenv("TERM", "xterm-256color", 1);
	}
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
}

KeyboardController::~KeyboardController() {
	endwin();
}

int KeyboardController::get_key() const {
	(void)this;
	return getch();
}

std::vector<int> KeyboardController::get_buffered() const {
	(void)this;
	std::vector<int> buffered;

	int key;
	while ((key = getch()) != ERR) {
		buffered.push_back(key);
	}
	return buffered;
}