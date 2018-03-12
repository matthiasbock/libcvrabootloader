
#include <Datagram.hpp>


Datagram::Datagram() {};


Datagram::~Datagram() {};


void Datagram::inputByte(uint8_t b)
{
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
        destination_nodes = malloc(b);
        if (destination_nodes == NULL)
        {
            printf("Error: Failed to allocate %d bytes for destination_nodes.\n", b);
        }
        input_cursor++;
        return;
    }

    const uint32_t index_first_destination_node = 6;
    uint32_t index_last_destination_node = 5 + destination_nodes_len;

    // Array of destination nodes
    if (input_cursor <= index_last_destination_node)
    {
        data[input_cursor - index_first_destination_node] = b;
        input_cursor++;
        return;
    }

    // Data length
    if (input_cursor == index_last_destination_node + 1)
    {
        data_len = b;
        if (data != NULL)
        {
            free(data);
        }
        data = malloc(b);
        input_cursor++;
        return;
    }

    uint32_t index_first_data_byte = index_last_destination_node + 2;
    uint32_t index_last_data_byte = index_first_data_byte + data_len - 1;

    // Data
    if (input_cursor <= index_last_data_byte)
    {
        data[input_cursor - index_first_data_byte] = b;
        input_cursor++;
        return;
    }

    // Trailing data
    printf("Warning: Datagram received superfluous trailing data.\n");
}


bool Datagram::isComplete()
{
    return (destination_nodes != NULL && data != NULL && input_cursor >= 1+4+1+destination_nodes_len+1+data_len);
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
