









#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>



struct Map {
	// there must be other ways to initialize this as supposed,
	// for now as abstraction designer I'll leave it as that.
	char value[256][256];
};

struct Map graph(const uint8_t size) {
	#define EMPTY_WALL '#';

	// const uint8_t height = size;
	// const uint8_t width = size;

	struct Map self;

	uint8_t cursor = 0;
	
	while (size > cursor) {
		for (uint8_t block_id = 0; size > block_id; block_id++) 
		{
			self.value[cursor][block_id] = EMPTY_WALL;

		};	

		cursor += 1;
	};
	

	return self;	
};


void debug_graph(struct Map self, const uint8_t size) {

	uint8_t cursor = 0;

	while (size > cursor) {
		for (uint8_t block_id = 0; size > block_id; block_id++) 
		{
			printf("%c", self.value[cursor][block_id]);

		};	
		printf("\n");
		cursor += 1;
	};
	
}


