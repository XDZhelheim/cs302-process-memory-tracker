int main()
{
    int *a = new int[100000];
    // delete  a;
    a[99999] = 100;
    // delete[] a;
}

// LD_PRELOAD=`pwd`/libredirect.so ./tt