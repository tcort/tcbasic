/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016  Thomas Cort <linuxgeek@gmail.com>

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

#ifndef __BUFFER_H
#define __BUFFER_H

typedef struct Buffer {
	char *buf;      /* pointer returned by malloc */
	size_t maxsize;    /* size of space allocated by malloc */
	size_t cursize;    /* amount of space currently used */
	size_t incr;       /* increment to add when growing maxsize */
} Buffer;

/* allocates a new Buffer structure */
Buffer *bf_alloc(size_t size, size_t incr);

/* adds the character ch to the end of the given Buffer */
void bf_addch(Buffer *buf, char ch);

/* adds the string str to the end of the given Buffer */
void bf_addstr(Buffer *buf, char *str);

/* re-initializes the given Buffer, such that the next call to
   bf_addch will put the character at the start of the buffer */
void bf_clear(Buffer *buf);

/* tests that the given pointer is a valid Buffer */
void bf_valid(Buffer *buf);

/* de-allocates the Buffer and any memory it may contain */
void bf_free(Buffer *buf);

#endif
