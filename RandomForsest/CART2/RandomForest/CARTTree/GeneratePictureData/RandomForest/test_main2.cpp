/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 实现C45下的随机森林，测试最优抉择
*       20200818 实现PermutationImportance，来对属性作用排序
*       20200831 生成随机森林*
*       20200903 对于多个函数重构，高内聚低耦合，少在函数内部使用全局变量
*                使用引用做为替代
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
//map<string, vector<string>>Train_TableData;//用于子树的训练集
//map<string, vector<string>>Test_TableData;//用于子树的测试集
//vector<int> Numsign_TrainData;//记录子树样本编号
//vector<int> Numsign_TestData;//记录子树测试集样本编号
map<int,map<int,int>> Numsign_RanForestTrain;
//随机森林属性集
map<int,vector<string>> Atts_RanForest;
//随机森林样本集
map<int,map<string,vector<string>>> data_TableRanForest;
map<int,Tree_Node> RanForest;
typedef struct node{
    string node_Attribute;
    Tree_Node *p;
    Tree_Node *ftp;
    int level;
} childtree;
int attribute_Num;
double max_Gaint;
Tree_Node Tree[56];
//Tree_Node * test;
//输入数据生成数据集
void ReadSampleData(string NameDataFile,vector<string>& AttsDataFile_tmp,map<string, vector<string>>& TableDataFile_tmp);//从数据文件中读取样本到对应的map和vector中去
double CheckTree(Tree_Node * root,map<string, vector<string>>TableData_tmp);//对TableData_tmp中的每一个样本都进行预测，并比较预测结果和真实结果
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child,vector<int>& Numsign_TrainData,vector<int>& Numsign_TestData,map<string, vector<string>>& Train_TableData,map<string, vector<string>>& Test_TableData);//输入特征集和样本集通过自取样后将Train和Test样本分别存入对应样本集
void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData,vector<int> Numsign_TrainData,vector<int> Numsign_TestData,map<string, vector<string>> Train_TableData,map<string, vector<string>> Test_TableData);//将Train和Test的样本集保存进入数据文件
vector<string> Shuffle_ArrayOrder(vector<string> Array_tmp);//对于Array_tmp里面的值进行随机排列
void Calcu_PermutationImportance();//通过随机排列某个特征的取值后，计算随机前后该决策树的样本准确率来对特征重要性，数据存储到Per...
vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K);//从Atts_tmp中随机选择k个特征
void Generate_ChildTreeSampleFile(int K,int N);//K是每棵子树特征数目，N是子树数目
void Read_ChildTreeSampleFileAll(int N,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp,map<int,vector<string>>& Atts_RanForest_tmp);//N是子树数目
void Read_ChildTreeSampleFile(string NameDataFile,map<int,int>& NumSign_tmp,vector<string>& AttsChildTree_tmp,map<string, vector<string>>& TableDataChildTree_tmp);//将样本文件读取到对应的样本表里面
void Generate_ChildTree(int N,map<int,map<string,vector<string>>> data_TableRanForest_tmp,map<int,vector<string>> Atts_RanForest_tmp,map<int,Tree_Node>& RanForest_tmp);//怎么读树或者说怎么调用
void Check_RandomForest(int N,map<string, vector<string>>TableData_test);
int main()
{
	//输入
    int K = 2;
    int N = 50;
    string NameDataFile = "Treedata.txt";
    ReadSampleData(NameDataFile,AttsData_tmp,TableData_tmp);//读取文件存放到AttsData_tmp和TableData_tmp里面
    //srand((unsigned int)time(0));//只需要一个种子就好了
    for(int K = 8; K <= 9; K++)
    {
        sleep(3);
        srand((unsigned int)time(0));
        for(int N = 20; N <=150; N++)
        {
            cout << "K = " << K << "     N = " << N << endl;
            Generate_ChildTreeSampleFile(K,N);
            Read_ChildTreeSampleFileAll(N,Numsign_RanForestTrain,data_TableRanForest,Atts_RanForest);
            Generate_ChildTree(N,data_TableRanForest,Atts_RanForest,RanForest);
            Check_RandomForest(N,TableData_tmp);
        }
    }
    /*Generate_ChildTreeSampleFile(K,N);
    Read_ChildTreeSampleFileAll(N,Numsign_RanForestTrain,data_TableRanForest,Atts_RanForest);
    Generate_ChildTree(N,data_TableRanForest,Atts_RanForest,RanForest);
    Check_RandomForest(N,TableData_tmp);*/
    /*for(int i = 0; i < N; i++)
    {
        int flag_ChildTree = i;
        string Path_tmp = "ChildTreeDotFile/";
        string str_tmp;
        stringstream ss;
        string FileNameDot_tmp;
        ss << flag_ChildTree;
        ss >> str_tmp;
        FileNameDot_tmp = Path_tmp + str_tmp;
        Tree_Node *test = &RanForest[flag_ChildTree];
        RanForest[flag_ChildTree].GenerateDot(test,FileNameDot_tmp);
    }*/
    /*TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    AttsData_tmp.clear();
    map<int,int> NumSign_tmp;
    vector<string> AttsChildTree_tmp;
    map<string, vector<string>> TableDataChildTree_tmp;
    Read_ChildTreeSampleFile("ChildTreeSampl/TrainData_Child0",NumSign_tmp,AttsChildTree_tmp,TableDataChildTree_tmp);
    for(int i = 0; i < NumSign_tmp.size(); i++)
        cout << NumSign_tmp[i] << " ";
    cout << endl;
    for( int i = 0; i < AttsChildTree_tmp.size(); i++ )
        cout << AttsChildTree_tmp[i] << " ";
    cout << endl;
    for(auto iter = TableDataChildTree_tmp.rbegin(); iter != TableDataChildTree_tmp.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = iter->second.begin(); it != iter->second.end(); it++)
            cout << *it <<" ";
        cout << endl;
    }
    *//*srand((unsigned int)time(0));//Choose_RandomFeatures专用随机种子
    int K = 3;
    vector<string> Features_ChildTree = Choose_RandomFeatures(AttsData_tmp,K);
    map<string, vector<string>>TableDataChild_tmp;
    for(int i = 0; i < Features_ChildTree.size(); i++)
    {
        string Feature_tmp = Features_ChildTree[i];
        TableDataChild_tmp[Feature_tmp] = TableData_tmp[Feature_tmp];
    }
    Bootstrap_Sampling(Features_ChildTree,TableDataChild_tmp);
    int NumFlag_Child = 0;
    string NumFlagstr_tmp;
    stringstream ss;
    ss << NumFlag_Child;
    ss >> NumFlagstr_tmp;
    string FileNameChildData_tmp = "_Child" + NumFlagstr_tmp;
    cout << FileNameChildData_tmp << endl;
    Save_BootstrapSample(Features_ChildTree,FileNameChildData_tmp);
    *//*Bootstrap_Sampling();
    Save_BootstrapSample();
    TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    AttsData_tmp.clear();
    NameDataFile = "TrainData.txt";
    ReadSampleData(NameDataFile);//读取文件存入TableData_tmp
    cout << "开始生成决策树" << endl;
	Tree_Node myTree(TableData_tmp, AttsData_tmp);
	//进行训练
    test = &myTree;
    myTree.generate_ChildNode();
    myTree.PrintDecisionTree(test);
    myTree.GenerateDot(test);
    myTree.PrintDecisionTree(test);
    *//*NameDataFile = "TestData.txt";
    TableData_tmp.erase(TableData_tmp.begin(),TableData_tmp.end());
    AttsData_tmp.clear();
    ReadSampleData(NameDataFile);
    Calcu_PermutationImportance();
    */system("pause");
	return 0;
}
void Check_RandomForest(int N,map<string, vector<string>>TableData_test)
{
    int NumSample_TestData = (int)TableData_test[AttsData_tmp[0]].size();
    //int NumSample_TestData = 2;
    int Num_PredictionCorrect = 0;
    int Num_PredictionError = 0;
    double ErrorRate_OOB = 0;
    for(int i = 0; i < NumSample_TestData; i++)
    {
        map<string,string> maptest_tmp;
        for(int m = 0; m < (AttsData_tmp.size() - 1); m++)
        {
            maptest_tmp[AttsData_tmp[m]] = TableData_test[AttsData_tmp[m]][i];
        }
        int NumAtt_tmp = AttsData_tmp.size() - 1;
        string Result_Correct = TableData_test[AttsData_tmp[NumAtt_tmp]][i];
        string Result_Prediction;
        map<string,int> ClassNum_tmp;
        for(int m = 0; m < N; m++)
        {
            if(Numsign_RanForestTrain[m][i] == 1)
                continue;
            //cout <<"第" << m <<"棵子树    检测第" << i <<"个样本" << endl;
            //cout <<"第" << m <<"棵子树特征集"<< endl;
            //for(int l = 0; l < Atts_RanForest[m].size(); l++)
            //    cout << Atts_RanForest[m][l] <<"--";
            string ResultPrediction_tmp = RanForest[m].PredictionSampleClass(maptest_tmp);
            //cout << Result_Correct << "---" << endl;
            ClassNum_tmp[ResultPrediction_tmp] ++;
        }
        //cout << endl;
        //cout << "正确结果: " << Result_Correct << endl;
        //for(auto it = ClassNum_tmp.rbegin(); it != ClassNum_tmp.rend(); it++)
        //    cout << it->first <<": " << it->second <<endl;
        map<string,int>::iterator it = ClassNum_tmp.begin();
        int MaxNum_Class = it -> second;
        for(; it != ClassNum_tmp.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNum_Class)//>=保证至少有一个值被赋予Att_CurrentNode
            {
                Result_Prediction = it->first;
                MaxNum_Class = ClassNum_tmp;
            }
        }
        if( Result_Correct == Result_Prediction )
            Num_PredictionCorrect ++;
        else
            Num_PredictionError ++;
    }
    ErrorRate_OOB = (double)Num_PredictionError / (double)NumSample_TestData;
    cout << "样本总数目为: " << NumSample_TestData << "预测正确数目为: " << Num_PredictionCorrect << "预测错误数目为: " << Num_PredictionError ;
    cout << "子树数目为: " << N << "包外错误率为: " << ErrorRate_OOB << endl;
}
void Generate_ChildTree(int N,map<int,map<string,vector<string>>> data_TableRanForest_tmp,map<int,vector<string>> Atts_RanForest_tmp,map<int,Tree_Node>& RanForest_tmp)
{
    for(int i = 0; i < N; i++)
    {
        Tree_Node myTree(data_TableRanForest_tmp[i], Atts_RanForest_tmp[i]);
        myTree.generate_ChildNode();
        RanForest_tmp[i] = myTree;
    }
}
void Read_ChildTreeSampleFileAll(int N,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp,map<int,vector<string>>& Atts_RanForest_tmp)
{
    for(int i = 0; i < N; i++)
    {
        map<int,int> NumSign_tmp;
        vector<string> AttsChildTree_tmp;
        map<string, vector<string>> TableDataChildTree_tmp;
        string Front_ChildTreeFileName = "ChildTreeSampl/TrainData_Child";
        string NumFlagstr_tmp;
        stringstream ss;
        ss << i;
        ss >> NumFlagstr_tmp;
        string ChildTreeFileName = Front_ChildTreeFileName + NumFlagstr_tmp;
        Read_ChildTreeSampleFile(ChildTreeFileName,NumSign_tmp,AttsChildTree_tmp,TableDataChildTree_tmp);
        Numsign_RanForestTrain_tmp[i] = NumSign_tmp;
        Atts_RanForest_tmp[i] = AttsChildTree_tmp;
        data_TableRanForest_tmp[i] = TableDataChildTree_tmp;
    }
}
void Read_ChildTreeSampleFile(string NameDataFile,map<int,int>& NumSign_tmp,vector<string>& AttsChildTree_tmp,map<string, vector<string>>& TableDataChildTree_tmp)
{
    int NumSample_tmp;
    ifstream infile;//样本数据文件
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//过渡变量
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//将读取到的行解析到ReadLine上
    while(ReadLine >> string_tmp)
        AttsChildTree_tmp.push_back(string_tmp);
    infile >> NumSample_tmp;
    getline(infile,Line_tmp);//指针跳过换行符
    getline(infile,Line_tmp);
    istringstream ReadNumFlag(Line_tmp);
    for(int i = 0; i < NumSample_tmp; i++)
    {
        int Numflag_tmp;
        ReadNumFlag >> Numflag_tmp;
        NumSign_tmp[Numflag_tmp] = 1;
    }
    for(int i = 0; i < NumSample_tmp; i++)
    {
        getline(infile,Line_tmp);
        istringstream ReadLine(Line_tmp);
        for(int i = 0; i < AttsChildTree_tmp.size(); i++)
        {
            ReadLine >> string_tmp;
            TableDataChildTree_tmp[AttsChildTree_tmp[i]].push_back(string_tmp);
        }
    }
}
void Generate_ChildTreeSampleFile(int K,int N)
{
    //srand((unsigned int)time(0));//Choose_RandomFeatures专用随机种子
    for(int i = 0; i < N; i++)//每次循环结束需要清理tmp集合里面的数据
    {
        vector<string> Features_ChildTree = Choose_RandomFeatures(AttsData_tmp,K);
        map<string, vector<string>>TableDataChild_tmp;
        vector<int> Numsign_TrainData;//存储Train样本的序号
        vector<int> Numsign_TestData;
        map<string, vector<string>> Train_TableData;//存储Train样本
        map<string, vector<string>> Test_TableData;
        for(int i = 0; i < Features_ChildTree.size(); i++)
        {
            string Feature_tmp = Features_ChildTree[i];
            TableDataChild_tmp[Feature_tmp] = TableData_tmp[Feature_tmp];
        }
        Bootstrap_Sampling(Features_ChildTree,TableDataChild_tmp,Numsign_TrainData,Numsign_TestData,Train_TableData,Test_TableData);
        string NumFlagstr_tmp;
        stringstream ss;
        ss << i;
        ss >> NumFlagstr_tmp;
        string FileNameChildData_tmp = "_Child" + NumFlagstr_tmp;
        Save_BootstrapSample(Features_ChildTree,FileNameChildData_tmp,Numsign_TrainData,Numsign_TestData,Train_TableData,Test_TableData);
    }
}
vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K)
{
    //sleep(3);//因为这个函数会被多次调用，所以需要sleep来重置初始种子也可以将种子放到循环外，就不内聚了
    int NumAtt_tmp = Atts_tmp.size() - 1;
    vector<int> Numflag_tmp;
    vector<string> FeatureRandom_tmp;
    for(int i = 0; i < NumAtt_tmp; i++)
    {
        if(rand() % (NumAtt_tmp - i) < K)
        {
            Numflag_tmp.push_back(i);
            K--;
        }
    }
    for(int i = 0; i < Numflag_tmp.size(); i++)
    {
        int Num_selected = Numflag_tmp[i];
        string Att_selected = Atts_tmp[Num_selected];
        FeatureRandom_tmp.push_back(Att_selected);
    }
    FeatureRandom_tmp.push_back(Atts_tmp[NumAtt_tmp]);//添加样本类型
    return FeatureRandom_tmp;
}
void ReadSampleData(string NameDataFile,vector<string>& AttsDataFile_tmp,map<string, vector<string>>& TableDataFile_tmp)
{
    int NumSample_tmp;
    ifstream infile;//样本数据文件
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//过渡变量
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//将读取到的行解析到ReadLine上
    while(ReadLine >> string_tmp)
        AttsDataFile_tmp.push_back(string_tmp);
    infile >> NumSample_tmp;
    getline(infile,Line_tmp);//指针跳过换行符
    for(int i = 0; i < NumSample_tmp; i++)
    {
        getline(infile,Line_tmp);
        istringstream ReadLine(Line_tmp);
        for(int i = 0; i < AttsData_tmp.size(); i++)
        {
            ReadLine >> string_tmp;
            TableDataFile_tmp[AttsDataFile_tmp[i]].push_back(string_tmp);
        }
    }
}
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child,vector<int>& Numsign_TrainData,vector<int>& Numsign_TestData,map<string, vector<string>>& Train_TableData,map<string, vector<string>>& Test_TableData)
{
    int NumSample = (int)TableData_Child[Atts_ChildData[0]].size();//样本数量
    map<int,int> Numsign_Data;
    //sleep(3);
    //srand((int)time(0));//只需要一个种子就好了
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
        for(int k = 0; k < Atts_ChildData.size(); k++)
            Train_TableData[Atts_ChildData[k]].push_back(TableData_Child[Atts_ChildData[k]][Numsign_TrainData[i]]);
    for(int i = 0; i < Numsign_TestData.size(); i++)//存入Test样本集
        for(int k = 0; k < Atts_ChildData.size(); k++)
            Test_TableData[Atts_ChildData[k]].push_back(TableData_Child[Atts_ChildData[k]][Numsign_TestData[i]]);
}
void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData,vector<int> Numsign_TrainData,vector<int> Numsign_TestData,map<string, vector<string>> Train_TableData,map<string, vector<string>> Test_TableData)//将Train和Test的样本集保存进入数据文件                                 
{
    ofstream Train_outfile;
    ofstream Test_outfile;
    int Num_TrainSample;
    int Num_TestSample;
    string Front_TrainFilename = "ChildTreeSampl/TrainData";
    string Front_TestFilename = "ChildTreeSampl/TestData";
    string FileName_Train = Front_TrainFilename + FileName_ChildData;
    string FileName_Test = Front_TestFilename + FileName_ChildData;
    Train_outfile.open(FileName_Train);
    Test_outfile.open(FileName_Test);
    Train_outfile << Atts_ChildData[0];
    for(int i = 1; i < Atts_ChildData.size(); i++)
    {
        Train_outfile <<" "<< Atts_ChildData[i];
    }
    Train_outfile << endl;
    Num_TrainSample = (int)Train_TableData[Atts_ChildData[0]].size();
    Train_outfile << Num_TrainSample;
    Train_outfile << endl;
    Train_outfile << Numsign_TrainData[0];
    for(int i = 1; i < Num_TrainSample; i++)//样本数目必须大于等于1，不然就会出现无穷循环
        Train_outfile << " " << Numsign_TrainData[i];
    for(int i = 0; i < Num_TrainSample; i++)//将Train保存进入Traindata中
    {
        Train_outfile << endl;
        Train_outfile << Train_TableData[Atts_ChildData[0]][i];
        for(int m = 1; m < Atts_ChildData.size(); m++)
        {
            Train_outfile <<" "<< Train_TableData[Atts_ChildData[m]][i];
        }
    }
    Train_outfile.close();
    Test_outfile << Atts_ChildData[0];
    for(int i = 1; i < Atts_ChildData.size(); i++)
    {
        Test_outfile <<" "<< Atts_ChildData[i];
    }
    Test_outfile << endl;
    Num_TestSample = (int)Test_TableData[Atts_ChildData[0]].size();
    Test_outfile << Num_TestSample;
    Test_outfile << endl;
    Test_outfile << Numsign_TestData[0];
    for(int i = 1; i < Num_TestSample; i++)//样本数目必须大于等于1，不然就会出现无穷循环
        Test_outfile << " " << Numsign_TestData[i];
    for(int i = 0; i < Num_TestSample; i++)
    {
        Test_outfile << endl;
        Test_outfile << Test_TableData[Atts_ChildData[0]][i];
        for(int m = 1; m < Atts_ChildData.size(); m++)
        {
            Test_outfile <<" "<< Test_TableData[Atts_ChildData[m]][i];
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
    cout << "测试样本数目: " << NumSample_TestData << "预测正确样本数目: " << Num_Correct << endl;
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
/*void Calcu_PermutationImportance()
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

}*/
