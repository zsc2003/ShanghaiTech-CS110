#include<stdio.h>

int bitSigner(unsigned int field, unsigned int size)
{
    int sign = field & (1 << (size - 1));
    if(sign == 0)
        return (int)field;
    return (int)field - (1 << size);
    /* YOUR CODE HERE */
}

int main()
{

    while(1)
    {
        int n;
        scanf("%d", &n);
        printf("%#x\n", bitSigner((unsigned int)n, 8));
    }
    return 0;
}