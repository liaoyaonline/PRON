#include <stdio.h>

int main()
{
    int n = 0;
    scanf("%d",&n);
    if(n <= 0)
        return -1;
    for(int i = 1; i < n;i ++)
    {
        for(int j = 1; j <= i; j++)
            printf("*");
        printf("\n");
    }
    return 0;
}

