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
#include "io.h"
#include "str.h"
#include "sys.h"

/*
 * Write string s to file descriptor fd
 *
 * Parameters:
 *  fd - file descriptor to write to
 *  s - NUL terminated string to write
 *
 * Returns TC_OK upon success, TC_ERR upon error.
 */
int tc_puts(int fd, char *s) {

	int rc;
	int i;

	for (i = 0; i < tc_strlen(s); i++) {
		rc = tc_putc(fd, s[i]);
		if (rc == TC_ERR) {
			return TC_ERR;
		}
	}

	return TC_OK;
}
