/**
 * @file
 * This file declares a class representing the
 * structure of a CVRA CAN bootloader datagram.
 */

#ifndef _DATAGRAM
#define _DATAGRAM

#include <stdint.h>
#include <stddef.h>
#include <CANFrame.h>
#include <Command.hpp>

#define DATAGRAM_VERSION        1
#define DATAGRAM_HEADER_SIZE    5

/**
 * Structure of the handler to be called,
 * when a complete datagram has been received
 */
class Datagram;
typedef void (*datagram_complete_handler_t)(Datagram* datagram);


class Datagram
{
  private:
    /**
     * Pointer to array of data bytes
     */
    uint8_t* data = NULL;

    /**
     * True, if all bytes of a datagram have been received
     * regardless of whether the datagram is valid or not
     */
    bool datagram_is_complete = false;

  public:
    /**
     * Buffer position where to write the next input byte
     */
    uint32_t input_cursor = 0;

    /**
     * 8-bit datagram version
     * must equal CAN_DATAGRAM_VERSION
     */
    uint8_t version = DATAGRAM_VERSION;

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
     * Function to execute, when a complete datagram has been received
     */
    datagram_complete_handler_t datagram_complete_handler = NULL;


    Datagram();
    ~Datagram();

    /**
     * Inputs a byte into the datagram
     */
    void inputByte(uint8_t);

    /**
     * Use stream operator to input bytes into the datagram
     */
    void operator<<(uint8_t b);

    /**
     * Append the payload of a CAN frame
     */
    void operator<<(can_frame_t*);

    /**
     * Append a command MsgPack to the data buffer
     */
    void operator<<(Command* cmd);

    /**
     * Clear all datagram properties and buffers
     */
    void reset();

    /**
     * Return a byte from the datagram payload buffer
     */
    uint8_t getPayloadByte(uint8_t index);

    /**
     * Returns true if the datagram is complete
     */
    bool isComplete();

    /**
     * Return the number of bytes this datagram occupies when assembled
     */
    uint32_t getSize();

    /**
     * Writes the datagram to a buffer
     *
     * Make sure, you have allocated at least getSize() bytes for buffer.
     */
    void writeToBuffer(uint8_t* buffer);

    /**
     * Allocate space for a buffer and write datagram to this buffer
     *
     * @return  Pointer to created buffer
     */
    uint8_t* asBuffer();

    /**
     * Computes the CRC32 of the datagram
     */
    uint32_t computeCRC();

    /**
     * Compute a CRC assuming the required buffer
     * (without the datagram header)
     * has already been written
     */
    uint32_t computeCRC(uint8_t* buffer, uint32_t size);

    /**
     * Compute and insert CRC into already prepared datagram buffer
     * (including the datagram header)
     */
    void insetCRC(uint8_t* buffer, uint32_t size);

    /**
     * Returns true if the datagram is valid (complete and CRC match)
     */
    bool isValid();
};

#endif
