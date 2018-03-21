
#include <stdio.h>
#include <SocketCAN.h>
#include <Datagram.hpp>
#include <time.h>


SocketCAN* can;

using namespace std;


void datagram_handler(Datagram* d)
{
    printf("Received a datagram.\n");

    if (d->isValid())
    {
        printf("It's even valid!\n");
    }
    else
    {
        printf("It's not valid though.\n");
    }
}


void stream_handler(can_frame_t* frame)
{
    static Datagram* d = NULL;
    if (d == NULL)
    {
        d = new Datagram();
        d->datagram_complete_handler = &datagram_handler;
    }

    printf("Received CAN frame with ID %#04x.\n", frame->can_id);

    switch (frame->can_id)
    {
        case 0x81:
            d->reset();
            *d << frame;
            break;

        case 0x01:
            *d << frame;
            break;

        default:
            break;
    }
}


int main()
{
    can = new SocketCAN();
    can->open("can0");
    can->reception_handler = &stream_handler;

    sleep(5);

    delete can;
    return 0;
}
