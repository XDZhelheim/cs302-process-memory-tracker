#include <stdlib.h>

int main(void)
{
    free(malloc(100));
    return 0;
}