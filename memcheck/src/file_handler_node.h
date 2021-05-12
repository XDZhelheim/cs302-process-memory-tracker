#ifndef FHNODE_H
#define FHNODE_H

#include <stdio.h>

#include "trace.h"

#ifdef __cplusplus
extern "C"
{
#endif

    class fhnode
    {
    private:
        trace *tr;
        const char *name;
        FILE *fh;

    public:
        fhnode(const char *n, FILE *fh);

        trace *get_trace();

        const char *get_name();

        FILE *get_fh();

        ~fhnode();
    };

#ifdef __cplusplus
}
#endif

#endif