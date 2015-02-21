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

#ifndef __LINE_H
#define __LINE_H

struct statement;
struct number;

struct line {
	struct statement *statement;
	int number;
	struct line *next;
};

struct line *new_line(struct statement *statement, struct number *number);
int eval_line(struct line *l);
void print_line(struct line *l);
void free_line(struct line *l);

#endif