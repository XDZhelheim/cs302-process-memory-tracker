#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    free(malloc(100));
    malloc(200);
    fclose(fopen("./Makefile", "r"));
    open("./test.c", O_RDONLY);
    return 0;
}