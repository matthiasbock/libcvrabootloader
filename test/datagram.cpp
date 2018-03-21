/**
 * @file
 * This program tests the class Datagram by inputing
 * byte-by-byte of a test datagram and check,
 * whether the datagram values are initialized correctly.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <Datagram.hpp>


bool assertion_failed = false;


void assert(char* s, bool test)
{
    if (test)
    {
        printf("Assertion passed: %s\n", s);
    }
    else
    {
        printf("Assertion failed: %s\n", s);
        assertion_failed = true;
    }
}


int main()
{
    printf("Testing Datagram class...\n");

    /*
     * Test data
     */
    const uint8_t test_data[] = {
            // Version
            0x02,
            // CRC
            0x12, 0x34, 0x56, 0x78,
            // Destination node count
            0x01,
            // Destination nodes
            0x42,
            // Data length
            0x04,
            // Data
            0x12, 0x34, 0x56, 0x78,
            // Trailing data
            0x00
    };

    /*
     * Construct a datagram from the test data
     */
    Datagram* d = new Datagram();
    for (uint8_t i=0; i<sizeof(test_data); i++)
    {
        d->inputByte(test_data[i]);
    }

    /*
     * Evaluate result
     */
    assert("Version", d->version == 2);
    assert("CRC", d->crc == 0x12345678);
    assert("Destination node count", d->destination_nodes_len == 1);
    assert("Destination node 1", d->destination_nodes[0] == 0x42);
    assert("Data length", d->data_len == 4);
    assert("Data integrity",
           d->getPayloadByte(0) == 0x12 &&
           d->getPayloadByte(1) == 0x34 &&
           d->getPayloadByte(2) == 0x56 &&
           d->getPayloadByte(3) == 0x78
           );

    if (!assertion_failed)
        printf("All assertions passed.\n");

    delete d;
    return 0;
}
