#include "httpserver.h"
int main()
{
    chdir("/home/kiosk/picture/www/picture");
    httpserver p;
    p.start();
    return 0;
}