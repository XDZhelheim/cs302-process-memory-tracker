#include "logger.h"
#include <map>

bool enabled = false;

void logger_start(void)
{
    enabled = true;
}

void logger_record(int type, void *ptr, size_t size)
{
    if (enabled)
    {
        if (type)
        {
            fprintf(stderr, "Allocate memory at %p, size %ld\n", ptr, size);
        }
        else
        {
            fprintf(stderr, "Release  memory at %p\n", ptr);
        }
    }
}

void logger_finish(void)
{
    enabled = false;
}