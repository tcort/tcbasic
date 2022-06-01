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
#include "str.h"

/*
 * fills memory pointed to by s with ch in every byte
 *
 * Parameters:
 *  s - pointer to memory to set
 *  ch - character to set every byte to
 *  len - length of memory to overwrite
 */
void tc_memset(char *s, char ch, int len) {
	int i;

	for (i = 0; i < len; i++) {
		s[i] = ch;
	}
}
