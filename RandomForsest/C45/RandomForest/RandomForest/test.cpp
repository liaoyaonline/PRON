#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    int num[100];
    for(int i = 0; i < 100; i++)
        num[i] = i+1;
    for(int i = 0; i < 100; i++)
        printf("%d  ",num[i]);
    srand((int)time(0));  // 产生随机种子  把0换成NULL也行
    for (int i = 0; i < 100; i++)
    {
        int temp = rand()%10;
        if(temp == 2)
            num[i] = 0;

    }
    cout << endl;
    for(int i = 0; i < 100; i++)
        printf("%d  ",num[i]);
    for (int i = 0; i < 100; i++)
    {
        int temp = rand()%10;
        if(temp == 2)
            num[i] = 233;
    }
    cout << endl;
    for(int i = 0; i < 100; i++)
        printf("%d  ",num[i]);
    return 0;
}
