#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include "main_redirect.h"

extern int __open(const char *file, int oflag, ...);
extern int __close(int fd);

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

    log_enable(false);

    int fd = __open(file, oflag, mode);

    log_enable(true);

    if (fd != -1)
    {
        file_descriptor_log_record(ALLOCATE, fd, file);
    }

    return fd;
}

int close(int fd)
{
    log_enable(false);

    int r = __close(fd);

    log_enable(true);
    file_descriptor_log_record(RELEASE, fd, NULL);

    return r;
}