#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    free(malloc(100));
    malloc(100);
    FILE *f = fopen("./Makefile", "rb");
    printf("1234\n");
    printf("%p\n", f);
    fflush(stdout);
    f = freopen("./cmake_install.cmake", "rb", f);
    printf("1234\n");
    printf("%p\n", f);
    fflush(stdout);
    // fclose(f);
    return 0;
}
