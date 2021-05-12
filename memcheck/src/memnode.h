#ifndef MEMNODE_H
#define MEMNODE_H

#include <stddef.h>
#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif
    class memnode
    {
    private:
        trace *tr;
        void *ptr;
        size_t size;
        size_t block;

    public:
        memnode(void *p, size_t s, size_t b);

        trace *get_trace();

        void *get_ptr();

        size_t get_size();

        size_t get_block();

        ~memnode();
    };
#ifdef __cplusplus
}
#endif

#endif