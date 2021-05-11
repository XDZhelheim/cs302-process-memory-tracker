#include "redirect.h"

void __attribute__((constructor)) nu_init(void)
{
    logger_start();
}

void __attribute__((destructor)) nu_fini(void)
{
    logger_finish();
}

extern void *__libc_malloc(size_t size);
extern void *__libc_realloc(void *ptr, size_t size);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void __libc_free(void *ptr);

void *malloc(size_t size)
{
    void *ptr = __libc_malloc(size);
    logger_record(ALLOCATE, ptr, size, 1);
    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *newptr = __libc_realloc(ptr, size);
    logger_record(RELEASE, ptr, 0, 0);
    logger_record(ALLOCATE, newptr, size, 1);
    return newptr;
}

void *calloc(size_t nmemb, size_t size)
{
    void *ptr = __libc_calloc(nmemb, size);
    logger_record(ALLOCATE, ptr, size, nmemb);
    return ptr;
}

void free(void *ptr)
{
    logger_record(RELEASE, ptr, 0, 0);
    __libc_free(ptr);
}