/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015  Thomas Cort <linuxgeek@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buffer.h"
#include "readaline.h"

/*
 * prints the prompt on the terminal only if standard input
 * is a terminal. Additionally, it reads stdin into buf until EOF or NL
 *
 * f from where to read input
 * prompt is the string to print to a terminal
 * buf is the buffer put characters
 * returns a pointer to the buffer
 * no error return value; any errors cause the program to exit
 */
Buffer *readaline(FILE *f, char *prompt, Buffer *lbuf) {

	char ch;
	int fd, rc;

	bf_valid(lbuf); /* check for valid Buffer */

	/* get the file descriptor number if standard input */
	fd = fileno(f);
	if (fd == -1) {
		perror("fileno");
		exit(1);
	}

	/* print the prompt on the terminal only if standard input
           is a terminal */
	rc = isatty(fd);
	if (rc == 1) {

		fprintf(stderr, "%s", prompt);
		fflush(stderr);
	}

	do {

		ch = (char) fgetc(f);
		if (ch == '\n' || feof(f)) {

			/* work until EOF or a newline */
			break;
		}

		/* put characters in the buffer */
		bf_addch(lbuf, ch);

	} while (1);

	/* terminate the buffer with a NULL character when done */
	bf_addch(lbuf, '\0');

	return lbuf;
}

