#include "drawing_lib.h"
#include "gamma.h"


#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

char *drawing_lib_plain_board_small (gamma_t *gamma) {
	uint64_t size = 2048;
	char *result = malloc(sizeof(char) * size);

	if (result == NULL) return NULL;

	uint64_t index = 0;

	for (uint32_t i = gamma->height; i > 0; --i) {
		for (uint32_t j = 0; j < gamma->width; ++j) {
			// extend size if necessary.
			if (index >= size - 3) {
				size = size * 2;
				util_extend(&result, size);
				if (result == NULL) return NULL;
			}

			if (gamma->board[i - 1][j] == 0) {
				result[index++] = '.';
			} else {
				result[index++] = gamma->board[i - 1][j] + '0';
				// insert a number
			}
		}
		result[index++] = '\n';
	}

	result[index++] = '\0';

	return result;
}


char *drawing_lib_plain_board_big (gamma_t *gamma) {
	uint64_t size = 2048;
	char *result = malloc(sizeof(char) * size);

	if (result == NULL) return NULL;

	uint64_t index = 0;
	uint64_t spaces = util_size_of_number(gamma->players_number);
	uint64_t extra_size = 2 + spaces + 2;

	for (uint32_t i = gamma->height; i > 0; --i) {
		for (uint32_t j = 0; j < gamma->width; ++j) {
			// extend size if necessary.
			if (index >= size - extra_size) {
				size = size * 2;
				util_extend(&result, size);
				if (result == NULL) return NULL;
			}

			result[index++] = '[';
			if (gamma->board[i - 1][j] == 0) {
				// calculate width
				uint32_t left = (spaces - 1) / 2;
				uint32_t right = spaces - left - 1;
				// add '   .    '
				for (uint32_t k = 0; k < left; ++k) result[index++] = ' ';
				result[index++] = '.';
				for (uint32_t k = 0; k < right; ++k) result[index++] = ' ';

			} else {
				uint32_t signs =
						spaces - util_size_of_number(gamma->board[i - 1][j]);
				uint32_t left = signs / 2;
				uint32_t right = signs - left;
				// insert a number
				for (uint32_t k = 0; k < left; ++k) result[index++] = ' ';

				char number[10];
				sprintf(number, "%d", gamma->board[i - 1][j]);
				for (uint32_t k = 0; k < strlen(number); k++) {
					result[index++] = number[k];
				}

				for (uint32_t k = 0; k < right; ++k) result[index++] = ' ';
			}
			result[index++] = ']';
		}
		result[index++] = '\n';
	}

	result[index++] = '\0';

	return result;
}


// color
const char reset_colors[] = "\e[39m\e[49m";
const char cursor_colors[] = "\e[38;5;15m\e[48;5;177m";

const char taken_colors[] = "\e[38;5;15m\e[48;5;27m";
const char other_colors[] = "\e[38;5;15m\e[48;5;243m";


