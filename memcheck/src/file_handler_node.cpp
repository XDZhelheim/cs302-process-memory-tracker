#include "file_handler_node.h"

file_handler_node::file_handler_node(const char *n, FILE *_fh)
{
    tr = new trace;
    name = n;
    fh = _fh;
}

trace *file_handler_node::get_trace()
{
    return tr;
}

const char *file_handler_node::get_name()
{
    return name;
}

FILE *file_handler_node::get_fh()
{
    return fh;
}

file_handler_node::~file_handler_node()
{
    delete tr;
}