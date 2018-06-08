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

#ifndef __COT_H
#define __COT_H

struct number;
struct tokenizer;

struct cot {
	struct expression *e;
};

struct cot *new_cot(struct expression *e);
struct cot *parse_cot(struct tokenizer *t);
struct number *eval_cot(struct cot *c);
void print_cot(struct cot *c);
void free_cot(struct cot *c);

#endif