char *drawing_lib_color_board_small (gamma_t *gamma, uint32_t x, uint32_t y,
									 uint32_t player) {
	uint64_t size = 2048;
	char *result = malloc(sizeof(char) * size);

	if (result == NULL) return NULL;

	uint64_t index = 0;

	// clear screen
	result[index++] = '\e';
	result[index++] = 'c';

	// draw board
	for (uint32_t i = gamma->height; i > 0; --i) {
		for (uint32_t j = 0; j < gamma->width; ++j) {
			// extend size if necessary.
			if (index >=
				size - 4 - strlen(reset_colors) - strlen(other_colors)) {
				size = size * 2;
				util_extend(&result, size);
				if (result == NULL) return NULL;
			}

			if (i - 1 == y && x == j) { // rysuje kursor
				for (uint32_t k = 0; k < strlen(cursor_colors); k++)
					result[index++] = cursor_colors[k];
			} else if (gamma->board[i - 1][j] == player) { // nalezy do gracza
				for (uint32_t k = 0; k < strlen(taken_colors); k++)
					result[index++] = taken_colors[k];
			} else if (gamma->board[i - 1][j] != 0) { // naezy do innego gracza
				for (uint32_t k = 0; k < strlen(other_colors); k++)
					result[index++] = other_colors[k];
			}

			if (gamma->board[i - 1][j] == 0) {
				result[index++] = '.';
			} else {
				result[index++] = gamma->board[i - 1][j] + '0';
			}

			if ((i - 1 == y && x == j) || gamma->board[i - 1][j] != 0 ||
				gamma->board[i - 1][j] == player) {
				for (uint32_t k = 0; k < strlen(reset_colors); k++)
					result[index++] = reset_colors[k];
			}
		}
		result[index++] = '\r';
		result[index++] = '\n';
	}


	char *turn = "\r\nRuch gracza numer: ";
	char number[10];
	sprintf(number, "%d", player);
	char *taken_fields = "\r\nIlość zajętych pól przez gracza: ";
	char taken_f[20];
	sprintf(taken_f, "%ld", gamma_busy_fields(gamma, player));
	char *golden = "\n\rZlotych ruchow: [] (zero)";
	if (gamma_golden_possible(gamma, player) == true) {
		golden = "\n\rZlotych ruchow: [G]";
	}

	uint32_t additional_space =
			strlen(turn) + strlen(number) + strlen(taken_fields) +
			strlen(taken_colors)
			+ strlen(taken_f) + strlen(reset_colors) + strlen(golden) + 3;

	if (index >= size - additional_space) {
		size = size * 2;
		util_extend(&result, size);
		if (result == NULL) {
			return NULL;
		}
	}

	for (uint32_t k = 0; k < strlen(turn); k++) result[index++] = turn[k];
	for (uint32_t k = 0; k < strlen(number); k++) result[index++] = number[k];
	for (uint32_t k = 0; k < strlen(taken_fields); k++)
		result[index++] = taken_fields[k];
	for (uint32_t k = 0; k < strlen(taken_colors); k++)
		result[index++] = taken_colors[k];
	for (uint32_t k = 0; k < strlen(taken_f); k++) result[index++] = taken_f[k];
	for (uint32_t k = 0; k < strlen(reset_colors); k++)
		result[index++] = reset_colors[k];
	for (uint32_t k = 0; k < strlen(golden); k++) result[index++] = golden[k];

	result[index++] = '\r';
	result[index++] = '\n';
	result[index++] = '\0';

	return result;
}

