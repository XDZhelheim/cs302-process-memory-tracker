#include <cstring>
#include "memory_node.h"
#include <stdio.h>

memory_node::memory_node(void *p, size_t s, size_t b)
{
    tr = new trace;
    ptr = p;
    size = s;
    block = b;
}

trace *memory_node::get_trace()
{
    return tr;
}

void *memory_node::get_ptr()
{
    return ptr;
}

size_t memory_node::get_size()
{
    return size;
}

size_t memory_node::get_block()
{
    return block;
}

memory_node::~memory_node()
{
    delete tr;
}
