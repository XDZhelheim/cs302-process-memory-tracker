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
        FILE *file;
        const char *name;
        int fd;

    public:
        fdnode(FILE *f, const char *n, int _fd);

        trace *get_trace();

        FILE *get_file();

        const char *get_name();

        int get_fd();

        ~fdnode();
    };

#ifdef __cplusplus
}
#endif

#endif