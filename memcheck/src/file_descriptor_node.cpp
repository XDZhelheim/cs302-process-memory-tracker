#include "file_descriptor_node.h"

file_descriptor_node::file_descriptor_node(const char *n, int _fd)
{
    tr = new trace;
    name = n;
    fd = _fd;
}

trace *file_descriptor_node::get_trace()
{
    return tr;
}

const char *file_descriptor_node::get_name()
{
    return name;
}

int file_descriptor_node::get_fd()
{
    return fd;
}

file_descriptor_node::~file_descriptor_node()
{
    delete tr;
}