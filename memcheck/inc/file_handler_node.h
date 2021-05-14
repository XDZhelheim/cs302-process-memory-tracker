#ifndef FILE_HANDLER_NODE_H
#define FILE_HANDLER_NODE_H

#include <stdio.h>

#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif

    class file_handler_node
    {
    private:
        trace *tr;
        const char *name;
        FILE *fh;
        int ftype;

    public:
        file_handler_node(const char *n, FILE *fh, int ftype);

        trace *get_trace();

        const char *get_name();

        FILE *get_fh();

        int get_ftype();

        ~file_handler_node();
    };

#ifdef __cplusplus
}
#endif

#endif