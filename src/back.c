#include <stdio.h>
#include <execinfo.h>
#include <inttypes.h>

int main()
{
    void *back[20];
    int size = backtrace(back, 20);
    char ** strs = backtrace_symbols(back, size);
    for (int i = 0; i < size; i++)
    {
        printf("==%d==    at 0x%016x %s\n", 0, (unsigned long) back[i], strs[i]);
    }
    
}
// -rdynamic