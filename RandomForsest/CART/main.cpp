/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                          *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200709 实现CART                                             *
*                                                                           *
*                                                                           *
*                                                                           *
***************************************************************************/ 
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include "Tree_Node.h"
using namespace std;


//定义属性数组，存放可能的属性，包括类别
vector<string> data_Attributes;
//定义各属性对应的属性值
map<string,vector<string>> data_AttValues;
//定义剩余属性，不包括类别
vector<string> remain_Attributes;
//定义数据表，属性-属性值(全部数据的属性值放在同一个数组)
map<string,vector<string>> data_Table;





void data_Input();
void Traversal(Tree_Node* root);
void CheckTree(Tree_Node* root);
int main()
{
    //ifstream infile;
    //infile.open("check.txt");
    data_Input();
    //Tree_Node myTree(data_Table,data_Attributes,data_AttValues,remain_Attributes);
    //myTree.generate_ChildNode();
    //Tree_Node *test = &myTree;
    //Traversal(test);
    //CheckTree(test);
    return 0;
}
void data_Input()
{
    int N;
    ifstream infile;
    infile.open("test.txt");
    string input_Line,temp_Attributes;
    cout << "请输入属性" << endl;
    getline(infile,input_Line);
    cout << "属性：\n" << input_Line << endl;
    istringstream input_Attributes(input_Line);
    while(input_Attributes >> temp_Attributes)
        data_Attributes.push_back(temp_Attributes);
    for(auto it = data_Attributes.begin();it != data_Attributes.end();it++)
        cout << *it << " ";
    cout << "请输入样本数目" << endl;
    infile >> N;
    cout << N << endl;
    getline(infile,input_Line);
    cout << "请输入样本：" << endl;
    input_Line ="";
    temp_Attributes = "";
    for(int i = 0; i < N; i++)
    {
        getline(infile,input_Line);
        cout << input_Line << endl;
        istringstream input_Attributesc(input_Line);
        for(int i = 0; i < data_Attributes.size();i++)
        {
            input_Attributesc >> temp_Attributes;
            data_Table[data_Attributes[i]].push_back(temp_Attributes);
            cout << data_Attributes[i] << temp_Attributes;
        }
        cout << endl;
    }
    for(auto iter = data_Table.rbegin();iter != data_Table.rend();iter++)
    {
        cout << iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
    cout << "hahahahahs" << endl;
    //将属性和属性里面的可能值存入data_AttValues
    for(auto iter = data_Table.rbegin();iter != data_Table.rend();iter++)
    {
        set<string> attValues;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            attValues.insert(*it);
        for(auto it=attValues.begin();it != attValues.end();it++)
            data_AttValues[iter->first].push_back(*it);
        //cout << endl;
    }
    cout << " hasdjflskjfksljf" << endl;
    for(auto iter = data_AttValues.rbegin();iter != data_AttValues.rend();iter++)
    {
        cout << iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
}
void Traversal(Tree_Node* root)
{
    vector<Tree_Node*> test;
    Tree_Node *k;
    Tree_Node *knext;
    test.push_back(root);
    while(test.size() > 0)
    {
        k = test.front();
        cout << "node_Attribute" << k->node_Attribute << endl;
        for(auto it = k->childNode.rbegin();it != k->childNode.rend();it++)
        {
            knext = it->second;
            cout << "当前值:" << it->first << "->子节点:" << knext->node_Attribute<< endl;
            test.push_back(it->second);
            
        }
        auto det = test.begin();
        test.erase(det);
    }
}
void CheckTree(Tree_Node* root)
{
    float num = 0.0;
    ifstream infile;
    infile.open("check.txt");
    string testresult;
    string answer;
    float ResultRate;
    float goodresultnum = 0.0;
    vector<string> check_attributes;
    string input_Line,temp_Attributes;
    cout << "开始读取数据，进行计算决策树成功率" << endl;
    infile >> num;
    cout << "读取数据成功: " << num << endl;
    getline(infile,input_Line);
    cout << "开始读取检测样本" << endl;
    for(int i = 0; i < num; i++)
    {
        getline(infile,input_Line);
        cout << input_Line << endl;
        istringstream input_Attributesc(input_Line);
        for(int i = 0; i < data_Attributes.size();i++)
        {
            input_Attributesc >> temp_Attributes;
            check_attributes.push_back(temp_Attributes);
        }
        for(auto it= check_attributes.begin();it != check_attributes.end();it++)
            cout << *it << " ";
        cout << endl;
        answer = check_attributes[6];
        testresult = root -> findClass(check_attributes);
        cout << "答案是：" << answer << "检测结果是: "<< testresult << endl;
        if(answer == testresult)
            goodresultnum ++;
        check_attributes.clear();
    }
    cout << "goodresult :" << goodresultnum << "num" << num << endl;
    ResultRate = (float)(goodresultnum / num);
    cout << " 检测成功率:"<< ResultRate << endl;
}
