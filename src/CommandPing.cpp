
#include <CommandPing.hpp>


CommandPing::CommandPing() {}


CommandPing::~CommandPing() {}


uint32_t CommandPing::encode(uint8_t* buffer, uint32_t* size, uint32_t max_size)
{
    buffer = msgpack.pack(COMMAND_SET_VERSION, COMMAND_PING);
}
