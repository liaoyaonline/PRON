#include "Tree_Node.h"
#include <math.h>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

Tree_Node::Tree_Node(map<string, vector<string>> temp_Table,vector<string> temp_Attribute)
{
	//全部属性,包括类别
	FullAtt_DataTable = temp_Attribute;
	//属性表
	Table_Data = temp_Table;
    if(!Table_Data.empty())
        GiniValue_CurrentNode = Calculation_GiniValue(Table_Data);//计算当前节点Gini值
    NumSample_CurrentNode = (int)Table_Data[FullAtt_DataTable[0]].size();//计算当前节点样本数目
    NumAtt_CurrentNode = FullAtt_DataTable.size() - 1;
    for(int i = 0; i < NumSample_CurrentNode; i++)
    {
        string Class_tmp;
        Class_tmp = Table_Data[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ClassSample_CurrentNode[Class_tmp]++;
    }
}
Tree_Node::Tree_Node()
{
    //cout << "A Tree..." << endl;
}
void Tree_Node::generate_ChildNode()
{
    vector<string> AttDataTable_tmp;//去除类别后的属性集合
    AttDataTable_tmp = FullAtt_DataTable;
    AttDataTable_tmp.pop_back();
    if (ClassSample_CurrentNode.size() == 1)//第一种情况，判定时不时所有类别相同
    {
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        Attribute_CurrentNode = it->first;
        return;
    }
    bool AttValues_AllSame = false;
    for (int i = 0; i < AttDataTable_tmp.size(); i++)
    {
        AttValues_AllSame = true;
        vector<string> AttValues_temp;
        AttValues_temp = Table_Data[AttDataTable_tmp[i]];
        for(int j = 1; j < AttValues_temp.size(); j++)
        {
            if (AttValues_temp[0] != AttValues_temp[j])
            {
                AttValues_AllSame = false;
                break;
            }
        }
        if (AttValues_AllSame == false)
        {
            break;
        }
    }
    if (AttDataTable_tmp.empty() || AttValues_AllSame)//第二三种情况如果属性取值相同或者样本属性集合为空
    {
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        int MaxNum_Class = it -> second;
        for(; it != ClassSample_CurrentNode.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNum_Class)//>=保证至少有一个值被赋予Att_CurrentNode
            {
                Attribute_CurrentNode = it->first;
                MaxNum_Class = ClassNum_tmp;
            }
        }
        return;
    }
    Attribute_CurrentNode = findBestAttribute();
    cout << "FindBestAttribute : " << Attribute_CurrentNode << endl;
    map<string,vector<string>> DataTable_plus_signdivide;
    map<string,vector<string>> DataTable_less_signdivide;
    for(int i = 0; i < Table_Data[Attribute_CurrentNode].size(); i++)
    {
        stringstream ss;
        double AttValue_temp;
        ss << Table_Data[Attribute_CurrentNode][i];
        ss >> AttValue_temp;
        if( AttValue_temp <= BestSignDivide_CurrentNode )
        {
            for (int m = 0; m < FullAtt_DataTable.size(); m++)
            {
                DataTable_less_signdivide[FullAtt_DataTable[m]].push_back(Table_Data[FullAtt_DataTable[m]][i]);
            }
            
        }
        else
        {
            for (int m = 0; m < FullAtt_DataTable.size(); m++)
            {
                DataTable_plus_signdivide[FullAtt_DataTable[m]].push_back(Table_Data[FullAtt_DataTable[m]][i]);
            }
        }
    }
    if(DataTable_less_signdivide.empty())
    {
        string signless_tmp = "<";
        string string_tmp;
        string AttValue_tmp;
        string AttChildNode_tmp;
        stringstream ss;
        ss << BestSignDivide_CurrentNode;
        ss >> string_tmp;
        AttValue_tmp = signless_tmp + string_tmp;
        Tree_Node *p = new Tree_Node(DataTable_less_signdivide,FullAtt_DataTable);
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        int MaxNumclass_tmp = it -> second;
        for(; it != ClassSample_CurrentNode.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNumclass_tmp)
            {
                AttChildNode_tmp = it->first;
                MaxNumclass_tmp = ClassNum_tmp;
            }
        }
        p->SetNodeAttribute(AttChildNode_tmp);
        childNode[AttValue_tmp] = p;
    }
    else//如果样本属性内还有值，就不需要去除属性
    {
        string signless_tmp = "<";
        string string_tmp;
        string AttValue_tmp;
        stringstream ss;
        ss << BestSignDivide_CurrentNode;
        ss >> string_tmp;
        AttValue_tmp = signless_tmp + string_tmp;
        Tree_Node *pt = new Tree_Node(DataTable_less_signdivide,FullAtt_DataTable);
        childNode[AttValue_tmp] = pt;
        pt->generate_ChildNode();
    }
    if(DataTable_plus_signdivide.empty())
    {
        string signplus_tmp = ">";
        string string_tmp;
        string AttValue_tmp;
        string AttChildNode_tmp;
        stringstream ss;
        ss << BestSignDivide_CurrentNode;
        ss >> string_tmp;
        AttValue_tmp = signplus_tmp + string_tmp;
        Tree_Node *p = new Tree_Node(DataTable_plus_signdivide,FullAtt_DataTable);
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        int MaxNumclass_tmp = it -> second;
        for(; it != ClassSample_CurrentNode.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNumclass_tmp)
            {
                AttChildNode_tmp = it->first;
                MaxNumclass_tmp = ClassNum_tmp;
            }
        }
        p->SetNodeAttribute(AttChildNode_tmp);
        childNode[AttValue_tmp] = p;
    }
    else//如果样本属性内还有值，就不需要去除属性
    {
        string signplus_tmp = ">";
        string string_tmp;
        string AttValue_tmp;
        stringstream ss;
        ss << BestSignDivide_CurrentNode;
        ss >> string_tmp;
        AttValue_tmp = signplus_tmp + string_tmp;
        Tree_Node *pt = new Tree_Node(DataTable_plus_signdivide,FullAtt_DataTable);
        childNode[AttValue_tmp] = pt;
        pt->generate_ChildNode();

    }
}

