
#include <SocketCAN.h>


SocketCAN* can;


int main()
{
    can = new SocketCAN();
    can->open("can0");



    delete can;
    return 0;
}
