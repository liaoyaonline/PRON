/*****************************************************************************
* Written by Liaoya                                                         *
*                                                                           *
* Function:                                                                 *
* Decision tree CART implementation                                         *        
*                                                                           *
*                                                                           *
* history:                                                                  *
*       20200716 ʵ��C45�µ����ɭ�֣��������ž���                                                   *
*                                                                           *
*                                                                           *
*                                                                           *
***************************************************************************/ 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <sstream>
#include <fstream>
#include <set>
#include <math.h>
#include <algorithm>
#include "Tree_Node.h"
using namespace std;

//�����������飬��ſ��ܵ����ԣ��������
vector<string> data_Attributes;
//��������Զ�Ӧ������ֵ
map<string, vector<string>> data_AttValues;
//����ʣ�����ԣ����������
vector<string> remain_Attributes;
//�������ݱ�,����-����ֵ��ȫ�����ݵ�����ֵ����ͬһ�����飩
map<string, vector<string>>data_Table;
//���ɭ�����Լ�
map<int,vector<string>> RanForest_Atts;
//���ɭ��������
map<int,map<string,vector<string>>> data_TableRanForest;
//���ɭ��ʣ�����ԣ����������
map<int,vector<string>> RanForest_remainAtts;
//���ɭ�ָ����Զ�Ӧ��ֵ
map<int,map<string,vector<string>>> data_RanForsetAttValues;
typedef struct node{
    string node_Attribute;
    Tree_Node *p;
    Tree_Node *ftp;
    int level;
} childtree;
int attribute_Num;
double max_Gaint;
Tree_Node Tree[56];
Tree_Node * test;
//���������������ݼ�
void data_Input();
void data_RanForsetInput();
void Traversal(Tree_Node * root);
double CheckTree(Tree_Node * root);
void PostPruning(Tree_Node * root);
void generate_ChildNode(Tree_Node temp_Table);
string findBestAtt(string con);
double calc_Entropy(map<string, vector<string>> temp_Table);
void TreeRep(Tree_Node * root);
int main()
{
	//����
	data_Input();
    //data_RanForsetInput();
    /*for(int i = 0; i < 56; i++)
    {
        Tree[i] = Tree_Node(data_TableRanForest[i],RanForest_Atts[i],data_RanForsetAttValues[i],RanForest_remainAtts[i]);
        test = &Tree[i];
        Tree[i].generate_ChildNode();
        TreeRep(test);
    }*/
    cout << "��ʼ���ɾ�����" << endl;
	Tree_Node myTree(data_Table, data_Attributes, data_AttValues, remain_Attributes);
	//����ѵ��
    test = &myTree;
    //Traversal(test);
    myTree.generate_ChildNode();
    Traversal(test);
    //CheckTree(test);
    //TreeRep(test);
    //Traversal(test);
    //Traversal(test);
    //CheckTree(test);
	//����Ԥ������������Ԥ��
	/*vector<string> predict_Sample;
	string input_Line, temp;
	cout << "���������Խ���Ԥ��:" << endl;
	getline(cin, input_Line);
	istringstream input_Sample(input_Line);
	while (input_Sample >> temp)
	{
		//������Ԥ�����������Զ�����predict_Sample���Ա㴫��
		predict_Sample.push_back(temp);
	}
	cout << endl;
	//Ԥ��
	cout << "������Ϊ��" << myTree.findClass(predict_Sample) << endl;
	*/
    system("pause");
	return 0;
}
void data_RanForsetInput()
{
    int num = 0;
    for(auto it = data_Attributes.begin();it != data_Attributes.end();it++)
        cout << *it << " ";
    auto temp = data_Attributes.end();
    temp --;
    temp --;
    cout << *temp << endl;
    //����ͬ���Ķ�Ӧ����ѹ���Ӧ���
    for(auto it = data_Attributes.begin(); it != temp; it++)
        for(auto jt = (it+1); jt != (temp + 1); jt++)
            for(auto kt = (jt + 1); kt != (temp + 1); kt++)
            {
                RanForest_Atts[num].push_back(*it);
                RanForest_Atts[num].push_back(*jt);
                RanForest_Atts[num].push_back(*kt);
                RanForest_Atts[num].push_back(*(temp+1));
                num++;
            }
    for(auto iter = RanForest_Atts.rbegin();iter != RanForest_Atts.rend();iter++)
    {
        cout << "���:" <<  iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
    //����ͬ����Ӧ��ʣ�����Լ���ѹ���Ӧ���
    RanForest_remainAtts = RanForest_Atts;
    for(int i = 0; i < 56; i++)
    {
        RanForest_remainAtts[i].pop_back();
    }
    for(auto iter = RanForest_remainAtts.rbegin();iter != RanForest_remainAtts.rend();iter++)
    {
        cout << "���:" <<  iter->first << endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout << *it << " ";
        cout << endl;
    }
    //����ͬ����Ӧ��������ѹ���Ӧ���
    for(int i = 0; i < 56; i++)
    {
        for(auto it = RanForest_Atts[i].begin(); it != RanForest_Atts[i].end(); it++)
        {
            string temp_Att = *it;
            data_TableRanForest[i][temp_Att] = data_Table[temp_Att];
        }
    }
    for(int i = 0; i < 56; i++)
    {
        cout << "�������:" << i << endl;
        for(auto iter = data_TableRanForest[i].rbegin();iter != data_TableRanForest[i].rend();iter++)
        {
            cout << iter->first << endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
    }
    //�������������ݽ������ѡ��
    for(int i = 0; i < 56; i++)
    {
        int seed = (int)time(0) + i;
        srand(seed);
        //srand((int)time(seed));  // �����������  ��0����NULLҲ��
        auto iter = data_TableRanForest[i].begin();
        auto jter = iter++;
        auto kter = iter++;
        auto mter = iter++;
        cout << iter->first << endl;
        cout << jter->first << endl;
        cout << kter->first << endl;
        cout << mter->first << endl;
        auto jt=(jter->second).begin();
        auto kt=(kter->second).begin();
        auto mt=(mter->second).begin();
        auto it=(iter->second).begin();
        while(it != (iter->second).end())
        {
            int temp = rand()%2;
            if(temp == 1)
            {
                it = (iter->second).erase(it);
                jt = (jter->second).erase(jt);
                kt = (kter->second).erase(kt);
                mt = (mter->second).erase(mt);
            }
            else
            {
                it++;
                jt++;
                kt++;
                mt++;
            }
        }
    }
    for(int i = 0; i < 56; i++)
    {
        cout << "�������:" << i << endl;
        for(auto iter = data_TableRanForest[i].rbegin();iter != data_TableRanForest[i].rend();iter++)
        {
            cout << iter->first << endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
    }
    //����ͬ����Ӧ������ֵ����ѹ���Ӧ���
    /*for(int i = 0; i < 56; i++)
    {
        for(auto iter = data_TableRanForest[i].rbegin(); iter != data_TableRanForest[i].rend(); iter++)
        {
            set<string> attValues;
            for(auto it=(iter->second).begin(); it != (iter->second).end(); it++)
                attValues.insert(*it);
            for(auto it=attValues.begin();it != attValues.end();it++)
                data_RanForsetAttValues[i][iter->first].push_back(*it);
        }
    }
    for(int i = 0; i < 56; i++)
    {
        cout << "�������:" << i << endl;
        for(auto iter = data_RanForsetAttValues[i].rbegin();iter != data_RanForsetAttValues[i].rend();iter++)
        {
            cout << iter->first << endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
    }*/
    
}
void TreeRep(Tree_Node * root)
{
    stack< childtree> child_trees;
    queue< childtree> child_treeq;
    childtree Temp_Tree;
    Temp_Tree.node_Attribute = root->node_Attribute;
    Temp_Tree.p = root;
    Temp_Tree.ftp = NULL;
    Temp_Tree.level = 1;
    child_treeq.push(Temp_Tree);
    while(child_treeq.size() != 0)
    {
        Temp_Tree = child_treeq.front();
        child_treeq.pop();
        Tree_Node* p = Temp_Tree.p;
        for(auto iter = p->childNode.rbegin();iter != p->childNode.rend();iter++)
        {
            childtree childtrees;
            childtrees.p = iter->second;
            childtrees.ftp = p;
            childtrees.node_Attribute = childtrees.p->node_Attribute;
            childtrees.level = Temp_Tree.level + 1;
            child_treeq.push(childtrees);
        }
        if(p->childNode.size() != 0)
            child_trees.push(Temp_Tree);
    }
    /*while(!child_trees.empty())
    {
        cout << "child_trees.size:" << child_trees.size() << endl;
        Temp_Tree = child_trees.top();
        child_trees.pop();
        cout << "Tree_Att:" << Temp_Tree.node_Attribute << endl;
        //cout << "Tree_Attfather:"<< Temp_Tree.ftp->node_Attribute << endl;
        cout << "Tree_Level:" << Temp_Tree.level << endl;
    }*/
    while(!child_trees.empty())
    {
        Temp_Tree = child_trees.top();
        child_trees.pop();
        Tree_Node * p = Temp_Tree.p;
        double p1 = CheckTree(root);
        int N = (int)p->MyDateTable[p->data_Attribute[0]].size();
        cout <<"N:"<< N;
        map<string,int> category;
        for(int i = 0; i < N;i++)
        {
            vector<string> temp_Class;
            temp_Class = p->MyDateTable[p->data_Attribute[p->attribute_Num]];
            category[temp_Class[i]]++;
        }
        map<string, int>::iterator it = category.begin();
        string childNode_Attribute = it->first;
        int max_Num = it->second;
        it++;
        for (; it != category.end(); it++)
        {
            if(it->second > max_Num)
            {
                max_Num = it->second;
                childNode_Attribute = it->first;
            }
        }
        string temp_Att = p->node_Attribute;
        map<string,Tree_Node *> temp_childNode = p->childNode;
        p->node_Attribute = childNode_Attribute;
        p->childNode.erase(p->childNode.begin(),p->childNode.end());
        double p2 = CheckTree(root);
        if(p1 > p2)
        {
            p->node_Attribute = temp_Att;
            p->childNode = temp_childNode;
        }
    }
}
void data_Input()
{
    int N;
    ifstream infile;
    infile.open("Treedata.txt");
    string input_Line,temp_Attributes;
    cout << "����������:" <<endl;
    getline(infile,input_Line);
    cout <<"����:\n"<<input_Line<<endl;
    istringstream input_Attributes(input_Line);
    while(input_Attributes >> temp_Attributes)
        data_Attributes.push_back(temp_Attributes);
    for(auto it = data_Attributes.begin();it != data_Attributes.end();it++)
        cout << *it << " ";
    remain_Attributes = data_Attributes;
    remain_Attributes.pop_back();
    for(auto it = data_Attributes.begin();it != data_Attributes.end();it++)
        cout << *it << " ";
    cout << " ������������Ŀ"<<endl;
    infile >> N;
    //infile >> a;
    //cin.ignore(1024,'\n');
    cout << N << endl;
    getline(infile,input_Line);
    cout << "����������:" <<endl;
    input_Line = "";
    temp_Attributes = "";
    //�������������ݴ���Data_table
    for(int i = 0 ; i < N; i++)
    {
        getline(infile,input_Line);
        cout << input_Line << endl;
        istringstream input_Attributesc(input_Line);
        for(int i = 0; i < data_Attributes.size();i++)
        {
            input_Attributesc >> temp_Attributes;
            data_Table[data_Attributes[i]].push_back(temp_Attributes);
            cout << data_Attributes[i] << temp_Attributes;
        }
        cout << endl;
    }
    //map<string,vector<string>>::reverse_iterator iter;
    for(auto iter = data_Table.rbegin();iter != data_Table.rend();iter++)
    {
        cout << iter->first <<endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout <<*it << " ";
        cout << endl;
    }
    //�����Ժ���������Ŀ���ֵ����data_AttValues
    for(auto iter = data_Table.rbegin();iter != data_Table.rend();iter++)
    {
        set<string> attValues;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            attValues.insert(*it);
        for(auto it=attValues.begin();it != attValues.end();it++)
            data_AttValues[iter->first].push_back(*it);
    }
    for(auto iter = data_AttValues.rbegin();iter != data_AttValues.rend();iter++)
    {
        cout << iter->first <<endl;
        for(auto it=(iter->second).begin();it != (iter->second).end();it++)
            cout <<*it << " ";
        cout << endl;
    }
}
void Traversal(Tree_Node * root)
{
    vector<Tree_Node*> test;
    Tree_Node *k;
    Tree_Node *knext;
    test.push_back(root);
    while(test.size() > 0)
    {
        k = test.front();
        cout <<"node_Attribute"<< k->node_Attribute<< endl;
        //for (int i = 0; i < k->data_AttValues[k->node_Attribute].size(); i++)
        //    cout <<"data_AttValues"<< k->data_AttValues[k->node_Attribute][i];
        for(auto it = k->childNode.rbegin();it != k->childNode.rend();it++)
        {
            knext = it->second;
            cout << "��ǰֵ:"<<it->first<<"->�ӽڵ�:"<< knext->node_Attribute<< endl;
            test.push_back(it->second);
        }
        auto det = test.begin();
        test.erase(det);
    }
}
double CheckTree(Tree_Node * root)
{
    float num = 0.0;
    ifstream infile;
    infile.open("Treedatatest.txt");
    string testresult;
    string answer;
    float ResultRate;
    float goodresultnum = 0.0;
    string input_Line,temp_Attributes;
    queue<string> data_class;
    queue<map<string,string>> test_data;
    cout << "��ʼ��ȡ����" << endl;
    getline(infile,input_Line);
    istringstream input_Attributes(input_Line);
    while(input_Attributes >> temp_Attributes)
        data_class.push(temp_Attributes);
    cout << "����Ԫ�ظ���" << data_class.size() << endl;
    cout << "��ʼ��ȡ���ݣ����м�����߳ɹ���" <<endl;
    infile >> num;
    cout << "��ȡ���ݳɹ�:"<< num<< endl;
    getline(infile,input_Line);
    cout << "��ʼ��ȡ�������"<< endl;
    for(int i = 0;i < num;i++)
    {
        map<string,string> temp_map;
        string temp_string;
        queue<string> temp_class = data_class;
        getline(infile,input_Line);
        cout << input_Line << endl;
        istringstream input_Attributesc(input_Line);
        for(int i = 0; i < data_Attributes.size();i++)
        {
            input_Attributesc >> temp_Attributes;
            temp_string = temp_class.front();
            temp_class.pop();
            temp_map[temp_string] = temp_Attributes;
        }
        for(auto it=temp_map.begin();it != temp_map.end();it++)
            cout << it->first << ": " << it->second << endl;
        test_data.push(temp_map);
    }
    while(!test_data.empty())
    {
        map<string,string> temp_map = test_data.front();
        test_data.pop();
        for(auto it=temp_map.begin();it != temp_map.end();it++)
            cout << it->first << ": " << it->second << endl;
        answer = temp_map["���"];
        testresult = root->findClass(temp_map);
        cout << "����:"<< answer << "�������:"<< testresult<< endl;
        if(answer == testresult)
            goodresultnum ++;
    }
    cout << "goodresult :"<< goodresultnum << "num" << num << endl;
    if(num > 0)
        ResultRate = (float)(goodresultnum / num);
    else
        ResultRate = 0;
    cout << "���ɹ���Ϊ:"<< ResultRate << endl;
    return ResultRate;
}
string findBestAtt(string con)
{
    int N = (int)data_Table[data_Attributes[0]].size();
    string temp_Att = con;
    double temp_num = 0.0;
    double temp_t = 0.0;
    string temp_snum;
    double temp_Gain = 0;
    double max_Gain = 0;
    //double max_Gaint = 0;
    string max_Att;
    vector<double> remain_AttValues;
    vector<double> remain_tempt;
    map<double,string> table_con;
    cout <<"sdfsdfksfksjkfj"<< endl;
    for(int j = 0; j < data_AttValues[temp_Att].size(); j++)
    {
        stringstream ss;
        cout << " ko "<< data_AttValues[temp_Att][j] << endl;
        temp_snum = data_AttValues[temp_Att][j];
        cout << " kotemp_snum "<<temp_snum << endl;
        ss << temp_snum;
        ss >> temp_num;
        cout << "temp_num:" << temp_num << endl;
        //ss << data_AttValues[temp_Att][j];
        //ss >> temp_num;
        //remain_AttValues.push_back(con)
        //remain_AttValues.push_back(data_AttValues[temp_Att][j]);
        remain_AttValues.push_back(temp_num);
    }
    for(int j = 0; j < remain_AttValues.size(); j++)
        cout << "  "<< remain_AttValues[j] << endl;
    sort(remain_AttValues.begin(),remain_AttValues.end());
    cout << "\n\n" << endl;
    for(int j = 0; j < remain_AttValues.size(); j++)
        cout << "  "<< remain_AttValues[j] << endl;
    for(int i = 0; i < remain_AttValues.size() - 1;i++)
    {
        temp_t = (remain_AttValues[i] + remain_AttValues[i+1]) / 2;
        cout << " t: "<< temp_t << endl;
        remain_tempt.push_back(temp_t);
    }
    for(int j = 0; j < remain_tempt.size(); j++)
        cout << " tempt "<< remain_tempt[j] << endl;
    for(int j = 0; j < remain_tempt.size(); j++)
    {
        temp_t = remain_tempt[j];
        temp_Gain = calc_Entropy(data_Table);
        //������Ŷ�Ӧ������
        double temp_con;
        int sample_Numplus = 0;//����t������Ŀ
        int sample_Numless = 0;//С��t������Ŀ
        map<string, vector<string>>sub_DataTablePlus;
        map<string, vector<string>>sub_DataTableLess;
        for(int l = 0; l < data_Table[temp_Att].size();l++)
        {
            stringstream ss;
            ss <<  data_Table[temp_Att][l];
            ss >> temp_con;
            cout << "temp_con:" << temp_con << endl;
            if(temp_con <= temp_t)
            {
                sample_Numless++;
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTableLess[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
            else
            {
                sample_Numplus++;
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTablePlus[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
        }
        cout <<"sample_NUmplus :"<< sample_Numplus<< endl;
        cout <<"sample_NUmless :"<< sample_Numless<< endl;
        for(auto iter = sub_DataTableLess.rbegin();iter != sub_DataTableLess.rend();iter++)
        {
            cout <<"first" <<iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
             cout << endl;
        }
        for(auto iter = sub_DataTablePlus.rbegin();iter != sub_DataTablePlus.rend();iter++)
        {
            cout << iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        temp_Gain = temp_Gain - ((double)sample_Numplus / N * calc_Entropy(sub_DataTablePlus) + (double)sample_Numless / N * calc_Entropy(sub_DataTableLess));
        cout << "t:" << temp_t <<"Ent : "<< temp_Gain;
        cout << endl;
        if(temp_Gain >= max_Gain)
        {
            max_Gaint = temp_t;
            max_Att = temp_Att;
            max_Gain = temp_Gain;
        }
    }
    cout << "max_Att:" << max_Att << "max_Gain:" << max_Gain << "max_Gaint:" << max_Gaint << endl;
    return temp_Att;
}
double calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//���ݼ�������������
	int sample_Num = (int)temp_Table[data_Attributes[0]].size();//����TreeӦ����data_Attribute
	//�������ݼ��е��������
	map<string, int> class_Map;
    //���λ�÷���TreeӦ��ȥ��
    int attribute_Num = (int)data_Attributes.size() -1;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]��Ӧ�ľ������ݼ������
		string class_String = table[data_Attributes[attribute_Num]][i];
		class_Map[class_String]++;
	}

    for(auto iter = class_Map.rbegin();iter != class_Map.rend();iter++)
        cout <<"string :" << iter->first << "int :" << iter->second << endl;
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
void generate_ChildNode(Tree_Node  temp_Table)
{
	//��������
	int N = (int)data_Table[data_Attributes[0]].size();
    cout << N << endl;
    string node_Attribute;

	//�����ݼ�������������������map���棬ֻ���ж����һ�м���
	map<string,int> category;
	for (int i = 0; i < N; i++)
	{
		vector<string> temp_Class;
		temp_Class = data_Table[data_Attributes[attribute_Num]];
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
        cout << "ִ��ֻ��һ�����" << endl;
		return;
	}
	//�ڶ������
	//���ж����������Ƿ�ȡ��ֵͬ
	bool isAllSame = false;
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		isAllSame = true;
		vector<string> temp;
		temp = data_Table[remain_Attributes[i]];
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
        cout<< "ִ���������Զ���ͬѡ��"<< endl;
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
	//string best_Attribute = 
    double best_t = temp_Table.findBestT("�ܶ�");
    string best_Attribute = "�ܶ�";
	//�����ڵ�����Ϊ��������
	node_Attribute = best_Attribute;
    //���������Ե�ÿ������ֵ
	int checkcon = 0;
    if(node_Attribute == "�ܶ�" || node_Attribute == "������")
        checkcon = 1;
    if(checkcon == 1)
    {
        double temp_t = best_t;//���t��findAtt����ȡ��
        double temp_con;
        map<string, vector<string>>sub_DataTablePlus;
        map<string, vector<string>>sub_DataTableLess;
        for(int l = 0; l < data_Table[best_Attribute].size();l++)
        {
            stringstream ss;
            ss <<  data_Table[best_Attribute][l];
            ss >> temp_con;
            cout << "temp_con:" << temp_con << endl;
            cout << "temp_t:" << temp_t << endl;
            if(temp_con <= temp_t)
            {
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTableLess[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
            else
            {
                for (int m = 0; m < data_Attributes.size(); m++)
                {
                    sub_DataTablePlus[data_Attributes[m]].push_back(data_Table[data_Attributes[m]][l]);
                }
            }
        }
        cout << "�м�㡣�����ָ���" << endl;
        for(auto iter = sub_DataTableLess.rbegin();iter != sub_DataTableLess.rend();iter++)
        {
            cout <<"first" <<iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        cout << "�ڶ����ָ��ߡ�������������"<< endl;
        for(auto iter = sub_DataTablePlus.rbegin();iter != sub_DataTablePlus.rend();iter++)
        {
            cout << iter->first <<endl;
            for(auto it=(iter->second).begin();it != (iter->second).end();it++)
                cout <<*it << " ";
            cout << endl;
        }
        if (sub_DataTablePlus.empty())
        {
            //�����ӽ�
            Tree_Node * p = new Tree_Node(sub_DataTablePlus, data_Attributes, data_AttValues, remain_Attributes);
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
    		cout<< "�м�sub_DataTablePlusΪempty������"<< endl;
            //���ӽڵ����childNode��Ԥ��������ʱ����õ�
    		//childNode[best_AttValues] = p;
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
            cout << "��sub_DataTablePlus��Ϊempty������"<< endl;

    		//�����ӽڵ�
    		//Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//���ӽڵ����childNode
            //childNode[best_AttValues] = pt;
			//�ӽڵ��ٵ���generate_ChildNode����
            //
            //pt->generate_ChildNode();
    	}
        if (sub_DataTableLess.empty())
        {
            //�����ӽ�
            Tree_Node * p = new Tree_Node(sub_DataTableLess, data_Attributes, data_AttValues, remain_Attributes);
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
            cout<<"sub_DataTableLessΪempty�������"<< endl;
			//������Ҷ�ڵ�����
            //p->set_NodeAttribute(childNode_Attribute);
    		//���ӽڵ����childNode��Ԥ��������ʱ����õ�
    		//childNode[best_AttValues] = p;
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
            cout << "��sub_DataTableLess��Ϊempty������"<< endl;
    		//�����ӽڵ�
    		//Tree_Node * pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//���ӽڵ����childNode
            //childNode[best_AttValues] = pt;
			//�ӽڵ��ٵ���generate_ChildNode����
            //
            //pt->generate_ChildNode();
    	}
    }
    else
    {
        cout <<"checkcon == 0 �������" << endl;
    }
}

