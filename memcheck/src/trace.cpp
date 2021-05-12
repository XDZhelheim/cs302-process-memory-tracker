#include <execinfo.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "trace.h"

char *get_local_time(void)
{
    char *timing = new char[35];

    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);

    char *curr = ctime(&t.tv_sec);
    curr[strlen(curr) - 1] = '\0';

    strncpy(timing, curr, 19);
    timing[19] = '.';
    sprintf(timing + 20, "%09ld", t.tv_nsec);
    strcpy(timing + 29, curr + 19);
    
    return timing;
}

trace::trace()
{
    trace_size = backtrace(back_trace, MAX_STACK_TRACE);
    symbols = backtrace_symbols(back_trace, trace_size);
    trace_time = get_local_time();
}

void **trace::get_back_trace()
{
    return back_trace;
}

char **trace::get_symbols()
{
    return symbols;
}

int trace::get_trace_size()
{
    return trace_size;
}

char *trace::get_trace_time()
{
    return trace_time;
}

trace::~trace()
{
    delete[] symbols;
    delete[] trace_time;
}