
#include <DatagramStream.hpp>


DatagramStream::DatagramStream() {}


DatagramStream::~DatagramStream()
{
    if (datagram != NULL)
    {
        delete datagram;
    }
}


/*
ostream& operator<<(ostream& stream)
{
    if (datagram == NULL)
    {
        datagram = new Datagram();
    }

    // TODO: Append stream bytes to datagram
    // datagram.appendBytesToBuffer(streamBytes);
    // if (datagram.isComplete()) { yield };

    return new ostream();
}
*/
