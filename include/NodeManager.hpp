/**
 * @file
 * This file declares a class, which receives all incoming CAN frames
 * and creates individual datagrams for all target node IDs.
 * Also the class offers methods to generate
 * and send datagrams to target nodes.
 */

#ifndef _NODE_MANAGER
#define _NODE_MANAGER

#include <CANAdapter.h>
#include <CANFrameParser.h>
#include <Datagram.hpp>

#define MIN_NODE_ID                 0x001
#define MAX_NODE_ID                 0x07f
#define NODE_ID_DATAGRAM_START_MASK 0x080


class NodeManager: public CANFrameParser
{
  public:
    /**
     * Pointer to the CAN adapter,
     * this manager listens on
     */
    CANAdapter* adapter = NULL;

    /**
     * Datagram buffers by target node ID
     */
    Datagram* datagram[MAX_NODE_ID];

    /**
     * Pointers to functions to call, when the corresponding ID
     * has received a complete and valid datagram
     */
    datagram_complete_handler_t datagram_rx_handler[MAX_NODE_ID];

    NodeManager(CANAdapter*);
    ~NodeManager();

    void parse_frame(can_frame_t*);
};


#endif
