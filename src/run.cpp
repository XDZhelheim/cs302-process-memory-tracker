#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <wait.h>

using namespace std;

int main(int argc, char *argv[])
{
    int pid = fork();
    if (!pid)
    {
        execvp(argv[1], argv + 1);
    }
    else
    {
        wait(NULL);
    }
    
}