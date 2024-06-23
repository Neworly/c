#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Data
{
        struct Pair **values;


        size_t _length;
        size_t _capacity;
};

struct Pair
{
        char *k;
        void *v;
};

struct HashTable
{
        struct Data _bucket;

        void *(*insert)(struct Data*, char *const, void*);
        void *(*search)(struct Data*, char *const);
};

#define TABLE_CAPACITY_OFFSET 2
#define TABLE_BASE_CAPACITY 10
#define TABLE_GROWTH_CAPACITY 12


void *push (struct Data*, char*const, void*);
void deconstruct_bucket (struct Data*);

void rehash (struct Data *bucket)
{
        size_t length = bucket->_length, *capacity = &(bucket->_capacity);
        size_t max = *capacity;

        size_t lfactor = length / max;
        if (length >= max
                || lfactor*max == max
                || length < (lfactor*max/4)
        ) {
                size_t oldcap = max;
                *capacity += TABLE_GROWTH_CAPACITY;


                struct Data *oldb = bucket;
                struct Pair **oldv = oldb->values;

                bucket->values = calloc(*capacity, sizeof(void*));
                bucket->_length = 0;

                for (size_t i = 0; i < oldcap; i++) {
                        if (oldv[i] != NULL) {
                                push(bucket, oldv[i]->k, oldv[i]->v);
                        }
                }

                deconstruct_bucket (oldb);
        }
}

struct Data construct_bucket ()
{

        struct Data it = { ._length = 0, ._capacity = TABLE_BASE_CAPACITY };

        if (!(it.values = calloc(TABLE_BASE_CAPACITY, sizeof(struct Pair*)))) exit(0);

        return it;
}

void deconstruct_bucket (struct Data *bucket)
{
        for (size_t i = 0; i < bucket->_capacity; i++) {
                if (bucket->values[i]) free(bucket->values[i]);
        }

        free(bucket->values);
}

size_t collide (size_t hashc, struct Data *bucket)
{
        size_t loffset = 1, roffset = 1;

        while (bucket->values[hashc]) {
                size_t l = (hashc - loffset) % bucket->_capacity, r = (hashc + roffset) % bucket->_capacity;

                if (!bucket->values[l]) {
                        hashc = l;
                        break;
                } else if (!bucket->values[r]) {
                        hashc = r;
                        break;
                }

                (loffset += loffset + 1, roffset += 1);
        }

        return hashc;
}

size_t bytestrton (char *str)
{
        size_t size = 0;
        for (char x = *str; x; (x = *(++str))) {
                size += x;
        }

        return size;
}

size_t hashf (char *key, struct Data *bucket)
{
        return bytestrton(key) % bucket->_capacity;
}

void *push (struct Data *bucket, char *const key, void *value)
{
        size_t hashcode = collide(hashf(key, bucket), bucket);

        struct Pair **this = &(*((bucket->values) + hashcode));
        *this = malloc(sizeof(struct Pair));
        ((*this)->k = key, (*this)->v = value);

        bucket->_length += 1;
        rehash(bucket);

        return NULL;
}

void *get (struct Data *bucket, char *const key)
{
        size_t hashcode = hashf(key, bucket);
        for (;;) {
                if (bucket->values[hashcode] && bytestrton((*(bucket->values + hashcode))->k) == bytestrton(key)) {
                        return (*(bucket->values + hashcode))->v;
                }
                hashcode = (hashcode + 1) % bucket->_capacity;
        }

        return NULL;
}

void __display_bucket (struct Data *bucket)
{
        size_t capacity = bucket->_capacity;
        size_t length = bucket->_length;

        puts("");
        for (size_t i = 0; i < capacity; i++)
        {
                printf("[%zu] -> %p\n", i, bucket->values[i]);
        }
}

int main (void)
{
        struct HashTable table = {
                ._bucket = construct_bucket(),
                .insert = push,
                .search = get,

        };


        table.insert(&table._bucket, "Hello", "World!");

        __display_bucket(&table._bucket);


        printf("%s", (char*) table.search(&table._bucket, "Hello"));

        return 0;
}
