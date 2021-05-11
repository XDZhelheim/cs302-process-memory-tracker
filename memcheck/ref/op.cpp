#include <iostream>
#include <iomanip>

using namespace std;

void *operator new(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);

    cout << "mem_allocate at " << hex << ptr << dec << " size " << setw(5) << dec << size;
    if (file != nullptr)
    {
        cout << " at file " << file << ":" << line;
    }
    cout << endl;

    return ptr;
}

void *operator new[](size_t size, const char *file, int line)
{
    void *ptr = malloc(size);

    cout << "arr_allocate at " << hex << ptr << " size " << setw(5) << dec << size;
    if (file != nullptr)
    {
        cout << " at file " << file << ":" << line;
    }
    cout << endl;

    return ptr;
}

void operator delete(void *ptr, size_t size)
{
    cout << "mem_release  at " << hex << ptr << " size " << setw(5) << size << endl;

    free(ptr);
}

void operator delete[](void *ptr)
{
    cout << "arr_release  at " << hex << ptr << dec << endl;

    free(ptr);
}

#define new new (__FILE__, __LINE__)

int main(void)
{
    int *a = new int;
    int *b = new int[8];
    delete a;
    delete[] b;
}