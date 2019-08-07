#include <iostream>
#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h> 
using namespace std;
using namespace cgicc;
int main ()
{
    Cgicc formData;
    bool maths_flag, physics_flag;
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html lang=en>\n";
    std::cout << "<head>\n";
    std::cout << "<meta charset=UTF-8>\n";
    std::cout << "<title>CGI 程序接收复选框数据</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "你曾经使用过的语言有: ";

    
    form_iterator name = formData.getElement("NAME");
    cout << **name << endl;
    maths_flag = formData.queryCheckbox("python");
    if( maths_flag ) {  
        std::cout << "Python ";  
    }

    maths_flag = formData.queryCheckbox("c");
    if( maths_flag ) {  
        std::cout << "C ";  
    }

    maths_flag = formData.queryCheckbox("java");
    if( maths_flag ) {  
        std::cout << "Java ";  
    }

    maths_flag = formData.queryCheckbox("php");
    if( maths_flag ) {  
        std::cout << "PHP";  
    }
    std::cout <<"</body>\n";
    std::cout << "</html>\n";
   return 0;
}
