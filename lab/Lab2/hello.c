#include <stdio.h>

int main (int argc, char *argv[])
{
  int i, *p, count = 0;
  p = &count;

  printf("%d\n", argc);
  for(int i = 0; i < argc; i++)
    printf("%s\n", argv[i]);

  int test[10] = {1,2,3,4,5,6,7,8,9,10};
  for (int i = 0; i < 10; i++) {
  	printf("%d ", test[i]);
    printf("test[%d] ", i);
  }

  for (i = 0; i < 10; i++) {
  	(*p)++;	//Do you understand this line of code and all the other permutations of the operators? ;)
  }

  printf("Thanks for waddling through this program. Have a nice day.");
  return 0;
}
