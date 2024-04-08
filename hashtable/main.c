
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	void** bucket;
	char** keys;

	unsigned width;
	unsigned scale;

	unsigned size;
} HashTable;


HashTable 
new_hashmap(unsigned size, unsigned capacity)
{
	return (HashTable)
	{
		.bucket = NULL,
		.keys = NULL,
		.width = 0,
		.scale = size,
		.size = capacity,
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
hash_insert(HashTable* table, char* const key, void* value)
{

	unsigned width = table->width;
	unsigned scale = table->scale;
	unsigned psize = table->size;

	if (width == table->size) {
		table->size += 1;
	}

	unsigned size = table->size;

	if (width == 0 || width == psize) {
		table->bucket = realloc(table->bucket, size * sizeof(void*));
		table->keys = realloc(table->keys, size * sizeof(void*));
	}

	void** bucket = table->bucket;
	char** keys = table->keys;

	int index = __hash_index_transfomer(table, key);
	if (hash_code(table, key) == -1) {
		return;
	}
	
	while (keys[index] != NULL) {
		index = (index + 1) % table->size;
	}

	bucket[index] = realloc(table->bucket[index], scale);
	keys[index] = realloc(table->keys[index], strlen(key));
	bucket[index] = value;
	table->width += 1;
	strcpy(keys[index], key);
}	


void*
hash_look(HashTable* table, char* const key)
{
	if (hash_code(table, key) != -1) {
		return NULL;
	}
	 
	unsigned index = __hash_index_transfomer(table, key);
	while (strcmp(table->keys[index], key) != 0) {
		index = (index + 1) % table->size;
	}

	return table->bucket[index];
}

int 
main(void)
{

	HashTable table = new_hashmap(sizeof(int), 10);

	int a = 2;
	int b = 4;

	hash_insert(&table, "John", &a);
	hash_insert(&table, "John2", &a);
	hash_insert(&table, "John3", &b);
	hash_insert(&table, "John4", &b);
	hash_insert(&table, "o234234hn5", &b);
	hash_insert(&table, "o234234hn9", &b);
	hash_insert(&table, "o234234hn12", &b);
	hash_insert(&table, "o234234hn11", &b);

	printf("%i\n", *(int*) hash_look(&table, "John4"));



	return EXIT_SUCCESS;
}
