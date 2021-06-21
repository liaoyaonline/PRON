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
	//ȫ������,�������
	FullAtt_DataTable = temp_Attribute;
	//���Ա�
	Table_Data = temp_Table;
    NumSample_CurrentNode = (int)Table_Data[FullAtt_DataTable[0]].size();//���㵱ǰ�ڵ�������Ŀ
    NumAtt_CurrentNode = FullAtt_DataTable.size() - 1;
    GiniValue_CurrentNode = Calculation_GiniValue(Table_Data);//���㵱ǰ�ڵ�Giniֵ
    for(int i = 0; i < NumSample_CurrentNode; i++)
    {
        string Class_tmp;
        Class_tmp = Table_Data[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ClassSample_CurrentNode[Class_tmp]++;
    }
    cout << "��ǰ������Ŀ:  "<< NumSample_CurrentNode << endl;
    cout << "��ǰ�ڵ�������Ŀ: " << NumAtt_CurrentNode << endl;
    cout << "��ǰ�ڵ�Giniֵ�� " << GiniValue_CurrentNode << endl;
    map<string,int>::iterator it = ClassSample_CurrentNode.begin();
    for(;it != ClassSample_CurrentNode.end(); it++)
    {
        cout << it->first<< "�� " << it->second << "  "<< endl;
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
    ofstream outfile;
    outfile.open("TreeData.dot");
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        outfile << NumLabel << endl;
        for(auto it = Node_Current->childNode.rbegin(); it != Node_Current->childNode.rend(); it++)
        {
            Node_Child = it->second;
            outfile << NumLabel <<" -> " << Number_Node << endl;
            Number_Node ++;
            Queue_TreeNode.push_back(it->second);
        }
        NumLabel ++;
        auto Delete_Node = Queue_TreeNode.begin();
        Queue_TreeNode.erase(Delete_Node);
    }
    outfile.close();
}
Tree_Node::Tree_Node()
{
    //cout << "A Tree..." << endl;
}
void Tree_Node::generate_ChildNode()
{
    vector<string> AttDataTable_tmp;//ȥ����������Լ���
    AttDataTable_tmp = FullAtt_DataTable;
    AttDataTable_tmp.pop_back();
    if (ClassSample_CurrentNode.size() == 1)//��һ��������ж�ʱ��ʱ���������ͬ
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
    if (AttDataTable_tmp.empty() || AttValues_AllSame)//�ڶ���������������ȡֵ��ͬ�����������Լ���Ϊ��
    {
        map<string,int>::iterator it = ClassSample_CurrentNode.begin();
        int MaxNum_Class = it -> second;
        for(; it != ClassSample_CurrentNode.end(); it++)
        {
            int ClassNum_tmp = it->second;
            if(ClassNum_tmp >= MaxNum_Class)//>=��֤������һ��ֵ������Att_CurrentNode
            {
                Attribute_CurrentNode = it->first;
                MaxNum_Class = ClassNum_tmp;
            }
        }
        return;
    }
    Attribute_CurrentNode = findBestAttribute();
    cout << "FindBestAttribute : " << Attribute_CurrentNode << endl;
    cout << "GiniValue_CurrentNode:  " << GiniValue_CurrentNode << endl;
    cout << "BestSignDivide_CurrentNode: " << BestSignDivide_CurrentNode << endl;
    map<string,vector<string>> DataTable_plus_signdivide;
    map<string,vector<string>> DataTable_less_signdivide;
    for(int i = 0; i < Table_Data[Attribute_CurrentNode].size(); i++)
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
    else//������������ڻ���ֵ���Ͳ���Ҫȥ������
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
    else//������������ڻ���ֵ���Ͳ���Ҫȥ������
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

//Ѱ�����Ż���
string Tree_Node::findBestAttribute()
{
    string BestAtt;//�������
    double BestSign_Divide;
    double Gini_BestMin = 1;//��ͻ���ָ��
    vector<string> AttDataTable_tmp;//ȥ����������Լ���
    AttDataTable_tmp = FullAtt_DataTable;
    AttDataTable_tmp.pop_back();
    for(int i = 0; i < AttDataTable_tmp.size(); i++)
    {
        string Att_selected;//��ѡ�е�����
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
            BestSign_Divide = BestSign_Divide_tmp;
        }
    }
    BestSignDivide_CurrentNode = BestSign_Divide;//����ǰ�ڵ���ѻ��ֱ�Ǹ�ֵ
    return BestAtt;
    
}
void Tree_Node::FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide)
{
    string Att_selected;//��ѡ�е�����
    Att_selected = Attribute;
    vector<double> Value_Att_selected;//��ѡ�����Ե�ȡֵ����
    double sign_divide = 0.0;//���ֱ��
    vector<double> Value_sign_divide;//���ֱ��ȡֵ����
    double Gini_min_signdivide = 1;
    double BestSign_Divide_tmp;
    map<string,vector<string>> TableAttValue_tmp;//���Ժ�ȡֵ��Ӧ��
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
        string string_temp;//�м����ʹ�ñ���
        double Value_temp;//�м����ʹ�ñ���
        string_temp = TableAttValue_tmp[Att_selected][i];
        ss << string_temp;
        ss >> Value_temp;
        Value_Att_selected.push_back(Value_temp);
    }
    sort(Value_Att_selected.begin(),Value_Att_selected.end());
    cout << "У�鱻ѡ�е����Ե�ȡֵ���ϰ�С���������Ƿ���ȷ" << endl;
    for(int i = 0; i < Value_Att_selected.size();i++)
        cout << Value_Att_selected[i] << "---";
    cout << endl;
    for(int i = 0; i < Value_Att_selected.size() - 1; i++)
    {
        double Value_temp;//�м���ɱ���
        Value_temp = (Value_Att_selected[i] + Value_Att_selected[i+1]) / 2;
        Value_sign_divide.push_back(Value_temp);
    }
    cout << "У�鱻ѡ�е�����ֵ��"<< Att_selected << "���ܵĻ��ּ��ϰ���С���������Ƿ���ȷ" << endl;
    for(int i = 0; i < Value_sign_divide.size(); i++)
        cout << Value_sign_divide[i] << "-----";
    cout << endl;
    for(int i = 0; i < Value_sign_divide.size(); i++)
    {
        sign_divide = Value_sign_divide[i];
        int Num_plus_signdivide = 0;//�洢���ڱ�ǵ���������
        int Num_less_signdivide = 0;
        double Proportion_plus_signdivide = 0;//���ڱ�ǵ�����ռ����������
        double Proportion_less_signdivide = 0;
        double Gini_tmp;//�м���ɱ���
        map<string,vector<string>> DataTable_plus_signdivide;//�洢���ڱ�ǵ���������
        map<string,vector<string>> DataTable_less_signdivide;
        cout << "Value_temp:";
        for(int j = 0; j < Value_Att_selected.size();j++)
        {
            stringstream ss;
            double Value_temp;//�м���ɱ���
            ss << Table_Data[Att_selected][j];//����ʹ��ȡֵ���϶���Ҫ��map����ļ���
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
        cout << "���DataTable_plus_signdivide��������: " << endl;
        for(auto iter = DataTable_plus_signdivide.rbegin(); iter != DataTable_plus_signdivide.rend(); iter++)
        {
            cout << iter->first << endl;
            for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
            {
                cout << *it << "-----";
            }
            cout << endl;
        }
        cout << "���DataTable_less_signdivide��������: " << endl;
        for(auto iter = DataTable_less_signdivide.rbegin(); iter != DataTable_less_signdivide.rend(); iter++)
        {
            cout << iter->first << endl;
            for(auto it = (iter->second).begin(); it != (iter->second).end(); it++)
            {
                cout << *it << "-----";
            }
            cout << endl;
        }
        Proportion_plus_signdivide = (double)Num_plus_signdivide / (double)NumSample_CurrentNode;
        Proportion_less_signdivide = (double)Num_less_signdivide / (double)NumSample_CurrentNode;
        double GiniPlus_tmp = Calculation_GiniValue(DataTable_plus_signdivide);
        double Giniless_tmp = Calculation_GiniValue(DataTable_less_signdivide);
        Gini_tmp = (Proportion_plus_signdivide * GiniPlus_tmp) + (Proportion_less_signdivide * Giniless_tmp);
        cout << "Proportion_plus_signdivide: "<< Proportion_plus_signdivide<< "  Proportion_less_signdivide:   " << Proportion_less_signdivide << endl;
        cout << "Calculation_GiniValue(DataTable_plus_signdivide):   "<<GiniPlus_tmp << "   Calculation_GiniValue(DataTable_less_signdivide):   "<< Giniless_tmp<< endl;;
        cout<<"��ǰ���ԣ�" << Att_selected <<"sign_divide : " << sign_divide <<"Gini_tmp:  " << Gini_tmp << endl;
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


double Tree_Node::Calculation_GiniValue(map<string, vector<string>> Table_Data)//�����ǿռ�..�����������ļ���GIniֵ
{
    if(Table_Data.empty())
        return 0;
    map<string,vector<string>> TableData_tmp = Table_Data;
    double Gini;
    int NumSample_tmp = TableData_tmp[FullAtt_DataTable[0]].size();
    cout << "Calucation����������Ŀ" << NumSample_tmp << endl;
    map<string,int>ClassNum_tmp;
    for (int i = 0; i < NumSample_tmp; i++)
    {
        string Class_tmp;
        Class_tmp = TableData_tmp[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ClassNum_tmp[Class_tmp]++;
    }
    map<string,int>::iterator it = ClassNum_tmp.begin();
    for(;it != ClassNum_tmp.end(); it++)
    {
        cout << it->first<< "�� " << it->second << "  "<< endl;
    }
    cout << endl;
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
	//���ýڵ������
	this->Attribute_CurrentNode = attribute;
}

//����Ϊ��Ԥ���������������Լ���
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
void Tree_Node::PrintDecisionTree(Tree_Node* root)
{
    vector<Tree_Node*> Queue_TreeNode;
    Tree_Node* Node_Current;
    Tree_Node* Node_Child;
    Queue_TreeNode.push_back(root);
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        cout << "��ǰ�ڵ�����:  " << Node_Current->Attribute_CurrentNode << endl;
        cout << "��ǰ�ڵ���ѻ���ѡ��ֵ:  "<< Node_Current->BestSignDivide_CurrentNode << endl;
        cout << "��ǰ������Ŀ:  "<< Node_Current->NumSample_CurrentNode << endl;
        cout << "��ǰ�ڵ�������Ŀ: " << Node_Current->NumAtt_CurrentNode << endl;
        cout << "��ǰ�ڵ�Giniֵ�� " << Node_Current->GiniValue_CurrentNode << endl;
        map<string,int>::iterator it = Node_Current->ClassSample_CurrentNode.begin();
        for(;it != Node_Current->ClassSample_CurrentNode.end(); it++)
        {
            cout << it->first<< "�� " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->childNode.rbegin(); it != Node_Current->childNode.rend(); it++)
        {
            Node_Child = it->second;
            cout << "Att_CurrentNode : "<< it->first<< "->Att_ChildNode: " << Node_Child->Attribute_CurrentNode << endl;
            cout << "���ӽڵ�������Ŀ: " << Node_Child->NumSample_CurrentNode << endl;
            Queue_TreeNode.push_back(it->second);
        }
        cout << endl << endl;
        auto Delete_Node = Queue_TreeNode.begin();
        Queue_TreeNode.erase(Delete_Node);
    }
}
Tree_Node::~Tree_Node()
{
}
