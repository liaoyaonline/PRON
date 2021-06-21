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
*       20200817 ���NumSample�޷������������Ϊ0����������
*       20200822 ��CART���������ΪCART�ع���*
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
    NumSample_CurrentNode = (int)Table_Data[FullAtt_DataTable[0]].size();//���㵱ǰ�ڵ�������Ŀ
    NumAtt_CurrentNode = FullAtt_DataTable.size() - 1;
    AverageValue_CurrentNode = 0;
    for(int i = 0; i < NumSample_CurrentNode; i++)
    {
        stringstream ss;
        string Class_tmp;
        double ClassValue_tmp;
        Class_tmp = Table_Data[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ss << Class_tmp;
        ss >> ClassValue_tmp;
        AverageValue_CurrentNode += ClassValue_tmp;
    }
    if(AverageValue_CurrentNode != 0)
        AverageValue_CurrentNode = AverageValue_CurrentNode / NumSample_CurrentNode;
    cout << "AverageValue_CurrentNode: " << AverageValue_CurrentNode<< endl;
    VarianceValue_CurrentNode = Calculation_VarianceValue(Table_Data);//���㵱ǰ�ڵ㷽��ֵ
    if(VarianceValue_CurrentNode == 0.08 || AverageValue_CurrentNode == 2.3)
        cout << "testnode1" << endl;
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
    if (VarianceValue_CurrentNode <= 0.02)//��һ��������ж�ʱ��ʱ���������ͬ�Ժ�Ӧ�ñ��Ϊ����С����ֵȻ��ƽ������ΪҶ�ӽڵ�
    {
        stringstream ss;
        string Average_tmp;
        ss << AverageValue_CurrentNode;
        ss >> Average_tmp;
        Attribute_CurrentNode = Average_tmp;
        BestSignDivide_CurrentNode = 0;
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
        Attribute_CurrentNode = AverageValue_CurrentNode;;
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
    if(!DataTable_less_signdivide.empty())//Gini�����ϲ�����ֿյģ���һֱ���ֵ�ֻ��һ������Ϊֹ,���������õ����������֮
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
    if(!DataTable_plus_signdivide.empty())
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
    double Variance_BestMin = VarianceValue_CurrentNode;//��ͻ���ָ��
    vector<string> AttDataTable_tmp;//ȥ����������Լ���
    AttDataTable_tmp = FullAtt_DataTable;
    AttDataTable_tmp.pop_back();
    for(int i = 0; i < AttDataTable_tmp.size(); i++)
    {
        string Att_selected;//��ѡ�е�����
        Att_selected = AttDataTable_tmp[i];
        double BestSign_Divide_tmp;
        double Variance_BestSignDivide_tmp;
        double *pbestsign = &BestSign_Divide_tmp;
        double *pginibestsign = &Variance_BestSignDivide_tmp;
        FindBestDivideSign(Att_selected,pbestsign,pginibestsign);
        if(Variance_BestSignDivide_tmp <= Variance_BestMin)
        {
            Variance_BestMin = Variance_BestSignDivide_tmp;
            BestAtt = Att_selected;
            BestSign_Divide = BestSign_Divide_tmp;
        }
    }
    BestSignDivide_CurrentNode = BestSign_Divide;//����ǰ�ڵ���ѻ��ֱ�Ǹ�ֵ
    return BestAtt;
    
}
void Tree_Node::FindBestDivideSign(string Attribute,double* BestSign_Divide,double* Variance_BestSignDivide)
{
    string Att_selected;//��ѡ�е�����
    Att_selected = Attribute;
    vector<double> Value_Att_selected;//��ѡ�����Ե�ȡֵ����
    double sign_divide = 0.0;//���ֱ��
    vector<double> Value_sign_divide;//���ֱ��ȡֵ����
    double Variance_min_signdivide = VarianceValue_CurrentNode;
    double BestSign_Divide_tmp;
    map<string,vector<string>> TableAttValue_tmp;//���Ժ�ȡֵ��Ӧ��о����Ż�����أ����Ӧ��û�б�Ҫ���ڣ�����ͷ�Բ����ã��Ժ��޸�
    for(auto iter = Table_Data.rbegin();iter != Table_Data.rend();iter++)//����Ҫ�޸ģ������Ϊ��ȥ���ظ�ȡֵ  
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
    for(int i = 0; i < Value_Att_selected.size() - 1; i++)
    {
        double Value_temp;//�м���ɱ���
        Value_temp = (Value_Att_selected[i] + Value_Att_selected[i+1]) / 2;
        Value_sign_divide.push_back(Value_temp);
    }
    for(int i = 0; i < Value_sign_divide.size(); i++)
    {
        sign_divide = Value_sign_divide[i];
        double Variance_tmp;//�м���ɱ���
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
                for(int k = 0; k < FullAtt_DataTable.size(); k++)
                {
                    DataTable_less_signdivide[FullAtt_DataTable[k]].push_back(Table_Data[FullAtt_DataTable[k]][j]);
                }
            }
            else
            {
                for(int k = 0; k < FullAtt_DataTable.size(); k++)
                {
                    DataTable_plus_signdivide[FullAtt_DataTable[k]].push_back(Table_Data[FullAtt_DataTable[k]][j]);
                }
            }
        }
        double VariancePlus_tmp = Calculation_VarianceValue(DataTable_plus_signdivide);
        double VarianceLess_tmp = Calculation_VarianceValue(DataTable_less_signdivide);
        Variance_tmp = VariancePlus_tmp + VarianceLess_tmp;
        if(Variance_tmp <= Variance_min_signdivide)
        {
            Variance_min_signdivide = Variance_tmp;
            BestSign_Divide_tmp = sign_divide;
        }
    }
    *BestSign_Divide = BestSign_Divide_tmp;
    *Variance_BestSignDivide = Variance_min_signdivide;
}


