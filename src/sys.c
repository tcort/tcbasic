/*

  tcbasic - an interpreter for the programming language BASIC
  Copyright (C) 2022 Thomas Cort

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "config.h"

#include <fcntl.h> /* open(2)/O_RDONLY */
#include <stdlib.h> /* exit(3)/free(2)/malloc(2) */
#include <unistd.h> /* close(2)/read(2)/write(2) */

#include "str.h"
#include "sys.h"

/*
 * Terminates the current process
 *
 * Parameters:
 *  exit_code - integer indicating status (0=success, 1=failure, etc)
 *
 * Notes:
 *  should call with TC_EXIT_SUCCESS or TC_EXIT_FAILURE
 */
void tc_exit(int exit_code) {
	exit(exit_code);
}


/*
 * Open the file at filepath for reading
 *
 * Parameters:
 *  filepath - path to file to open
 *
 * Returns file descriptor or TC_ERR
 */
int tc_open_reader(char *filepath) {
	int fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		return TC_ERR;
	}

	return fd;
}

/*
 * Read a character from file descriptor fd
 *
 * Parameters:
 *  fd - file descriptor to read from
 *
 * Returns the character or TC_EOF or TC_ERR
 */
char tc_getc(int fd) {

	int rc;
	char s[1];

	rc = read(fd, s, 1);
	if (rc == 0) {
		return TC_EOF;
	} else if (rc == -1) {
		return TC_ERR;
	}

	return s[0];
}

/*
 * Write a character ch to file descriptor fd
 *
 * Parameters:
 *  fd - file descriptor to write to
 *  ch - character to write
 *
 * Returns TC_ERR or TC_OK
 */
int tc_putc(int fd, char ch) {

	int rc;
	char s[1];

	s[0] = ch;

	rc = write(fd, s, 1);
	if (rc == -1) {
		return TC_ERR;
	}

	return TC_OK;
}

/*
 * Close a file descriptor
 *
 * Parameters:
 *  fd - file descriptor to close
 *
 * Returns TC_OK or TC_ERR
 */
int tc_close(int fd) {
	int rc;

	rc = close(fd);
	if (rc == -1) {
		return TC_ERR;
	}

	return TC_OK;
}

/*
 * Allocates size bytes of memory on the heap
 * and initializes it to '\0'
 *
 * Parameters:
 *  size - number of bytes to allocate
 *
 * Returns pointer to the memory or TC_NULL
 */
void *tc_malloc(int size) {

	void *m;

	if (size <= 0) {
		return TC_NULL;
	}

	m = malloc(size);
	if (m == TC_NULL) {
		return TC_NULL;
	}
	tc_memset(m, TC_ENDSTR, size);

	return m;
}

/*
 * Frees heap memory pointed to by pointer m
 *
 * Parameters:
 *  m - pointer to heap memory
 *
 * Returns TC_NULL
 */
void *tc_free(void *m) {
	if (m != TC_NULL) {
		free(m);
		m = TC_NULL;
	}
	return TC_NULL;
}

