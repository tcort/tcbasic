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

#ifndef TC_SYS_H
#define TC_SYS_H

/* EXIT CODES */
#define TC_EXIT_SUCCESS (0)
#define TC_EXIT_FAILURE (1)

/* FILE NUMBERS */
#define TC_STDIN  (0)
#define TC_STDOUT (1)
#define TC_STDERR (2)

/* RETURN CODES */
#define TC_OK (0)
#define TC_ERR (-1)
#define TC_EOF (-2)

/* MISC */
#define TC_NULL ((void *) 0)

/* prototypes */
void tc_exit(int exit_code);
int tc_open_reader(char *filepath);
char tc_getc(int fd);
int tc_putc(int fd, char ch);
int tc_close(int fd);
void *tc_malloc(int size);
void *tc_free(void *m);

#endif
