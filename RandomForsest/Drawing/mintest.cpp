#include <iostream>
using namespace std;
int main()
{
    for(int i = 768; i < 1001; i++)
    {
        int y;
        y = rand() % 30 + 285;
        double z = (double)y / 1000;
        cout << i << "," << z << endl;
    }
    return 0;
}