//寻找最优划分
string Tree_Node::findBestAttribute()
{
    string BestAtt;//最佳属性
    double Gini_BestMin = 1;//最低基尼指数
    vector<string> AttDataTable_tmp;//去除类别后的属性集合
    AttDataTable_tmp = FullAtt_DataTable;
    AttDataTable_tmp.pop_back();
    for(int i = 0; i < AttDataTable_tmp.size(); i++)
    {
        string Att_selected;//被选中的属性
        Att_selected = AttDataTable_tmp[i];
        double BestSign_Divide_tmp;
        double Gini_BestSignDivide_tmp;
        double *pbestsign = &BestSign_Divide_tmp;
        double *pginibestsign = &Gini_BestSignDivide_tmp;
        FindBestDivideSign(Att_selected,pbestsign,pginibestsign);
        if(Gini_BestSignDivide_tmp <= Gini_BestMin)
        {
            Gini_BestMin = Gini_BestSignDivide_tmp;
            BestAtt = Att_selected;
        }
    }
    BestSignDivide_CurrentNode = Gini_BestMin;//给当前节点最佳划分标记赋值
    return BestAtt;
    
}
void Tree_Node::FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide)
{
    string Att_selected;//被选中的属性
    Att_selected = Attribute;
    vector<double> Value_Att_selected;//被选中属性的取值集合
    double sign_divide = 0.0;//划分标记
    vector<double> Value_sign_divide;//划分标记取值集合
    double Gini_min_signdivide = 1;
    double BestSign_Divide_tmp;
    map<string,vector<string>> TableAttValue_tmp;//属性和取值对应表
    for(auto iter = Table_Data.rbegin();iter != Table_Data.rend();iter++)
    {
        set<string> attValues;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            attValues.insert(*it);
        for(auto it=attValues.begin();it != attValues.end();it++)
            TableAttValue_tmp[iter->first].push_back(*it);
    }
    for(int i = 0; i < TableAttValue_tmp[Att_selected].size();i++)
    {
        stringstream ss;
        string string_temp;//中间过渡使用变量
        double Value_temp;//中间过渡使用变量
        string_temp = TableAttValue_tmp[Att_selected][i];
        ss << string_temp;
        ss >> Value_temp;
        Value_Att_selected.push_back(Value_temp);
    }
    sort(Value_Att_selected.begin(),Value_Att_selected.end());
    /*cout << "校验被选中的属性的取值集合按小到大排列是否正确" << endl;
    for(int i = 0; i < Value_Att_selected.size();i++)
        cout << Value_Att_selected[i] << "---";
    cout << endl;*/
    for(int i = 0; i < Value_Att_selected.size() - 1; i++)
    {
        double Value_temp;//中间过渡变量
        Value_temp = (Value_Att_selected[i] + Value_Att_selected[i+1]) / 2;
        Value_sign_divide.push_back(Value_temp);
    }
    /*cout << "校验被选中的属性值："<< Att_selected << "可能的划分集合按从小到大排列是否正确" << endl;
    for(int i = 0; i < Value_sign_divide.size(); i++)
        cout << Value_sign_divide[i] << "-----";
    cout << endl;*/
    for(int i = 0; i < Value_sign_divide.size(); i++)
    {
        sign_divide = Value_sign_divide[i];
        int Num_plus_signdivide = 0;//存储大于标记的样本数量
        int Num_less_signdivide = 0;
        double Proportion_plus_signdivide = 0;//大于标记的样本占总样本比例
        double Proportion_less_signdivide = 0;
        double Gini_tmp;//中间过渡变量
        map<string,vector<string>> DataTable_plus_signdivide;//存储大于标记的样本数据
        map<string,vector<string>> DataTable_less_signdivide;
        cout << "Value_temp:";
        for(int j = 0; j < Value_Att_selected.size();j++)
        {
            stringstream ss;
            double Value_temp;//中间过渡变量
            ss << Value_Att_selected[j];
            ss >> Value_temp;
            cout << Value_temp << "-----";
            if(Value_temp <= sign_divide)
            {
                Num_less_signdivide ++;
                for(int k = 0; k < FullAtt_DataTable.size(); k++)
                {
                    DataTable_less_signdivide[FullAtt_DataTable[k]].push_back(Table_Data[FullAtt_DataTable[k]][j]);
                }
            }
            else
            {
                Num_plus_signdivide ++;
                for(int k = 0; k < FullAtt_DataTable.size(); k++)
                {
                    DataTable_plus_signdivide[FullAtt_DataTable[k]].push_back(Table_Data[FullAtt_DataTable[k]][j]);
                }
            }
        }
        cout << endl;
        Proportion_plus_signdivide = (double)Num_plus_signdivide / (double)NumSample_CurrentNode;
        Proportion_less_signdivide = (double)Num_less_signdivide / (double)NumSample_CurrentNode;
        Gini_tmp = (Proportion_plus_signdivide * Calculation_GiniValue(DataTable_plus_signdivide)) + (Proportion_less_signdivide * Calculation_GiniValue(DataTable_less_signdivide));
        cout << "Num_plus_signdivide:  "<< Num_plus_signdivide << "  Num_less_signdivide:  " << Num_less_signdivide << "  N:  "<< NumSample_CurrentNode << endl;
        cout << "Proportion_plus_signdivide: "<< Proportion_plus_signdivide<< "  Proportion_less_signdivide:   " << Proportion_less_signdivide << endl;
        cout << "Calculation_GiniValue(DataTable_plus_signdivide):   "<<Calculation_GiniValue(DataTable_plus_signdivide) << "   Calculation_GiniValue(DataTable_less_signdivide):   "<< Calculation_GiniValue(DataTable_less_signdivide)<< endl;;
        cout<<"当前属性：" << Att_selected <<"sign_divide : " << sign_divide <<"Gini_tmp:  " << Gini_tmp << endl;
        cout << endl;
        if(Gini_tmp <= Gini_min_signdivide)
        {
            Gini_min_signdivide = Gini_tmp;
            BestSign_Divide_tmp = sign_divide;
        }
    }
    *BestSign_Divide = BestSign_Divide_tmp;
    *Gini_BestSignDivide = Gini_min_signdivide;
}


double Tree_Node::Calculation_GiniValue(map<string, vector<string>> Table_Data)
{
    map<string,vector<string>> TableData_tmp = Table_Data;
    double Gini;
    map<string,int>Table_ClassNum;
    for (int i = 0; i < NumSample_CurrentNode; i++)
    {
        string Class_tmp;
        Class_tmp = TableData_tmp[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        Table_ClassNum[Class_tmp]++;
    }
    for(auto it = Table_ClassNum.begin(); it != Table_ClassNum.end(); it++)
    {
        double p_Class;
        p_Class = (double) it->second / NumSample_CurrentNode;
        Gini +=(p_Class * p_Class);
    }
    Gini = 1 - Gini;
    return Gini;
}

void Tree_Node::SetNodeAttribute(string attribute)
{
	//设置节点的属性
	this->Attribute_CurrentNode = attribute;
}

//输入为待预测样本的所有属性集合
string Tree_Node::PredictionSampleClass(map<string,string> Set_AttSample)
{
    if(childNode.size() == 0)
    {
        return Attribute_CurrentNode;
    }
    else
    {
        string Value_AttCurrentSample;
        double Value_AttCurrentSample_tmp;
        double BestSign_Divide_tmp;
        double Gini_BestSignDivide_tmp;
        double *pbestsign = &BestSign_Divide_tmp;
        double *pginibestsign = &Gini_BestSignDivide_tmp;
        FindBestDivideSign(Attribute_CurrentNode,pbestsign,pginibestsign);
        stringstream ss;
        ss << Set_AttSample[Attribute_CurrentNode];
        ss >> Value_AttCurrentSample_tmp;
        if(Value_AttCurrentSample_tmp <= BestSign_Divide_tmp)
        {
            string signless_tmp = "<";
            string string_tmp;
            string AttValue_tmp;
            stringstream ss;
            ss << BestSign_Divide_tmp;
            ss >> string_tmp;
            Value_AttCurrentSample = signless_tmp + string_tmp;
            Tree_Node *p = childNode[Value_AttCurrentSample];
            return p->PredictionSampleClass(Set_AttSample);
        }
        else
        {
            string signplus_tmp = ">";
            string string_tmp;
            string AttValue_tmp;
            stringstream ss;
            ss << BestSign_Divide_tmp;
            ss >> string_tmp;
            Value_AttCurrentSample = signplus_tmp + string_tmp;
            Tree_Node *p = childNode[Value_AttCurrentSample];
            return p->PredictionSampleClass(Set_AttSample);
        }
    }

}
Tree_Node::
Tree_Node::~Tree_Node()
{
}
void Tree_Node::PrintDecisionTree(Tree_Node* root)
{
    vector<Tree_Node*> Queue_TreeNode;
    Tree_Node* Node_Current;
    Tree_Node* Node_Child;
    Queue_TreeNode.push_back(root);
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        cout << "当前节点属性:  " << Attribute_CurrentNode << endl;
        cout << "当前节点最佳划分选择值:  "<< BestSignDivide_CurrentNode << endl;
        cout << "当前样本数目:  "<< NumSample_CurrentNode << endl;
        cout << "当前节点属性数目: " << NumAtt_CurrentNode << endl;
        cout << "当前节点Gini值： " << GiniValue_CurrentNode << endl;
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        for(;it != ClassSample_CurrentNode.end(); it++)
        {
            cout << it->first<< "： " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->childNode.rbegin(); it != Node_Current->childNode.rend(); it++)
        {
            Node_Child = it->second;
            cout << "Att_CurrentNode : "<< it->first<< "->Att_ChildNode: " << Node_Child->Attribute_CurrentNode << endl;
            cout << "该子节点样本数目: " << Node_Child->NumSample_CurrentNode << endl;
            Queue_TreeNode.push_back(it->second);
        }
        cout << endl << endl;
        auto Delete_Node = Queue_TreeNode.begin();
        Queue_TreeNode.erase(Delete_Node);
    }
}
