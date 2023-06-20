#include<stdio.h>
#include<string.h>

void transfer(char* num_before, int from, int to)
{
    // transfer num_before(from system) into num_after(to system)
    char out[10];
    
    int mul = 1;
    int num = 0;
    int digit;
    for(int i = strlen(num_before) - 1; i >= 0; --i)
    {
        if('0' <= num_before[i] && num_before[i] <= '9')
        {
            digit = num_before[i] -'0';
        }
        else
        {
            digit = num_before[i] - 'A' + 10;
        }
        num += digit * mul;
        mul *= from;
    }


    int len = 0;
    while(num > 0)
    {
        digit = num % to;
        num /= to;
        if(0 <= digit && digit <= 9)
        {
            out[len] = digit + '0';
        }
        else
        {
            out[len] = digit + 'A' - 10;
        }
        ++len;
    }

    if (to != 2)
    {
        for(int i = len - 1; i >= 0; --i)
            putchar(out[i]);
        putchar('\n');
    }
    else
    {
        for(int i = len; i < 4; ++i)
        {
            out[i] = '0';
            ++len;
        }
        for(int i = len - 1; i >= 0; --i)
        {
            if(out[i] == '0')
            {
                printf("□");
            }
            else
            {
                printf("■");
            }
        }
        putchar('\n');
    }
    
    return;
}

int main()
{
    char input[100];
    
    int opt;
    scanf("%d",&opt);

    switch (opt)
    {
    case 1: // part1 output1
    {
        for(int i = 1; i <= 5; ++i)
        {
            scanf("%s", input);
            transfer(input, 2, 10);
        }
        break;
    }

    case 2: // part1 output2
    {
        for(int i = 1; i <= 5; ++i)
        {
            scanf("%s", input);
            transfer(input, 2, 16);
        }
        break;
    }
    
    case 3: // part2 output1
    {
        for(int i = 1; i <= 5; ++i)
        {
            scanf("%s", input);
            transfer(input, 10, 2);
        }
        break;
    }

    case 4: // part2 output2
    {
        for(int i = 1; i <= 5; ++i)
        {
            scanf("%s", input);
            transfer(input, 16, 2);
        }
        break;
    }

    case 5: // part3 output1
    {
        for(int i = 1; i <= 5; ++i)
        {
            scanf("%s", input);
            transfer(input, 2, 16);
        }
        break;
    }

    case 6: // part3 output2
    {
        for(int i = 1; i <= 4; ++i)
        {
            scanf("%s", input);
            transfer(input, 2, 16);
        }
        break;
    }

    default:
        break;
    }
    

    return 0;
}