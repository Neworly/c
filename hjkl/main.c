
  


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "player.h"
#include "map.h"
#include "input.h"

#define PLAYER_SYMBOL 'T'
#define WALL_SYMBOL '#'
#define TARGET_SYMBOL 'X'

uint8_t graph_size = 0;
uint8_t max_graph_size = 10;

void update_player_position(struct Map* grid) {
	system("clear");
	

	struct Player* data = &player_data;

	grid->value[player_prev_space.y][player_prev_space.x] = EMPTY_WALL;
	grid->value[data->space.y][data->space.x] = PLAYER_SYMBOL;

	debug_graph(*grid, graph_size);
}


char current_tile(struct Map grid) {
	struct Player* data = &player_data;
	return grid.value[data->space.y][data->space.x];
}


void relocate_player() {
	struct Player *pdata = &player_data;
	
	player_prev_space.x = pdata->space.x;
	player_prev_space.y = pdata->space.y;

	const char key = get_key();

	switch (key) {
		case 'h': pdata->space.x = pdata->space.x - 1; break;
		case 'j': pdata->space.y = pdata->space.y + 1; break;
		case 'k': pdata->space.y = pdata->space.y - 1; break;
		case 'l': pdata->space.x = pdata->space.x + 1; break;
		default: break;
	}


	if ((pdata->space.x < 0 || pdata->space.y < 0) 
		|| (pdata->space.x >= graph_size || pdata->space.y >= graph_size))
	{
		pdata->space.x = player_prev_space.x;
		pdata->space.y = player_prev_space.y;
	}
}

struct Map new() {
	graph_size = (rand() % (max_graph_size-5)) + 5;

	struct Map grid = graph(graph_size);
	struct Player* data = &player_data;

	data->space.x = graph_size / 2;
	data->space.y = graph_size / 2;

	uint8_t tx = (rand() % (graph_size-2)) + 2;
	uint8_t ty = (rand() % (graph_size-2)) + 2;
	grid.value[ty][tx] = TARGET_SYMBOL;

	update_player_position(&grid);

	return grid;
}

int main() {

	printf("\n\n\n");

	uint64_t per_victory = 5;

	struct Player* data = &player_data;
	struct Map grid = new();

	while (is_player_alive() == Alive) {
		relocate_player();
		
		if (current_tile(grid) == TARGET_SYMBOL) {
			grid = new();
			data->score += per_victory;
			per_victory = per_victory * 2;
		}

		update_player_position(&grid);
		printf("\n\n<Score: %lu, Lives: %u>\n", data->score, data->lives);
	}

	return EXIT_SUCCESS;
}

