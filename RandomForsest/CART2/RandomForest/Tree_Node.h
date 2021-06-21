#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;


class Tree_Node
{
public:
    Tree_Node();
	//���캯������������Ϊ�����ݼ����������ݱ����������е����԰������ÿ�����Կ��ܵ�ȡֵ���ɵı�ʣ���δ�����ֵ�����
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute);
	//�����ӽڵ�
	void generate_ChildNode();
	//������Ϣ���� Ѱ�����Ż�������
	string findBestAttribute();
    //Ѱ����ѻ��ֽڵ�
	void FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide);
	double Calculation_GiniValue(map<string, vector<string>> Table_Data);//����Giniֵ
    //���ýڵ������
	void SetNodeAttribute(string atttribute);
	//�����������ԣ�Ԥ����������
    string PredictionSampleClass(map<string,string> Set_AttSample);
	void PrintDecisionTree(Tree_Node* root);
    void GenerateDot(Tree_Node* root);
    virtual ~Tree_Node();
private:
    map<string, vector<string>> Table_Data;//���ݱ�
    vector<string> FullAtt_DataTable;//���ڵ���������԰������
    string Attribute_CurrentNode;//��ǰ�ڵ�����
    double GiniValue_CurrentNode;//��ǰ�ڵ�Giniֵ
    double NumSample_CurrentNode;//��ǰ�ڵ�������Ŀ
    int NumAtt_CurrentNode;//��ǰ�ڵ�������Ŀ
    int Number_CurrentNode;//��ǰ�ڵ��ţ��������dot
    map<string,int>ClassSample_CurrentNode;//��ǰ�ڵ����������Լ����Ӧ��Ŀ
    double BestSignDivide_CurrentNode;//��ǰ�ڵ���ѻ��ֱ��
    map<string, Tree_Node *> childNode;//�ӽڵ�
};
