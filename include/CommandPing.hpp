
#ifndef _COMMAND_PING
#define _COMMAND_PING

#include <Command.hpp>


class CommandPing: public Command
{
  public:
    CommandPing();
    ~CommandPing();

    uint32_t encode(uint8_t* buffer, uint32_t* size, uint32_t max_size);
};

#endif
