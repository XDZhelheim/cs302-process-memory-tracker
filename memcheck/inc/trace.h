#ifndef TRACE_H
#define TRACE_H

#include <stddef.h>

#define MAX_STACK_TRACE 100

#ifdef __cplusplus
extern "C"
{
#endif
    char *get_local_time(void);

    class trace
    {
    private:
        void *back_trace[MAX_STACK_TRACE];
        char **symbols;
        int trace_size;
        char *trace_time;

    public:
        trace();

        void **get_back_trace();

        char **get_symbols();

        int get_trace_size();

        char *get_trace_time();

        ~trace();
    };

#ifdef __cplusplus
}
#endif

#endif
