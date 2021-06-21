/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 实现CART决策树
*       20200810 输出符合规则的决策树的dot方便可视化决策树
*       20200817 解决NumSample无法输出类型数量为0的类型问题*
*       20200831 实现通过度量GiniImportance从而对特征的重要性进行度量
*                详情见FeatureGini和PrintTree函数中的新增部分
*                                                                           *
*                                                                           *
*                                                                           *
***************************************************************************/ 
#include "Tree_Node.h"
#include <math.h>
#include <sstream>
#include <fstream>
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
    BestSignDivide_CurrentNode = 0;
    NumSample_CurrentNode = (int)Table_Data[FullAtt_DataTable[0]].size();//计算当前节点样本数目
    NumAtt_CurrentNode = FullAtt_DataTable.size() - 1;
    GiniValue_CurrentNode = Calculation_GiniValue(Table_Data);//计算当前节点Gini值
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
    map<string,vector<string>> DataTable_plus_signdivide;
    map<string,vector<string>> DataTable_less_signdivide;
    for(int i = 0; i < Table_Data[Attribute_CurrentNode].size(); i++)//对于每个样本
    {
        stringstream ss;
        double AttValue_tmp;
        ss << Table_Data[Attribute_CurrentNode][i];
        ss >> AttValue_tmp;
        if( AttValue_tmp <= BestSignDivide_CurrentNode )
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
    if(DataTable_less_signdivide.empty())//Gini理论上不会出现空的，会一直划分到只有一种类型为止,反正不会用到这里，姑且听之
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
    double BestSign_Divide;
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
        FeatureGini_CurrentNode[Att_selected] = Gini_BestSignDivide_tmp;//0830修改，用于对特征重要性排序
        if(Gini_BestSignDivide_tmp >= 1)
        {
            cout << "Gini_BestSignDivide_tmp: " << Gini_BestSignDivide_tmp << "Att_selected: " << Att_selected << endl;
            cout <<" 读取当前样本集合" << endl;
            for(auto iter = Table_Data.rbegin(); iter != Table_Data.rend(); iter++)
            {
                cout << iter->first << endl;
                for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
                {
                    cout << *it << "-----";
                }
                cout << endl;

            }
        }
        if(Gini_BestSignDivide_tmp <= Gini_BestMin)
        {
            Gini_BestMin = Gini_BestSignDivide_tmp;
            BestAtt = Att_selected;
            BestSign_Divide = BestSign_Divide_tmp;
        }
    }
    BestSignDivide_CurrentNode = BestSign_Divide;//给当前节点最佳划分标记赋值
    return BestAtt;
    
}
void Tree_Node::FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide)//对于某个特征的取值恒定怎么处理
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
    if(Value_Att_selected.size() == 1)//假如该特征在该样本集合取值相同
    {
        *Gini_BestSignDivide = GiniValue_CurrentNode;
        *BestSign_Divide = Value_Att_selected[0];
        return;
    }
    for(int i = 0; i < Value_Att_selected.size() - 1; i++)
    {
        double Value_temp;//中间过渡变量
        Value_temp = (Value_Att_selected[i] + Value_Att_selected[i+1]) / 2;
        Value_sign_divide.push_back(Value_temp);
    }
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
        for(int j = 0; j < NumSample_CurrentNode;j++)//对于所有样本划分为plus和less两个数据集
        {
            stringstream ss;
            double Value_temp;//中间过渡变量
            ss << Table_Data[Att_selected][j];//不能使用取值集合而是要用map里面的集合
            ss >> Value_temp;
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
        Proportion_plus_signdivide = (double)Num_plus_signdivide / (double)NumSample_CurrentNode;
        Proportion_less_signdivide = (double)Num_less_signdivide / (double)NumSample_CurrentNode;
        double GiniPlus_tmp = Calculation_GiniValue(DataTable_plus_signdivide);
        double Giniless_tmp = Calculation_GiniValue(DataTable_less_signdivide);
        Gini_tmp = (Proportion_plus_signdivide * GiniPlus_tmp) + (Proportion_less_signdivide * Giniless_tmp);
        if(Gini_tmp <= Gini_min_signdivide)
        {
            Gini_min_signdivide = Gini_tmp;
            BestSign_Divide_tmp = sign_divide;
        }
    }
    *BestSign_Divide = BestSign_Divide_tmp;
    *Gini_BestSignDivide = Gini_min_signdivide;
}


