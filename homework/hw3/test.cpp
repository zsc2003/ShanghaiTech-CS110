#include<cstdio>
#include<iostream>

int main()
{
    int a;
    long int b;
    unsigned int c;
    unsigned long int d;
    char c1;
    unsigned char c2;
    a = -3;
    b = a;
    c = a;
    d = a;
    c1 = a;
    c2 = a;
    printf("%d %ld %u %lu %d %u", a, b, c, d, c1, c2);
    // std::cout<<a<<' '<<b<<' '<<c<<' '<<d<<' '<<c1<<' '<<c2<<'\n';
    return 0;
}