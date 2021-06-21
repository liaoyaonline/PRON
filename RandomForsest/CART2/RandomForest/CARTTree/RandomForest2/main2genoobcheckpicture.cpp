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
*       20200907 ʵ��LVM���������Ӽ���ˢѡ*
*       20200911 �鿴ÿ��������׼ȷ��
*       20200916 �޸�Ϊ����Ҫ�����ı���ֱ�Ӵ���map��������������
*       20200918 �����˵ɭ�ֽ����޸�����Ϊ֮ǰ���㷨������Ӧ�����ڽ�㻮��
*                ʱ�������������ѡ��m������������������ѡ�񣬶�����һ��ʼ��ѡ��
*       20201003 ����OOB�������б�
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
#include <numeric>
#include <algorithm>
#include "Tree_Node.h"
using namespace std;

//�����������飬��ſ��ܵ����ԣ��������
vector<string> AttsData_tmp;
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>TableData_tmp;
//map<string, vector<string>>Train_TableData;//����������ѵ����
//map<string, vector<string>>Test_TableData;//���������Ĳ��Լ�
//vector<int> Numsign_TrainData;//��¼�����������
//vector<int> Numsign_TestData;//��¼�������Լ��������
map<int,map<int,int>> Numsign_RanForestTrain;
//���ɭ�����Լ�
//���ɭ��������
map<int,map<string,vector<string>>> data_TableRanForest;
map<int,map<string,int>> OOBdata_RanForest;
map<int,string> OOBResult_RanForest;
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
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child,vector<int>& Numsign_TrainData,vector<int>& Numsign_TestData,map<string, vector<string>>& Train_TableData,map<string, vector<string>>& Test_TableData);//������������������ͨ����ȡ����Train��Test�����ֱ�����Ӧ������
void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData,vector<int> Numsign_TrainData,vector<int> Numsign_TestData,map<string, vector<string>> Train_TableData,map<string, vector<string>> Test_TableData);//��Train��Test��������������������ļ�
vector<string> Shuffle_ArrayOrder(vector<string> Array_tmp);//����Array_tmp�����ֵ�����������
void Calcu_PermutationImportance();//ͨ���������ĳ��������ȡֵ�󣬼������ǰ��þ�����������׼ȷ������������Ҫ�ԣ����ݴ洢��Per...
vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K);//��Atts_tmp�����ѡ��k������
void Generate_ChildTreeSampleFile(int N,vector<string> Feature_Set,map<string,vector<string>> Data_Set,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp);//K��ÿ������������Ŀ��N��������Ŀ�޸ĺ��������������ݵ�ʱ��ֱ�Ӵ����Ӧ��map���棬���ͼ��������
//void Generate_ChildTreeSampleFile(int K,int N,vector<string> Feature_Set,map<string,vector<string>> Data_Set,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp,map<int,vector<string>>& Atts_RanForest_tmp);
void Read_ChildTreeSampleFileAll(int N,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp,map<int,vector<string>>& Atts_RanForest_tmp);//N��������Ŀ
void Read_ChildTreeSampleFile(string NameDataFile,map<int,int>& NumSign_tmp,vector<string>& AttsChildTree_tmp,map<string, vector<string>>& TableDataChildTree_tmp);//�������ļ���ȡ����Ӧ������������
void Generate_ChildTree(int K,int N,vector<string> Atts_tmp,map<int,map<string,vector<string>>> data_TableRanForest_tmp,map<int,Tree_Node>& RanForest_tmp);//��ô��������˵��ô����
double Check_RandomForest(int N,map<string, vector<string>>TableData_test , map<int,map<string,int>>& OOBdata,map<int,string>& OOBResult);//����OOB������
//double Check_RandomForest(int N,map<string, vector<string>>TableData_test , map<int,map<string,int>>& OOBdata);//����OOB������
map<int,map<string,string>> Generate_TwoFeature(vector<string> Atts_tmp);//��������������ͬ���������
double Calcu_PearsonCorrelationCoefficient(map<string,string> Atts_tmp,map<string,vector<string>> Table_Data);//����Ƥ��ѷ���ϵ��
vector<string> LVM(map<string,vector<string>> Data_Set,vector<string> Feature_Set,int Num_Stop,int K);//LVM����ѡ�񷽷�
int main()
{
	//����
    string NameDataFile = "Treedata.txt";
    ReadSampleData(NameDataFile,AttsData_tmp,TableData_tmp);//��ȡ�ļ���ŵ�AttsData_tmp��TableData_tmp����
    //���Ƴ�һЩ����ʱ�������˶δ��󣬵���־����ʱ����Ҫ����������������Ժ���Բ����Ƿ���
    vector<string> FeatureSet_tmp = AttsData_tmp;//����������
    /*auto it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"OneStarRate");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"StandardDeviation");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"Quartile");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"DispersionCoefficient");
    FeatureSet_tmp.erase(it);
    */map<string,vector<string>> DataSet_tmp;//�������ݼ�
    map<string,vector<string>> Data_Set = TableData_tmp;//������֤����
    for(int i = 0; i < FeatureSet_tmp.size(); i++)
    {
        string Feature_tmp = FeatureSet_tmp[i];
        DataSet_tmp[Feature_tmp] = Data_Set[Feature_tmp];
    }
    /*for(int K = 9 ; K > 8; K--)
    {
        srand((unsigned int)time(0));
        cout <<"Kֵ���Ϊ: " << K << endl;
        //vector<string> Feature_tmp = LVM(DataSet_tmp,FeatureSet_tmp,10,K);
        vector<string> Feature_tmp = LVM(TableData_tmp,AttsData_tmp,20,K);
        for(int i = 0; i < Feature_tmp.size(); i++)
        {
            cout << Feature_tmp[i] << "---" << endl;
        }
        cout << endl;
    }*/
    srand((unsigned int)time(0));
    int N =799;
    int K = 3;
    //vector<string> FeatureSet_tmp = AttsData_tmp;
    //map<string, vector<string>>DataSet_tmp = TableData_tmp;
    /*for(int i = 0; i < AttsData_tmp.size(); i++)
        cout << AttsData_tmp[i] << "---";
    cout << endl;
    for(auto iter = TableData_tmp.rbegin(); iter != TableData_tmp.rend(); iter++)
    {
        cout << iter->first << ": " << endl;
        for(it = iter->second.begin(); it != iter->second.end(); it++)
            cout << *it <<" ";
        cout << endl;
    }*/
    Generate_ChildTreeSampleFile(N,FeatureSet_tmp,DataSet_tmp,Numsign_RanForestTrain,data_TableRanForest);
    Generate_ChildTree(K,N,FeatureSet_tmp,data_TableRanForest,RanForest);
    double ErrorRate_OOB = Check_RandomForest(N,DataSet_tmp,OOBdata_RanForest,OOBResult_RanForest);
    for(int i = 0; i < OOBdata_RanForest.size(); i++)
    {
        int NumAtt_tmp = FeatureSet_tmp.size() - 1;
        cout << i << ":       ";
        cout << "All: " << OOBdata_RanForest[i]["All"] <<"----";
        cout << "good: "<< OOBdata_RanForest[i]["good"] << "----";
        cout << "bad: " << OOBdata_RanForest[i]["bad"] << "----";
        cout << "Result: " << OOBResult_RanForest[i] <<"  ";
        cout << "True: " << DataSet_tmp[FeatureSet_tmp[NumAtt_tmp]][i] << endl;
    }
    auto it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"OneStarRate");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"TwoStarRate");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"ThreeStarRate");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"FourStarRate");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"StandardDeviation");
    FeatureSet_tmp.erase(it);
    it = find(FeatureSet_tmp.begin(),FeatureSet_tmp.end(),"Quartile");
    FeatureSet_tmp.erase(it);
    ofstream OOBdata_outfile;
    int Num_Sample;
    string Name_File = "OOBdata.txt";
    OOBdata_outfile.open(Name_File);
    OOBdata_outfile << FeatureSet_tmp[0];
    for(int i = 1; i < FeatureSet_tmp.size(); i++)
    {
        OOBdata_outfile <<" "<< FeatureSet_tmp[i];
    }
    Num_Sample = (int)DataSet_tmp[FeatureSet_tmp[0]].size();
    for(int i = 0; i < Num_Sample; i++)//��Train�������Traindata��
    {
        OOBdata_outfile << endl;
        int NumAtt_tmp = FeatureSet_tmp.size() - 1;
        OOBdata_outfile << DataSet_tmp[FeatureSet_tmp[0]][i];
        for(int m = 1; m < FeatureSet_tmp.size() - 1; m++)
        {
            OOBdata_outfile <<" "<< DataSet_tmp[FeatureSet_tmp[m]][i];
        }
        OOBdata_outfile <<" "<< OOBdata_RanForest[i]["All"];
        OOBdata_outfile <<" "<< OOBdata_RanForest[i]["good"];
        OOBdata_outfile <<" "<< OOBdata_RanForest[i]["bad"];
        OOBdata_outfile <<" "<< OOBResult_RanForest[i];
        OOBdata_outfile <<" "<< DataSet_tmp[FeatureSet_tmp[NumAtt_tmp]][i];
    }
    OOBdata_outfile.close();
    cout << "ErrorRate_OOB: " << ErrorRate_OOB << endl;
    system("pause");
	return 0;
}
/*vector<string> LVM(map<string,vector<string>> Data_Set,vector<string> Feature_Set,int Num_Stop,int K)
{
    vector<string> FeatureSet_Best = Feature_Set;
    double OOBErrorRate = 1;
    int Len_FeatureSet = Feature_Set.size();
    int t = 0;
    while(t < Num_Stop)
    {
        //int K = (rand() % 6) + 4;
        int flag_same = 0;
        int flag_less = 0;
        vector<string> FeatureSet_tmp = Choose_RandomFeatures(Feature_Set,K);
        for(int i = 0; i < FeatureSet_tmp.size(); i++)
        {
            cout << FeatureSet_tmp[i] << "---";
        }
        cout << endl;
        map<string, vector<string>>DataSet_tmp;//��������������������
        for(int i = 0; i < FeatureSet_tmp.size(); i++)
        {
            string Feature_tmp = FeatureSet_tmp[i];
            DataSet_tmp[Feature_tmp] = Data_Set[Feature_tmp];
        }
        double Sum_OOBErrorRate = 0;
        int Num_Forest = 0;
        for(int N = 400; N < 410; N++)
        {
            Generate_ChildTreeSampleFile(3,N,FeatureSet_tmp,DataSet_tmp,Numsign_RanForestTrain,data_TableRanForest,Atts_RanForest);//�������Ϊ�����Ӽ��������Ӽ�
            //Read_ChildTreeSampleFileAll(N,Numsign_RanForestTrain,data_TableRanForest,Atts_RanForest);//mapÿ�θ��¾Ͳ���Ҫ��Ϊ������
            Generate_ChildTree(N,data_TableRanForest,Atts_RanForest,RanForest);
            double OOBErrorRate_tmp = Check_RandomForest(N,Data_Set);//����OOB������
            cout << N <<"," << OOBErrorRate_tmp << endl;
            Sum_OOBErrorRate += OOBErrorRate_tmp;
            Num_Forest ++;
        }
        double Average_OOBErrorRate = Sum_OOBErrorRate / (double)Num_Forest;
        cout << Average_OOBErrorRate << endl;
        int lenFenture_tmp = FeatureSet_tmp.size();
        if(Average_OOBErrorRate - OOBErrorRate < 0.01 && Average_OOBErrorRate - OOBErrorRate > -0.01)
            flag_same = 1;
        if(OOBErrorRate - Average_OOBErrorRate > 0.01)
            flag_less = 1;
        //if(OOBErrorRate_tmp <= OOBErrorRate && lenFenture_tmp < Len_FeatureSet)
        if(flag_same == 1 && lenFenture_tmp < Len_FeatureSet)
        {
            cout <<"�������Ӽ���ѡ��Ϊ����������" << endl;
            t = 0;
            OOBErrorRate = Average_OOBErrorRate;
            Len_FeatureSet = lenFenture_tmp;
            FeatureSet_Best = FeatureSet_tmp;
        }
        else
            t = t + 1;
        if(flag_less == 1)
        {
            cout <<"�������Ӽ���ѡ��Ϊ���ٴ�����" << endl;
            t = 0;
            OOBErrorRate = Average_OOBErrorRate;
            Len_FeatureSet = lenFenture_tmp;
            FeatureSet_Best = FeatureSet_tmp;
        }
        else
            t = t + 1;
    }
    return FeatureSet_Best;
}
double Calcu_PearsonCorrelationCoefficient(map<string,string> Atts_tmp,map<string,vector<string>> Table_Data)
{
    auto it = Atts_tmp.rbegin();
    double Num_Pearson;
    string Feature_First = it->first;
    string Feature_Second = it->second;
    vector<double> First_tmp;
    vector<double> Second_tmp;
    double CovFirst_Second = 0;
    double Variance_First;
    double Variance_Second;
    for(int i = 0; i < Table_Data[Feature_First].size(); i++)
    {
        stringstream ss;
        string str_tmp = Table_Data[Feature_First][i];
        double Num_tmp;
        ss << str_tmp;
        ss >> Num_tmp;
        First_tmp.push_back(Num_tmp);
    }
    for(int i = 0; i < Table_Data[Feature_First].size(); i++)
    {
        stringstream ss;
        string str_tmp = Table_Data[Feature_Second][i];
        double Num_tmp;
        ss << str_tmp;
        ss >> Num_tmp;
        Second_tmp.push_back(Num_tmp);
    }
    double Average_first = accumulate(First_tmp.begin(),First_tmp.end(),0) / First_tmp.size();
    double Average_second = accumulate(Second_tmp.begin(),Second_tmp.end(),0) / Second_tmp.size();
    for_each (First_tmp.begin(), First_tmp.end(), [&](const double d) {
              Variance_First  += (d-Average_first)*(d-Average_first);
              });
    Variance_First = sqrt(Variance_First);
    for_each (Second_tmp.begin(), Second_tmp.end(), [&](const double d) {
              Variance_Second  += (d-Average_second)*(d-Average_second);
              });
    Variance_Second = sqrt(Variance_Second);
    for(int i = 0; i < First_tmp.size(); i++)
    {
        double Diff_First = First_tmp[i] - Average_first;
        double Diff_Second = Second_tmp[i] - Average_second;
        double Product_tmp = Diff_First * Diff_Second;
        CovFirst_Second += Product_tmp;
    }
    Num_Pearson = CovFirst_Second / (Variance_First * Variance_Second);
    return Num_Pearson;
}
map<int,map<string,string>> Generate_TwoFeature(vector<string> Atts_tmp)
{
    map<int,map<string,string>> TwoFeature_tmp;
    int Sign_Features = 0;
    for(int i = 0; i < Atts_tmp.size() - 1; i++)
    {
        for(int j = i + 1; j < Atts_tmp.size(); j++)
        {
            string first_tmp = Atts_tmp[i];
            string second_tmp = Atts_tmp[j];
            map<string,string> FeatureToFeature_tmp;
            FeatureToFeature_tmp[first_tmp] = second_tmp;
            cout << Atts_tmp[i] <<"--" <<Atts_tmp[j] << endl;
            TwoFeature_tmp[Sign_Features] = FeatureToFeature_tmp;
            Sign_Features ++;
        }
    }
    return TwoFeature_tmp;
}*/
double Check_RandomForest(int N,map<string, vector<string>>TableData_test , map<int,map<string,int>>& OOBdata,map<int,string>& OOBResult)
{
    int NumSample_TestData = (int)TableData_test[AttsData_tmp[0]].size();
    //int NumSample_TestData = 2;
    int Num_PredictionCorrect = 0;
    int Num_PredictionError = 0;
    double ErrorRate_OOB = 0;
    map<string,int> P_Result;
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
            //cout <<"��" << m <<"������    ����" << i <<"������" << endl;
            //cout <<"��" << m <<"������������"<< endl;
            //for(int l = 0; l < Atts_RanForest[m].size(); l++)
            //    cout << Atts_RanForest[m][l] <<"--";
            OOBdata[i]["All"]++;
            string ResultPrediction_tmp = RanForest[m].PredictionSampleClass(maptest_tmp);
            OOBdata[i][ResultPrediction_tmp]++;
            //cout << Result_Correct << "---" << endl;
            ClassNum_tmp[ResultPrediction_tmp] ++;
        }
        //cout << endl;
        //cout << "��ȷ���: " << Result_Correct << endl;
        //for(auto it = ClassNum_tmp.rbegin(); it != ClassNum_tmp.rend(); it++)
        //    cout << it->first <<": " << it->second <<endl;
        map<string,int>::iterator it = ClassNum_tmp.begin();
        int MaxNum_Class = it -> second;
        for(; it != ClassNum_tmp.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNum_Class)//>=��֤������һ��ֵ������Att_CurrentNode
            {
                Result_Prediction = it->first;
                MaxNum_Class = ClassNum_tmp;
            }
        }
        if(Result_Prediction == "good")
        {
            P_Result["good"] ++;
            if(Result_Correct == Result_Prediction)
            {
                P_Result["tgood"] ++;
            }
        }
        OOBResult[i] = Result_Prediction;
        if( Result_Correct == Result_Prediction )
            Num_PredictionCorrect ++;
        else
            Num_PredictionError ++;
    }
    ErrorRate_OOB = (double)Num_PredictionError / (double)NumSample_TestData;
    cout <<"googNum: " << P_Result["good"] << "---- TruegoodNum: " << P_Result["tgood"] << endl;
    //cout << "��������ĿΪ: " << NumSample_TestData << "Ԥ����ȷ��ĿΪ: " << Num_PredictionCorrect << "Ԥ�������ĿΪ: " << Num_PredictionError ;
    //cout << "������ĿΪ: " << N << "���������Ϊ: " << ErrorRate_OOB << endl;
    return ErrorRate_OOB;
}
void Generate_ChildTree(int K,int N,vector<string> Atts_tmp,map<int,map<string,vector<string>>> data_TableRanForest_tmp,map<int,Tree_Node>& RanForest_tmp)
{
    for(int i = 0; i < N; i++)
    {
        Tree_Node myTree(data_TableRanForest_tmp[i], Atts_tmp);
        myTree.generate_ChildNode(K);
        RanForest_tmp[i] = myTree;
    }
}/*
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
    ifstream infile;//���������ļ�
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//���ɱ���
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//����ȡ�����н�����ReadLine��
    while(ReadLine >> string_tmp)
        AttsChildTree_tmp.push_back(string_tmp);
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
    infile.close();
}*/
//void Generate_ChildTreeSampleFile(int K,int N,vector<string> Feature_Set,map<string,vector<string>> Data_Set,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp,map<int,vector<string>>& Atts_RanForest_tmp)
void Generate_ChildTreeSampleFile(int N,vector<string> Feature_Set,map<string,vector<string>> Data_Set,map<int,map<int,int>>& Numsign_RanForestTrain_tmp,map<int,map<string,vector<string>>>& data_TableRanForest_tmp)
{
    //srand((unsigned int)time(0));//Choose_RandomFeaturesר���������
    for(int i = 0; i < N; i++)//ÿ��ѭ��������Ҫ����tmp�������������
    {
        vector<int> Numsign_TrainData;//�洢Train���������
        vector<int> Numsign_TestData;
        map<string, vector<string>> Train_TableData;//�洢Train����
        map<string, vector<string>> Test_TableData;
        Bootstrap_Sampling(Feature_Set,Data_Set,Numsign_TrainData,Numsign_TestData,Train_TableData,Test_TableData);
        map<int,int> NumSignTrain_tmp;
        for(int i = 0; i < Numsign_TrainData.size(); i++)
        {
            int NumSign_tmp = Numsign_TrainData[i];
            NumSignTrain_tmp[NumSign_tmp] = 1;
        }
        Numsign_RanForestTrain_tmp[i] = NumSignTrain_tmp;
        data_TableRanForest_tmp[i] = Train_TableData;
        /*string NumFlagstr_tmp;
        stringstream ss;
        ss << i;
        ss >> NumFlagstr_tmp;
        string FileNameChildData_tmp = "_Child" + NumFlagstr_tmp;
        Save_BootstrapSample(Features_ChildTree,FileNameChildData_tmp,Numsign_TrainData,Numsign_TestData,Train_TableData,Test_TableData);
        */
    }
}
/*vector<string> Choose_RandomFeatures(vector<string> Atts_tmp,int K)//����������ѡ��K������(���������),�ٽ�������
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
            K--;
        }
    }
    for(int i = 0; i < Numflag_tmp.size(); i++)
    {
        int Num_selected = Numflag_tmp[i];
        string Att_selected = Atts_tmp[Num_selected];
        FeatureRandom_tmp.push_back(Att_selected);
    }
    FeatureRandom_tmp.push_back(Atts_tmp[NumAtt_tmp]);//�����������
    return FeatureRandom_tmp;
}*/
void ReadSampleData(string NameDataFile,vector<string>& AttsDataFile_tmp,map<string, vector<string>>& TableDataFile_tmp)
{
    int NumSample_tmp;
    ifstream infile;//���������ļ�
    infile.open(NameDataFile);
    string Line_tmp,string_tmp;//���ɱ���
    getline(infile,Line_tmp);
    istringstream ReadLine(Line_tmp);//����ȡ�����н�����ReadLine��
    while(ReadLine >> string_tmp)
        AttsDataFile_tmp.push_back(string_tmp);
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
    infile.close();
}
void Bootstrap_Sampling(vector<string> Atts_ChildData,map<string, vector<string>>TableData_Child,vector<int>& Numsign_TrainData,vector<int>& Numsign_TestData,map<string, vector<string>>& Train_TableData,map<string, vector<string>>& Test_TableData)
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
/*void Save_BootstrapSample(vector<string> Atts_ChildData,string FileName_ChildData,vector<int> Numsign_TrainData,vector<int> Numsign_TestData,map<string, vector<string>> Train_TableData,map<string, vector<string>> Test_TableData)//��Train��Test��������������������ļ�                                 
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
}*/
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
