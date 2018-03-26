
#include <stdio.h>
#include <time.h>
#include <SocketCAN.h>
#include <Datagram.hpp>
#include <NodeManager.hpp>
#include <Command.hpp>
#include <CommandPing.hpp>


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

    Command* cmd = new CommandPing();
    uint8_t nodes[] = {1};
    manager->sendCommand(cmd, nodes, 1);
    delete cmd;

    sleep(3);

    delete can;
    return 0;
}
