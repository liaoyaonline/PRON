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
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute,map<string, vector<string>> data_AttValues, vector<string> temp_remain);
	//�����ӽڵ�
	void generate_ChildNode();
	//������Ϣ���� Ѱ�����Ż�������
	string findBestAttribute();
    //Ѱ�����t
    double findBestT(string Att);
	//������Ϣ��
	double calc_Entropy(map<string, vector<string>> temp_Table);
	//���ýڵ������
	void set_NodeAttribute(string atttribute);
	//�����������ԣ������ݽ��з���
	string findClass(map<string,string> attributes);
    string readAttr();
    int attribute_Num;
    string node_Attribute;
    vector<string> node_AttValues;
    map<string, Tree_Node *> childNode;
    vector<string> data_Attribute;
    map<string, vector<string>> data_AttValues;
    map<string, vector<string>> MyDateTable;
	virtual ~Tree_Node();
private:
	//���Ը��������������
	//int attribute_Num;
	//���ڵ������
	//string node_Attribute;
	//���ݼ�����
	//vector<string> data_Attribute;
	//���ڵ����������ֵ
	//vector<string> node_AttValues;
	//ʣ�����Լ�
	vector<string>remain_Attributes;
	//�ӽڵ�,���ڵ����Զ�Ӧ������ֵ���ӽڵ��ַ����һһӳ��
	//Ϊ��˵���ýڵ�ΪҶ�ڵ�
	//map<string, Tree_Node *> childNode;
	//�������ϱ�
	//map<string, vector<string>> MyDateTable;
	//��������Զ�Ӧ������ֵ
	//map<string, vector<string>> data_AttValues;
};
