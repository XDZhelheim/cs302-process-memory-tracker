int main(void)
{
    int **m;
    m = new int *[4];
    for (int i = 0; i < 4; i++)
    {
        m[i] = new int[4];
    }
}
