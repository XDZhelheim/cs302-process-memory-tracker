#include <stdlib.h>

int main(void)
{
    int **m;
    m = (int **)calloc(4, sizeof(int *));
    for (int i = 0; i < 4; i++)
    {
        m[i] = (int *)calloc(4, sizeof(int));
    }
}
