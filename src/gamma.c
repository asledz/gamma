//
// Created by Anita on 29/07/2020.
//
#include "gamma.h"
#include "drawing_lib.h"

gamma_t* gamma_new(uint32_t width, uint32_t height,
				   uint32_t players, uint32_t areas) {
	// Checking arguments.
	if (width == 0 || height == 0 || players == 0 || areas == 0) {
		return NULL;
	}
	return gamma_lib_create_new_gamma(width, height, players, areas);
}

void gamma_delete(gamma_t *g) {
	if (g == NULL) return;
	gamma_lib_delete_gamma(g);
	// TODO check if I have to free(g)
	free(g);
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
	if (g == NULL) return false;
	if (player <= 0 || player > g->players_number || x >= g->width || y >= g->height) return false;
	if (g->board[y][x] != 0) return false;

	return gamma_lib_make_move(g, player, x, y);
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
	if (g == NULL) return false;
	if (player <= 0 || player > g->players_number || x >= g->width || y >= g->height) return false;
	if (g->board[y][x] == 0 || g->board[y][x] == player) return false;
	if (g->players[player-1].golden_move == true) return false;

	bool ret_value = gamma_lib_make_move(g, player, x, y);

	if (ret_value) {
		g->players[player-1].golden_move = true;
	}
	return ret_value;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
	if (g == NULL || player == 0 || player > g->players_number) return 0;
	return gamma_lib_count_fields(g, player);
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
	if (g == NULL || player == 0 || player > g->players_number) return 0;
	return gamma_lib_count_free_fields(g, player);
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
	if (g == NULL || player == 0 || player > g->players_number) {
		// Invalid argument.
		return 0;
	}
	return gamma_lib_can_golden_move_possible(g, player);
}

char* gamma_board(gamma_t *g) {
	if (g == NULL) return NULL;
	if (g->players_number < 10) {
		return drawing_lib_plain_board_small(g);
	}
	else {
		return drawing_lib_plain_board_big(g);
	}
}

