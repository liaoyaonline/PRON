/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 实现C45下的随机森林，测试最优抉择
*       20200818 实现PermutationImportance，来对属性作用排序*
*                                                                           *
*                                                                           *
*                                                                           *
***************************************************************************/ 
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
#include "Tree_Node.h"
using namespace std;

//定义属性数组，存放可能的属性，包括类别
vector<string> AttsData_tmp;
//定义数据表,属性-属性值（全部数据的属性值放在同一个数组）
map<string, vector<string>>TableData_tmp;
map<string, vector<string>>Train_TableData;
map<string, vector<string>>Test_TableData;
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
void ReadSampleData(string NameDataFile);
void data_RanForsetInput();
double CheckTree(Tree_Node * root,map<string, vector<string>>TableData_tmp);
void PostPruning(Tree_Node * root);
void generate_ChildNode(Tree_Node temp_Table);
string findBestAtt(string con);
double calc_Entropy(map<string, vector<string>> temp_Table);
void TreeRep(Tree_Node * root);
void Bootstrap_Sampling();
void Save_BootstrapSample();
vector<string> Shuffle_ArrayOrder(vector<string> Array_tmp);
void Calcu_PermutationImportance();
void FindData();
int main()
{
	//输入
    string NameDataFile = "Treedata.txt";
	ReadSampleData(NameDataFile);
    //FindData();
    /*Bootstrap_Sampling();
    Save_BootstrapSample();
    TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    AttsData_tmp.clear();
    NameDataFile = "TrainData.txt";
    ReadSampleData(NameDataFile);//读取文件存入TableData_tmp
    cout << "开始生成决策树" << endl;
	*/Tree_Node myTree(TableData_tmp, AttsData_tmp);
	//进行训练
    test = &myTree;
    myTree.generate_ChildNode();
    myTree.PrintDecisionTree(test);
    myTree.GenerateDot(test);
    TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    AttsData_tmp.clear();
    NameDataFile = "TestData.txt";
    ReadSampleData(NameDataFile);//读取文件存入TableData_tmp
    CheckTree(test,TableData_tmp);
    //Calcu_PermutationImportance();
    system("pause");
	return 0;
}
void FindData()
{
    int NumSample_tmp = (int)TableData_tmp[AttsData_tmp[0]].size();
    map<string,vector<string>> data_tmp;
    for(int i = 0; i < NumSample_tmp; i++)
    {
        stringstream ss;
        double Petal_length;
        double Sepal_Width;
        ss << TableData_tmp[AttsData_tmp[2]][i];
        ss >> Petal_length;
        //ss << TableData_tmp[AttsData_tmp[1]][i];
        //ss >> Sepal_Width;
        if(Petal_length > 5.95 )
        {
            for (int m = 0; m < AttsData_tmp.size(); m++)
                data_tmp[AttsData_tmp[m]].push_back(TableData_tmp[AttsData_tmp[m]][i]);
        }
    }
    cout << "helloworld" << endl;
    for(auto iter = data_tmp.rbegin(); iter != data_tmp.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
        {
            cout << *it << "-----";
        }
        cout << endl;
    }
}
void ReadSampleData(string NameDataFile)
{
    int NumSample_tmp;
    ifstream infile;//样本数据文件
    infile.open(NameDataFile);
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
}
void Bootstrap_Sampling()
{
    int NumSample = (int)TableData_tmp[AttsData_tmp[0]].size();//样本数量
    map<int,int> Numsign_Data;
    vector<int> Numsign_TrainData;
    vector<int> Numsign_TestData;
    sleep(3);
    srand((int)time(0));
    for(int i = 0; i < NumSample; i++)//自助取样
    {
        int Num_random = (rand() % NumSample);
        Numsign_Data[Num_random] ++;
    }
    for(int i = 0; i < NumSample; i++)//将Train标号和Test标号分别存入
    {
        if(Numsign_Data[i] > 0)
            Numsign_TrainData.push_back(i);
        else
            Numsign_TestData.push_back(i);
    }
    for(int i = 0; i < Numsign_TrainData.size(); i++)//存入Train样本集
        for(int k = 0; k < AttsData_tmp.size(); k++)
            Train_TableData[AttsData_tmp[k]].push_back(TableData_tmp[AttsData_tmp[k]][Numsign_TrainData[i]]);
    for(int i = 0; i < Numsign_TestData.size(); i++)//存入Test样本集
        for(int k = 0; k < AttsData_tmp.size(); k++)
            Test_TableData[AttsData_tmp[k]].push_back(TableData_tmp[AttsData_tmp[k]][Numsign_TestData[i]]);
}
void Save_BootstrapSample()
{
    ofstream Train_outfile;
    ofstream Test_outfile;
    int Num_TrainSample;
    int Num_TestSample;
    Train_outfile.open("TrainData.txt");
    Test_outfile.open("TestData.txt");
    Train_outfile << AttsData_tmp[0];
    for(int i = 1; i < AttsData_tmp.size(); i++)
    {
        Train_outfile <<" "<< AttsData_tmp[i];
    }
    Train_outfile << endl;
    Num_TrainSample = (int)Train_TableData[AttsData_tmp[0]].size();
    Train_outfile << Num_TrainSample;
    for(int i = 0; i < Num_TrainSample; i++)
    {
        Train_outfile << endl;
        Train_outfile << Train_TableData[AttsData_tmp[0]][i];
        for(int m = 1; m < AttsData_tmp.size(); m++)
        {
            Train_outfile <<" "<< Train_TableData[AttsData_tmp[m]][i];
        }
    }
    Train_outfile.close();
    Test_outfile << AttsData_tmp[0];
    for(int i = 1; i < AttsData_tmp.size(); i++)
    {
        Test_outfile <<" "<< AttsData_tmp[i];
    }
    Test_outfile << endl;
    Num_TestSample = (int)Test_TableData[AttsData_tmp[0]].size();
    Test_outfile << Num_TestSample;
    for(int i = 0; i < Num_TestSample; i++)
    {
        Test_outfile << endl;
        Test_outfile << Test_TableData[AttsData_tmp[0]][i];
        for(int m = 1; m < AttsData_tmp.size(); m++)
        {
            Test_outfile <<" "<< Test_TableData[AttsData_tmp[m]][i];
        }
    }
    Test_outfile.close();
}
double CheckTree(Tree_Node * root,map<string, vector<string>>TableData_test)
{
    double VarianceValue = 0;
    string Result_Prediction;
    string Result_Correct;
    int NumSample_TestData = (int)TableData_test[AttsData_tmp[0]].size();
    for(int i = 0; i < NumSample_TestData; i++)
    {
        stringstream ss;
        double ResultCorrect_tmp;
        double ResultPrediction_tmp;
        map<string,string> maptest_tmp;
        for(int m = 0; m < (AttsData_tmp.size() - 1); m++)
        {
            maptest_tmp[AttsData_tmp[m]] = TableData_test[AttsData_tmp[m]][i];
        }
        int NumAtt_tmp = AttsData_tmp.size() - 1;
        Result_Correct = TableData_test[AttsData_tmp[NumAtt_tmp]][i];
        ss << Result_Correct;
        ss >> ResultCorrect_tmp;
        Result_Prediction = root->PredictionSampleClass(maptest_tmp);
        ss << Result_Prediction;
        ss >> ResultPrediction_tmp;
        VarianceValue += (ResultCorrect_tmp - ResultPrediction_tmp) * (ResultCorrect_tmp - ResultPrediction_tmp);
    }
    VarianceValue = VarianceValue / (double)NumSample_TestData;
    VarianceValue = sqrt(VarianceValue);
    cout << "测试样本数目: " << NumSample_TestData << " 测试样本方差: " << VarianceValue<< endl;
    return VarianceValue;
}
vector<string> Shuffle_ArrayOrder(vector<string> Array_tmp)
{
    int Num_Sleep = rand() % (10 -2) + 2;
    sleep(Num_Sleep);
    srand((int)time(0));
    map<int, int> NumSignMap_tmp;
    for(int i = 0; i < Array_tmp.size(); i++)
    {
        int Num_random = (rand() % 10000);
        NumSignMap_tmp[i] = Num_random;
    }
    vector<pair<int, int>> VectorMap_tmp;
    for (auto it = NumSignMap_tmp.begin(); it != NumSignMap_tmp.end(); it++)
        VectorMap_tmp.push_back(make_pair(it->first, it->second));
    sort(VectorMap_tmp.begin(), VectorMap_tmp.end(),
        [](const pair<int, int> &x, const pair<int, int> &y) -> int {
        return x.second < y.second;
    });
    vector<string> Array_result = Array_tmp;
    auto it = VectorMap_tmp.begin();
    for(int i = 0; i < Array_result.size(); i++)
    {
        Array_result[i] = Array_tmp[it->first];
        it++;
    }
    return Array_result;
}
void Calcu_PermutationImportance()
{
    ofstream outfile;
    outfile.open("PermutaImport.txt");
    int Num_Cycle = 10;
    for(int k = 0; k < AttsData_tmp.size() - 1; k++)
    {
        double Num_Average = 0;
        vector<double> Accuracy_Diff;
        double Standard_Deviation;
        for(int i = 0; i < Num_Cycle; i++)
        {
            double Accuracy_tmp;
            double ShuffleLatel_tmp;
            double AccuracyDiff_tmp;
            string NameDataFile = "Treedata.txt";//计算正常预测准确率
            outfile << AttsData_tmp[k] << " ";
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            Train_TableData.erase(Train_TableData.begin(),Train_TableData.end());
            Test_TableData.erase(Test_TableData.begin(),Test_TableData.end());
            AttsData_tmp.clear();
            ReadSampleData(NameDataFile);
            Bootstrap_Sampling();
            Save_BootstrapSample();
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            AttsData_tmp.clear();
            NameDataFile = "TrainData.txt";
            ReadSampleData(NameDataFile);
            Tree_Node TrainTree(TableData_tmp, AttsData_tmp);
            Tree_Node *Train_Tree = &TrainTree;
            TrainTree.generate_ChildNode();
            TrainTree.GenerateDot(Train_Tree);
            NameDataFile = "TestData.txt";
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            AttsData_tmp.clear();
            ReadSampleData(NameDataFile);
            Accuracy_tmp = CheckTree(Train_Tree,TableData_tmp);
            outfile << Accuracy_tmp << " ";

            NameDataFile = "Treedata.txt";//计算指定特征随机排列后的预测准确率
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            Train_TableData.erase(Train_TableData.begin(),Train_TableData.end());
            Test_TableData.erase(Test_TableData.begin(),Test_TableData.end());
            AttsData_tmp.clear();
            ReadSampleData(NameDataFile);
            vector<string> Array_tmp = TableData_tmp[AttsData_tmp[k]];
            TableData_tmp[AttsData_tmp[k]] = Shuffle_ArrayOrder(Array_tmp);
            Bootstrap_Sampling();
            Save_BootstrapSample();
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            AttsData_tmp.clear();
            NameDataFile = "TrainData.txt";
            ReadSampleData(NameDataFile);
            Tree_Node TestTree(TableData_tmp, AttsData_tmp);
            Tree_Node *Test_Tree = &TestTree;
            TestTree.generate_ChildNode();
            TestTree.GenerateDot(Test_Tree);
            NameDataFile = "TestData.txt";
            TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
            AttsData_tmp.clear();
            ReadSampleData(NameDataFile);
            ShuffleLatel_tmp = CheckTree(Test_Tree,TableData_tmp);
            AccuracyDiff_tmp = Accuracy_tmp - ShuffleLatel_tmp;
            Accuracy_Diff.push_back(AccuracyDiff_tmp);
            outfile << ShuffleLatel_tmp << " "<< AccuracyDiff_tmp<< endl;
            Num_Average +=AccuracyDiff_tmp;
            sleep(3);
        }
        Num_Average = (Num_Average / Num_Cycle);
        for(int i = 0; i < Accuracy_Diff.size(); i++)
        {
            double Square_tmp = (Accuracy_Diff[i] - Num_Average) * (Accuracy_Diff[i] - Num_Average);
            Standard_Deviation += Square_tmp;
        }
        Standard_Deviation = (Standard_Deviation / Num_Cycle);
        Standard_Deviation = sqrt(Standard_Deviation);
        outfile << Num_Average << " " << Standard_Deviation << endl;
    }
    outfile.close();

}