double Tree_Node::Calculation_VarianceValue(map<string, vector<string>> Table_Data)//�����ǿռ���������..�����������ļ���GIniֵ
{
    if(Table_Data.empty())
        return 0;
    map<string,vector<string>> TableData_tmp = Table_Data;
    double Value_Variance = 0;
    double Value_Average = 0;
    double Sum_ClassValue = 0;
    int NumSample_tmp = TableData_tmp[FullAtt_DataTable[0]].size();
    vector<double> Value_Class;
    for (int i = 0; i < NumSample_tmp; i++)
    {
        stringstream ss;
        string Class_tmp;
        double ClassValue_tmp = 0;
        Class_tmp = TableData_tmp[FullAtt_DataTable[NumAtt_CurrentNode]][i];
        ss << Class_tmp;
        ss >> ClassValue_tmp;
        Value_Class.push_back(ClassValue_tmp);
        Sum_ClassValue += ClassValue_tmp;
    }
    if(Sum_ClassValue != 0)
        Value_Average = Sum_ClassValue / (double)NumSample_tmp;
    for(int i = 0; i < Value_Class.size(); i++)
    {
        double x_tmp = 0;
        if((Value_Class[i] - Value_Average) < 0.00001 && (Value_Class[i] - Value_Average) > -0.00001)
            x_tmp = 0;
        else
            x_tmp = (Value_Class[i] - Value_Average);
        double sum_tmp = x_tmp * x_tmp;
        Value_Variance += sum_tmp;
    }
    return Value_Variance;
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
    while(!Queue_TreeNode.empty())
    {
        Node_Current = Queue_TreeNode.front();
        cout << "��ǰ�ڵ�����:  " << Node_Current->Attribute_CurrentNode << endl;
        cout << "��ǰ�ڵ���ѻ���ѡ��ֵ:  "<< Node_Current->BestSignDivide_CurrentNode << endl;
        cout << "��ǰ������Ŀ:  "<< Node_Current->NumSample_CurrentNode << endl;
        cout << "��ǰ�ڵ�������Ŀ: " << Node_Current->NumAtt_CurrentNode << endl;
        cout << "��ǰ�ڵ�Averageֵ�� " << Node_Current->AverageValue_CurrentNode << endl;
        cout << "��ǰ�ڵ�Varianceֵ�� " << Node_Current->VarianceValue_CurrentNode << endl;
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
        if(Node_Current->childNode.size() != 0)
            outfile<< " <= "<<Node_Current->BestSignDivide_CurrentNode;
        outfile << "\\nAverage = "<< Node_Current->AverageValue_CurrentNode;
        outfile << "\\nVariance = "<< Node_Current->VarianceValue_CurrentNode<<"\\n"<<"Num_Sample = "<< Node_Current->NumSample_CurrentNode << "\\n";
        outfile << "\"] ;"<< endl;
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
