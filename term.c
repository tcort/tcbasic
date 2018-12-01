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

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "factor.h"
#include "mulop.h"
#include "number.h"
#include "term.h"

struct term *new_term(struct factor *left, struct mulop *op, struct factor *right) {

	struct term *t;

	t = (struct term *) malloc(sizeof(struct term));
	if (t == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(t, '\0', sizeof(struct term));

	t->left = left;
	t->op = op;
	t->right = right;

	return t;
}

struct term *parse_term(struct tokenizer *t) {
	struct factor *f1;
	struct mulop *op;
	struct factor *f2;

	f1 = parse_factor(t);
	if (f1 == NULL) {
		return NULL;
	}

	op = parse_mulop(t);
	if (op == NULL) {
		return new_term(f1, NULL, NULL);
	}

	f2 = parse_factor(t);
	if (f2 == NULL) {
		free_factor(f1);
		free_mulop(op);
		return NULL;
	}

	return new_term(f1, op, f2);
}

struct number * eval_term(struct term *t) {

	struct number *l;
	struct number *r;
	struct number *result;

	if (t == NULL) {
		return new_number_from_int(0);
	}

	l = eval_factor(t->left);
	r = eval_factor(t->right);

	if (t->op != NULL && t->op->type == DIVIDE) {
		result = divide_number(l, r);
	} else if (t->op != NULL && t->op->type == MOD) {
		result = modulus_number(l, r);
	} else {
		result = multiply_number(l, r);
	}

	free_number(l);
	free_number(r);

	return result;
}

void print_term(struct term *t) {

	if (t == NULL) {
		return;
	}

	print_factor(t->left);
	if (t->op != NULL) {
		printf(" ");
		print_mulop(t->op);
		printf(" ");
		print_factor(t->right);
	}
}

void free_term(struct term *t) {
	if (t != NULL) {
		free_factor(t->left);
		free_mulop(t->op);
		free_factor(t->right);
		free(t);
	}
}
