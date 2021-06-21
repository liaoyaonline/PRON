#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include <math.h>
#include <algorithm>
using namespace std;
void RemoveFeatures_WithTheSameValue(map<string, vector<string>>& temp_Table, vector<string>& temp_Attribute);
int main()
{
    vector<string> AttDataTable_tmp;
    AttDataTable_tmp.push_back("Quartile");
    AttDataTable_tmp.push_back("Median");
    AttDataTable_tmp.push_back("AddListCount");
    AttDataTable_tmp.push_back("Class");
    map<string,vector<string>> Table_Data;
    vector<string> data_tmp;
    data_tmp.push_back("4.0");
    data_tmp.push_back("4.0");
    data_tmp.push_back("4.0");
    data_tmp.push_back("4.0");
    Table_Data["Quartile"] = data_tmp;
    data_tmp.clear();
    data_tmp.push_back("6.0");
    data_tmp.push_back("6.0");
    data_tmp.push_back("6.0");
    data_tmp.push_back("6.0");
    Table_Data["Median"] = data_tmp;
    data_tmp.clear();
    data_tmp.push_back("53");
    data_tmp.push_back("57");
    data_tmp.push_back("53");
    data_tmp.push_back("57");
    Table_Data["AddListCount"] = data_tmp;
    data_tmp.clear();
    data_tmp.push_back("bad");
    data_tmp.push_back("bad");
    data_tmp.push_back("good");
    data_tmp.push_back("good");
    Table_Data["Class"] = data_tmp;
    for(int i = 0; i < AttDataTable_tmp.size(); i++)
        cout << AttDataTable_tmp[i] << " ";
    cout << endl;
    for(auto iter = Table_Data.rbegin(); iter != Table_Data.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = iter->second.begin(); it != iter->second.end(); it++)
            cout << *it << " ";
        cout << endl;
    }
    cout << endl;
    RemoveFeatures_WithTheSameValue(Table_Data,AttDataTable_tmp);
    cout <<"删除后" << endl;
    for(int i = 0; i < AttDataTable_tmp.size(); i++)
        cout << AttDataTable_tmp[i] << " ";
    cout << endl;
    for(auto iter = Table_Data.rbegin(); iter != Table_Data.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = iter->second.begin(); it != iter->second.end(); it++)
            cout << *it << " ";
        cout << endl;
    }

}
void RemoveFeatures_WithTheSameValue(map<string, vector<string>>& temp_Table, vector<string>& temp_Attribute)
{
    vector<string> AttDataTable_tmp;//去除类别后的属性集合
    AttDataTable_tmp = temp_Attribute;
    AttDataTable_tmp.pop_back();
    string Class_tmp = temp_Attribute[temp_Attribute.size() - 1];
    for (auto it = AttDataTable_tmp.begin(); it != AttDataTable_tmp.end();)//移除特征值相同的特征
    {
        bool AttValues_AllSame = true;
        vector<string> AttValues_temp;
        AttValues_temp = temp_Table[*it];
        for(int j = 1; j < AttValues_temp.size(); j++)
        {
            if(AttValues_temp[0] != AttValues_temp[j])
            {
                AttValues_AllSame = false;
                break;
            }
        }
        if(AttValues_AllSame == true)
        {
            cout <<*it << endl;
            AttDataTable_tmp.erase(it);
        }
        else
            it++;
    }
    AttDataTable_tmp.push_back(Class_tmp);
    temp_Attribute = AttDataTable_tmp;
    map<string, vector<string>> Table_tmp;
    for(int i = 0; i < temp_Attribute.size(); i++)
    {
        string Feature_tmp = temp_Attribute[i];
        Table_tmp[Feature_tmp] = temp_Table[Feature_tmp];
    }
    temp_Table = Table_tmp;
}
