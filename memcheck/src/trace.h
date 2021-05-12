#ifndef TRACE_H
#define TRACE_H

#include <stddef.h>

#define MAX_STACK_TRACE 30

#ifdef __cplusplus
extern "C"
{
#endif

    class trace
    {
    private:
        void *back_trace[MAX_STACK_TRACE];
        char **symbols;
        int trace_size;

    public:
        trace();

        void **get_back_trace();

        char **get_symbols();

        int get_trace_size();

        ~trace();
    };

#ifdef __cplusplus
}
#endif
#endif