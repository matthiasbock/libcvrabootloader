/**
 * @file
 * This file declares constants and a parent class
 * for the different types of bootloader commands
 */

#ifndef _COMMAND
#define _COMMAND

#include <stdint.h>

#define COMMAND_SET_VERSION 3

typedef enum
{
    COMMAND_JUMP_TO_MAIN = 1,
    COMMAND_CRC = 2,
    COMMAND_ERASE = 3,
    COMMAND_WRITE = 4,
    COMMAND_PING = 5,
    COMMAND_READ = 6,
    COMMAND_UPDATE_CONFIG = 7,
    COMMAND_SAVE_CONFIG = 8,
    COMMAND_READ_CONFIG = 9,
    COMMAND_GET_STATUS = 10,
} command_t;


class Command
{
  public:
    Command();
    virtual ~Command();

    /**
     * Write the command's serialized i.e. binary representation to a buffer
     */
    virtual uint32_t encode(uint8_t* buffer, uint32_t* size, uint32_t max_size);
};


#endif
