#include <iostream>

using namespace std;

template <typename T>
class line
{
private:
    size_t size;
    T *arr;

public:
    line(size_t s = 10)
    {
        size = s;
        arr = new T[s];
    }

    ~line()
    {
        delete[] arr;
    }
};

int main()
{
    line<double> l(100);
    return 0;
}