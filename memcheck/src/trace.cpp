#include <execinfo.h>

#include "trace.h"

trace::trace()
{
    trace_size = backtrace(back_trace, MAX_STACK_TRACE);
    symbols = backtrace_symbols(back_trace, trace_size);
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

trace::~trace()
{
    for (int i = 0; i < trace_size; i++)
    {
        delete[] symbols[i];
        delete[](unsigned long *) back_trace[i];
    }
    delete[] symbols;
}