#define _GNU_SOURCE

#include "game_lib.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "gamma.h"
#include "batch_lib.h"
#include "interactive_lib.h"

#define INVALID false

void game_lib_start_game() {
	uint64_t number_of_lines = 0;

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	bool start_line = false;

	gamma_t *g = NULL;

	while (((read = getline(&line, &len, stdin)) != -1)) {
		++number_of_lines;
		if (line[0] != '#' && line[0] != '\n') {
			if (game_lib_start_line(line)) {
                g = game_lib_create_gamma_from_line(line);

                if (g != NULL) {
                    start_line = true;
                    break;
                } else {
                    fprintf(stderr, "ERROR %lu\n", number_of_lines);
                }

			} else {
				fprintf(stderr, "ERROR %lu\n", number_of_lines);
			}
		}
	}

	if (start_line) {

		if (line[0] == 'B') {
			printf("OK %ld\n", number_of_lines);
			batch_lib_play(&g, number_of_lines);
		} else {
			interactive_lib_play(&g, number_of_lines);
		}

		gamma_delete(g);

	}
    free(line);
}

gamma_t *game_lib_create_gamma_from_line(char *line) {
	uint64_t width = util_read_argument(line, 1);
	uint64_t height = util_read_argument(line, 2);
	uint64_t players = util_read_argument(line, 3);
	uint64_t areas = util_read_argument(line, 4);

	return gamma_new(width, height, players, areas);
}

bool game_lib_start_line(char *line) {

	if (strlen(line) < 9) return INVALID;
	if(line[0] != 'B' && line[0] != 'I') return INVALID;
	if (!util_is_whitespace(line[1])) return INVALID;
	if (!util_valid_characters(line)) return INVALID;
	if (util_number_of_breaks(line) != 4) return INVALID;


	uint64_t width = util_read_argument(line, 1);
	uint64_t height = util_read_argument(line, 2);
	uint64_t players = util_read_argument(line, 3);
	uint64_t areas = util_read_argument(line, 4);

	if (width > UINT32_MAX || height > UINT32_MAX || players > UINT32_MAX ||
		areas > UINT32_MAX) return INVALID;

	return (width < 1 || height < 1 || players < 1 || areas < 1) == INVALID;

	// Valid start line.
}
