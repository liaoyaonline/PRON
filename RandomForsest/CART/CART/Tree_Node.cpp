#include "Tree_Node.h"
#include <math.h>
#include <iostream>


Tree_Node::Tree_Node(map<string, vector<string>> temp_Table,vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain)
{
	//ȫ������,�������
	data_Attribute = temp_Attribute;
	//���Ը��������������
	attribute_Num = (int)temp_Attribute.size() - 1;
	//�����Զ�Ӧ������ֵ
	this->data_AttValues = data_AttValues;
	//���Ա�
	MyDateTable = temp_Table;
	//ʣ�����Լ�
	remain_Attributes = temp_remain;
}

void Tree_Node::generate_ChildNode()
{
	//��������
	int N = (int)MyDateTable[data_Attribute[0]].size();
    cout << N << endl;
	
	//�����ݼ�������������������map���棬ֻ���ж����һ�м���
	map<string,int> category;
	for (int i = 0; i < N; i++)
	{	
		vector<string> temp_Class;
		temp_Class = MyDateTable[data_Attribute[attribute_Num]];
		category[temp_Class[i]]++;
	}
    for(auto iter = category.rbegin();iter != category.rend();iter++)
    {
        cout << iter->first << ":"<< iter->second<<endl;
        cout << endl;
    }

	//��һ�����
	//ֻ��һ����𣬱��ΪҶ�ڵ�
	if (1 == category.size())
	{
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		return;
	}
	//�ڶ������
	//���ж����������Ƿ�ȡ��ֵͬ
	bool isAllSame = false;
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = MyDateTable[remain_Attributes[i]];
		for (int j = 1; j < temp.size(); j++)
		{
			//ֻҪ��һ����ͬ�������˳�
			if (temp[0] != temp[j])
			{
				isAllSame = false;
				break;
			}
		}
		if (isAllSame == false)
		{
			break;
		}
	}
	//�����Լ�Ϊ�ջ��������е�ȫ������ȡֵ��ͬ
	if (remain_Attributes.empty()||isAllSame)
	{
		//�ҳ����������������ֵĸ����������ýڵ���Ϊ����
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		int max = it->second;
		it++;
		for (; it != category.end(); it++)
		{
			int num = it->second;
			if (num > max)
			{
				node_Attribute = it->first;
				max = num;
			}
		}
		return;
	}
	//���������
	//��remian_attributes�л�����������
	string best_Attribute = findBestAttribute();
	//�����ڵ�����Ϊ��������
	node_Attribute = best_Attribute;
	//���������Ե�ÿ������ֵ
	for (int i = 0; i < data_AttValues[best_Attribute].size(); i++)
	{
		string best_AttValues = data_AttValues[best_Attribute][i];
		//�������Զ�Ӧ�����ݼ�D
		//����map������Ÿ�����ֵ�µ������Ӽ�
		map<string, vector<string>> sub_DataTable;
		for (int j = 0; j < MyDateTable[best_Attribute].size(); j++)
		{
			//Ѱ���������������ݼ�������ֵ��ͬ����������
			if (best_AttValues == MyDateTable[best_Attribute][j])
			{
				//�ҵ���Ӧ�����ݼ��������Ӽ���sub_DataTable����������ȫ�����Զ�Ҫ���룩
				for (int k = 0; k < data_Attribute.size(); k++)
				{
					sub_DataTable[data_Attribute[k]].push_back(MyDateTable[data_Attribute[k]][j]);
				}
			}
		}
		//���Ӽ�Ϊ�գ�����֧�ڵ㣨�ӽڵ㣩���ΪҶ�ڵ㣬���ΪMyDateTable����������
		if (sub_DataTable.empty())
		{
			//�����ӽڵ�
			Tree_Node * p = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, remain_Attributes);
			//�ҳ����������࣬��Ϊ�ӽڵ������
			map<string, int>::iterator it = category.begin();
			string childNode_Attribute = it->first;
			int max_Num = it->second;
			it++;
			for (; it != category.end(); it++)
			{
				if (it->second > max_Num)
				{
					max_Num = it->second;
					childNode_Attribute = it->first;
				}
			}
			//������Ҷ�ڵ�����
			p->set_NodeAttribute(childNode_Attribute);
			//���ӽڵ����childNode��Ԥ��������ʱ����õ�
			childNode[best_AttValues] = p;
		}
		else//����Ϊ�գ����ʣ������ֵ�޳������ԣ�����generate_ChildNode��������ϸ��
		{
			vector<string> child_RemainAtt;
			child_RemainAtt = remain_Attributes;
			//�ҳ�child_RemainAtt�е�������������ȵ�����
			vector<string>::iterator it = child_RemainAtt.begin();
			for (; it != child_RemainAtt.end(); it++)
			{
				if (*it == best_Attribute)
				{
					break;
				}
			}
			//ɾ��
			child_RemainAtt.erase(it);

			//�����ӽڵ�
			Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//���ӽڵ����childNode
			childNode[best_AttValues] = pt;
			//�ӽڵ��ٵ���generate_ChildNode����
			pt->generate_ChildNode();
		}
	}

}

