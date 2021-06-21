/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* GenerateCsv by Treedata.txt to csv                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200809 ��ʼ                                                   *
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

//�����������飬��ſ��ܵ����ԣ��������
vector<string> AttsData_tmp;
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>TableData_tmp;
//���ɭ�����Լ�
map<int,vector<string>> RanForest_Atts;
//���ɭ��������
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
//���������������ݼ�
void ReadSampleData();
void GrenerateCsv();
int main()
{
	//����
	ReadSampleData();
    GrenerateCsv();
    system("pause");
	return 0;
}
void GrenerateCsv()
{
    int NumSample_tmp;
    ofstream outfile;//csv�ļ�
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
    ifstream infile;//���������ļ�
    infile.open("Treedata.txt");
    string Line_tmp,string_tmp;//���ɱ���
    cout << "��ʼ��ȡ����: "<< endl;
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//����ȡ�����н�����ReadLine��
    while(ReadLine >> string_tmp)
        AttsData_tmp.push_back(string_tmp);
    cout << "��ȡ������Ŀ: " << endl;
    infile >> NumSample_tmp;
    getline(infile,Line_tmp);//ָ���������з�
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
    //��ȡ�������԰������
    cout << "������Լ���" << endl;
    for(int i = 0; i < AttsData_tmp.size();i++)
    {
        cout << AttsData_tmp[i] << "______" ;
    }
    cout << endl;
    //��ȡ������������
    cout << "�����������" << endl;
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
