/*
 * this basically does the same thing as hexdump.c, except it's a function that
 * you can call in your own programs. handy for debugging net code.
 */

#ifndef _HEXDUMP_H
#define _HEXDUMP_H

#include <stdio.h>

void hexdump(const char *ptr, const size_t len) {
	unsigned cols = 16;
	unsigned i = 0, j;

	while (i < len) {
		printf("%04x: ", i, i);
		
		j = 0;
		while (j < cols && i + j < len) {
			printf("%02x ", ((char *) ptr)[i + j]);
			j++;
		}

		while (j < cols) {
			printf("   ");
			j++;
		}
		
		printf("|% 5u ", i);

		j = 0;
		while (j < cols && i + j < len) {
			if (((char*)ptr)[i+j] >= 32 && ((char*)ptr)[i+j] <= 126) {
				putchar(((char*)ptr)[i+j]);
			} else {
				putchar('.');
			}

			j++;
		}

		i += cols;
		putchar('\n');
	}
}

#endif
