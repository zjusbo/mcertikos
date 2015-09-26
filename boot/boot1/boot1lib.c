#include <boot1lib.h>

/**
 * video
 */
volatile char *video = (volatile char*) 0xB8000;

void
putc (int l, int color, char ch)
{
	volatile char * p = video + l * 2;
	* p = ch;
	* (p + 1) = color;
}


int
puts (int r, int c, int color, const char *string)
{
	int l = r * 80 + c;
	while (*string != 0)
	{
		putc (l++, color, *string++);
	}
	return l;
}

static int row = 0;

char * blank =
"                                                                                ";

void
putline (char * s)
{
	puts (row = (row >= CRT_ROWS) ? 0 : row + 1, 0, VGA_CLR_BLACK, blank);
	puts (row, 0, VGA_CLR_WHITE, s);
}

void
roll (int r)
{
	row = r;
}

void
panic (char * m)
{
	puts (0, 0, VGA_CLR_RED, m);
	while (1)
	{
		asm volatile("hlt");
	}
}

static char puti_str[40];

void
puti (int32_t i)
{
	itoh (i, puti_str);
	putline (puti_str);
}


/**
 * string
 */

int
strlen (const char *s)
{
	int n;

	for (n = 0; *s != '\0'; s++)
		n++;
	return n;
}

/* reverse:  reverse string s in place */
void
reverse (char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen (s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* itoa:  convert n to characters in s */
void
itox (int n, char s[], int root, char * table)
{
	int i, sign;

	if ((sign = n) < 0) /* record sign */
		n = -n; /* make n positive */
	i = 0;
	do
	{ /* generate digits in reverse order */
		s[i++] = table[n % root]; /* get next digit */
	} while ((n /= root) > 0); /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse (s);
}

void
itoa (int n, char s[])
{
	static char dec[] = "0123456789";
	itox(n, s, 10, dec);
}


void
itoh (int n, char* s)
{
	static char hex[] = "0123456789abcdef";
	itox(n, s, 16, hex);
}


/**
 * disk
 */
static void
waitdisk (void)
{
	// wait for disk reaady
	while ((inb (0x1F7) & 0xC0) != 0x40)
		/* do nothing */;
}

void
readsector (void *dst, uint32_t offset)
{
	// wait for disk to be ready
	waitdisk ();

	outb (0x1F2, 1);		// count = 1
	outb (0x1F3, offset);
	outb (0x1F4, offset >> 8);
	outb (0x1F5, offset >> 16);
	outb (0x1F6, (offset >> 24) | 0xE0);
	outb (0x1F7, 0x20);	// cmd 0x20 - read sectors

	// wait for disk to be ready
	waitdisk ();

	// read a sector
	insl (0x1F0, dst, SECTOR_SIZE / 4);
}

// Read 'count' bytes at 'offset' from kernel into virtual address 'va'.
// Might copy more than asked
void
readsection (uint32_t va, uint32_t count, uint32_t offset, uint32_t lba)
{
	uint32_t end_va;

	va &= 0xFFFFFF;
	end_va = va + count;
	// round down to sector boundary
	va &= ~(SECTOR_SIZE - 1);

	// translate from bytes to sectors, and kernel starts at sector 1
	offset = (offset / SECTOR_SIZE) + lba;

	// If this is too slow, we could read lots of sectors at a time.
	// We'd write more to memory than asked, but it doesn't matter --
	// we load in increasing order.
	while (va < end_va)
	{
		readsector ((uint8_t*) va, offset);
		va += SECTOR_SIZE;
		offset++;
	}
}
