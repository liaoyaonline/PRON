#include <iostream>
#include <vector>
 
using namespace std;
 
int main()
{
    vector<int> a = {12, 23, 34, 45, 56, 67, 78, 89};
     
    auto iter = a.begin();
    srand((int)time(0));
    while (iter != a.end()) {
        int temp = rand() % 3;
        if (temp == 0) {
            iter = a.erase(iter);
        }
        else {
            ++iter;
        }
    }
     
    for (const auto &element : a) {
        cout<<element<<endl;
    }
     
    return 0;
}
