#include "memnode.h"

memnode::memnode(void *p, size_t s, size_t b)
{
    ptr = p;
    size = s;
    block = b;
}

void memnode::set_back_trace(char **s, int t)
{
    symbols = s;
    trace_size = t;
}

void **memnode::get_back_trace()
{
    return back_trace;
}

char **memnode::get_symbols()
{
    return symbols;
}

int memnode::get_trace_size()
{
    return trace_size;
}

void *memnode::get_ptr()
{
    return ptr;
}

size_t memnode::get_size()
{
    return size;
}

size_t memnode::get_block()
{
    return block;
}

memnode::~memnode()
{
    for (int i = 0; i < trace_size; i++)
    {
        delete[] symbols[i];
        delete[](unsigned long *) back_trace[i];
    }
    delete[] symbols;
}