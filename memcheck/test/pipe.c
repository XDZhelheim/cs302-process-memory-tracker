#include <stdio.h>

int main()
{
    pclose(popen("ls", "r"));
    fclose(fopen("Makefile", "rb"));
    return 0;
}