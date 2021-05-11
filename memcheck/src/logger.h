#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define ALLOCATE 1
#define RELEASE 0

#ifdef __cplusplus
extern "C" {
#endif

void logger(int type, void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif