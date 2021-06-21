#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;


class Tree_Node
{
public:
    Tree_Node();
	//构造函数，参数依次为：数据集表（西瓜数据表）、西瓜所有的属性包括类别、每个属性可能的取值构成的表、剩余的未被划分的属性
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute);
	//生成子节点
	void generate_ChildNode();
	//计算信息增益 寻找最优划分属性
	string findBestAttribute();
    //寻找最佳划分节点
	void FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide);
	double Calculation_GiniValue(map<string, vector<string>> Table_Data);//计算Gini值
    //设置节点的属性
	void SetNodeAttribute(string atttribute);
	//根据所给属性，预测数据类型
    string PredictionSampleClass(map<string,string> Set_AttSample);
	void PrintDecisionTree(Tree_Node* root);
    void GenerateDot(Tree_Node* root);
    virtual ~Tree_Node();
private:
    map<string, vector<string>> Table_Data;//数据表
    vector<string> FullAtt_DataTable;//本节点的所有属性包括类别
    string Attribute_CurrentNode;//当前节点属性
    double GiniValue_CurrentNode;//当前节点Gini值
    double NumSample_CurrentNode;//当前节点样本数目
    int NumAtt_CurrentNode;//当前节点属性数目
    int Number_CurrentNode;//当前节点编号，用于输出dot
    map<string,int>ClassSample_CurrentNode;//当前节点样本类型以及其对应数目
    double BestSignDivide_CurrentNode;//当前节点最佳划分标记
    map<string, Tree_Node *> childNode;//子节点
};
