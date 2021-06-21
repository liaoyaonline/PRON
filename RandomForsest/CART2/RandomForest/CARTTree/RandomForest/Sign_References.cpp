#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
using namespace std;
map<string,string> Sign_References;
void Read_References(string NameFile,map<string,string>& Sign_References_tmp);
void Print_References(map<string,string> Sign_References_tmp);
void Write_References(string NameFile,map<string,string> Sign_References_tmp);
void Plus_References(map<string,string>& Sign_References_tmp);
int main()
{
    string NameFile ="test.txt";
    while(1)
    {
        string Sign_end = "end";
        string Sign_Again = "again";
        string Sign_tmp;
        string References_tmp;
        Read_References(NameFile,Sign_References);
        Print_References(Sign_References);
        cout << "输入参考文献标记: " << endl;
        cin >> Sign_tmp;
        if(Sign_tmp == Sign_end )
        {
            cout <<"结束输入" << endl;
            break;
        }
        if(Sign_tmp == Sign_Again )
        {
            cout << "重新输入" << endl;
            continue;
        }
        cout << "输入参考文献: " << endl;
        cin >> References_tmp;
        if(Sign_tmp == Sign_end || References_tmp == Sign_end)
        {
            cout <<"结束输入" << endl;
            break;
        }
        if(Sign_tmp == Sign_Again || References_tmp == Sign_Again)
        {
            cout << "重新输入" << endl;
            continue;
        }
        cout << "将此参考文献压入文件: " << endl;
        cout << Sign_tmp <<": " << References_tmp << endl;
        Sign_References[Sign_tmp] = References_tmp;
        Write_References(NameFile,Sign_References);
    }
    return 0;
}
void Write_References(string NameFile,map<string,string> Sign_References_tmp)
{
    ofstream outfile;
    outfile.open(NameFile);
    for(auto iter = Sign_References_tmp.rbegin(); iter != Sign_References_tmp.rend(); iter++)
    {
        outfile << iter->first <<" " << iter->second << endl;
    }
    outfile.close();
}
void Print_References(map<string,string> Sign_References_tmp)
{
    for(auto iter = Sign_References_tmp.rbegin(); iter != Sign_References_tmp.rend(); iter++)
    {
        cout << iter->first <<":" << iter->second << endl;
    }
}
void Read_References(string NameFile,map<string,string>& Sign_References_tmp)
{
    ifstream infile;
    infile.open(NameFile);
    if(infile.peek() == EOF)
    {
        cout << "file is empty." << endl;
        return;
    }
    while(!infile.eof())
    {
        string Line_tmp,Sign_tmp,References_tmp;
        getline(infile,Line_tmp);
        if(infile.eof())//对于feof机制的处理
            break;
        istringstream ReadLine(Line_tmp);
        ReadLine >> Sign_tmp;
        ReadLine >> References_tmp;
        Sign_References_tmp[Sign_tmp] = References_tmp;
    }
    infile.close();
}
