#ifndef MEMNODE_H
#define MEMNODE_H

#include <stddef.h>

#define MAX_STACK_TRACE 20

#ifdef __cplusplus
extern "C"
{
#endif
    class memnode
    {
    private:
        void *back_trace[MAX_STACK_TRACE];
        char **symbols;
        int trace_size;
        void *ptr;
        size_t size;
        size_t block;

    public:
        memnode(void *p, size_t s, size_t b);

        void set_back_trace(char **s, int t);

        void **get_back_trace();

        char **get_symbols();

        int get_trace_size();

        void *get_ptr();

        size_t get_size();

        size_t get_block();

        ~memnode();
    };
#ifdef __cplusplus
}
#endif

#endif