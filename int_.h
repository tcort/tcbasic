/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017, 2018, 2020  Thomas Cort <linuxgeek@gmail.com>

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

#ifndef __INT__H
#define __INT__H

struct number;
struct tokenizer;

struct int_ {
	struct expression *e;
};

struct int_ *new_int(struct expression *e);
struct int_ *parse_int(struct tokenizer *t);
struct number *eval_int(struct int_ *i);
void print_int(struct int_ *i);
void free_int(struct int_ *i);

#endif
