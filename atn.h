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

#ifndef __ATN_H
#define __ATN_H

struct number;
struct tokenizer;

struct atn {
	struct expression *e;
};

struct atn *new_atn(struct expression *e);
struct atn *parse_atn(struct tokenizer *t);
struct number *eval_atn(struct atn *a);
void print_atn(struct atn *a);
void free_atn(struct atn *a);

#endif
