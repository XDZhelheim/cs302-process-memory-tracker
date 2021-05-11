#include "mem.h"
#include <stdio.h>

int main(void)
{
    void * a = malloc(100);
    void * b = calloc(1, 100);
    free(a);
    free(b);
}