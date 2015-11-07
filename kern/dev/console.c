#include <lib/string.h>
#include <lib/types.h>
#include <lib/debug.h>
#include <lib/spinlock.h>

#include "video.h"
#include "console.h"
#include "serial.h"
#include "keyboard.h"

#define BUFLEN 1024
static char linebuf[BUFLEN];

struct {
	char buf[CONSOLE_BUFFER_SIZE];
	uint32_t rpos, wpos;
} cons; // shared object, need lock to protect it

// lock for cons buffer
spinlock_t cons_lock;

void
cons_init()
{
        spinlock_init(&cons_lock);
	memset(&cons, 0x0, sizeof(cons));
	serial_init();
	video_init();
}

void
cons_intr(int (*proc)(void))
{
	int c;
        spinlock_acquire(&cons_lock);
	while ((c = (*proc)()) != -1) {
		if (c == 0)
			continue;
		cons.buf[cons.wpos++] = c;
		if (cons.wpos == CONSOLE_BUFFER_SIZE)
			cons.wpos = 0;
	}
        spinlock_release(&cons_lock);

}
// read a character from console buffer
char
cons_getc(void)
{
  int c;

  spinlock_acquire(&cons_lock);
  // poll for any pending input characters,
  // so that this function works even when interrupts are disabled
  // (e.g., when called from the kernel monitor).
  serial_intr();
  keyboard_intr();

  // grab the next character from the input buffer.
  if (cons.rpos != cons.wpos) {
    c = cons.buf[cons.rpos++];
    if (cons.rpos == CONSOLE_BUFFER_SIZE)
      cons.rpos = 0;
    spinlock_release(&cons_lock);
    return c;
  }
  spinlock_release(&cons_lock);
  return 0;
}

void
cons_putc(char c)
{
	serial_putc(c);
  video_putc(c);
}
// blocking read a char from console
char
getchar(void)
{
  char c;

  while ((c = cons_getc()) == 0)
    /* do nothing */;
  return c;
}

// put a char to console
void
putchar(char c)
{
  cons_putc(c);
}
// prompt is the output message 
char *
readline(const char *prompt)
{
  int i;
  char c;

  if (prompt != NULL)
    dprintf("%s", prompt);

  i = 0;
  while (1) {
    c = getchar();
    if (c < 0) {
      dprintf("read error: %e\n", c);
      return NULL;
    // backspace
    } else if ((c == '\b' || c == '\x7f') && i > 0) {
      putchar('\b');
      i--;
    // valid ascii char
    } else if (c >= ' ' && i < BUFLEN-1) {
      putchar(c);
      linebuf[i++] = c;
    // newline character, end of this line
    // return linebuff
    } else if (c == '\n' || c == '\r') {
      putchar('\n');
      linebuf[i] = 0;
      return linebuf;
    }
  }
}
