#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

#include "redirect.h"
#include "logger.h"

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

extern FILE *_IO_fopen(const char *filename, const char *modes);
extern int _IO_fclose(FILE *stream);

extern int __open(const char *file, int oflag, ...);
extern int __close(int fd);

void *malloc(size_t size)
{
    void *ptr = __libc_malloc(size);

    logger_mem_record(ALLOCATE, ptr, size, 1);

    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    void *newptr = __libc_realloc(ptr, size);

    logger_mem_record(RELEASE, ptr, 0, 0);
    logger_mem_record(ALLOCATE, newptr, size, 1);

    return newptr;
}

void *calloc(size_t nmemb, size_t size)
{
    void *ptr = __libc_calloc(nmemb, size);

    logger_mem_record(ALLOCATE, ptr, size, nmemb);

    return ptr;
}

void free(void *ptr)
{
    logger_mem_record(RELEASE, ptr, 0, 0);

    __libc_free(ptr);
}

FILE *fopen(const char *filename, const char *modes)
{
    logger_enable(false);

    FILE *f = _IO_fopen(filename, modes);

    logger_enable(true);

    if (f)
    {
        logger_fd_record(ALLOCATE, f, -1, filename);
    }

    return f;
}

int fclose(FILE *stream)
{
    int fd = fileno(stream);
    logger_enable(false);

    int r = _IO_fclose(stream);

    logger_enable(true);
    logger_fd_record(RELEASE, stream, fd, NULL);

    return r;
}

int open(const char *file, int oflag, ...)
{
    int mode = 0;

    if (__OPEN_NEEDS_MODE(oflag))
    {
        va_list arg;
        va_start(arg, oflag);
        mode = va_arg(arg, int);
        va_end(arg);
    }

    logger_enable(false);

    int fd = __open(file, oflag, mode);

    logger_enable(true);

    if (fd != -1)
    {
        logger_fd_record(ALLOCATE, NULL, fd, file);
    }

    return fd;
}

int close(int fd)
{
    logger_enable(false);

    int r = __close(fd);

    logger_enable(true);
    logger_fd_record(RELEASE, NULL, fd, NULL);

    return r;
}