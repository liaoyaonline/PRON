/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 ʵ��CART������
*       20200810 ������Ϲ���ľ�������dot������ӻ�������
*       20200817 ���NumSample�޷������������Ϊ0����������*
*       20200831 ʵ��ͨ������GiniImportance�Ӷ�����������Ҫ�Խ��ж���
*                �����FeatureGini��PrintTree�����е���������
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
	//ȫ������,�������
	FullAtt_DataTable = temp_Attribute;
	//���Ա�
	Table_Data = temp_Table;
    RemoveFeatures_WithTheSameValue(Table_Data,FullAtt_DataTable);
    NumAtt_CurrentNode = FullAtt_DataTable.size() - 1;
    BestSignDivide_CurrentNode = 0;
    NumSample_CurrentNode = (int)Table_Data[FullAtt_DataTable[0]].size();//���㵱ǰ�ڵ�������Ŀ
    GiniValue_CurrentNode = Calculation_GiniValue(Table_Data);//���㵱ǰ�ڵ�Giniֵ
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
void Tree_Node::RemoveFeatures_WithTheSameValue(map<string, vector<string>>& temp_Table, vector<string>& temp_Attribute)
{
    vector<string> AttDataTable_tmp;//ȥ����������Լ���
    AttDataTable_tmp = temp_Attribute;
    AttDataTable_tmp.pop_back();
    string Class_tmp = temp_Attribute[temp_Attribute.size() - 1];
    for (auto it = AttDataTable_tmp.begin(); it != AttDataTable_tmp.end();)//�Ƴ�����ֵ��ͬ������
    {
        bool AttValues_AllSame = true;
        vector<string> AttValues_temp;
        AttValues_temp = temp_Table[*it];
        for(int j = 1; j < AttValues_temp.size(); j++)
        {
            if(AttValues_temp[0] != AttValues_temp[j])
            {
                AttValues_AllSame = false;
                break;
            }
        }
        if(AttValues_AllSame == true)
        {
            AttDataTable_tmp.erase(it);
        }
        else
            it++;
    }
    AttDataTable_tmp.push_back(Class_tmp);
    temp_Attribute = AttDataTable_tmp;
    map<string, vector<string>> Table_tmp;
    for(int i = 0; i < temp_Attribute.size(); i++)
    {
        string Feature_tmp = temp_Attribute[i];
        Table_tmp[Feature_tmp] = temp_Table[Feature_tmp];
    }
    temp_Table = Table_tmp;

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
    for (int i = 0; i < AttDataTable_tmp.size(); i++)//�ڶ�������ж�����ȡֵ�Ƿ���ͬ
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
    map<string,vector<string>> DataTable_plus_signdivide;
    map<string,vector<string>> DataTable_less_signdivide;
    for(int i = 0; i < Table_Data[Attribute_CurrentNode].size(); i++)//����ÿ������
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
    if(DataTable_less_signdivide.empty())//Gini�����ϲ�����ֿյģ���һֱ���ֵ�ֻ��һ������Ϊֹ,���������õ����������֮
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
        FeatureGini_CurrentNode[Att_selected] = Gini_BestSignDivide_tmp;//0830�޸ģ����ڶ�������Ҫ������
        if(Gini_BestSignDivide_tmp >= 1)
        {
            cout << "Gini_BestSignDivide_tmp: " << Gini_BestSignDivide_tmp << "Att_selected: " << Att_selected << endl;
            cout <<" ��ȡ��ǰ��������" << endl;
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
    BestSignDivide_CurrentNode = BestSign_Divide;//����ǰ�ڵ���ѻ��ֱ�Ǹ�ֵ
    return BestAtt;
    
}
void Tree_Node::FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Gini_BestSignDivide)//����ĳ��������ȡֵ�㶨��ô����
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
    if(Value_Att_selected.size() == 1)//����������ڸ���������ȡֵ��ͬ
    {
        *Gini_BestSignDivide = GiniValue_CurrentNode;
        *BestSign_Divide = Value_Att_selected[0];
        return;
    }
    for(int i = 0; i < Value_Att_selected.size() - 1; i++)
    {
        double Value_temp;//�м���ɱ���
        Value_temp = (Value_Att_selected[i] + Value_Att_selected[i+1]) / 2;
        Value_sign_divide.push_back(Value_temp);
    }
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
        for(int j = 0; j < NumSample_CurrentNode;j++)//����������������Ϊplus��less�������ݼ�
        {
            stringstream ss;
            double Value_temp;//�м���ɱ���
            ss << Table_Data[Att_selected][j];//����ʹ��ȡֵ���϶���Ҫ��map����ļ���
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


double Tree_Node::Calculation_GiniValue(map<string, vector<string>> Table_Data)//�����ǿռ�..�����������ļ���GIniֵ
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
	//���ýڵ������
	this->Attribute_CurrentNode = attribute;
}

