#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
int main()
{
    vector<string> data_Attributes;
    int N;
    ifstream infile;
    ifstream outfile;
    string tmpstring;
    infile.open("Treedata.txt");
    outfile.open("irisdata2.txt");
    string input_Line,temp_Attributes;
    cout << "请输入属性: " << endl;
    getline(infile,input_Line);
    cout << "属性：\n" << input_Line<< endl;
    istringstream input_Attributes(input_Line);
    input_Attributes >> temp_Attributes;
    getline(input_Attributes,tmpstring);
    cout << tmpstring << endl;
    infile.close();
    outfile.close();
    return 0;
}

