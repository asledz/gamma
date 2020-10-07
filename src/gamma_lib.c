#include "gamma_lib.h"
#include "gamma.h"

static void gamma_lib_fill_gamma(gamma_t *g) {
	for (uint32_t i = 0; i < g->height; ++i) {
		for (uint32_t j = 0; j < g->width; ++j) {
			g->board[i][j] = 0;
			g->board[i][j] = 0;
		}
	}
	for (uint32_t i = 0; i < g->players_number; ++i) {
		g->players[i] = create_new_player();
	}
}

gamma_t *gamma_lib_create_new_gamma (uint32_t width, uint32_t height,
									 uint32_t players, uint32_t areas) {
	// creating gamma.
	gamma_t *new_gamma = malloc(sizeof(gamma_t));
	// malloc error.
	if (new_gamma == NULL) return new_gamma;

	// creating players array.
	new_gamma->players = malloc(sizeof(player_t) * players);
	// malloc error.
	if (new_gamma->players == NULL) {
		free(new_gamma);
		return NULL;
	}

	// creating board.
	new_gamma->board = malloc(sizeof(uint32_t *) * height);
	// malloc error.
	if (new_gamma->board == NULL) {
		free(new_gamma->players);
		free(new_gamma);
		return NULL;
	}
	for (uint32_t i = 0; i < height; ++i) {
		new_gamma->board[i] = malloc(sizeof(uint32_t) * width);
		// malloc error.
		if (new_gamma->board[i] == NULL) {
			while (i > 0) {
				free(new_gamma->board[i-1]);
				--i;
			}
			free(new_gamma->board);
			free(new_gamma->players);
			free(new_gamma);
		}
	}

	// creating dfs board.
	new_gamma->visited = malloc(sizeof(uint32_t *) * height);
	// malloc error.
	if (new_gamma->visited == NULL) {
		// deleting rows.
		for (uint32_t i = 0; i < height; ++i) {
			free(new_gamma->board[i]);
		}
		// deleting other parameters.
		free(new_gamma->board);
		free(new_gamma->players);
		free(new_gamma);
		return NULL;
	}
	// creating dfs rows.
	for (uint32_t i = 0; i < height; i++) {
		new_gamma->visited[i] = malloc(sizeof(uint32_t) * width);
		// malloc error.
		if (new_gamma->visited[i] == NULL) {
			// backtrack to delete all of the rows.
			while (i > 0) {
				free(new_gamma->visited[i - 1]);
				--i;
			}
			for (uint32_t j = 0; j < height; ++j) {
				free(new_gamma->board[j]);
			}
			// deleting other parameters.
			free(new_gamma->visited);
			free(new_gamma->board);
			free(new_gamma->players);
			free(new_gamma);
			return NULL;
		}
	}

	new_gamma->width = width;
	new_gamma->height = height;
	new_gamma->players_number = players;
	new_gamma->areas_number = areas;

	gamma_lib_fill_gamma(new_gamma);

	return new_gamma;
}

void gamma_lib_delete_gamma(gamma_t *g) {
	if (g->players != NULL) free(g->players);

	for (uint32_t i = 0; i < g->height; i++) {
		if (g->board[i] != NULL) free(g->board[i]);
		if (g->visited[i] != NULL) free(g->visited[i]);
	}

	if (g->board != NULL) free(g->board);

	if (g->visited != NULL) free(g->visited);
}


static void dfs_clear_dfs (gamma_t *g) {
	for (uint32_t i = 0; i < g->height; ++i) {
		for (uint32_t j = 0; j < g->width; ++j) {
			g->visited[i][j] = 0;
		}
	}
	for (uint32_t i = 0; i < g->players_number; ++i) {
		g->players[i].dfs = 0;
	}
}

static bool dfs_correct (const int32_t x, const int32_t y, const gamma_t *g) {
	if (x < 0 || y < 0) {
		return false;
	}
	if ((uint32_t) x < g->height && (uint32_t) y < g->width) {
		return true;
	}
	return false;
}


