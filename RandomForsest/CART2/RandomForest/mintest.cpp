#include <iostream>
#include <string>
#include <ctime>
#include<algorithm>
#include <map>
#include <vector>
using namespace std;
int main()
{
    srand((int)time(0));
    vector<int> Array_tmp;
    for(int i = 0; i < 11; i++)
    {
        Array_tmp.push_back(i);
    }
    for(int i = 0; i < 11; i++)
        cout << Array_tmp[i] << "---";
    map<int, int> iMap;
    for(int i = 0; i < Array_tmp.size(); i++)
    {
        int Num_random = (rand() % 10000);
        iMap[i] = Num_random;
    }
    for(int i = 0; i < Array_tmp.size(); i++)
    {
        cout << i << ": " << iMap[i] << endl;
    }
    vector<pair<int, int>> vtMap;
    for (auto it = iMap.begin(); it != iMap.end(); it++)
        vtMap.push_back(make_pair(it->first, it->second));

    sort(vtMap.begin(), vtMap.end(),
        [](const pair<int, int> &x, const pair<int, int> &y) -> int {
        return x.second < y.second;
    });

    for (auto it = vtMap.begin(); it != vtMap.end(); it++)
        cout << it->first << ':' << it->second << '\n';
    vector<int> Array_result = Array_tmp;
    auto it = vtMap.begin();
    for(int i = 0; i < Array_result.size(); i++)
    {
        Array_result[i] = Array_tmp[it->first];
        it++;
    }
    cout << endl << endl;
    for(int i = 0; i < 11; i++)
    {
        cout << Array_result[i] << "----";
    }
    cout << endl;
    return 0;
}
