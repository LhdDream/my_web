#include "httpserver.h"
#include "../util/threads.h"
int main()
{
    chdir("/home/kiosk/picture/www/picture");
    signal(SIGPIPE,SIG_IGN);

        httpserver p;
        p.start();
    return 0;
}