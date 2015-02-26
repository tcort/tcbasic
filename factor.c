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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "expression.h"
#include "factor.h"
#include "number.h"
#include "rnd.h"
#include "var.h"

struct factor *new_factor(int type, void *value) {

	struct factor *f = NULL;

	f = (struct factor *) malloc(sizeof(struct factor));
	if (f == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(f, '\0', sizeof(struct factor));

	f->type = type;
	switch (f->type) {
		case EXPRESSION:
			f->u.e = (struct expression *) value;
			break;
		case NUMBER:
			f->u.n = (struct number *) value;
			break;
		case RND:
			f->u.r = (struct rnd *) value;
			break;
		case VAR:
			f->u.v = (struct var *) value;
			break;
	}

	return f;
}

struct factor *parse_factor(struct tokenizer *t) {

	static struct var *v;
	static struct number *num;
	static struct rnd *r;

	v = parse_var(t);
	if (v != NULL) {
		return new_factor(VAR, v);
	}

	num = parse_number(t);
	if (num != NULL) {
		return new_factor(NUMBER, num);
	}

	token_get(t);
	if (t->token.type == OPAREN) {
		static struct expression *expr;
		expr = parse_expression(t);
		if (expr == NULL) {
			return NULL;
		}
		token_get(t);
		if (t->token.type == CPAREN) {
			return new_factor(EXPRESSION, expr);
		}
		token_unget(t);
		free_expression(expr);
		return NULL;
	}
	token_unget(t);
	r = parse_rnd(t);
	if (r != NULL) {
		return new_factor(RND, r);
	}

	return NULL;
}

struct number * eval_factor(struct factor *f) {

	struct number *n = NULL;

	if (f == NULL) {
		return new_number_from_int(1);
	}

	switch (f->type) {
		case EXPRESSION:
			n = eval_expression(f->u.e);
			break;
		case NUMBER:
			n = clone_number(f->u.n);
			break;
		case RND:
			n = eval_rnd(f->u.r);
			break;
		case VAR:
			n = eval_var(f->u.v);
			break;
	}

	return n;
}

void print_factor(struct factor *f) {

	if (f == NULL) {
		return;
	}

	switch (f->type) {
		case EXPRESSION:
			printf("(");
			print_expression(f->u.e);
			printf(")");
			break;
		case NUMBER:
			print_number(f->u.n);
			break;
		case RND:
			print_rnd(f->u.r);
			break;
		case VAR:
			print_var(f->u.v);
			break;
	}
}

void free_factor(struct factor *f) {
	if (f != NULL) {
		switch (f->type) {
			case EXPRESSION:
				free_expression(f->u.e);
				f->u.e = NULL;
				break;
			case NUMBER:
				free_number(f->u.n);
				f->u.n = NULL;
				break;
			case RND:
				free_rnd(f->u.r);
				f->u.r = NULL;
				break;
			case VAR:
				free_var(f->u.v);
				f->u.v = NULL;
				break;
		}
		free(f);
	}
}
