#include "util.h"


uint32_t util_size_of_number (uint32_t number) {
	uint32_t size = 0;
	while (number > 0) {
		++size;
		number /= 10;
	}
	return size;
}

char *util_extend (char **array, size_t size) {
	char *tmp = realloc(*array, sizeof(char) * size);
	if (tmp == NULL) {
		free(*array);
		return NULL;
	} else {
		return tmp;
	}
}

bool util_is_whitespace (char c) {
	if (c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == ' ' ||
		c == '\n') {
		return true;
	}
	return false;
}

bool util_valid_characters (char *line) {

	for (uint32_t i = 2; i < strlen(line); ++i) {
		if (util_is_whitespace(line[i]) || (line[i] >= '0' && line[i] <= '9')) {
			continue;
		} else {
			return false;
		}
	}
	if (line[strlen(line) - 1] != '\n') {
	    return false;
	}
	return true;
}

uint32_t util_number_of_breaks(char *line) {
	uint32_t result = 0;
	for (uint32_t i = 1; i < strlen(line) - 1; ++i)
		if (util_is_whitespace(line[i]) && !util_is_whitespace(line[i + 1])) ++result;
	return result;
}

uint64_t util_read_argument(char *line, uint32_t index) {
	if (strlen(line) == 0) return 0;
	uint64_t result = 0;
	uint32_t i = 0;

	while (index > 0) {
		while (!util_is_whitespace(line[i])) ++i;
		while (util_is_whitespace(line[i])) ++i;
		--index;
	}

	while (!util_is_whitespace(line[i])) {
		uint64_t last_result = result;
		result = result * 10 + line[i] - '0';
		if (result < last_result) return 0;
		++i;
	}

	return result;
}