
#include <stdio.h>
#include <SocketCAN.h>
#include <Datagram.hpp>
#include <NodeManager.hpp>
#include <time.h>

using namespace std;

SocketCAN* can = NULL;
NodeManager* manager = NULL;


void bootloader_rx_handler_1(Datagram* d)
{
    printf("Received a datagram.\n");

    if (d->isValid())
    {
        printf("Wow, it's valid!\n");
    }
    else
    {
        printf("It's not valid though.\n");
    }
}


int main()
{
    can = new SocketCAN();
    manager = new NodeManager(can);
    manager->datagram_rx_handler[1] = &bootloader_rx_handler_1;
    can->parser = manager;

    can->open("can0");

    sleep(5);

    delete can;
    return 0;
}
