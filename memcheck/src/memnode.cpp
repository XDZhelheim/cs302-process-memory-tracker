#include "memnode.h"

memnode::memnode(void *p, size_t s, size_t b)
{
    tr = new trace;
    ptr = p;
    size = s;
    block = b;
}

trace *memnode::get_trace()
{
    return tr;
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
    delete tr;
}