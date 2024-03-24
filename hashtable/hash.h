




/* THIS IS INCOMPLETE, A PROTOTYPE */


#include <stdio.h>
#include <string.h>

typedef struct 
{
	void** bucket;
	void** water;

	unsigned n;
	unsigned m;

	unsigned windex;
} HashTable;

enum { FOUND = 1, NOT_FOUND = -1 };

#include <stdlib.h>

HashTable
new_hashtable(unsigned m, unsigned byte)
{
	void** bucket = (void**) malloc(m);
	void** water = (void**) malloc(m);

	for (int i = 0; i < m; (bucket[i] = malloc(m * byte), water[i] = malloc(m * byte), i++));

	return (HashTable) { bucket, water, 0, m, 0 };
}

unsigned hash_function(HashTable* table, char* str);

short
hash_code(HashTable* table, char* key)
{
	for (int i = 0; i < table->n; i++) {
		if (key == table->water[i]) return FOUND;
	}

	return NOT_FOUND;
}

void* 
hash_get(HashTable* table, char* key)
{
	short k = hash_code(table, key);
	if (k == NOT_FOUND) {
		return NULL;
	}

	return table->bucket[k];
}

unsigned 
hash_function(HashTable* table, char* str)
{
	
	table->water[table->windex] = str;
	char* cstr = str;
	unsigned accum = 0;

	for (; *cstr != '\0'; cstr++) {
		if (*cstr == ' ') continue;
		accum += *cstr;
	};

	unsigned max = table->m;
	unsigned index = accum % max;

	short not_found_space;
	
	while ((not_found_space = NOT_FOUND != hash_code(table, str))) {
		index = (index + 1) % max;
	}
	
	return index;
}

void
hash_insert(HashTable* table, char* key, void* value)
{
	if (table->n >= table->m) {
		return;
	}

	table->bucket[hash_function(table, key)] = value;
	table->n += 1;
	table->windex += 1;
}


