#include <stdlib.h>

int
atoi(const char *buf, int *i)
{
	int loc = 0;
	int numstart = 0;
	int acc = 0;
	int negative = 0;
	if (buf[loc] == '+')
		loc++;
	else if (buf[loc] == '-') {
		negative = 1;
		loc++;
	}
	numstart = loc;
	// no grab the numbers
	while ('0' <= buf[loc] && buf[loc] <= '9') {
		acc = acc*10 + (buf[loc]-'0');
		loc++;
	}
	if (numstart == loc) {
		// no numbers have actually been scanned
		return 0;
	}
	if (negative)
		acc = - acc;
	*i = acc;
	return loc;
}
