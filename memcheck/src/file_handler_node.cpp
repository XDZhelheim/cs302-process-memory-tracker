#include "file_handler_node.h"

file_handler_node::file_handler_node(const char *n, FILE *_fh_, int _ftype_)
{
    tr = new trace;
    name = n;
    fh = _fh_;
    ftype = _ftype_;
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

int file_handler_node::get_ftype()
{
    return ftype;
}

file_handler_node::~file_handler_node()
{
    delete tr;
}