#include "my_json_encode.h"
string my_json_encode(vector<pair<string,string> > data){
  /*
    vector data内の要素をJSON形式に変換して返す
        data[vector<pair<string,string> >]: {{"aaa",""bbb""},{"ccc","123"},{"ddd","45.60"}}
    JSON[string]:
    {
       "aaa":"bbb", 
       "ccc":123 ,
       "ddd":45.60
    } 
  */
  int data_num = data.size();
  string send_str;
  send_str.append("{ \n");
  for(int i=0;i<data_num;i++){
    
    send_str.append("\t\""+data[i].first+"\":"+data[i].second);   

    if(i+1 != data_num){
      send_str.append(",\n");
    }
  }
  send_str.append("\n}\n");

  return send_str;
}

string my_json_encode_noindent(vector<pair<string,string> > data){
  /*
    vector data内の要素をJSON形式(インデントなし)に変換して返す
    data[vector<pair<string,string> >]: {{"aaa",""bbb""},{"ccc","123"},{"ddd","45.60"}}
    JSON[string]: "{"aaa":""bbb"","ccc":123,"ddd":45.60}" 
  */
  int data_num = data.size();
  string send_str;
  send_str.append("{");
  for(int i=0;i<data_num;i++){

    send_str.append("\""+data[i].first+"\":"+data[i].second);  
    
    if(i+1 != data_num){
      send_str.append(",");
    }
  }
  send_str.append("}");

  return send_str;
}
