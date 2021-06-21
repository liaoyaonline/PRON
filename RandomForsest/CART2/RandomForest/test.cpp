/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* GenerateCsv by Treedata.txt to csv                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200809 开始                                                   *
*                                                                           *
*                                                                           *
*                                                                           *
***************************************************************************/ 
#include <iostream>
#include <cstdlib>
#include <ctime>
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
#include "Tree_Node.h"
using namespace std;

//定义属性数组，存放可能的属性，包括类别
vector<string> AttsData_tmp;
//定义数据表,属性-属性值（全部数据的属性值放在同一个数组）
map<string, vector<string>>TableData_tmp;
//随机森林属性集
map<int,vector<string>> RanForest_Atts;
//随机森林样本集
map<int,map<string,vector<string>>> data_TableRanForest;
typedef struct node{
    string node_Attribute;
    Tree_Node *p;
    Tree_Node *ftp;
    int level;
} childtree;
int attribute_Num;
double max_Gaint;
Tree_Node Tree[56];
Tree_Node * test;
//输入数据生成数据集
void ReadSampleData();
void GrenerateCsv();
int main()
{
	//输入
	ReadSampleData();
    GrenerateCsv();
    system("pause");
	return 0;
}
void GrenerateCsv()
{
    int NumSample_tmp;
    ofstream outfile;//csv文件
    outfile.open("TreeData.csv");
    for(int i = 0; i < TableData_tmp[AttsData_tmp[0]].size();i++)
    {
        outfile << TableData_tmp[AttsData_tmp[0]][i] << ","<<TableData_tmp[AttsData_tmp[1]][i] << "," <<"\"" <<TableData_tmp[AttsData_tmp[2]][i] << "\"" << endl;
    }
    outfile.close();
}
void ReadSampleData()
{
    int NumSample_tmp;
    ifstream infile;//样本数据文件
    infile.open("Treedata.txt");
    string Line_tmp,string_tmp;//过渡变量
    cout << "开始读取属性: "<< endl;
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//将读取到的行解析到ReadLine上
    while(ReadLine >> string_tmp)
        AttsData_tmp.push_back(string_tmp);
    cout << "读取样本数目: " << endl;
    infile >> NumSample_tmp;
    getline(infile,Line_tmp);//指针跳过换行符
    for(int i = 0; i < NumSample_tmp; i++)
    {
        getline(infile,Line_tmp);
        istringstream ReadLine(Line_tmp);
        for(int i = 0; i < AttsData_tmp.size(); i++)
        {
            ReadLine >> string_tmp;
            TableData_tmp[AttsData_tmp[i]].push_back(string_tmp);
        }
    }
    //读取所有属性包括类别
    cout << "输出属性集合" << endl;
    for(int i = 0; i < AttsData_tmp.size();i++)
    {
        cout << AttsData_tmp[i] << "______" ;
    }
    cout << endl;
    //读取所有样本集合
    cout << "输出样本集合" << endl;
    for(auto iter = TableData_tmp.rbegin(); iter != TableData_tmp.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
        {
            cout << *it << "-----";
        }
        cout << endl;
    }
}
