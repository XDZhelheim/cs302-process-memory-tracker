#include <stdbool.h>
#include <stdio.h>
#include "main_redirect.h"

extern FILE *_IO_fopen(const char *filename, const char *modes);
extern int _IO_fclose(FILE *stream);

FILE *fopen(const char *filename, const char *modes)
{
    log_enable(false);

    FILE *f = _IO_fopen(filename, modes);

    log_enable(true);

    if (f)
    {
        file_handler_log_record(ALLOCATE, f, filename);
    }

    return f;
}

int fclose(FILE *stream)
{
    file_handler_log_record(RELEASE, stream, NULL);

    log_enable(false);

    int r = _IO_fclose(stream);

    log_enable(true);

    return r;
}