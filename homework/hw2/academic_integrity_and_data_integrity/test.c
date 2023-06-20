#include<stdio.h>

int main()
{
	unsigned int a = 2;
	unsigned int b = (a << 32) >> 32 , c = a << 32, d = a >> 32;
	printf("%u %u %u\n", b, c, d);
	while(1)
	{
		unsigned int n;
		scanf("%u",&n);
		printf("%u\n",(n >> 32));
	}
	
	return 0;
}