//����Ϊ��Ԥ���������������Լ���
string Tree_Node::PredictionSampleClass(map<string,string> Set_AttSample)
{
    if(childNode.size() == 0)
    {
        //cout << "���ؽ������: " << Attribute_CurrentNode << endl;
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
            //cout <<"����: "<< Attribute_CurrentNode << "ȡֵΪ: " << Set_AttSample[Attribute_CurrentNode] << "<= ��ǰ�ڵ���ѻ���" << BestSignDivide_CurrentNode << "True"<<  endl; 
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
            //cout <<"����: "<< Attribute_CurrentNode << "ȡֵΪ: " << Set_AttSample[Attribute_CurrentNode] << "> ��ǰ�ڵ���ѻ���" << BestSignDivide_CurrentNode << "False"<< endl;
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
    map<string,double>FeatureGini_tmp;//���ڶ�������������
    int Num_AllSample = root->NumSample_CurrentNode;
    cout << "Num_AllSample: " << Num_AllSample << endl;
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        cout << "��ǰ�ڵ�����:  " << Node_Current->Attribute_CurrentNode << endl;
        cout << "��ǰ�ڵ���ѻ���ѡ��ֵ:  "<< Node_Current->BestSignDivide_CurrentNode << endl;
        cout << "��ǰ������Ŀ:  "<< Node_Current->NumSample_CurrentNode << endl;
        cout << "��ǰ�ڵ�������Ŀ: " << Node_Current->NumAtt_CurrentNode << endl;
        cout << "��ǰ�ڵ�Giniֵ�� " << Node_Current->GiniValue_CurrentNode << endl;
        //map<string,int>::iterator it = Node_Current->ClassSample_CurrentNode.begin();
        for(auto it = Node_Current->ClassSample_CurrentNode.begin();it != Node_Current->ClassSample_CurrentNode.end(); it++)
        {
            cout << it->first<< "�� " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->FeatureGini_CurrentNode.begin();it != Node_Current->FeatureGini_CurrentNode.end(); it++)//0831���ڼ���GiniImportance
        {
            cout << it->first<< "�� " << it->second << "  ";
        }
        cout << endl;
        for(auto it = Node_Current->FeatureGini_CurrentNode.begin();it != Node_Current->FeatureGini_CurrentNode.end(); it++)//0831���ڼ���GiniImportance
        {
            double Rate_Sample = (double)Node_Current->NumSample_CurrentNode / (double)Num_AllSample;
            double EffectGini_tmp = Node_Current->GiniValue_CurrentNode - it->second;
            double GiniImportance_tmp = Rate_Sample * EffectGini_tmp;
            cout <<"Att_selected: " << it->first << "    Rate_Sample: " << Rate_Sample << "    EffectGini_tmp: " << EffectGini_tmp << "   GiniImportance_tmp: " << GiniImportance_tmp << endl;
            FeatureGini_tmp[it->first] += GiniImportance_tmp;
        }
        for(auto it = FeatureGini_tmp.begin();it != FeatureGini_tmp.end(); it++)
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
    for(auto it = FeatureGini_tmp.begin();it != FeatureGini_tmp.end(); it++)
    {
        cout << it->first<< "�� " << it->second << "  ";
    }
    cout << endl;
}
void Tree_Node::GenerateDot(Tree_Node* root,string DotFileName)
{
    vector<Tree_Node*> Queue_TreeNode;
    Tree_Node* Node_Current;
    Tree_Node* Node_Child;
    Queue_TreeNode.push_back(root);
    int NumLabel = 0;
    int Number_Node = 1;
    root->Number_CurrentNode = 0;
    ofstream outfile;
    outfile.open(DotFileName);
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
        for(;it != ClassSample_CurrentNode.end(); it++)//���ÿ�����͵���Ŀ����0
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
