#include "player_lib.h"

player_t create_new_player () {
	player_t player;

	player.areas = 0;
	player.golden_move = 0;
	player.fields = 0;
	player.dfs = 0;

	return player;
}