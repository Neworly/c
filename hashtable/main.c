




#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	void** bucket;
	char** keys;

	unsigned offset;
	unsigned scale;

	unsigned size;
	unsigned capacity;
} HashTable;


HashTable 
new_hashmap(unsigned size, unsigned capacity)
{
	return (HashTable)
	{
		.bucket = NULL,
		.keys = NULL,
		.scale = size,
		.size = 10
	};
}


unsigned
__hash_index_transfomer(HashTable* table, char* const key)
{
	unsigned code = 0;

	for (char* tracker = key; *tracker != '\0'; tracker++) 
	{
		code = code + (*tracker);
	}
	
	return code % table->size;
}

int
hash_code(HashTable* table, char* const key)
{

	int index = __hash_index_transfomer(table, key);

	char** ptr = table->keys;

	for (int at = 0; at < table->size; (at++, ptr++)) {
		if (((*ptr != NULL)) && (strcmp(table->keys[at], key) == 0)) {
			return -1;
		}
	}

	return index; 
}

void
hash_push(HashTable* table, char* const key, void* value)
{
	


	unsigned size = table->size;
	unsigned scale = table->scale;

	table->bucket = realloc(table->bucket, size * scale);
	table->keys = realloc(table->keys, size * sizeof(void*));

	int index = __hash_index_transfomer(table, key);


	while (table->keys[index] != NULL) {
		index = (index + 1) % table->size;
	}

	table->keys[index] = realloc(table->keys[index], strlen(key));
	table->bucket[index] = value;
	strcpy(table->keys[index], key);
}	


void*
hash_look(HashTable* table, char* const key)
{
	int index = __hash_index_transfomer(table, key);

	while (table->keys[index] != NULL) {
		index = (index + 1) % table->size;
	}
	
	return table->bucket[index];
}

int 
main(void)
{

	HashTable table = new_hashmap(sizeof(char), 10);

	int a = 2;
	int b = 4;

	hash_push(&table, "John", &a);
	hash_push(&table, "smothjon", &b);

	printf("%i", *(int*)hash_look(&table, "smothjon"));

	return EXIT_SUCCESS;
}
