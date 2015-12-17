#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf("ping started.\n");
    unsigned int val = 100;
    unsigned int *addr = (unsigned int *)0xe0000000;
    printf("ping: the value at address %x: %d\n", addr, *addr);
    printf("ping: writing the value %d to the address %x\n", val, addr);
    *addr = val;
    yield();
    printf("ping: the new value at address %x: %d\n", addr, *addr);

    return 0;
}
