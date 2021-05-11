#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define ALLOCATE 1
#define RELEASE 0

#ifdef __cplusplus
extern "C"
{
#endif
    void logger_start(void);

    void logger_record(int type, void *ptr, size_t size, size_t block);

    void logger_finish(void);

#ifdef __cplusplus
}
#endif

#endif