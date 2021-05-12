#include "file_descriptor_node.h"

fdnode::fdnode(const char *n, int _fd)
{
    tr = new trace;
    name = n;
    fd = _fd;
}

trace *fdnode::get_trace()
{
    return tr;
}

const char *fdnode::get_name()
{
    return name;
}

int fdnode::get_fd()
{
    return fd;
}

fdnode::~fdnode()
{
    delete tr;
}