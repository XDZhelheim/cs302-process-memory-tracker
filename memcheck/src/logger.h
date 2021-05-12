#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define ALLOCATE 1
#define RELEASE 0

#ifdef __cplusplus
extern "C"
{
#endif
    char *get_local_time(void);

    void logger_enable(int en);

    void logger_start(void);

    void logger_mem_record(int type, void *ptr, size_t size, size_t block);

    void logger_fd_record(int type, FILE *f, int fd, const char *filename);

    void logger_finish(void);

    void logger_mem_finish(void);

    void logger_fd_finish(void);

#ifdef __cplusplus
}
#endif

#endif