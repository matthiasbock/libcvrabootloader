
#ifndef _COMMAND_PING
#define _COMMAND_PING

#include <Command.hpp>


class CommandPing: public Command
{
  public:
    CommandPing();
    ~CommandPing();

    /**
     * Write the command's serialized i.e. binary representation to a buffer
     *
     * @param buffer    Allocated buffer to write to
     * @param max_size  Maximum number of bytes that can be written to the provided buffer
     * @return          Number of bytes written to buffer
     */
    uint32_t encode(uint8_t* buffer, uint32_t max_size);
};

#endif
