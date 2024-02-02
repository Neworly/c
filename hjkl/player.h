



  
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>



struct Space {
	uint8_t x;
	uint8_t y;
};

static struct Player {

	uint64_t score;
	uint8_t lives;
	
	struct Space space;
} player_data = { .score = 0, .lives = 3, .space = { .x = 0, .y = 0 } };

typedef enum {
	Alive = 0,
	Died = 1,
} PLAYER_STATE;


PLAYER_STATE is_player_alive() {
	struct Player *pdata = &player_data;
	
	if (pdata->lives > 0) {
		return Alive;
	}

	return Died;
};

PLAYER_STATE gain_live() {
	struct Player *pdata = &player_data;
	
	if (pdata->lives > 0) {
		pdata->lives = pdata->lives + 1;
		return Alive;
	}

	printf("[PLAYER_STATUS]: cannot gain lives while you're dead!");
	return Died;
}

PLAYER_STATE waste_live() {
	struct Player *pdata = &player_data;
	
	if (pdata->lives > 0) {
		pdata->lives = pdata->lives - 1;
		return Alive;
	}

	return Died;
}

void update_score(uint64_t n) {
	struct Player *pdata = &player_data;

	pdata->score = pdata->score + n;
}


struct Space player_prev_space = { .x = 0, .y = 0 };


