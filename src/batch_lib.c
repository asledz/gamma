#define _GNU_SOURCE

#include "batch_lib.h"
#include "util.h"


/** @brief Sprawdza, czy podana linia jest poprawną linią do gamma_move,
* jeśli tak, wykonuje ruch.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_move_line (char *line, gamma_t **gamma) {
	if (line[0] == 'm' && util_number_of_breaks(line) == 3 && util_valid_characters(line)) {
		uint64_t player = util_read_argument(line, 1);
		uint64_t x = util_read_argument(line, 2);
		uint64_t y = util_read_argument(line, 3);
		if (player > UINT32_MAX || x > UINT32_MAX || y > UINT32_MAX) return false;

		if (x > (*gamma)->width || y > (*gamma)->height) return false;

		printf("%d\n", gamma_move(*gamma, player, x, y));

		return true;
	}
	return false;
}

/** @brief Sprawdza, czy podana linia jest poprawną linią do gamma_golden_move,
* jeśli tak, wykonuje ruch.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_golden_move_line (char *line, gamma_t **gamma) {
	if (line[0] == 'g' && util_number_of_breaks(line) == 3 && util_valid_characters(line)) {
		uint64_t player = util_read_argument(line, 1);
		uint64_t x = util_read_argument(line, 2);
		uint64_t y = util_read_argument(line, 3);
		if (player > UINT32_MAX || x > UINT32_MAX || y > UINT32_MAX) return false;

		if (x >= (*gamma)->width || y >= (*gamma)->height) return false;

		printf("%d\n", gamma_golden_move(*gamma, player, x, y));
		return true;
	}
	return false;
}

/** @brief Sprawdza, czy podana linia jest poprawną linią do busy_fields,
* wykonuje jeśli tak.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_busy_fields_line (char *line, gamma_t **gamma) {
	if (line[0] == 'b' && util_number_of_breaks(line) == 1 && util_valid_characters(line)) {
		uint64_t player = util_read_argument(line, 1);
		if (player > UINT32_MAX) return false;

		printf("%ld\n", gamma_busy_fields(*gamma, player));
		return true;
	}
	return false;
}

/** @brief Sprawdza, czy podana linia jest poprawną linią do free_fields
* wykonuje jeśli tak.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_free_fields_line (char *line, gamma_t **gamma) {
	if (line[0] == 'f' && util_number_of_breaks(line) == 1 && util_valid_characters(line)) {
		uint64_t player = util_read_argument(line, 1);
		if (player > UINT32_MAX || player < 1) return false;

		printf("%ld\n", gamma_free_fields(*gamma, player));
		return true;
	}
	return false;
}

/** @brief Sprawdza, czy podana linia jest poprawną linią do golden_possible
* wykonuje jeśli tak.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_golden_possible_line (char *line, gamma_t **gamma) {
	if (line[0] == 'q' && util_number_of_breaks(line) == 1 && util_valid_characters(line)) {
		uint64_t player = util_read_argument(line, 1);
		if (player > UINT32_MAX) return false;

		printf("%d\n", gamma_golden_possible(*gamma, player));
		return true;
	}
	return false;
}

/** @brief Sprawdza, czy podana linia jest poprawną linią do board,
* wykonuje jeśli tak.
* @param[in] line       – linia do sprawdzenia.
* @param[in] gamma       – gamma.
* @return @p true jeśli poprawne, @p false w przeciwnym wypadku.
*/
bool check_gamma_board_line(const char *line, gamma_t **gamma) {
	if (line[0] == 'p' && line[1] == '\n') {
		char *p = gamma_board(*gamma);
		printf("%s", p);
		free(p);
		return true;
	}
	return false;
}

void batch_lib_play (gamma_t **gamma_ptr, uint64_t number_of_lines) {
	gamma_t *gamma = *gamma_ptr;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (((read = getline(&line, &len, stdin)) != -1)) {
		++number_of_lines;

		if (line[0] == '#' || line[0] == '\n') continue; // ignore the line

		if (check_gamma_move_line(line, &gamma)) continue; // move line

		if (check_gamma_golden_move_line(line, &gamma)) continue; // golden move line

		if (check_gamma_busy_fields_line(line, &gamma)) continue; // busy fields line

		if (check_gamma_free_fields_line(line, &gamma)) continue; // free fields line

		if (check_gamma_golden_possible_line(line, &gamma)) continue;

		if (check_gamma_board_line(line, &gamma)) continue;

		fprintf(stderr, "ERROR %lu\n", number_of_lines);
	}
	free(line);
}
