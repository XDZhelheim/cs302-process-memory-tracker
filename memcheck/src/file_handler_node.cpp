#include "file_handler_node.h"

fhnode::fhnode(const char *n, FILE *_fh)
{
    tr = new trace;
    name = n;
    fh = _fh;
}

trace *fhnode::get_trace()
{
    return tr;
}

const char *fhnode::get_name()
{
    return name;
}

FILE *fhnode::get_fh()
{
    return fh;
}

fhnode::~fhnode()
{
    delete tr;
}