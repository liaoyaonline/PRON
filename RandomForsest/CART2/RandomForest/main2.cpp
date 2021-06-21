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
int main()
{
	//输入
   // string NameDataFile = "Treedata.txt";
	//ReadSampleData(NameDataFile);
    //Bootstrap_Sampling();
    //Save_BootstrapSample();
    //TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    //AttsData_tmp.clear();
    NameDataFile = "TrainData.txt";
    ReadSampleData(NameDataFile);//读取文件存入TableData_tmp
    cout << "开始生成决策树" << endl;
	Tree_Node myTree(TableData_tmp, AttsData_tmp);
	//进行训练
    test = &myTree;
    myTree.generate_ChildNode();
    myTree.PrintDecisionTree(test);
    myTree.GenerateDot(test);
    //NameDataFile = "TestData.txt";
    //TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    //AttsData_tmp.clear();
    //ReadSampleData(NameDataFile);
    //Calcu_PermutationImportance();
    //system("pause");
	return 0;
}
/*void data_RanForsetInput()
{
    int num = 0;
    for(auto it = data_Attributes.begin();it != data_Attributes.end();it++)
        cout << *it << " ";
    auto temp = data_Attributes.end();
    temp --;
    temp --;
    cout << *temp << endl;
    //将不同树的对应属性压入对应编号
    for(auto it = data_Attributes.begin(); it != temp; it++)
        for(auto jt = (it+1); jt != (temp + 1); jt++)
            for(auto kt = (jt + 1); kt != (temp + 1); kt++)
            {
                RanForest_Atts[num].push_back(*it);
                RanForest_Atts[num].push_back(*jt);
                RanForest_Atts[num].push_back(*kt);
                RanForest_Atts[num].push_back(*(temp+1));
                num++;
            }
    for(auto iter = RanForest_Atts.rbegin();iter != RanForest_Atts.rend();iter++)
    {
        cout << "编号:" <<  iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
    //将不同树对应的剩余属性集合压入对应编号
    RanForest_remainAtts = RanForest_Atts;
    for(int i = 0; i < 56; i++)
    {
        RanForest_remainAtts[i].pop_back();
    }
    for(auto iter = RanForest_remainAtts.rbegin();iter != RanForest_remainAtts.rend();iter++)
    {
        cout << "编号:" <<  iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
    //将不同树对应的样本集压入对应编号
    for(int i = 0; i < 56; i++)
    {
        for(auto it = RanForest_Atts[i].begin(); it != RanForest_Atts[i].end(); it++)
        {
            string temp_Att = *it;
            data_TableRanForest[i][temp_Att] = data_Table[temp_Att];
        }
    }
    for(int i = 0; i < 56; i++)
    {
        cout << "样本编号:" << i << endl;
        for(auto iter = data_TableRanForest[i].rbegin();iter != data_TableRanForest[i].rend();iter++)
        {
            cout << iter->first << endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
    }
    //对样本集中数据进行随机选择
    for(int i = 0; i < 56; i++)
    {
        int seed = (int)time(0) + i;
        srand(seed);
        //srand((int)time(seed));  // 产生随机种子  把0换成NULL也行
        auto iter = data_TableRanForest[i].begin();
        auto jter = iter++;
        auto kter = iter++;
        auto mter = iter++;
        cout << iter->first << endl;
        cout << jter->first << endl;
        cout << kter->first << endl;
        cout << mter->first << endl;void PrintDecisionTree(Tree_Node* root)
        auto jt=(jter->second).begin();
        auto kt=(kter->second).begin();
        auto mt=(mter->second).begin();
        auto it=(iter->second).begin();
        while(it != (iter->second).end())
        {
            int temp = rand()%2;
            if(temp == 1)
            {
                it = (iter->second).erase(it);
                jt = (jter->second).erase(jt);
                kt = (kter->second).erase(kt);
                mt = (mter->second).erase(mt);
            }
            else
            {
                it++;
                jt++;
                kt++;
                mt++;
            }
        }
    }
    for(int i = 0; i < 56; i++)
    {
        cout << "样本编号:" << i << endl;
        for(auto iter = data_TableRanForest[i].rbegin();iter != data_TableRanForest[i].rend();iter++)
        {
            cout << iter->first << endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
    }
}*/
/*void TreeRep(Tree_Node * root)
{
    stack< childtree> child_trees;
    queue< childtree> child_treeq;
    childtree Temp_Tree;
    Temp_Tree.node_Attribute = root->node_Attribute;
    Temp_Tree.p = root;
    Temp_Tree.ftp = NULL;
    Temp_Tree.level = 1;
    child_treeq.push(Temp_Tree);
    while(child_treeq.size() != 0)
    {
        Temp_Tree = child_treeq.front();
        child_treeq.pop();
        Tree_Node* p = Temp_Tree.p;
        for(auto iter = p->childNode.rbegin();iter != p->childNode.rend();iter++)
        {void PrintDecisionTree(Tree_Node* root)
            childtree childtrees;
            childtrees.p = iter->second;
            childtrees.ftp = p;
            childtrees.node_Attribute = childtrees.p->node_Attribute;
            childtrees.level = Temp_Tree.level + 1;
            child_treeq.push(childtrees);
        }
        if(p->childNode.size() != 0)
            child_trees.push(Temp_Tree);
    }
    while(!child_trees.empty())
    {
        Temp_Tree = child_trees.top();
        child_trees.pop();
        Tree_Node * p = Temp_Tree.p;
        double p1 = CheckTree(root);
        int N = (int)p->MyDateTable[p->data_Attribute[0]].size();
        cout <<"N:"<< N;
        map<string,int> category;
        for(int i = 0; i < N;i++)
        {
            vector<string> temp_Class;
            temp_Class = p->MyDateTable[p->data_Attribute[p->attribute_Num]];
            category[temp_Class[i]]++;
        }
        map<string, int>::iterator it = category.begin();
        string childNode_Attribute = it->first;
        int max_Num = it->second;
        it++;
        for (; it != category.end(); it++)
        {
            if(it->second > max_Num)
            {
                max_Num = it->second;
                childNode_Attribute = it->first;
            }
        }
        string temp_Att = p->node_Attribute;
        map<string,Tree_Node *> temp_childNode = p->childNode;
        p->node_Attribute = childNode_Attribute;
        p->childNode.erase(p->childNode.begin(),p->childNode.end());
        double p2 = CheckTree(root);
        if(p1 > p2)
        {
            p->node_Attribute = temp_Att;
            p->childNode = temp_childNode;
        }
    }
}*/
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
    cout << "Train_Data:" << endl;
    for(int i = 0; i < Numsign_TrainData.size(); i++)
        cout << Numsign_TrainData[i] << endl;
    cout << endl;
    cout << "Test_Data:" << endl;
    for(int i = 0; i < Numsign_TestData.size(); i++)
        cout << Numsign_TestData[i] << endl;
    cout << endl;
    for(int i = 0; i < Numsign_TrainData.size(); i++)//存入Train样本集
        for(int k = 0; k < AttsData_tmp.size(); k++)
            Train_TableData[AttsData_tmp[k]].push_back(TableData_tmp[AttsData_tmp[k]][Numsign_TrainData[i]]);
    for(int i = 0; i < Numsign_TestData.size(); i++)//存入Test样本集
        for(int k = 0; k < AttsData_tmp.size(); k++)
            Test_TableData[AttsData_tmp[k]].push_back(TableData_tmp[AttsData_tmp[k]][Numsign_TestData[i]]);
    cout << "输出Train样本集合" << endl;
    for(auto iter = Train_TableData.rbegin(); iter != Train_TableData.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
        {
            cout << *it << "-----";
        }
        cout << endl;
    }
    cout << "输出Test样本集合" << endl;
    for(auto iter = Test_TableData.rbegin(); iter != Test_TableData.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
        {
            cout << *it << "-----";
        }
        cout << endl;
    }
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
    double Accuracy_Prediction = 0.0;
    string Result_Prediction;
    string Result_Correct;
    int Num_Correct = 0;
    int NumSample_TestData = (int)TableData_test[AttsData_tmp[0]].size();
    for(int i = 0; i < NumSample_TestData; i++)
    {
        map<string,string> maptest_tmp;
        for(int m = 0; m < (AttsData_tmp.size() - 1); m++)
        {
            maptest_tmp[AttsData_tmp[m]] = TableData_test[AttsData_tmp[m]][i];
        }
        int NumAtt_tmp = AttsData_tmp.size() - 1;
        Result_Correct = TableData_test[AttsData_tmp[NumAtt_tmp]][i];
        Result_Prediction = root->PredictionSampleClass(maptest_tmp);
        if(Result_Prediction == Result_Correct)
        {
            Num_Correct ++;
        }
    }
    Accuracy_Prediction = (double)Num_Correct / (double)NumSample_TestData;
    return Accuracy_Prediction;
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
    int Num_Cycle = 100;
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
            TrainTree.PrintDecisionTree(Train_Tree);
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
            TestTree.PrintDecisionTree(Test_Tree);
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
/*string findBestAtt(string con)
{
    int N = (int)data_Table[data_Attributes[0]].size();
    string temp_Att = con;
    double temp_num = 0.0;
    double temp_t = 0.0;
    string temp_snum;
    double temp_Gain = 0;
    double max_Gain = 0;
    //double max_Gaint = 0;
    string max_Att;
    vector<double> remain_AttValues;
    vector<double> remain_tempt;
    map<double,string> table_con;
    cout <<"sdfsdfksfksjkfj"<< endl;
    for(int j = 0; j < data_AttValues[temp_Att].size(); j++)
    {
        stringstream ss;
        cout << " ko "<< data_AttValues[temp_Att][j] << endl;
        temp_snum = data_AttValues[temp_Att][j];
        cout << " kotemp_snum "<<temp_snum << endl;
        ss << temp_snum;
        ss >> temp_num;
        cout << "temp_num:" << temp_num << endl;
        //ss << data_AttValues[temp_Att][j];
        //ss >> temp_num;
        //remain_AttValues.push_back(con)
        //remain_AttValues.push_back(data_AttValues[temp_Att][j]);
        remain_AttValues.push_back(temp_num);
    }
    for(int j = 0; j < remain_AttValues.size(); j++)
        cout << "  "<< remain_AttValues[j] << endl;
    sort(remain_AttValues.begin(),remain_AttValues.end());
    cout << "\n\n" << endl;
    for(int j = 0; j < remain_AttValues.size(); j++)
        cout << "  "<< remain_AttValues[j] << endl;
    for(int i = 0; i < remain_AttValues.size() - 1;i++)
    {
        temp_t = (remain_AttValues[i] + remain_AttValues[i+1]) / 2;
        cout << " t: "<< temp_t << endl;
        remain_tempt.push_back(temp_t);
    }
    for(int j = 0; j < remain_tempt.size(); j++)
        cout << " tempt "<< remain_tempt[j] << endl;
    for(int j = 0; j < remain_tempt.size(); j++)
    {
        temp_t = remain_tempt[j];
        temp_Gain = calc_Entropy(data_Table);
        //用来存放对应的数据
        double temp_con;
        int sample_Numplus = 0;//大于t样本数目
        int sample_Numless = 0;//小于t样本数目
        map<string, vector<string>>sub_DataTablePlus;
        map<string, vector<string>>sub_DataTableLess;
        for(int l = 0; l < data_Table[temp_Att].size();l++)
        {
            stringstream ss;
            ss <<  data_Table[temp_Att][l];
            ss >> temp_con;
            cout << "temp_con:" << temp_con << endl;
            if(temp_con <= temp_t)
            {
                sample_Numless++;
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTableLess[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
            else
            {
                sample_Numplus++;
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTablePlus[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
        }
        cout <<"sample_NUmplus :"<< sample_Numplus<< endl;
        cout <<"sample_NUmless :"<< sample_Numless<< endl;
        for(auto iter = sub_DataTableLess.rbegin();iter != sub_DataTableLess.rend();iter++)
        {
            cout <<"first" <<iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
             cout << endl;
        }
        for(auto iter = sub_DataTablePlus.rbegin();iter != sub_DataTablePlus.rend();iter++)
        {
            cout << iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        temp_Gain = temp_Gain - ((double)sample_Numplus / N * calc_Entropy(sub_DataTablePlus) + (double)sample_Numless / N * calc_Entropy(sub_DataTableLess));
        cout << "t:" << temp_t <<"Ent : "<< temp_Gain;
        cout << endl;
        if(temp_Gain >= max_Gain)
        {
            max_Gaint = temp_t;
            max_Att = temp_Att;
            max_Gain = temp_Gain;
        }
    }
    cout << "max_Att:" << max_Att << "max_Gain:" << max_Gain << "max_Gaint:" << max_Gaint << endl;
    return temp_Att;
}
double calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//数据集中样本的数量
	int sample_Num = (int)temp_Table[data_Attributes[0]].size();//放入Tree应该是data_Attribute
	//计算数据集中的类别数量
	map<string, int> class_Map;
    //类别位置放入Tree应该去除
    int attribute_Num = (int)data_Attributes.size() -1;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]对应的就是数据集的类别
		string class_String = table[data_Attributes[attribute_Num]][i];
		class_Map[class_String]++;
	}

    for(auto iter = class_Map.rbegin();iter != class_Map.rend();iter++)
        cout <<"string :" << iter->first << "int :" << iter->second << endl;
	map<string, int>::iterator it = class_Map.begin();
	//存放类别及其对应的数量
	//vector<string> m_Class;
	vector<int> n_Class;

	for (; it != class_Map.end(); it++)
	{
		//m_Class.push_back(it->first);
		n_Class.push_back(it->second);
	}
	//计算信息熵
	double Ent = 0;
	for (int k = 0; k < class_Map.size(); k++)
	{
		//比例
		double p = (double) n_Class[k] / sample_Num;
		if (p == 0)
		{
			//规定了p=0时，plogp=0
			continue;
		}
		//c++中只有log和ln，因此需要应用换底公式
		Ent -= p * (log(p) / log(2));//信息熵
	}
	cout << "Ent" << Ent<<endl;
	return Ent;
}
void generate_ChildNode(Tree_Node  temp_Table)
{
	//样本个数
	int N = (int)data_Table[data_Attributes[0]].size();
    cout << N << endl;
    string node_Attribute;

	//将数据集中类别种类和数量放入map里面，只需判断最后一列即可
	map<string,int> category;
	for (int i = 0; i < N; i++)
	{
		vector<string> temp_Class;
		temp_Class = data_Table[data_Attributes[attribute_Num]];
		category[temp_Class[i]]++;
	}
    for(auto iter = category.rbegin();iter != category.rend();iter++)
    {
        cout << iter->first << ":"<< iter->second<<endl;
        cout << endl;
    }

	//第一种情况
	//只有一个类别，标记为叶节点
	if (1 == category.size())
	{
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
        cout << "执行只有一个类别" << endl;
		return;
	}
	//第二种情况
	//先判断所有属性是否取相同值
	bool isAllSame = false;
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = data_Table[remain_Attributes[i]];
		for (int j = 1; j < temp.size(); j++)
		{
			//只要有一个不同，即可退出
			if (temp[0] != temp[j])
			{
				isAllSame = false;
				break;
			}
		}
		if (isAllSame == false)
		{
			break;
		}
	}
	//若属性集为空或者样本中的全部属性取值相同
	if (remain_Attributes.empty()||isAllSame)
	{
        cout<< "执行所有属性都相同选项"<< endl;
		//找出数量最多的类别及其出现的个数，并将该节点标记为该类
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		int max = it->second;
		it++;
		for (; it != category.end(); it++)
		{
			int num = it->second;
			if (num > max)
			{
				node_Attribute = it->first;
				max = num;
			}
		}
		return;
	}
	//第三种情况
	//从remian_attributes中划分最优属性
	//string best_Attribute = 
    double best_t = temp_Table.findBestT("密度");
    string best_Attribute = "密度";
	//将本节点设置为最优属性
	node_Attribute = best_Attribute;
    //对最优属性的每个属性值
	int checkcon = 0;
    if(node_Attribute == "密度" || node_Attribute == "含糖率")
        checkcon = 1;
    if(checkcon == 1)
    {
        double temp_t = best_t;//最佳t从findAtt里面取得
        double temp_con;
        map<string, vector<string>>sub_DataTablePlus;
        map<string, vector<string>>sub_DataTableLess;
        for(int l = 0; l < data_Table[best_Attribute].size();l++)
        {
            stringstream ss;
            ss <<  data_Table[best_Attribute][l];
            ss >> temp_con;
            cout << "temp_con:" << temp_con << endl;
            cout << "temp_t:" << temp_t << endl;
            if(temp_con <= temp_t)
            {
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTableLess[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
            else
            {
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTablePlus[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
        }
        cout << "中间点。。。分割线" << endl;
        for(auto iter = sub_DataTableLess.rbegin();iter != sub_DataTableLess.rend();iter++)
        {
            cout <<"first" <<iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        cout << "第二道分割线。。。。。。。"<< endl;
        for(auto iter = sub_DataTablePlus.rbegin();iter != sub_DataTablePlus.rend();iter++)
        {
            cout << iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        if (sub_DataTablePlus.empty())
        {
            //生成子节
            Tree_Node * p = new Tree_Node(sub_DataTablePlus, data_Attributes, data_AttValues, remain_Attributes);
	    	//找出样本最多的类，作为子节点的属性
		    map<string, int>::iterator it = category.begin();
            string childNode_Attribute = it->first;
            int max_Num = it->second;
    		it++;
            for (; it != category.end(); it++)
            {
		    	if (it->second > max_Num)
                {
    				max_Num = it->second;
					childNode_Attribute = it->first;
	    		}
            }
			//设置子叶节点属性
            p->set_NodeAttribute(childNode_Attribute);
    		cout<< "中间sub_DataTablePlus为empty处理方案"<< endl;
            //将子节点存入childNode，预测样本的时候会用到
    		//childNode[best_AttValues] = p;
        }
    	else//若不为空，则从剩余属性值剔除该属性，调用generate_ChildNode继续往下细分
    	{
			vector<string> child_RemainAtt;
            child_RemainAtt = remain_Attributes;
    		//找出child_RemainAtt中的与该最佳属性相等的属性
    		vector<string>::iterator it = child_RemainAtt.begin();
			for (; it != child_RemainAtt.end(); it++)
            {
    			if (*it == best_Attribute)
    			{
					break;
                }
 			}
            //删除
    		child_RemainAtt.erase(it);
            cout << "若sub_DataTablePlus不为empty处理方案"<< endl;

    		//生成子节点
    		//Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//将子节点存入childNode
            //childNode[best_AttValues] = pt;
			//子节点再调用generate_ChildNode函数
            //
            //pt->generate_ChildNode();
    	}
        if (sub_DataTableLess.empty())
        {
            //生成子节
            Tree_Node * p = new Tree_Node(sub_DataTableLess, data_Attributes, data_AttValues, remain_Attributes);
	    	//找出样本最多的类，作为子节点的属性
		    map<string, int>::iterator it = category.begin();
            string childNode_Attribute = it->first;
            int max_Num = it->second;
    		it++;
            for (; it != category.end(); it++)
            {
		    	if (it->second > max_Num)
                {
    				max_Num = it->second;
					childNode_Attribute = it->first;
	    		}
            }
            cout<<"sub_DataTableLess为empty处理策略"<< endl;
			//设置子叶节点属性
            //p->set_NodeAttribute(childNode_Attribute);
    		//将子节点存入childNode，预测样本的时候会用到
    		//childNode[best_AttValues] = p;
        }
    	else//若不为空，则从剩余属性值剔除该属性，调用generate_ChildNode继续往下细分
    	{
			vector<string> child_RemainAtt;
            child_RemainAtt = remain_Attributes;
    		//找出child_RemainAtt中的与该最佳属性相等的属性
    		vector<string>::iterator it = child_RemainAtt.begin();
			for (; it != child_RemainAtt.end(); it++)
            {
    			if (*it == best_Attribute)
    			{
					break;
                }
 			}
            //删除
    		child_RemainAtt.erase(it);
            cout << "若sub_DataTableLess不为empty处理方案"<< endl;
    		//生成子节点
    		//Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//将子节点存入childNode
            //childNode[best_AttValues] = pt;
			//子节点再调用generate_ChildNode函数
            //
            //pt->generate_ChildNode();
    	}
    }
    else
    {
        cout <<"checkcon == 0 处理策略" << endl;
    }
}
*/
