
















#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


struct vec_t {
	void** ptr;
	int capacity;
	int length;
};



void* INDEX_OUT_OF_BOUNDS_ERR(int item, int actual) {
	printf("indexing at '%i' causes an index out of bounds error, attempting to read an invalid memory location, try within this range [0/%i].\n", item, actual);
	return NULL;
}



struct vec_t vec_new() {
	static struct vec_t data = { NULL, 0, 0 };
	return data;
}

struct vec_t vec_with_cap(int capacity) {
	struct vec_t data = vec_new();

	if (capacity < 0) {
		return data;
	}

	data.capacity = capacity;
	data.ptr = calloc(capacity, sizeof(void*));

	return data;
}

void vec_push(struct vec_t* comp, void* data) {
	if (comp->length >= comp->capacity) {
		comp->capacity = comp->length;
		comp->ptr = reallocarray(comp->ptr, comp->capacity, sizeof(void*));
	}
	
	comp->ptr[comp->length] = data;
	comp->length += 1;
}

void* vec_get(struct vec_t from, int item) {
	if ((item > from.length) || item < 0) {
		return INDEX_OUT_OF_BOUNDS_ERR(item, from.length);
	}

	return from.ptr[item];
}

void vec_remove(struct vec_t* comp, int item) {
	if (comp->length == 0) return;

	if (comp->length > item) {
		for (int cursor = item; cursor < comp->length; cursor++) {
			if ((cursor + 1) >= comp->length) {
				continue;
			}

			void* p = comp->ptr[cursor];
			void* n = comp->ptr[cursor + 1];

			comp->ptr[cursor] = n;
			n--;
			comp->ptr[cursor + 1] = p;
		}
	}
	
	comp->length -= 1;
	comp->ptr[comp->length] = NULL;
	comp->ptr = reallocarray(comp->ptr, comp->length, sizeof(void*));

}

int main(void) {
	struct vec_t composite = vec_with_cap(24);

	vec_push(&composite, "this should print instead.");
	vec_push(&composite, "hello world");
	
	vec_remove(&composite, 0);
	printf("%s", vec_get(composite, 0));



	return 0;
}













