#include <cstring>
#include "memory_node.h"
#include <stdio.h>

#define PATTERN_SIZE 1

const char *pattern[PATTERN_SIZE] = {"_IO_file"};

bool contains(const char *str, const char *s)
{
    size_t strl = strlen(str);
    size_t sl = strlen(s);
    if (strl < sl)
    {
        return false;
    }
    for (size_t i = 0; i < strl - sl + 1; i++)
    {
        bool flag = true;
        for (size_t j = 0; j < sl; j++)
        {
            if (str[i + j] != s[j])
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            return true;
        }
    }
    return false;
}

memory_node::memory_node(void *p, size_t s, size_t b)
{
    tr = new trace;
    ptr = p;
    size = s;
    block = b;
}

bool memory_node::valid_memory_allocation()
{
    for (int i = 0; i < tr->get_trace_size(); i++)
    {
        for (int j = 0; j < PATTERN_SIZE; j++)
        {
            if (contains(tr->get_symbols()[i], pattern[j]))
            {
                return false;
            }
        }
    }
    return true;
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
