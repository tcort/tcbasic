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

#ifndef __FACTOR_H
#define __FACTOR_H

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

struct factor {
	int type;
	union factor_union {
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
	} u;
};

struct factor *new_factor(int type, void *value);
struct factor *parse_factor(struct tokenizer *t);
struct number *eval_factor(struct factor *f);
void print_factor(struct factor *f);
void free_factor(struct factor *f);

#endif
