#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
        IT_ETBASE = 2,
};

struct Metadata
{
        enum SIGERR sig;

        unsigned long long index;
        int mode;

        void *data;

        unsigned int status: 1;

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



struct Metadata iter_create (char *p, size_t length)
{

        struct Metadata iter = __iter_create(p, length);
        enum SIGRECV reterr = sigrecv(iter.sig);
        if (reterr != IGNORE)
                sigwarn(iter.sig);

        iter.mode = 0;
        iter.status = 0;
        return iter;
}

int iter_checkflag (struct Metadata *from, int flag)
{
        return (from->mode & flag) == flag);
}

void iter_mode (struct Metadata *iter, int flag)
{
        if (iter_checkflag(iter, flag)) {
                return;
        }

        iter->mode = iter->mode | flag;
}

int __iter_update_mode (struct Metadata *iter)
{
        switch(iter->mode)
        {
                case IT_BTEND: {
                        iter->cursor.offset = iter->cursor.base;
                        return 0;
                }

                case IT_ETBASE: {
                        char *end = iter->cursor.end;
                        iter->cursor.end = iter->cursor.base;
                        iter->cursor.base = end;
                        iter->cursor.offset = end;
                        return 0;
                }

                default:
                        fprintf(stdout, "this or mixture of flags aren't supported.");
                        return -1;
        }


}

void *iter_next(struct Metadata *iter)
{
        if (iter->mode == 0) {
                printf("a flag hasn't been set yet, please set one 'iter_mode(it, flag)'");
                return NULL;
        }

        if (iter->sig != OKAY) return NULL;
        if ((iter->cursor.end - iter->cursor.offset) == 0) return NULL;

        switch(iter->mode)
        {
                case IT_BTEND:
                        iter->data = iter->cursor.offset;
                        iter->cursor.offset += 1;
                        break;

                case IT_ETBASE:
                        iter->cursor.offset -= 1;
                        iter->data = iter->cursor.offset;
                        break;

                default:
                        fprintf(stdout, "this or mixture of flags aren't supported.");
                        iter->cursor.offset = iter->cursor.base;
                        iter->status = 0;
                        iter->mode = 0;
                        return NULL;
        }

        return iter->data;
}

/*struct __tdatawrap
{
        struct Metadata *info;
        void *(*fn)(void *arg);
};*/

static void **__iter_mapcall(struct Metadata *iter, void* (*fmap)(void*))
{
        if (iter->status) return NULL;

        void **retval = NULL;
        size_t index = 0;

        void *data = NULL;

        iter->status = 1;

        while ((data = iter_next(iter)) != NULL) {
                void* retfn = fmap(data);
                if (retfn == NULL) continue;

                size_t size = index + 1;
                retval = realloc(retval, size);
                retval[index++] = retfn;
        }

        iter->status = 0;
        return retval;
}


void **iter_mapcall(struct Metadata *iter, int flag, void *(*map)(void *))
{
        iter->mode = (iter->mode & 0x0) | flag;

        if (__iter_update_mode(iter) == -1)
                return NULL;

        return __iter_mapcall(iter, map);
}


void *printeach_char(void *i)
{
        printf("%c\n", *(char*)i);
        return "hello";
}

#define deref(data) *data
#define deref_as(data, type) (type*) ((deref(data)))

int main ()
{
        char str[] = {'a', 'b', 'c', 'd'};

        struct Metadata iter = iter_create(str, sizeof(str));
        void **data = iter_mapcall(&iter, IT_BTEND, printeach_char);
        printf("%p", *data);
        return 0;
}
