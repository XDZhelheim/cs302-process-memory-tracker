#ifndef MEMORY_NODE_H
#define MEMORY_NODE_H

#include <stddef.h>
#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif
    bool contains(const char *str, const char *s);

    class memory_node
    {
    private:
        trace *tr;
        void *ptr;
        size_t size;
        size_t block;

    public:
        memory_node(void *p, size_t s, size_t b);

        bool valid_memory_allocation();

        trace *get_trace();

        void *get_ptr();

        size_t get_size();

        size_t get_block();

        ~memory_node();
    };
    
#ifdef __cplusplus
}
#endif

#endif
