/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 ʵ��C45�µ����ɭ�֣��������ž���
*       20200818 ʵ��PermutationImportance������������������
*       20200831 �������ɭ��*
*       20200903 ���ڶ�������ع������ھ۵���ϣ����ں����ڲ�ʹ��ȫ�ֱ���
*                ʹ��������Ϊ���
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

//�����������飬��ſ��ܵ����ԣ��������
vector<string> AttsData_tmp;
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>TableData_tmp;
map<string, vector<string>>Train_TableData;//����������ѵ����
map<string, vector<string>>Test_TableData;//���������Ĳ��Լ�
vector<int> Numsign_TrainData;//��¼�����������
vector<int> Numsign_TestData;//��¼�������Լ��������
map<int,map<int,int>> Numsign_RanForestTrain;
//���ɭ�����Լ�
map<int,vector<string>> Atts_RanForest;
//���ɭ��������
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
//���������������ݼ�
void ReadSampleData(string NameDataFile,vector<string>& AttsDataFile_tmp,map<string, vector<string>>& TableDataFile_tmp);//�������ļ��ж�ȡ��������Ӧ��map��vector��ȥ
double CheckTree(Tree_Node * root,map<string, vector<string>>TableData_tmp);//��TableData_tmp�е�ÿһ������������Ԥ�⣬���Ƚ�Ԥ��������ʵ���
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child);//������������������ͨ����ȡ����Train��Test�����ֱ�����Ӧ������
void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData);//��Train��Test��������������������ļ�
vector<string> Shuffle_ArrayOrder(vector<string> Array_tmp);//����Array_tmp�����ֵ�����������
void Calcu_PermutationImportance();//ͨ���������ĳ��������ȡֵ�󣬼������ǰ��þ�����������׼ȷ������������Ҫ�ԣ����ݴ洢��Per...
vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K);//��Atts_tmp�����ѡ��k������
void Generate_ChildTreeSampleFile(int K,int N);//K��ÿ������������Ŀ��N��������Ŀ
void Read_ChildTreeSampleFileAll(int N);//N��������Ŀ
void Read_ChildTreeSampleFile(string NameDataFile,map<int,int>& NumSign_tmp,vector<string>& AttsChildTree_tmp,map<string, vector<string>>& TableDataChildTree_tmp);//�������ļ���ȡ����Ӧ������������
void Generate_ChildTree(int N);//��ô��������˵��ô����
void Check_RandomForest(int N,map<string, vector<string>>TableData_test);
int main()
{
	//����
    string NameDataFile = "Treedata.txt";
	ReadSampleData(NameDataFile,AttsData_tmp,TableData_tmp);//��ȡ�ļ���ŵ�AttsData_tmp��TableData_tmp����
    Generate_ChildTreeSampleFile(3,10);
    Read_ChildTreeSampleFileAll(10);
    Generate_ChildTree(10);
    for(int i = 0; i < AttsData_tmp.size(); i++)
    {
        cout << AttsData_tmp[i] << " ";
    }
    cout << endl;
    for(auto iter = TableData_tmp.rbegin(); iter != TableData_tmp.rend(); iter++)
    {
        cout << iter->first << endl;
        for(auto it = iter->second.begin(); it != iter->second.end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
    }
    //Check_RandomForest(10,TableData_tmp);
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
    *//*srand((unsigned int)time(0));//Choose_RandomFeaturesר���������
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
    ReadSampleData(NameDataFile);//��ȡ�ļ�����TableData_tmp
    cout << "��ʼ���ɾ�����" << endl;
	Tree_Node myTree(TableData_tmp, AttsData_tmp);
	//����ѵ��
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
    string Result_Correct;
    //int NumSample_TestData = (int)TableData_test[AttsData_tmp[0]].size();
    int NumSample_TestData = 1;
    for(int i = 0; i < NumSample_TestData; i++)
    {
        map<string,string> maptest_tmp;
        for(int m = 0; m < (AttsData_tmp.size() - 1); m++)
        {
            maptest_tmp[AttsData_tmp[m]] = TableData_test[AttsData_tmp[m]][i];
        }
        int NumAtt_tmp = AttsData_tmp.size() - 1;
        Result_Correct = TableData_test[AttsData_tmp[NumAtt_tmp]][i];
        map<string,int> ClassNum_tmp;
        for(int m = 0; m < N; m++)
        {
            if(Numsign_RanForestTrain[m][i] == 1)
                continue;
            string Result_Prediction = RanForest[i].PredictionSampleClass(maptest_tmp);
            ClassNum_tmp[Result_Prediction] ++;
        }
        cout << "��ȷ���: " << Result_Correct << endl;
        for(auto iter = ClassNum_tmp.rbegin(); iter != ClassNum_tmp.rend(); iter++)
            cout << iter->first << ": " << iter->second << endl;
    }
}
void Generate_ChildTree(int N)
{
    for(int i = 0; i < N; i++)
    {
        Tree_Node myTree(data_TableRanForest[i], Atts_RanForest[i]);
        myTree.generate_ChildNode();
        RanForest[i] = myTree;
    }
}
void Read_ChildTreeSampleFileAll(int N)
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
        Numsign_RanForestTrain[i] = NumSign_tmp;
        Atts_RanForest[i] = AttsChildTree_tmp;
        data_TableRanForest[i] = TableDataChildTree_tmp;
    }
}
void Read_ChildTreeSampleFile(string NameDataFile,map<int,int>& NumSign_tmp,vector<string>& AttsChildTree_tmp,map<string, vector<string>>& TableDataChildTree_tmp)
{
    int NumSample_tmp;
    ifstream infile;//���������ļ�
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//���ɱ���
    cout << "��ʼ��ȡ����: "<< endl;
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//����ȡ�����н�����ReadLine��
    while(ReadLine >> string_tmp)
        AttsChildTree_tmp.push_back(string_tmp);
    cout << "��ȡ������Ŀ: " << endl;
    infile >> NumSample_tmp;
    getline(infile,Line_tmp);//ָ���������з�
    getline(infile,Line_tmp);
    istringstream ReadNumFlag(Line_tmp);
    for(int i = 0; i < NumSample_tmp; i++)
    {
        int Numflag_tmp;
        ReadNumFlag >> Numflag_tmp;
        NumSign_tmp[Numflag_tmp] = 1;
    }
    cout << endl;
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
    srand((unsigned int)time(0));//Choose_RandomFeaturesר���������
    for(int i = 0; i < N; i++)//ÿ��ѭ��������Ҫ����tmp�������������
    {
        vector<string> Features_ChildTree = Choose_RandomFeatures(AttsData_tmp,K);
        map<string, vector<string>>TableDataChild_tmp;
        for(int i = 0; i < Features_ChildTree.size(); i++)
        {
            string Feature_tmp = Features_ChildTree[i];
            TableDataChild_tmp[Feature_tmp] = TableData_tmp[Feature_tmp];
        }
        Bootstrap_Sampling(Features_ChildTree,TableDataChild_tmp);
        int NumFlag_Child = i;
        string NumFlagstr_tmp;
        stringstream ss;
        ss << NumFlag_Child;
        ss >> NumFlagstr_tmp;
        string FileNameChildData_tmp = "_Child" + NumFlagstr_tmp;
        cout << FileNameChildData_tmp << endl;
        Save_BootstrapSample(Features_ChildTree,FileNameChildData_tmp);
        Train_TableData.erase(Train_TableData.begin(),Train_TableData.end());
        Test_TableData.erase(Test_TableData.begin(),Test_TableData.end());
        Numsign_TrainData.clear();
        Numsign_TestData.clear();
    }
}
vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K)
{
    //sleep(3);//��Ϊ��������ᱻ��ε��ã�������Ҫsleep�����ó�ʼ����Ҳ���Խ����ӷŵ�ѭ���⣬�Ͳ��ھ���
    int NumAtt_tmp = Atts_tmp.size() - 1;
    vector<int> Numflag_tmp;
    vector<string> FeatureRandom_tmp;
    for(int i = 0; i < NumAtt_tmp; i++)
    {
        if(rand() % (NumAtt_tmp - i) < K)
        {
            Numflag_tmp.push_back(i);
            cout << i;
            K--;
        }
    }
    cout << endl;
    for(int i = 0; i < Numflag_tmp.size(); i++)
    {
        int Num_selected = Numflag_tmp[i];
        string Att_selected = Atts_tmp[Num_selected];
        cout << Att_selected << "--";
        FeatureRandom_tmp.push_back(Att_selected);
    }
    cout << endl;
    FeatureRandom_tmp.push_back(Atts_tmp[NumAtt_tmp]);//�����������
    return FeatureRandom_tmp;
}
void ReadSampleData(string NameDataFile,vector<string>& AttsDataFile_tmp,map<string, vector<string>>& TableDataFile_tmp)
{
    int NumSample_tmp;
    ifstream infile;//���������ļ�
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//���ɱ���
    cout << "��ʼ��ȡ����: "<< endl;
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//����ȡ�����н�����ReadLine��
    while(ReadLine >> string_tmp)
        AttsDataFile_tmp.push_back(string_tmp);
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
            TableDataFile_tmp[AttsDataFile_tmp[i]].push_back(string_tmp);
        }
    }
}
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child)
{
    int NumSample = (int)TableData_Child[Atts_ChildData[0]].size();//��������
    map<int,int> Numsign_Data;
    //sleep(3);
    //srand((int)time(0));//ֻ��Ҫһ�����Ӿͺ���
    for(int i = 0; i < NumSample; i++)//����ȡ��
    {
        int Num_random = (rand() % NumSample);
        Numsign_Data[Num_random] ++;
    }
    for(int i = 0; i < NumSample; i++)//��Train��ź�Test��ŷֱ����
    {
        if(Numsign_Data[i] > 0)
            Numsign_TrainData.push_back(i);
        else
            Numsign_TestData.push_back(i);
    }
    for(int i = 0; i < Numsign_TrainData.size(); i++)//����Train������
        for(int k = 0; k < Atts_ChildData.size(); k++)
            Train_TableData[Atts_ChildData[k]].push_back(TableData_Child[Atts_ChildData[k]][Numsign_TrainData[i]]);
    for(int i = 0; i < Numsign_TestData.size(); i++)//����Test������
        for(int k = 0; k < Atts_ChildData.size(); k++)
            Test_TableData[Atts_ChildData[k]].push_back(TableData_Child[Atts_ChildData[k]][Numsign_TestData[i]]);
}
void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData)
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
    for(int i = 1; i < Num_TrainSample; i++)//������Ŀ������ڵ���1����Ȼ�ͻ��������ѭ��
        Train_outfile << " " << Numsign_TrainData[i];
    for(int i = 0; i < Num_TrainSample; i++)//��Train�������Traindata��
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
    for(int i = 1; i < Num_TestSample; i++)//������Ŀ������ڵ���1����Ȼ�ͻ��������ѭ��
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
    cout << "����������Ŀ: " << NumSample_TestData << "Ԥ����ȷ������Ŀ: " << Num_Correct << endl;
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
            string NameDataFile = "Treedata.txt";//��������Ԥ��׼ȷ��
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

            NameDataFile = "Treedata.txt";//����ָ������������к��Ԥ��׼ȷ��
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
