#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

void loopping() {
  while (1) {}
}

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
    printf("ping finished\n");

    return 0;
}
