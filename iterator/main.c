#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

enum SIGERR {
        OKAY,
        INVS,
        BOENULL,
        NOFFISNULL,
        PEND,
        OFFBASE
};

enum ITMODES {
        IT_BTEND = 1,
};

struct Metadata
{
        enum SIGERR sig;

        unsigned long long index;
        int mode;

        void *data;

        pthread_t id;
        pthread_mutex_t key;

        struct {
                char *base;
                char *offset;
                char *end;
        } cursor;
};



static struct Metadata
__iter_create(char *__restrict const p, size_t width)
{
        struct Metadata iter;
        iter.index = 0;
        iter.sig = OKAY;

        if (p != NULL) {
                iter.cursor.base = p;
                iter.cursor.offset = p;
                if (width > 0)
                {
                        iter.cursor.end = p + width;
                } else {
                        iter.sig = INVS;
                }
        } else {
                iter.sig = BOENULL;
        }

        return iter;
}

enum SIGRECV {
        IGNORE = 1,
        CANRECV = 2,
        CANNOT = 0
};

enum SIGRECV sigrecv(enum SIGERR sig)
{
        if (sig == OKAY) return IGNORE;
        if (sig == INVS) return CANRECV;
        return CANNOT;
}

void sigwarn (enum SIGERR sig)
{
        switch(sig)
        {
                case BOENULL:
                        fprintf(stdout, "a valid memory address is required.\n");
                        break;

                case INVS:
                        fprintf(stdout, "please set an appropriate size.\n");
                        break;

                default:
                        break;
        }
}



struct Metadata iter_create(char *p, size_t length)
{
        struct Metadata iter = __iter_create(p, length);
        enum SIGRECV reterr = sigrecv(iter.sig);
        if (reterr != IGNORE)
                sigwarn(iter.sig);

        iter.mode = 0;
        return iter;
}

int iter_checkflag(struct Metadata *from, int flag)
{
        if ((from->mode & flag) == flag) {
                return 1;
        }

        return 0;
}

void iter_mode(struct Metadata *iter, int flag)
{
        if (iter_checkflag(iter, flag)) {
                return;
        }

        iter->mode = iter->mode | flag;
}

void *iter_next(struct Metadata *iter)
{
        if (iter->mode == 0) {
                printf("a flag hasn't been set yet, please set one 'iter_mode(it, flag)'");
                return NULL;
        }

        if (iter->sig != OKAY) return NULL;
        if ((iter->cursor.end - iter->cursor.offset) <= 0) return NULL;

        iter->data = iter->cursor.offset;
        iter->cursor.offset += 1;
        return iter->data;
}

struct __tdatawrap
{
        struct Metadata *info;
        void *(*fn)(void *arg);
};

static void *__iter_mapcall(void *arg)
{

        struct __tdatawrap *iterdata = arg;

        void **retval = NULL;
        size_t index = 0;


        void *data = NULL;

        pthread_mutex_lock(&iterdata->info->key);
        while ((data = iter_next(iterdata->info)) != NULL) {
                void* retfn = iterdata->fn(data);
                if (retfn == NULL) continue;

                size_t size = index + 1;
                retval = realloc(retval, size);
                retval[index++] = retfn;
        }

        return retval;
}


// TODO: (Marling) threads might fail, alert the user in case that happens,
// this implementation seems to be AS-Unsafe, handle signals accordingly.
void *iter_mapcall(struct Metadata *iter, int flag, void *(*map)(void *))
{
        iter->mode = (iter->mode & 0x0) | flag;

        pthread_mutex_init(&iter->key, NULL);
        pthread_create(&iter->id, NULL, __iter_mapcall, (void*) &(struct __tdatawrap) {
                .info = iter,
                .fn = map
        });


        void **retval = NULL;

        pthread_join(iter->id, retval);
        pthread_mutex_unlock(&iter->key);
        pthread_mutex_destroy(&iter->key);

        return retval;
}

void *printeach_char(void *i)
{
        printf("%c", *(char*)i);
        return NULL;
}

int main ()
{
        char str[] = {'a', 'b', 'c', 'd'};

        struct Metadata iter = iter_create(str, sizeof(str));

        iter_mapcall(&iter, IT_BTEND, printeach_char);

        return 0;
}
