/**
 * @file
 * This file declares a class that receives CAN frames,
 * constructs datagrams and invokes a callback
 * whenever a complete datagram has been received.
 */

#ifndef _DATAGRAM_STREAM
#define _DATAGRAM_STREAM

#include <CANFrame.h>
#include <Datagram.hpp>

/**
 * Structure of the handler to be called,
 * when a complete datagram has been received
 */
typedef void (*datagram_complete_handler_t)(Datagram& datagram);


/**
 * Class handles the construction of datagrams
 * from individual CAN frames' bytes
 */
class DatagramStream
{
  public:
    Datagram* datagram = NULL;
    datagram_complete_handler_t datagram_complete_handler = NULL;

    DatagramStream();
    ~DatagramStream();

    /**
     * Append data to the current datagram
     */
    void operator<<(can_frame_t*);
};

#endif
