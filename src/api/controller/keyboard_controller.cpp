#include "keyboard_controller.hpp"

#include <clocale>
#include <ncurses.h>
#include <cstdlib>

KeyboardController::KeyboardController() {
	setlocale(LC_ALL, "");
	if (!getenv("TERM")) {
		setenv("TERM", "xterm-256color", 1);
	}

	initscr();
	curs_set(0);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	set_escdelay(25);
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