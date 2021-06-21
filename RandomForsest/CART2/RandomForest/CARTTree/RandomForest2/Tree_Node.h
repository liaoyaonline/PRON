#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;


class Tree_Node
{
public:
    Tree_Node();
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute);
	void RemoveFeatures_WithTheSameValue(map<string, vector<string>>& temp_Table, vector<string>& temp_Attribute);
    //�����ӽڵ�
	void generate_ChildNode(int K);
	//������Ϣ���� Ѱ�����Ż�������
	string findBestAttribute(vector<string> AttSet_tmp);
    //Ѱ����ѻ��ֽڵ�
	void FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide);
	double Calculation_GiniValue(map<string, vector<string>> Table_Data);//����Giniֵ
    //���ýڵ������
	void SetNodeAttribute(string atttribute);
	//�����������ԣ�Ԥ����������
    string PredictionSampleClass(map<string,string> Set_AttSample);
	void PrintDecisionTree(map<string,double>& FeatureGini,Tree_Node* root);
    void GenerateDot(Tree_Node* root, string DotFileName);
    vector<string> Choose_RandomFeatures(vector<string> Feature_Set, int K);
    virtual ~Tree_Node();
private:
    map<string, vector<string>> Table_Data;//���ݱ�
    vector<string> FullAtt_DataTable;//���ڵ���������԰������
    string Attribute_CurrentNode;//��ǰ�ڵ�����
    double GiniValue_CurrentNode;//��ǰ�ڵ�Giniֵ
    double NumSample_CurrentNode;//��ǰ�ڵ�������Ŀ
    map<string,double>FeatureGini_CurrentNode;//��ǰ�ڵ��������Giniֵ��Ӧ�����ڲ��Ը�����������Ҫ��
    int NumAtt_CurrentNode;//��ǰ�ڵ�������Ŀ
    int Number_CurrentNode;//��ǰ�ڵ��ţ��������dot
    map<string,int>ClassSample_CurrentNode;//��ǰ�ڵ����������Լ����Ӧ��Ŀ
    double BestSignDivide_CurrentNode;//��ǰ�ڵ���ѻ��ֱ��
    map<string, Tree_Node *> childNode;//�ӽڵ�
};
