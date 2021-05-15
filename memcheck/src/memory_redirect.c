#include <stdlib.h>
#include "main_redirect.h"

extern void *__libc_malloc(size_t size);
extern void *__libc_realloc(void *ptr, size_t size);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void __libc_free(void *ptr);

void *malloc(size_t size)
{
    void *ptr = __libc_malloc(size);

    memory_log_record(ALLOCATE, ptr, size, 1);

    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *newptr = __libc_realloc(ptr, size);

    memory_log_record(RELEASE, ptr, 0, 0);
    memory_log_record(ALLOCATE, newptr, size, 1);

    return newptr;
}

void *calloc(size_t nmemb, size_t size)
{
    void *ptr = __libc_calloc(nmemb, size);

    memory_log_record(ALLOCATE, ptr, size, nmemb);

    return ptr;
}

void free(void *ptr)
{
    if (ptr)
    {
        memory_log_record(RELEASE, ptr, 0, 0);
    }

    __libc_free(ptr);
}
