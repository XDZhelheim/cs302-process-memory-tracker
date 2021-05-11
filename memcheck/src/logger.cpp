#include "logger.h"

void logger(int type, void *ptr, size_t size)
{
    if (type)
    {
        fprintf(stderr, "Allocate memory at %16p, size %ld\n", ptr, size);
    }
    else
    {
        fprintf(stderr, "Release  memory at %16p\n", ptr);
    }
}