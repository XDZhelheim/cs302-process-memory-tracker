#include <stdlib.h>

int main(void)
{
    free(malloc(100));
    return 0;
}

// LD_PRELOAD=`pwd`/libredirect.so ./test