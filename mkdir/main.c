


















#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


#define BASEPATH "/mnt/c/Users/pioca/OneDrive/Desktop/coding/"
#define nullptr NULL

struct stdin_data_t {

	char* mem;
	size_t size;

} stdin_data;

void stdin_read() {
	#define END 10
	
	free(stdin_data.mem);
	stdin_data.mem = calloc(1, sizeof(char));
	stdin_data.size = 1;

	size_t indicator = 0;

	int keycode;

	while ((keycode = fgetc(stdin)) && keycode != EOF) {
		if (keycode == END && indicator > 0) {
			break;
		}
	
		if (keycode == ' ') {
			continue;
		}

		stdin_data.mem[indicator] = keycode;
		stdin_data.size += 1;
		stdin_data.mem = reallocarray(stdin_data.mem, stdin_data.size, sizeof(char));
		indicator++;
	}
}


int main(void) {
	char* members[]  = { ".learning", ".projects", ".practice", nullptr };
		
	stdin_read();

	char dependant[] = BASEPATH;
	

	size_t base_size = strlen(dependant) + stdin_data.size;


	char* type = malloc(base_size);
	
	strcat(type, BASEPATH);
	strcat(type, stdin_data.mem);
	mkdir(type, S_IFDIR);
	strcat(type, "/");

	size_t index = 0;

	while (members[index]) {
		char* pathname = malloc(base_size + strlen(members[index]));

		strcat(pathname, type);
		strcat(pathname, members[index]);

		mkdir(pathname, S_IFDIR);


		index++;
	}

	free(stdin_data.mem);
}