static void dfs_dfs (uint32_t x, uint32_t y, gamma_t *g) {
	g->visited[x][y] = 1;
	uint32_t x1 = x + 1, y1 = y;
	uint32_t x2 = x, y2 = y + 1;
	uint32_t x3 = x - 1, y3 = y;
	uint32_t x4 = x, y4 = y - 1;
	if (dfs_correct(x1, y1, g) && g->board[x][y] == g->board[x1][y1] &&
		g->visited[x1][y1] == 0) {
		dfs_dfs(x1, y1, g);
	}
	if (dfs_correct(x2, y2, g) && g->board[x][y] == g->board[x2][y2] &&
		g->visited[x2][y2] == 0) {
		dfs_dfs(x2, y2, g);
	}
	if (dfs_correct(x3, y3, g) && g->board[x][y] == g->board[x3][y3] &&
		g->visited[x3][y3] == 0) {
		dfs_dfs(x3, y3, g);
	}
	if (dfs_correct(x4, y4, g) && g->board[x][y] == g->board[x4][y4] &&
		g->visited[x4][y4] == 0) {
		dfs_dfs(x4, y4, g);
	}
}


bool gamma_lib_make_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
	dfs_clear_dfs(g);
	uint32_t former_player = g->board[y][x];
	g->board[y][x] = player;

	for (uint32_t i = 0; i < g->height; ++i) {
		for (uint32_t j = 0; j < g->width; ++j) {
			if (g->visited[i][j] == 0) {
				dfs_dfs(i, j, g);
				if (g->board[i][j] != 0) {
					++(g->players[g->board[i][j] - 1].dfs);
				}
			}
		}
	}


	for (uint32_t i = 0; i < g->players_number; ++i) {
		if (g->players[i].dfs > g->areas_number) {
			// task assumptions not met.
			g->board[y][x] = former_player;
			return false;
		}
	}
	for (uint32_t i = 0; i < g->players_number; i++) {
		g->players[i].areas = g->players[i].dfs;
	}

	if (former_player != 0) {
		--g->players[former_player-1].fields;
	}
	++g->players[player-1].fields;

	return true;
}

uint64_t gamma_lib_count_fields (gamma_t *g, uint32_t player) {
    uint64_t fields = 0;
    for (uint32_t i = 0; i < g->height; ++i) {
        for (uint32_t j = 0; j < g->width; j++) {
            if (g->board[i][j] == player) fields++;
        }
    }
    g->players[player-1].fields = fields;
	return g->players[player-1].fields;
}

uint64_t gamma_lib_count_free_fields(gamma_t *g, uint32_t player) {
	uint64_t result = 0;
	for (uint32_t i = 0; i < g->height; ++i) {
		for (uint32_t j = 0; j < g->width; j++) {
			if (g->board[i][j] == 0 &&
				g->players[player-1].areas < g->areas_number) {
				// field is not taken, player has free areas.
				++result;
				continue;
			}
			if (g->board[i][j] == 0) {
				// field is not taken, player doesnt has free areas.
				int32_t T[2][4] = {{1, 0, -1, 0},
								   {0, 1, 0,  -1}};
				uint32_t k;
				for (k = 0; k < 4; k++) {
					int32_t I = i + T[0][k];
					int32_t J = j + T[1][k];
					if (dfs_correct(I, J, g) && g->board[I][J] == player) {
						++result;
						k = 6;
					}
				}
				if (k == 0) {
					continue;
				}
			}
		}
	}
	return result;
}


bool gamma_lib_can_golden_move_possible (gamma_t *g, uint32_t player) {
	if (g->players[player-1].golden_move == true) {
		return false;
	}
	for (uint32_t i = 0; i < g->height; ++i) {
		for (uint32_t j = 0; j < g->width; j++) {
			uint32_t former = g->board[i][j];
			if ((g->board[i][j] != 0 && g->board[i][j] != player) && gamma_lib_make_move(g, player, j, i)) {
				g->board[i][j] = former;
				return true;
			}
		}
	}
	return false;
}

bool gamma_lib_is_valid_player (gamma_t **gamma_ptr, uint32_t player) {
	gamma_t *gamma = *gamma_ptr;
	if (gamma_free_fields(gamma, player) != 0 ||
		gamma_golden_possible(gamma, player) != 0) {
		return true;
	}
	return false;
}

bool gamma_lib_finish_game (gamma_t **gamma_ptr) {
	gamma_t *gamma = *gamma_ptr;
	for (uint32_t i = 1; i <= gamma->players_number; i++) {
		if (gamma_free_fields(gamma, i) != 0 ||
			gamma_golden_possible(gamma, i) != 0) {
			return false;
		}
	}
	return true;
}
