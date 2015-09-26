// Text-mode CGA/VGA display output

#include <lib/string.h>

#include <lib/x86.h>
#include <lib/debug.h>

#include "video.h"
//tmphack
//#include<dev/serial.h>
void
video_init(void)
{
	volatile uint16_t *cp;
	uint16_t was;
	unsigned pos;

	/* Get a pointer to the memory-mapped text display buffer. */
	cp = (uint16_t*) CGA_BUF;
	was = *cp;
	*cp = (uint16_t) 0xA55A;
	if (*cp != 0xA55A) {
		cp = (uint16_t*) MONO_BUF;
		addr_6845 = MONO_BASE;
		dprintf("addr_6845:%x\n",addr_6845);
	} else {
		*cp = was;
		addr_6845 = CGA_BASE;
		dprintf("addr_6845:%x\n",addr_6845);
	}
	
	/* Extract cursor location */
	outb(addr_6845, 14);
	pos = inb(addr_6845 + 1) << 8;
	outb(addr_6845, 15);
	pos |= inb(addr_6845 + 1);

	terminal.crt_buf = (uint16_t*) cp;
	terminal.crt_pos = pos;
	terminal.active_console = 0;
//  video_clear_screen();
}

void
video_putc(int c)
{

	// if no attribute given, then use black on white
	if (!(c & ~0xFF))
		c |= 0x0700;

	switch (c & 0xff) {
	case '\b':
		if (terminal.crt_pos > 0) {
			terminal.crt_pos--;
			terminal.crt_buf[terminal.crt_pos] = (c & ~0xff) | ' ';
		}
		break;
	case '\n':
		terminal.crt_pos += CRT_COLS;
		/* fallthru */
	case '\r':
		terminal.crt_pos -= (terminal.crt_pos % CRT_COLS);
		break;
	case '\t':
		video_putc(' ');
		video_putc(' ');
		video_putc(' ');
		video_putc(' ');
		video_putc(' ');
		break;
	default:
		terminal.crt_buf[terminal.crt_pos++] = c;		/* write the character */
		break;
	}

	if (terminal.crt_pos >= CRT_SIZE) {
		int i;
		memmove(terminal.crt_buf, terminal.crt_buf + CRT_COLS,
			(CRT_SIZE - CRT_COLS) * sizeof(uint16_t));
		for (i = CRT_SIZE - CRT_COLS; i < CRT_SIZE; i++)
			terminal.crt_buf[i] = 0x0700 | ' ';
		terminal.crt_pos -= CRT_COLS;
	}


	/* move that little blinky thing */
	outb(addr_6845, 14);
	outb(addr_6845 + 1, terminal.crt_pos >> 8);
	outb(addr_6845, 15);
	outb(addr_6845 + 1, terminal.crt_pos);
      
	//tmphack
        //static  int tmpcount = 0;
        /*static	int tmpcount = 0 ;
       	int i;
	if(tmpcount <11946)
	  {
        for (i = 0;
       !(inb(COM1 + COM_LSR) & COM_LSR_TXRDY) && i < 12800;//is_transimit_empty
       	     i++)
       	  {
       	    inb(0x84);
       	    inb(0x84);
       	    inb(0x84);
       	    inb(0x84);
       	  }
       outb(COM1+COM_TX, c);
       tmpcount++;
	  }
	*/
}

void
video_set_cursor (int x, int y)
{
    terminal.crt_pos = x * CRT_COLS + y;
}

void
video_clear_screen ()
{
    int i;
    for (i = 0; i < CRT_SIZE; i++)
    {
        terminal.crt_buf[i] = ' ';
    }
}