double Tree_Node::Calculation_GiniValue(map<string, vector<string>> Table_Data)//假如是空集..计算的是输入的集合GIni值
{
    if(Table_Data.empty())
        return 0;
    map<string,vector<string>> TableData_tmp = Table_Data;
    double Gini;
    int NumSample_tmp = TableData_tmp[FullAtt_DataTable[0]].size();
    map<string,int>ClassNum_tmp;
    for (int i = 0; i < NumSample_tmp; i++)
    {
        string Class_tmp;
        Class_tmp = TableData_tmp[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ClassNum_tmp[Class_tmp]++;
    }
    for(auto it = ClassNum_tmp.begin(); it != ClassNum_tmp.end(); it++)
    {
        double p_Class;
        p_Class = (double) it->second / NumSample_tmp;
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
        double ValueAtt_CheckSample;
        string Value_AttCurrentSample;
        stringstream ss;
        ss << Set_AttSample[Attribute_CurrentNode];
        ss >> ValueAtt_CheckSample;
        if(ValueAtt_CheckSample <= BestSignDivide_CurrentNode)
        {
            string signless_tmp = "<";
            string string_tmp;
            string AttValue_tmp;
            stringstream ss;
            ss << BestSignDivide_CurrentNode;
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
            ss << BestSignDivide_CurrentNode;
            ss >> string_tmp;
            Value_AttCurrentSample = signplus_tmp + string_tmp;
            Tree_Node *p = childNode[Value_AttCurrentSample];
            return p->PredictionSampleClass(Set_AttSample);
        }
    }

}
void Tree_Node::PrintDecisionTree(Tree_Node* root)
{
    vector<Tree_Node*> Queue_TreeNode;
    Tree_Node* Node_Current;
    Tree_Node* Node_Child;
    Queue_TreeNode.push_back(root);
    map<string,double>FeatureGini_tmp;//用于对特征作用排序
    int Num_AllSample = root->NumSample_CurrentNode;
    cout << "Num_AllSample: " << Num_AllSample << endl;
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        cout << "当前节点属性:  " << Node_Current->Attribute_CurrentNode << endl;
        cout << "当前节点最佳划分选择值:  "<< Node_Current->BestSignDivide_CurrentNode << endl;
        cout << "当前样本数目:  "<< Node_Current->NumSample_CurrentNode << endl;
        cout << "当前节点属性数目: " << Node_Current->NumAtt_CurrentNode << endl;
        cout << "当前节点Gini值： " << Node_Current->GiniValue_CurrentNode << endl;
        //map<string,int>::iterator it = Node_Current->ClassSample_CurrentNode.begin();
        for(auto it = Node_Current->ClassSample_CurrentNode.begin();it != Node_Current->ClassSample_CurrentNode.end(); it++)
        {
            cout << it->first<< "： " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->FeatureGini_CurrentNode.begin();it != Node_Current->FeatureGini_CurrentNode.end(); it++)//0831用于计算GiniImportance
        {
            cout << it->first<< "： " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->FeatureGini_CurrentNode.begin();it != Node_Current->FeatureGini_CurrentNode.end(); it++)//0831用于计算GiniImportance
        {
            double Rate_Sample = (double)Node_Current->NumSample_CurrentNode / (double)Num_AllSample;
            double EffectGini_tmp = Node_Current->GiniValue_CurrentNode - it->second;
            double GiniImportance_tmp = Rate_Sample * EffectGini_tmp;
            cout <<"Att_selected: " << it->first << "    Rate_Sample: " << Rate_Sample << "    EffectGini_tmp: " << EffectGini_tmp << "   GiniImportance_tmp: " << GiniImportance_tmp << endl;
            FeatureGini_tmp[it->first] += GiniImportance_tmp;
        }
        for(auto it = FeatureGini_tmp.begin();it != FeatureGini_tmp.end(); it++)
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
    for(auto it = FeatureGini_tmp.begin();it != FeatureGini_tmp.end(); it++)
    {
        cout << it->first<< "： " << it->second << "  ";
    }
    cout << endl;
}
void Tree_Node::GenerateDot(Tree_Node* root)
{
    vector<Tree_Node*> Queue_TreeNode;
    Tree_Node* Node_Current;
    Tree_Node* Node_Child;
    Queue_TreeNode.push_back(root);
    int NumLabel = 0;
    int Number_Node = 1;
    root->Number_CurrentNode = 0;
    ofstream outfile;
    outfile.open("TreeData.dot");
    outfile << "digraph Tree {"<< endl << "node [shape=box] ;"<< endl;
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        outfile << NumLabel << " [label=\"" <<Node_Current->Attribute_CurrentNode;
        if(Node_Current->GiniValue_CurrentNode != 0)
            outfile<< " <= "<<Node_Current->BestSignDivide_CurrentNode;
        outfile << "\\nGini = "<< Node_Current->GiniValue_CurrentNode<<"\\n"<<"Num_Sample = "<< Node_Current->NumSample_CurrentNode << "\\n" ;
        outfile << "Value = [";
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        outfile << Node_Current->ClassSample_CurrentNode[it->first];
        it++;
        for(;it != ClassSample_CurrentNode.end(); it++)//输出每个类型的数目包括0
        {
            outfile << ", "<< Node_Current->ClassSample_CurrentNode[it->first];
        }
        outfile << "]\"] ;"<< endl;
        for(auto it = Node_Current->childNode.rbegin(); it != Node_Current->childNode.rend(); it++)
        {
            Node_Child = it->second;
            Node_Child->Number_CurrentNode = Number_Node;
            outfile << NumLabel <<" -> " << Node_Child->Number_CurrentNode;
            if(Node_Current->Number_CurrentNode == 0 && Node_Child->Number_CurrentNode == 1)
                outfile << " [labeldistance=2.5, labelangle=45, headlabel=\"False\"] ;";
            if(Node_Current->Number_CurrentNode == 0 && Node_Child->Number_CurrentNode == 2)
                outfile << " [labeldistance=2.5, labelangle=-45, headlabel=\"True\"] ;";
            outfile << endl;
            Number_Node ++;
            Queue_TreeNode.push_back(it->second);
        }
        NumLabel ++;
        auto Delete_Node = Queue_TreeNode.begin();
        Queue_TreeNode.erase(Delete_Node);
    }
    outfile << "}";
    outfile.close();
}
Tree_Node::~Tree_Node()
{
}
