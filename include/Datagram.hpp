/**
 * @file
 * This file declares a class representing the
 * structure of a CVRA CAN bootloader datagram.
 */

#ifndef _DATAGRAM
#define _DATAGRAM

#include <stdint.h>
#include <stddef.h>

#define CAN_DATAGRAM_VERSION 1


class Datagram
{
//  private:
  public:
    /**
     * 8-bit datagram version
     * must equal CAN_DATAGRAM_VERSION
     */
    uint8_t version = CAN_DATAGRAM_VERSION;

    /**
     * 32-bit datagram content checksum
     */
    uint32_t crc = 0;

    /**
     * Number destination nodes
     * is also number of bytes to malloc() for destination_nodes
     */
    uint8_t destination_nodes_len = 0;
    /**
     * Pointer to array of destination node IDs
     */
    uint8_t* destination_nodes = NULL;

    /**
     * Number of payload bytes
     * is also number of bytes to malloc() for data
     */
    uint32_t data_len = 0;
    /**
     * Pointer to array of data bytes
     */
    uint8_t* data = NULL;

    /**
     * Position where to write the next input byte
     */
    uint32_t input_cursor = 0;

//  public:
    Datagram();
    ~Datagram();

    /**
     * Inputs a byte into the datagram
     */
    void inputByte(uint8_t);
    void operator<<(uint8_t b);

    /**
     * Returns true if the datagram is complete
     */
    bool isComplete();

    /**
     * Returns true if the datagram is valid (complete and CRC match)
     */
    bool isValid();

    /**
     * Computes the CRC32 of the datagram
     */
    uint32_t computeCRC();
};

#endif
