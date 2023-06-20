#include<stdio.h>
#include<string.h>

char *s = "abbbbb";
// char *s = "abbddbbb";
// char *s = "abbsdcds";

int max(int a,int b)
{
    return a>=b?a:b;
}
int LPS()
{
    int ans = 1;
    int len = strlen(s);
    for(int i = 1; i < len - 1; ++i) // the center of the palindromic substring
    {
        if(s[i] == s[i+1]) // even length
        {
            int num = 2;
            for(int offset = 1;;++offset)
            {
                if(i - offset < 0)
                    break;
                if(i + offset + 1 >= len)
                    break;
                if(s[i - offset] != s[i + offset + 1])
                    break;
                num+=2;
            }
            ans = max(ans, num);            
        }
        if(s[i-1] == s[i+1]) // odd length
        {
            int num = 3;
            for(int offset = 2;;++offset)
            {
                if(i - offset < 0)
                    break;
                if(i + offset >= len)
                    break;
                if(s[i - offset] != s[i + offset])
                    break;
                num+=2;
            }
            ans = max(ans, num);            
        }
    }
    return ans;
}

int main()
{
    printf("%d\n", LPS());
    return 0;
}