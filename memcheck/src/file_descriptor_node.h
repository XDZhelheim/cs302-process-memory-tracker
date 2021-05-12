#ifndef FDNODE_H
#define FDNODE_H

#include <stdio.h>

#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif

    class fdnode
    {
    private:
        trace *tr;
        const char *name;
        int fd;

    public:
        fdnode(const char *n, int _fd);

        trace *get_trace();

        const char *get_name();

        int get_fd();

        ~fdnode();
    };

#ifdef __cplusplus
}
#endif

#endif