//Ѱ�����Ż���
string Tree_Node::findBestAttribute()
{
	//��������
	int N = (int)MyDateTable[data_Attribute[0]].size();
	//�������ڴ����������
	string best_Attribute;
	//��Ϣ����
	double gain = 0;
    map<string,double> Attribute_Gain;
    map<string,double> Attribute_Gratio;
    double average = 0.0;
    double sum = 0.0;
    double temp_ratio = 0.0;
    double Gain_ratio = 0.0;
	//��ÿ��ʣ������
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		//������Ϣ���棬ѡȡ�����������������ּ�Ϊ���Ż���
		double temp_Gain = calc_Entropy(MyDateTable);//���ݹ�ʽ�Ƚ����ڵ����Ϣ�س�ʼ��������
		cout << "first Ent(D):" << temp_Gain<< endl;
        double temp_Iva = 0;
        double p = 1;
        //�Ը����Ե����ݼ����з��ࣨ��ȡ������ֵ�������Ӽ���
		string temp_Att = remain_Attributes[i];//����ѡȡ������
		vector<string> remain_AttValues;//���Կ��ܵ�ȡֵ
		for (int j = 0; j < data_AttValues[temp_Att].size(); j++)
		{
			remain_AttValues.push_back(data_AttValues[temp_Att][j]);
		}
		
		//��ÿ������ֵ����Ϣ��
		for (int k = 0; k < remain_AttValues.size(); k++)
		{
			//����ֵ
			string temp_AttValues = remain_AttValues[k];
			int sample_Num = 0;//������ֵ��Ӧ��������
			//����map������Ÿ�����ֵ�µ������Ӽ�
			map<string, vector<string>>sub_DataTable;
			for (int l = 0; l < MyDateTable[temp_Att].size(); l++)
			{
				if (temp_AttValues == MyDateTable[temp_Att][l])
				{
					sample_Num++;
					//������������ѵ��������sub_DataTable
					for (int m = 0; m < data_Attribute.size(); m++)
					{
						sub_DataTable[data_Attribute[m]].push_back(MyDateTable[data_Attribute[m]][l]);
					}
				}
			}
			//�ۼ�ÿ����ֵ����Ϣ��
			p = (double)sample_Num / (double)N;
            cout << "p:" << p << endl;
            temp_Gain -= (double)sample_Num / N * calc_Entropy(sub_DataTable);
            if(p > 0)
                temp_Iva -= p * (log(p) / log(2));
            else
                temp_Iva -= 0;
            cout << "sample_Num" << sample_Num << endl;
            cout << "D(v)/D:"<< p << "Iva:"<< temp_Iva<< endl;
		}
		//�Ƚ�Ѱ�����Ż�������
		cout << "temp_Gain:" << temp_Gain << "Iva:"<< temp_Iva<<"temp_Att:"<< temp_Att<< endl;
        if (temp_Gain > gain)
		{
			gain = temp_Gain;
			best_Attribute = temp_Att;
		}
        if(temp_Iva > 0)
            temp_ratio = temp_Gain / temp_Iva;
        else
            temp_ratio = 0;
        cout << "temp_Gain:" << temp_Gain << "temp_ratio:"<< temp_ratio<<"temp_Att:"<< temp_Att<< endl;
        Attribute_Gain[temp_Att] = temp_Gain;
        Attribute_Gratio[temp_Att] = temp_ratio;
	}
    for(auto iter = Attribute_Gain.rbegin();iter != Attribute_Gain.rend();iter++)
        cout << "������:"<< iter->first <<"����Gain:"<< iter->second<< endl;
    for(auto iter = Attribute_Gratio.rbegin();iter != Attribute_Gratio.rend();iter++)
        cout << "������:"<< iter->first <<"����Gain_ratio:"<< iter->second<< endl;
    for(auto iter = Attribute_Gain.rbegin();iter != Attribute_Gain.rend();iter++)
        sum += iter->second;
    average = sum / remain_Attributes.size();;
    cout << "sum :" << sum << " average: "<< average << endl;
    map<string,double>::iterator itGain = Attribute_Gain.begin();
    map<string,double>::iterator itIva = Attribute_Gratio.begin();
    while(itGain != Attribute_Gain.end())
    {
        cout << " itGain->first" << itGain->first<< endl;
        if(itGain->second < average)
        {
            itIva = Attribute_Gratio.find(itGain->first);
            Attribute_Gain.erase(itGain++);
            Attribute_Gratio.erase(itIva);
        }
        else{
            ++itGain;
        }
    }
    for(auto iter = Attribute_Gain.rbegin();iter != Attribute_Gain.rend();iter++)
        cout << "������:"<< iter->first <<"����Gain:"<< iter->second<< endl;
    for(auto iter = Attribute_Gratio.rbegin();iter != Attribute_Gratio.rend();iter++)
         cout << "������:"<< iter->first <<"����Gain_ratio:"<< iter->second<< endl;
    for(auto iter = Attribute_Gratio.rbegin();iter != Attribute_Gratio.rend();iter++)
    {
        if(Gain_ratio <= iter->second)
        {
            Gain_ratio = iter->second;
            best_Attribute = iter->first;
        }
    }
    cout << "best_Attribute:" << best_Attribute << endl;
	return best_Attribute;
}

