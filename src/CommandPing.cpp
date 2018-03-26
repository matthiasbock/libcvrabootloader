
#include <CommandPing.hpp>
#include <msgpack.h>


CommandPing::CommandPing() {}


CommandPing::~CommandPing() {}


uint32_t CommandPing::encode(uint8_t* buffer, uint32_t max_size)
{
    // Message buffer to write to
    msgpack_sbuffer msg;
    msg.data = (char*) buffer;
    msg.size = 0;
    msg.alloc = max_size;

    // Message writer
    msgpack_packer packer;

    msgpack_packer_init(&packer, &msg, msgpack_sbuffer_write);
    msgpack_pack_int(&packer, COMMAND_SET_VERSION);
    msgpack_pack_int(&packer, COMMAND_PING);
    msgpack_pack_array(&packer, 0);

    return msg.size;
}
