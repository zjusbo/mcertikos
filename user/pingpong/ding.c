#include <proc.h>
#include <stdio.h>
#include <syscall.h>

int main (int argc, char **argv)
{
    printf("ding started.\n");
    unsigned int val = 300;
    unsigned int *addr = (unsigned int *)0xe0000000;
    printf("ding: the value at address %x: %d\n", addr, *addr);
    printf("ding: writing the value %d to the address %x\n", val, addr);
    *addr = val;
    yield();
    printf("ding: the new value at address %x: %d\n", addr, *addr);

    return 0;
}
