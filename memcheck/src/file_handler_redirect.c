#include <stdbool.h>
#include <stdio.h>
#include "main_redirect.h"

extern FILE *_IO_fopen(const char *filename, const char *mode);
extern FILE* _IO_file_fopen (FILE *stream, const char *filename, const char *mode, int);
extern int _IO_file_close_it (FILE *stream);
extern int _IO_fclose(FILE *stream);

FILE *fopen(const char *filename, const char *mode)
{
    log_enable(false);

    FILE *f = _IO_fopen(filename, mode);

    log_enable(true);

    if (f)
    {
        file_handler_log_record(ALLOCATE, f, filename);
    }

    return f;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    log_enable(false);

    _IO_file_close_it(stream);

    log_enable(true);

    file_handler_log_record(RELEASE, stream, NULL);

    log_enable(false);

    FILE *f = _IO_file_fopen(stream, filename, mode, 1);

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