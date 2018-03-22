
#include <NodeManager.hpp>
#include <stdio.h>


NodeManager::NodeManager(CANAdapter* adapter)
{
    // Initialize datagram and handler function array
    for (uint8_t i=0; i<MAX_NODE_ID; i++)
    {
        datagram[i] = NULL;
        datagram_rx_handler[i] = NULL;
    }

    // Register a reception callback with the CAN adapter
    adapter->parser = this;
    this->adapter = adapter;
}


NodeManager::~NodeManager()
{
    // Unregister a reception callback with the CAN adapter
    if (adapter != NULL)
    {
        adapter->parser = NULL;
    }

    // Free allocated datagrams
    for (uint8_t i=0; i<MAX_NODE_ID; i++)
    {
        if (datagram[i] != NULL)
            delete datagram[i];
    }
}


void NodeManager::parse_frame(can_frame_t* frame)
{
    // Valid CAN ID and type?
    if (     (frame->can_id < MIN_NODE_ID || frame->can_id > MAX_NODE_ID) &&
            ((frame->can_id < (MIN_NODE_ID | NODE_ID_DATAGRAM_START_MASK)) ||
             (frame->can_id > (MAX_NODE_ID | NODE_ID_DATAGRAM_START_MASK)))
             )
        return;

    uint8_t dt_id = frame->can_id & ~NODE_ID_DATAGRAM_START_MASK;

    // Create new datagram, if necessary
    if (datagram[dt_id] == NULL)
    {
        datagram[dt_id] = new Datagram();
    }

    Datagram* dt = datagram[dt_id];

    // Detect beginning of new datagram
    if (frame->can_id & NODE_ID_DATAGRAM_START_MASK)
    {
        dt->reset();
    }
    else if (dt->input_cursor == 0)
    {
        printf("Warning: Discarding frame from CAN ID %#05x, because start bit was expected but not set.\n", frame->can_id);
        return;
    }

    // Append data to datagram
    if (dt->isComplete())
    {
        printf("Warning: Ignoring trailing data from CAN ID %#05x, because a complete datagram has already been received.\n", frame->can_id);
        return;
    }
    *dt << frame;

    if (dt->isValid())
    {
        if (datagram_rx_handler[dt_id] != NULL)
        {
            (*(datagram_rx_handler[dt_id]))(dt);
        }

        dt->reset();
    }
}
