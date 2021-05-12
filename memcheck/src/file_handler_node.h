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

    public:
        file_handler_node(const char *n, FILE *fh);

        trace *get_trace();

        const char *get_name();

        FILE *get_fh();

        ~file_handler_node();
    };

#ifdef __cplusplus
}
#endif

#endif