//������Ϣ��
double Tree_Node::calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//���ݼ�������������
	int sample_Num = (int)temp_Table[data_Attribute[0]].size();
	//�������ݼ��е��������
	map<string, int> class_Map;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]��Ӧ�ľ������ݼ������
		string class_String = table[data_Attribute[attribute_Num]][i];
		class_Map[class_String]++;
	}

	map<string, int>::iterator it = class_Map.begin();
	//���������Ӧ������
	//vector<string> m_Class;
	vector<int> n_Class;
	
	for (; it != class_Map.end(); it++)
	{
		//m_Class.push_back(it->first);
		n_Class.push_back(it->second);
	}
	//������Ϣ��
	double Ent = 0;
	for (int k = 0; k < class_Map.size(); k++)
	{
		//����
		double p = (double) n_Class[k] / sample_Num;
		if (p == 0)
		{
			//�涨��p=0ʱ��plogp=0
			continue;
		}
		//c++��ֻ��log��ln�������ҪӦ�û��׹�ʽ
		Ent -= p * (log(p) / log(2));//��Ϣ��
	}
	cout << "Ent" << Ent<<endl;
	return Ent;
}

void Tree_Node::set_NodeAttribute(string attribute)
{
	//���ýڵ������
	this->node_Attribute = attribute;
}

//����Ϊ��Ԥ���������������Լ���
string Tree_Node::findClass(vector<string> attributes)
{
	//�������ӽڵ�
	if (childNode.size() != 0)
	{
		//�ҳ�������������뱾�ڵ����Զ�Ӧ������ֵ���Ա�Ѱ���¸��ڵ㣬ֱ���ҵ�Ҷ�ڵ�
		string attribute_Value;
        cout << "node_Attribute:"<< node_Attribute << endl;
        cout << data_AttValues[node_Attribute].size()<< endl;
        //for(auto it = node_AttValues.begin();it != node_AttValues.end();it++)
          //  cout <<"node_AttValues:"<< *it << " "<<endl;
        for (int i = 0; i < data_AttValues[node_Attribute].size(); i++)
            cout <<"data_AttValues"<< data_AttValues[node_Attribute][i];
		for (int i = 0; i < data_AttValues[node_Attribute].size(); i++)
		{
			for (int j = 0; j < attributes.size(); j++)
			{
				//data_AttValues[node_Attribute]Ϊ����node_Attribute��Ӧ�����п��ܵ�ȡֵ����
				if (attributes[j] == data_AttValues[node_Attribute][i])
				{
					//�ҵ���������Ӧ������ֵ
					attribute_Value = attributes[j];
                    cout << "��������������:"<< attribute_Value << endl;
					break;
				}
			}
			//�ҵ����û��Ҫ����ѭ����
			if (!attribute_Value.empty())
			{
				break;
			}
		}
		//�ҳ�������ֵ��Ӧ���ӽڵ�ĵ�ַ���Ա���з���
		Tree_Node *p = childNode[attribute_Value];
		return p->findClass(attributes);//�ݹ�Ѱ�ң�ֱ���ҵ�Ҷ�ڵ�Ϊֹ
	}
	else//�������ӽڵ�˵���Ѿ��ҵ����࣬���Ϊ���ڵ��node_Attribute
	{
		return node_Attribute;
	}
}


Tree_Node::~Tree_Node()
{
}
