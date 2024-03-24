

#include <stdio.h>

#include "hash.h"


int 
main(void)
{
	HashTable self = new_hashtable(2, sizeof(void*));


	hash_insert(&self, "mem", "vvalue to unknown");
	printf("%s", (char*) hash_get(&self, "mem"));


}
