#include <stdio.h>

int main()
{
    int numdata[8] = {1,2,3,4,5,6,7,8};
    int num[56][3];
    int n = 0;
    for(int i = numdata[0] ;i < numdata[6];i++)
        for(int j = i+1; j < numdata[7];j++)
            for(int k = j+1; k <= numdata[7]; k++)
            {
                num[n][0] = i;
                num[n][1] = j;
                num[n][2] = k;
                n++;
            }
    for(int i = 0 ; i < 56; i++)
    {
        printf("%d%d%d\n",num[i][0],num[i][1],num[i][2]);
    }
    return 0;
}

