#include <iostream> 
#include <vector> 
#include <string>

#include "cgicc/Cgicc.h" 
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int  main（int argc，  char ** argv）
{
      Cgicc cgi;
      //发送HTTP标头
      cout << HTTPHTMLHeader（）<< endl;
      //设置HTML文档 
      cout << html()<< head（title（“cgicc example”））<< endl;
      cout << body()<< endl;

      //打印出提交的元素
      form_iterator name = cgi.getElement（“name”）;
      if(name！= cgi.getElements().end()){
         cout << “你的名字：” << ** name << endl;
      }
      //关闭HTML文档
      cout << body（）<< html（）;
}
