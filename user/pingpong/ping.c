#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf("ping started.\n");

    unsigned int i;
    //fast producing
    for (i = 0; i < 10; i++)
      produce();

    //slow producing
    for (i = 0; i < 40; i++) {
      if (i % 4 == 0)
        produce();
    }

    return 0;
}
