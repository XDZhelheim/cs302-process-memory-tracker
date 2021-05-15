#include <stdbool.h>
#include <stdio.h>
#include "main_redirect.h"
#include "file_handler_type.h"

extern FILE *_IO_fopen(const char *filename, const char *mode);
extern FILE *_IO_file_fopen(FILE *stream, const char *filename, const char *mode, int);
extern int _IO_file_close_it(FILE *stream);
extern int _IO_fclose(FILE *stream);

extern FILE *_IO_popen(const char *command, const char *nodes) __THROW;

FILE *fopen(const char *filename, const char *mode)
{
    log_enable(false);

    FILE *f = _IO_fopen(filename, mode);

    log_enable(true);

    if (f)
    {
        file_handler_log_record(ALLOCATE, f, filename, _FILE_);
    }

    return f;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    log_enable(false);

    _IO_file_close_it(stream);

    log_enable(true);

    if (stream)
    {
        file_handler_log_record(RELEASE, stream, NULL, _FILE_);
    }

    log_enable(false);

    FILE *f = _IO_file_fopen(stream, filename, mode, 1);

    log_enable(true);

    if (f)
    {
        file_handler_log_record(ALLOCATE, f, filename, _FILE_);
    }

    return f;
}

int fclose(FILE *stream)
{
    log_enable(false);

    int r = _IO_fclose(stream);

    log_enable(true);

    if (stream)
    {
        file_handler_log_record(RELEASE, stream, NULL, _FILE_);
    }

    return r;
}

FILE *popen(const char *command, const char *modes)
{
    log_enable(false);

    FILE *p = _IO_popen(command, modes);

    log_enable(true);

    if (p)
    {
        file_handler_log_record(ALLOCATE, p, command, _PIPE_);
    }

    return p;
}

int pclose(FILE *stream)
{
    log_enable(false);

    int r = _IO_fclose(stream);

    log_enable(true);

    if (stream)
    {
        file_handler_log_record(RELEASE, stream, NULL, _PIPE_);
    }

    return r;
}
