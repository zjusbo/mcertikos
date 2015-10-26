#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf("pong started.\n");

    unsigned int i;
    for (i = 0; i < 20; i++) {
      if (i % 2 == 0)
        consume();
    }

    return 0;
}
