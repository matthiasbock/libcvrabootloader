/**
 * @file
 * This program tests the class Datagram.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <Datagram.hpp>


void assert(char* s, bool test)
{
    if (test)
        printf("Assertion passed: %s\n", s);
    else
        printf("Assertion failed: %s\n", s);
}


int main()
{
    Datagram* d = new Datagram();

    assert("version", d->version == 2);

    delete d;
    return 0;
}