char *drawing_lib_color_board_big (gamma_t *gamma, uint32_t x, uint32_t y,
								   uint32_t player) {
	uint64_t size = 2048;
	char *result = malloc(sizeof(char) * size);

	if (result == NULL) return NULL;

	uint64_t index = 0;
	uint64_t spaces = util_size_of_number(gamma->players_number);
	uint64_t extra_size =
			3 + spaces + 2 + strlen(reset_colors) + strlen(other_colors);

	// clear screen
	result[index++] = '\e';
	result[index++] = 'c';


	for (uint32_t i = gamma->height; i > 0; --i) {
		for (uint32_t j = 0; j < gamma->width; ++j) {

			// extend size if necessary.
			if (index >= size - extra_size) {
				size = size * 2;
				util_extend(&result, size);
				if (result == NULL) return NULL;
			}

			if (i - 1 == y && x == j) {
				for (uint32_t k = 0; k < strlen(cursor_colors); k++)
					result[index++] = cursor_colors[k];
			} else if (gamma->board[i - 1][j] == player) {
				for (uint32_t k = 0; k < strlen(taken_colors); k++)
					result[index++] = taken_colors[k];
			} else if (gamma->board[i - 1][j] != 0) {
				for (uint32_t k = 0; k < strlen(other_colors); k++)
					result[index++] = other_colors[k];
			}

			result[index++] = '[';
			if (gamma->board[i - 1][j] == 0) {
				// calculate width
				uint32_t left = (spaces - 1) / 2;
				uint32_t right = spaces - left - 1;
				// add '   .    '
				for (uint32_t k = 0; k < left; ++k) result[index++] = ' ';
				result[index++] = '.';
				for (uint32_t k = 0; k < right; ++k) result[index++] = ' ';

			} else {
				uint32_t signs =
						spaces - util_size_of_number(gamma->board[i - 1][j]);
				uint32_t left = signs / 2;
				uint32_t right = signs - left;
				// insert a number
				for (uint32_t k = 0; k < left; ++k) result[index++] = ' ';

				char number[10];
				sprintf(number, "%d", gamma->board[i - 1][j]);
				for (uint32_t k = 0; k < strlen(number); k++) {
					result[index++] = number[k];
				}

				for (uint32_t k = 0; k < right; ++k) result[index++] = ' ';
			}
			result[index++] = ']';

			if ((i - 1 == y && x == j) || gamma->board[i - 1][j] != 0 ||
				gamma->board[i - 1][j] == player) {
				for (uint32_t k = 0; k < strlen(reset_colors); k++)
					result[index++] = reset_colors[k];
			}
		}
		result[index++] = '\r';
		result[index++] = '\n';
	}

	char *turn = "\r\nRuch gracza numer: ";
	char number[10];
	sprintf(number, "%d", player);
	char *taken_fields = "\r\nIlość zajętych pól przez gracza: ";
	char taken_f[20];
	sprintf(taken_f, "%ld", gamma_busy_fields(gamma, player));
	char *golden = "\n\rZlotych ruchow: [] (zero)";
	if (gamma_golden_possible(gamma, player) == true) {
		golden = "\n\rZlotych ruchow: [G]";
	}

	uint32_t additional_space =
			strlen(turn) + strlen(number) + strlen(taken_fields) +
			strlen(taken_colors)
			+ strlen(taken_f) + strlen(reset_colors) + strlen(golden) + 3;

	if (index >= size - additional_space) {
		size = size * 2;
		util_extend(&result, size);
		if (result == NULL) {
			return NULL;
		}
	}

	for (uint32_t k = 0; k < strlen(turn); k++) result[index++] = turn[k];
	for (uint32_t k = 0; k < strlen(number); k++) result[index++] = number[k];
	for (uint32_t k = 0; k < strlen(taken_fields); k++)
		result[index++] = taken_fields[k];
	for (uint32_t k = 0; k < strlen(taken_colors); k++)
		result[index++] = taken_colors[k];
	for (uint32_t k = 0; k < strlen(taken_f); k++) result[index++] = taken_f[k];
	for (uint32_t k = 0; k < strlen(reset_colors); k++)
		result[index++] = reset_colors[k];
	for (uint32_t k = 0; k < strlen(golden); k++) result[index++] = golden[k];


	result[index++] = '\r';
	result[index++] = '\n';
	result[index++] = '\0';

	return result;
}

void drawing_lib_ending_screen (gamma_t *gamma) {

	char *end_game = "\r\n------- KONIEC GRY! ------- \r\n";
	write(STDOUT_FILENO, end_game, strlen(end_game));


	for (uint32_t i = 0; i < gamma->players_number; i++) {
		char *player_name = "Gracz numer";
		char taken_fields[20];
		sprintf(taken_fields, "%ld", gamma->players[i].fields);
		char player_nr[10];
		sprintf(player_nr, "%d", i+1);
		char *request = malloc(
				strlen(player_name) + strlen(taken_fields) + strlen(player_nr) +
				20);
		if (request == NULL) {
			exit(1);
		}
		sprintf(request, "%s: %s	 	%s pól\r\n", player_name, player_nr, taken_fields);
		write(STDOUT_FILENO, request, strlen(request));
	}
}



