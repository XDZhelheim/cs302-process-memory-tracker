int main()
{
    int *a = new int[100];
    int *b = new int;
    delete b;
    delete[] a;
}