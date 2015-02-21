/*
    tcb - a small BASIC Interpreter written in C.
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

#include <string.h>

char *tcb_strsep(char **stringp, char *delim) {
	char *s;
	if (stringp == NULL || *stringp == NULL || *stringp[0] == '\0') {
		return NULL;
	}

	s = *stringp;
	if (delim == NULL || delim[0] == '\0') {
		*stringp = NULL;
		return s;
	}

	*stringp = strpbrk(s, delim);
	if (*stringp == NULL) {
		*stringp = NULL;
		return s;
	}

	*stringp[0] = '\0';
	*stringp += 1;

	return s;
}

