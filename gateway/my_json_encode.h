#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

string my_json_encode(vector<pair<string,string> > data);

 /*
    vector data���̗v�f��JSON�`���ɕϊ����ĕԂ�
    data: {{"aaa","bbb"},{"ccc","123"},{"ddd","45.60"}}
    -> 
    JSON[string]:
    {
       "aaa":"bbb", 
       "ccc":123 ,
       "ddd":45.60
    } 
  */

string my_json_encode_noindent(vector<pair<string,string> > data);
  /*
    vector data���̗v�f��JSON�`��(�C���f���g�Ȃ�)�ɕϊ����ĕԂ�
    data: {{"aaa","bbb"},{"ccc","123"},{"ddd","45.60"}}
    -> 
    JSON:
    {"aaa":"bbb","ccc":123,"ddd":"45.60"} 
  */
bool isInteger(string v); 
