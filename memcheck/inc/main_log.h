#ifndef MAIN_LOG_H
#define MAIN_LOG_H

#include <stdio.h>
#include <stdlib.h>

#define ALLOCATE 1
#define RELEASE 0

extern FILE *log_file;

#ifdef __cplusplus
extern "C"
{
#endif
    char *get_local_time(void);

    int log_enabled(void);

    void log_enable(int en);

    void log_start(void);

    void log_finish(void);

    void memory_log_init(void);

    void memory_log_record(int type, void *ptr, size_t size, size_t block);

    void memory_log_finish(void);

    void file_handler_log_init(void);

    void file_handler_log_record(int type, FILE *f, const char *filename);

    void file_handler_log_finish(void);
    
    void file_descriptor_log_init(void);

    void file_descriptor_log_record(int type, int fd, const char *filename);

    void file_descriptor_log_finish(void);

#ifdef __cplusplus
}
#endif

#endif