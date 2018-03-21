
#include <Datagram.hpp>
#include <stdio.h>
#include <stdlib.h>


Datagram::Datagram() {}


Datagram::~Datagram() {}


void Datagram::inputByte(uint8_t b)
{
//    printf("Constructing datagram: Adding one byte at index %d...\n", input_cursor);

    // Version
    if (input_cursor == 0)
    {
        version = b;
        input_cursor++;
        return;
    }

    // CRC
    if (input_cursor < 5)
    {
        crc = (crc << 8) + b;
        input_cursor++;
        return;
    }

    // Destination nodes length/count
    if (input_cursor == 5)
    {
        printf("Datagram CRC: %#010x\n", crc);

        if (b == 0)
        {
            printf("Error: Destination node length cannot be zero.\n");
            return;
        }
        destination_nodes_len = b;
        if (destination_nodes != NULL)
        {
            free(destination_nodes);
        }

        printf("Datagram destination node count: %d\n", destination_nodes_len);

        destination_nodes = (uint8_t*) malloc(b);
        if (destination_nodes == NULL)
        {
            printf("Error: Failed to allocate %d bytes for destination_nodes.\n", b);
            return;
        }
        input_cursor++;
        return;
    }

    const uint32_t index_first_destination_node = 6;
    uint32_t index_last_destination_node = 5 + destination_nodes_len;

    // Array of destination nodes
    if (input_cursor <= index_last_destination_node)
    {
        if (destination_nodes == NULL)
        {
            printf("Error: Datagram's destination_nodes array is suddenly NULL.\n");
            return;
        }
        uint8_t destination_node_index = input_cursor - index_first_destination_node;
        destination_nodes[destination_node_index] = b;

        printf("Datagram destination node %d: %d\n", destination_node_index, destination_nodes[destination_node_index]);

        input_cursor++;
        return;
    }

    // Data length
    if (input_cursor == index_last_destination_node + 1)
    {
        data_len = b << 24;
        input_cursor++;
        return;
    }
    if (input_cursor == index_last_destination_node + 2)
    {
        data_len |= b << 16;
        input_cursor++;
        return;
    }
    if (input_cursor == index_last_destination_node + 3)
    {
        data_len |= b << 8;
        input_cursor++;
        return;
    }
    if (input_cursor == index_last_destination_node + 4)
    {
        data_len |= b;

        printf("Datagram payload size: %d\n", data_len);

        if (data != NULL)
        {
            free(data);
        }
        if (data_len > 0)
        {
            data = (uint8_t*) malloc(data_len);
        }
        input_cursor++;
        return;
    }

    uint32_t index_first_data_byte = index_last_destination_node + 5;
    uint32_t index_last_data_byte = index_first_data_byte + data_len - 1;

    // Data
    if (input_cursor <= index_last_data_byte)
    {
        if (data == NULL)
        {
            printf("Error: Datagram's data buffer is suddenly NULL.\n");
            return;
        }
        uint32_t data_index = input_cursor - index_first_data_byte;
        data[data_index] = b;

        if (input_cursor == index_last_data_byte)
        {
            if (!datagram_is_complete)
            {
                if (datagram_complete_handler != NULL)
                {
                    (*datagram_complete_handler)(this);
                }
                datagram_is_complete = true;
            }
        }

        input_cursor++;
        return;
    }

    // Trailing data
    printf("Warning: Datagram received superfluous trailing data.\n");
}


void Datagram::operator<<(uint8_t b)
{
    inputByte(b);
}


void Datagram::operator<<(can_frame_t* frame)
{
    if (frame->can_dlc == 0)
        return;

    for (uint8_t i=0; i < frame->can_dlc; i++)
    {
        inputByte(frame->data[i]);
    }
}


void Datagram::reset()
{
    input_cursor = 0;
    datagram_is_complete = false;

    version = CAN_DATAGRAM_VERSION;
    crc = 0;
    destination_nodes_len = 0;
    if (destination_nodes != NULL)
    {
        free(destination_nodes);
        destination_nodes = NULL;
    }
    data_len = 0;
    if (data != NULL)
    {
        free(data);
        data = NULL;
    }
}


uint8_t Datagram::getPayloadByte(uint8_t index)
{
    if (index < data_len)
    {
        return data[index];
    }
    return 0;
}


bool Datagram::isComplete()
{
    return datagram_is_complete;
}


bool Datagram::isValid()
{
    return isComplete() && (computeCRC() == crc);
}


uint32_t Datagram::computeCRC()
{
    // TODO
    return 0;
}

