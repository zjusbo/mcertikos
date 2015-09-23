#include <stdio.h>
#include <types.h>

#define VM_TOP		0xffffffff
#define VM_USERHI	0xf0000000
#define VM_DYNLINK	0xe0000000
#define VM_STACKHI	0xd0000000
#define VM_USERLO	0x40000000
#define VM_BOTTOM	0x00000000


int
main (int argc, char **argv)
{
	printf ("Welcome to the user process! (Ctrl - Z to exit)\n\n");

  unsigned int i;
  unsigned int addr = 0;
  unsigned int val;
  char c;
  printf ("Specify a virtual address to read from or write to.\n");
  printf("Enter the address: ");
	while (1)
	{
		i = sys_getc();

		if (i == 0)
			continue;
		else if (i == 0x1a) { // exit when Ctrl-Z is entered
      printf("\nExiting from the user process.\n");
			break;
    }
    else if (i == 0x0a || i == 0x0d) { // output new line
      printf("\n");
      printf("Address entered: %u\n", addr);
      printf("Specify the action: r for read, w for write.\n");
      while (1) {
        c = sys_getc();
        if (c == 0)
          continue;
        printf("%c\n", c);
        if (c == 'r') {
            val = *((unsigned int*)addr);
            printf("The value at virtual address %u is %d.\n", addr, val);
            break;
        }
        else if (c == 'w') {
            printf("Enter the value you want to write to the address.\n");
            printf("Value to write (from 0 to 9): ");
            while(1) {
              val = sys_getc();
              if (val == 0 || val < '0' || val > '9')
                  continue;
              printf("%c\n", val);
              *((unsigned int*)addr) = val - '0';
              printf("Successfully wrote the value to the virtual address. You can double check it with the read command.\n");
              break;
            }
            break;
        }
        else {
            printf("\nInvalid action.\n");
            printf("Specify the action: r for read, w for write.\n");
        }
      }
      addr = 0;
      printf ("\nSpecify a virtual address to read from or write to.\n");
      printf("Enter the address: ");
      continue;
    }
    else if (i < '0' || i > '9')
      continue;
    else if (addr >= 0xffffffff / 10) {
      printf("\nAddress out of range.\n");
      addr = 0;
      printf("Enter the address: ");
      continue;
    }

    addr = addr * 10 + i - '0';
		printf("%c", i);
	}

	return 0;
}
