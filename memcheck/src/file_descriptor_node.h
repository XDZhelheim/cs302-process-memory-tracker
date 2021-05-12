#ifndef FILE_DESCRIPTOR_NODE_H
#define FILE_DESCRIPTOR_NODE_H

#include <stdio.h>

#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif

    class file_descriptor_node
    {
    private:
        trace *tr;
        const char *name;
        int fd;

    public:
        file_descriptor_node(const char *n, int _fd);

        trace *get_trace();

        const char *get_name();

        int get_fd();

        ~file_descriptor_node();
    };

#ifdef __cplusplus
}
#endif

#endif