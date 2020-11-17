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

#ifndef __NUMBER_H
#define __NUMBER_H

struct tokenizer;

struct number {
	enum number_type {
		INT,
		FLOAT
	} type;
	union number_value {
		int ival;
		float fval;
	} value;
};

#define INT_VALUE(n) (n->type == INT ? n->value.ival : (int)n->value.fval)
#define FLOAT_VALUE(n) (n->type == FLOAT ? n->value.fval : (float) n->value.ival)

struct number *new_number(char *s);
struct number *new_number_from_int(int i);
struct number *new_number_from_float(float f);
struct number *parse_number(struct tokenizer *t);
struct number *clone_number(struct number *n);
struct number *add_number(struct number *x, struct number *y);
struct number *multiply_number(struct number *x, struct number *y);
struct number *divide_number(struct number *x, struct number *y);
struct number *idivide_number(struct number *x, struct number *y);
struct number *modulus_number(struct number *x, struct number *y);
struct number *pow_number(struct number *x, struct number *y);
void print_number(struct number *n);
void free_number(struct number *n);

#endif
