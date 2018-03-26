
#include <Datagram.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>


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
//        printf("Datagram CRC: %#010x\n", crc);

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

//        printf("Datagram destination node count: %d\n", destination_nodes_len);

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

//        printf("Datagram destination node %d: %d\n", destination_node_index, destination_nodes[destination_node_index]);

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

//        printf("Datagram payload size: %d\n", data_len);

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

        if (input_cursor >= index_last_data_byte)
        {
//            printf("Datagram completed.\n");

            if (!datagram_is_complete)
            {
                datagram_is_complete = true;
                if (datagram_complete_handler != NULL)
                {
                    (*datagram_complete_handler)(this);
                }
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


void Datagram::operator<<(Command* cmd)
{
    const uint8_t DATA_DEFAULT_SIZE = 3;

    // Discard old payload
    if (data != NULL)
    {
        free(data);
    }

    // The MsgPack packer will reallocate more automatically, if needed.
    data = (uint8_t*) malloc(DATA_DEFAULT_SIZE);

    // Invoke MsgPack packer
    data_len = cmd->encode(data, DATA_DEFAULT_SIZE);
}


void Datagram::reset()
{
    input_cursor = 0;
    datagram_is_complete = false;

    version = DATAGRAM_VERSION;
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


uint32_t Datagram::getSize()
{
    return DATAGRAM_HEADER_SIZE + 1 + destination_nodes_len + 4 + data_len;
}


void Datagram::writeToBuffer(uint8_t* buffer)
{
    if (buffer == NULL)
        return;

    // Write datagram to buffer
    uint8_t* b = buffer;
    *(b++) = version;
    *(b++) = crc >> 24;
    *(b++) = (crc >> 16) & 0xFF;
    *(b++) = (crc >> 8) & 0xFF;
    *(b++) = crc & 0xFF;
    *(b++) = destination_nodes_len;
    for (uint8_t i=0; i<destination_nodes_len; i++)
    {
        *(b++) = destination_nodes[i];
    }
    *(b++) = data_len >> 24;
    *(b++) = (data_len >> 16) & 0xFF;
    *(b++) = (data_len >> 8) & 0xFF;
    *(b++) = data_len & 0xFF;
    for (uint32_t i=0; i<data_len; i++)
    {
        *(b++) = data[i];
    }
}


uint8_t* Datagram::asBuffer()
{
    // Allocate buffer
    uint8_t* buffer = (uint8_t*) malloc(getSize());

    if (buffer == NULL)
        return NULL;

    // Write datagram to buffer
    writeToBuffer(buffer);

    return buffer;
}


uint32_t Datagram::computeCRC()
{
    // Allocate memory and write datagram to buffer
    uint32_t size = getSize();
    uint8_t* buffer = asBuffer();

    // Compute CRC over buffer skipping the datagram header
    crc = computeCRC(buffer + DATAGRAM_HEADER_SIZE, size - DATAGRAM_HEADER_SIZE);

    free(buffer);
    return crc;
}


uint32_t Datagram::computeCRC(uint8_t* buffer, uint32_t size)
{
    // Initialize CRC32
    uint32_t _crc = crc32(0L, Z_NULL, 0);

    // Calculate CRC32 of the buffer
    _crc = crc32(_crc, buffer, size);

    printf("Calculated CRC: %#010x\n", _crc);

    return _crc;
}


void Datagram::insetCRC(uint8_t* buffer, uint32_t size)
{
    // Compute CRC over buffer skipping the datagram header
    uint32_t _crc = computeCRC(buffer + DATAGRAM_HEADER_SIZE, size - DATAGRAM_HEADER_SIZE);

    // Insert CRC into buffer
    buffer[1] = _crc >> 24;
    buffer[2] = (_crc >> 16) & 0xFF;
    buffer[3] = (_crc >> 8) & 0xFF;
    buffer[4] = _crc & 0xFF;
}
