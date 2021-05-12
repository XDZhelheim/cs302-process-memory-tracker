#include "fdnode.h"

fdnode::fdnode(FILE *f, const char *n, int _fd)
{
    tr = new trace;
    file = f;
    name = n;
    fd = _fd;
}

trace *fdnode::get_trace()
{
    return tr;
}

FILE *fdnode::get_file()
{
    return file;
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