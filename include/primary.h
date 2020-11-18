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

#ifndef __PRIMARY_H
#define __PRIMARY_H

struct expression;
struct number;
struct rnd;
struct time;
struct var;
struct sin;
struct cos;
struct tan;
struct cot;
struct atn;
struct exp;
struct log;
struct abs;
struct sgn;
struct sqr;
struct int_;
struct pi;

struct primary {
	int type;
	union primary_union {
		struct expression *e;
		struct number *n;
		struct rnd *r;
		struct time *time;
		struct sin *sin;
		struct cos *cos;
		struct tan *tan;
		struct cot *cot;
		struct atn *atn;
		struct exp *exp;
		struct log *log;
		struct abs *abs;
		struct sgn *sgn;
		struct sqr *sqr;
		struct int_ *int_;
		struct var *v;
		struct pi *pi;
	} u;
};

struct primary *new_primary(int type, void *value);
struct primary *parse_primary(struct tokenizer *t);
struct number *eval_primary(struct primary *f);
void print_primary(struct primary *f);
void free_primary(struct primary *f);

#endif
