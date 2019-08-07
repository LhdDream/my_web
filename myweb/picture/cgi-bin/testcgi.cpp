#include <iostream>
#include <vector>
#include <string>
 
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
 
#include <stdio.h>
#include <stdlib.h>
 
using namespace std;
using namespace cgicc;     // Or reference as cgicc::Cgicc formData; below in object instantiation.
 
int main(int argc, char **argv)
{
    try {
       Cgicc formData;
 
       // Send HTTP header: Content-type: text/html
       cout << HTTPHTMLHeader() << endl;
 
       // Print: <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
       cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
 
       // Print: <html lang="en" dir="LTR">
       cout << html().set("lang", "EN").set("dir", "LTR") << endl;
 
       // Set up the HTML document
       cout << html() << head() << title("Cgicc example") << head() << endl;
       cout << body().set("bgcolor","#cccccc").set("text","#000000").set("link","#0000ff").set("vlink","#000080") << endl;
 
       cout << h1("This is a demonstration of the GNU CgiCC library") << endl;
 
       form_iterator fvalue1 = formData.getElement("value1");
       if( !fvalue1->isEmpty() && fvalue1 != (*formData).end()) {
          cout << "Value1: " << **fvalue1 << endl;
       }
       else
          cout << "No text entered for value1" << endl;
 
       cout << p();
 
       form_iterator fvalue2 = formData.getElement("value2");
       if( !fvalue2->isEmpty() && fvalue2 != (*formData).end()) {
          // Note this is just a different way to access the string class.
          // See the YoLinux GNU string class tutorial.
          cout << "Value2: " << (**fvalue2).c_str() << endl;
       }
 
       cout << p();
 
       form_iterator fvalue3 = formData.getElement("value3");
       if( !fvalue3->isEmpty() && fvalue3 != (*formData).end()) {
          cout << "Value3: " << **fvalue3 << endl;
       }
 
       cout << p();
 
       form_iterator fvalue4 = formData.getElement("value4");
       if( !fvalue4->isEmpty() && fvalue4 != (*formData).end()) {
          cout << "Value4: " << **fvalue4 << endl;
       }
 
       // Close the HTML document
       cout << body() << html();
    }
    catch(exception& e) {
       // handle any errors here.
       cout << "ERROR!!" << endl;
    }
    ;   // To avoid Apache errors.
}
