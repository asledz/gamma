#include "interactive_lib.h"
#include "gamma_lib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "drawing_lib.h"
static struct termios orig_termios;

/** @brief Włącza raw mode w konsoli.
 * Włącza raw mode w konsoli.
 * Funkcja pisana w oparciu funkcji z tego projektu (na otwartej licencji):
 * https://github.com/krpors/hx/blob/develop/util.c?fbclid=IwAR36FPaPDu5UIB2jN345idyCtR0eGdulxs76sgL7zxvdxRuWrD6vDSFTIY8#L213
 * Ustawia odpowiednie flagi.
 */
void enable_raw_mode () {
	tcgetattr(STDIN_FILENO, &orig_termios);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0) {
		perror("Unable to set terminal to raw mode");
		exit(1);
	}
	write(STDOUT_FILENO, "\e[?25lm", strlen("\e[?25h"));
}

/** @brief Wyłącza raw mode w konsoli.
* Wyłącza raw mode w konsoli.
* Funkcja pisana w oparciu funkcji z tego projektu (na otwartej licencji):
* https://github.com/krpors/hx/blob/develop/util.c?fbclid=IwAR36FPaPDu5UIB2jN345idyCtR0eGdulxs76sgL7zxvdxRuWrD6vDSFTIY8#L213
* Ustawia odpowiednie flagi.
*/
void disable_raw_mode () {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	(void) (write(STDOUT_FILENO, "\x1b[?25h", 6) + 1);
}

/** @brief czyta wczytany znak w raw mode.
* Funkcja pisana w oparciu funkcji z tego projektu (na otwartej licencji):
* https://github.com/krpors/hx/blob/develop/util.c?fbclid=IwAR36FPaPDu5UIB2jN345idyCtR0eGdulxs76sgL7zxvdxRuWrD6vDSFTIY8#L213
* @return znak wczytanego kodu.
*/
int read_key () {
	char c;
	ssize_t nread;
	while ((nread = read(STDIN_FILENO, &c, 1)) == 0);
	if (nread == -1) {
		return -1;
	}
	char seq[4];
	switch (c) {
		case KEY_ESC:
			if (read(STDIN_FILENO, seq, 1) == 0) return KEY_ESC;
			if (read(STDIN_FILENO, seq + 1, 1) == 0) return KEY_ESC;
			if (seq[0] == '[') {
				switch (seq[1]) {
					case 'A':
						return KEY_UP;
					case 'B':
						return KEY_DOWN;
					case 'C':
						return KEY_RIGHT;
					case 'D':
						return KEY_LEFT;
				}
			}
	}
	return c;
}

/** @brief Zwraca numer kolejnego gracza.
* Zwraca numer kolejnego gracza w grze, który może wykonać ruch.
* @return numer nowego gracza, jeśli żaden nie może
* ykonać ruchu - numer poprzedniego.
*/
uint32_t get_new_player (gamma_t **gamma_ptr, uint32_t player) {
	gamma_t *gamma = *gamma_ptr;

	uint32_t player_new = player + 1;
	if (player_new > gamma->players_number) {
		player_new = 1;
	}

	while (!gamma_lib_is_valid_player(&gamma, player_new) && player != player_new) {
		player_new++;
		if (player_new > gamma->players_number) {
			player_new = 1;
		}
	}
	return player_new;
}


void interactive_lib_play (gamma_t **gamma_ptr, uint64_t number_of_lines) {
	if(gamma_ptr == NULL) return;

	gamma_t *gamma = *gamma_ptr;
	uint32_t x = 0, y = 0;
	uint32_t player = 1;

	enable_raw_mode();

	char *board;
	if (gamma->players_number < 10) {
		board = drawing_lib_color_board_small(gamma, x, y, 1);
	} else {
		board = drawing_lib_color_board_big(gamma, x, y, 1);
	}

	if (board == NULL) {
		disable_raw_mode();
		exit(1);
	}

	write(STDOUT_FILENO, board, strlen(board));
	free(board);

	while (true) {
		int c = read_key();

		if (c == KEY_CTRL_D) break;
		/* MOVE */
		if (c == KEY_UP) {
			++y;
			if (x >= gamma->width || y >= gamma->height) --y;
		}
		if (c == KEY_DOWN) {
			if (y != 0) --y;
		}
		if (c == KEY_RIGHT) {
			++x;
			if (x >= gamma->width || y >= gamma->height) --x;
		}
		if (c == KEY_LEFT) {
			if (x != 0) --x;
		}

		// REGULAR MOVE
		if (c == KEY_SPACE) {
			if (gamma_move(gamma, player, x, y)) {
				player = get_new_player(&gamma, player);
				if (!gamma_lib_is_valid_player(&gamma, player)) {
					break;
				}
			}
		}

		if (c == 'G' || c == 'g') {
			if (gamma_golden_move(gamma, player, x, y)) {
				player = get_new_player(&gamma, player);
				if (!gamma_lib_is_valid_player(&gamma, player)) {
					break;
				}
			}
		}

		if (c == 'C' || c == 'c') {
			player = get_new_player(&gamma, player);
			if (!gamma_lib_is_valid_player(&gamma, player)) {
				break;
			}

		}
		if (gamma_lib_finish_game(&gamma)) {
			break;
		}

		if (gamma->players_number < 10) {
			board = drawing_lib_color_board_small(gamma, x, y, player);
		} else {
			board = drawing_lib_color_board_big(gamma, x, y, player);
		}

		if (board == NULL) {
			disable_raw_mode();
			exit(1);
		}

		write(STDOUT_FILENO, board, strlen(board));
		free(board);
	}

	drawing_lib_ending_screen (gamma);

	disable_raw_mode();
	exit(1);
}
