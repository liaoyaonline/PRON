#include <stdio.h>
#include <ctype.h>
void str1(char *p);
int main()
{
    char str[100];
    scanf("%[^\n]",str);
    printf("%s\n",str);
    str1(str);
    printf("%s\n",str);
    return 0;
}
void str1(char *p)
{
    for(;*p;p++)
        if(*p == ' ')
        {
            *(p-1) = toupper(*(p-1));
        }
}
