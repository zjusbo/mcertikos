#include <string.h>

#define VM_DYNLINK	0xe0000000

#define dll			((unsigned int *)VM_DYNLINK)

typedef void (*print_t) (char *, ...);
print_t kprint;

typedef char (*getc_t)(void);
getc_t kgetc;


void
init()
{
	kprint = (print_t) (dll[0]);
	kgetc = (getc_t)(dll[1]);
}


void
yield ()
{

}

int
sys_getc (void)
{
	return kgetc();
}

char output[1024];

void
sys_puts (const char *s, unsigned int len)
{
	unsigned int rlen = len < 1023 ? len : 1023;
	strncpy(output, s, rlen);
	output[rlen] = '\0';
	kprint("%s", s);
}
