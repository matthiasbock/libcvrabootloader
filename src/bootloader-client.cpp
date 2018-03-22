
#include <stdio.h>
#include <SocketCAN.h>
#include <Datagram.hpp>
#include <NodeManager.hpp>
#include <time.h>

using namespace std;

SocketCAN* can = NULL;
NodeManager* manager = NULL;


void bootloader_rx_handler_1(uint8_t node_id, Datagram* d)
{
    printf("Received a datagram from node ID %#02x.\n", node_id);
}


int main()
{
    can = new SocketCAN();
    manager = new NodeManager(can);
    manager->datagram_rx_handler[1] = &bootloader_rx_handler_1;

    can->open("can0");

    sleep(5);

    delete can;
    return 0;
}
