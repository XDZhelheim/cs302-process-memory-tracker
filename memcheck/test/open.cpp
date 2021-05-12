#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <map>

using namespace std;

int main()
{
    map<int, int> a;
    a.erase(10);
    a.erase(10);
    a.erase(10);
}