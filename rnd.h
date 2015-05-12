/*
    tcbasic - a small BASIC Interpreter written in C.
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

#ifndef __RND_H
#define __RND_H

struct number;
struct tokenizer;

struct rnd {
	char xxx;
};

struct rnd *new_rnd(void);
struct rnd *parse_rnd(struct tokenizer *t);
struct number *eval_rnd(void);
void print_rnd(struct rnd *r);
void free_rnd(struct rnd *r);

#endif
