#include <stdio.h>
float compete(float a,float b);
float standard(float a,float b);
int main()
{
    float a,b,com,sta;
    scanf("%f,%f",&a,&b);
    com = compete(a,b);
    sta=standard(a,b);
    printf("compete= %.4f  nstandard = %.3f\n",com,sta);
    if(com > sta)
        printf("compete!\n");
    else
        printf("Standard!\n");
    return 0;
}
float compete(float a,float b)
{
    float c;
    c = (10* a) + (6*b);
    return c;
}
float standard(float a,float b)
{
    float s;
    s = (8*a) + (10*b);
    return s;
}
