#include "redirect.h"

void __attribute__((constructor)) nu_init(void)
{
    fprintf(stderr, "start\n");
}

void __attribute__((destructor)) nu_fini(void)
{
    fprintf(stderr, "finish\n");
}

extern void *__libc_malloc(size_t size);
extern void *__libc_realloc(void *ptr, size_t size);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void __libc_free(void *ptr);

void *malloc(size_t size)
{
    void *ptr = __libc_malloc(size);
    logger(ALLOCATE, ptr, size);
    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *newptr = __libc_realloc(ptr, size);
    logger(RELEASE, ptr, 0);
    logger(ALLOCATE, newptr, size);
    return newptr;
}

void *calloc(size_t nmemb, size_t size)
{
    void *ptr = __libc_calloc(nmemb, size);
    logger(ALLOCATE, ptr, nmemb * size);
    return ptr;
}

void free(void *ptr)
{
    logger(RELEASE, ptr, 0);
    __libc_free(ptr);
}