
#include <DatagramStream.hpp>


DatagramStream::DatagramStream() {}


DatagramStream::~DatagramStream()
{
    if (datagram != NULL)
    {
        delete datagram;
    }
}


void DatagramStream::operator<<(can_frame_t* frame)
{
    if (datagram == NULL)
    {
        datagram = new Datagram();
    }

    if (frame->can_dlc == 0)
        return;

    for (uint8_t i=0; i < frame->can_dlc; i++)
    {
        *(this->datagram) << frame->data[i];
    }
}
