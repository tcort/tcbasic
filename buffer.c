/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017, 2018  Thomas Cort <linuxgeek@gmail.com>

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

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

/*
 * allocates a new Buffer structure
 *
 * size is the total size of the space allocated by malloc/realloc.
 * incr is the amount to add to size when the buffer needs to grow.
 */
Buffer *bf_alloc(size_t size, size_t incr) {

	Buffer *buffer;

	/* sanity check */
	if (size < 1 || incr < 1) {
		fprintf(stderr, "bf_alloc: Invalid Arguments");
		exit(EXIT_FAILURE);
	}

	/* allocate a new Buffer structure */
	buffer = (Buffer *) malloc(sizeof(Buffer));
	if (buffer == NULL) {
		perror("bf_alloc");
		exit(EXIT_FAILURE);
	}

	/* give the internal buffer the given initial "size" using
	   malloc() */
	buffer->buf = (char *) malloc(size);
	if (buffer->buf == NULL) {
		perror("bf_alloc");
		free(buffer);
		exit(EXIT_FAILURE);
	}

	/* copy the given "size" into maxsize */
	buffer->maxsize = size;

	/* saves the value of "incr" in the buffer structure for
           later use by bf_addch() */
	buffer->incr = incr;

	buffer->cursize = 0;

	/* return a pointer to the buffer structure on success */
	return buffer;
}

/* adds the character ch to the end of the given Buffer */
void bf_addch(Buffer *buf, char ch) {

	bf_valid(buf); /* check for valid Buffer */

	/* if the buffer is full */
	if (buf->cursize ==  buf->maxsize) {

		size_t newmaxsize = buf->maxsize + buf->incr;

		/* check if increasing maxsize by incr would cause an overflow */
		if (newmaxsize < buf->maxsize) {
			errno = EOVERFLOW;
			perror("bf_addch");
			exit(EXIT_FAILURE);
		}

                /* add incr bytes to maxsize and use realloc() to make
		   the buffer larger before adding the character */
		buf->buf = (char *) realloc(buf->buf, newmaxsize);
		if (buf->buf == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		buf->maxsize = newmaxsize;
	}

	buf->buf[buf->cursize] = ch;

	/* increments cursize by one to count the character added */
	buf->cursize++;
}

/* re-initializes the given Buffer, such that the next call to
   bf_addch will put the character at the start of the buffer */
void bf_clear(Buffer *buf) {

	bf_valid(buf); /* check for valid Buffer */

	/* does not alter maxsize, buf, or incr */
	buf->cursize = 0;
	/* does not need to overwrite the existing contents */
}

/*
 * tests that the given pointer is a valid Buffer
 *
 * buf is a pointer to the Buffer to be tested
 */
void bf_valid(Buffer *buf) {

	/* are all Buffer values consistent with each other? */
	if (buf == NULL || buf->buf == NULL || buf->cursize > buf->maxsize ||
		buf->maxsize < 1 || buf->incr < 1) {
		/* invalid buffer */
		exit(EXIT_FAILURE);
	}
}

/*
 * de-allocates the Buffer and any memory it may contain
 *
 * buf is a pointer to a Buffer that needs to be free()'d
 */
void bf_free(Buffer *buf) {

	if (buf != NULL) {

		if (buf->buf != NULL) {
			free(buf->buf);
		}

		free(buf);
	}
}

