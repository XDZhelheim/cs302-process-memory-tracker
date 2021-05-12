#define _POSIX_C_SOURCE 100
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    free(malloc(100));
    malloc(200);
    FILE *f = fopen("./Makefile", "rb");
    // f = freopen("./CMakeCache.txt", "rb", f);
    printf("1234%d\n",1);
    fclose(f);
    return 0;
}