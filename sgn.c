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

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "sgn.h"
#include "expression.h"
#include "number.h"

struct sgn *new_sgn(struct expression *e) {

	struct sgn *a = NULL;

	a = (struct sgn *) malloc(sizeof(struct sgn));
	if (a == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(a, '\0', sizeof(struct sgn));

	a->e = e;

	return a;
}

struct sgn *parse_sgn(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != SGN) {
		token_unget(t);
		return NULL;
	}

	token_get(t);
	if (t->token.type != OPAREN) {
		token_unget(t);
		return NULL;
	}

	e = parse_expression(t);
	if (e == NULL) {
		return NULL;
	}

	token_get(t);
	if (t->token.type != CPAREN) {
		token_unget(t);
		free_expression(e);
		return NULL;
	}

	return new_sgn(e);
}

struct number *eval_sgn(struct sgn *a) {
	int r;
	struct number *n;

	r = 0;
	n = eval_expression(a->e);
	if (n->type == INT) {
		if (n->value.ival > 0) {
			r = 1;
		} else if (n->value.ival < 0) {
			r = -1;
		}
	} else { /* float */
		if (n->value.fval > 0.0) {
			r = 1;
		} else if (n->value.fval < 0.0) {
			r = -1;
		}
	}

	n->value.ival = r;
	n->type = INT; /* cast to int */

	return n;
}

void print_sgn(struct sgn *a) {
	if (a == NULL) {
		return;
	}

	printf("SGN (");
	print_expression(a->e);
	printf(")");
}

void free_sgn(struct sgn *a) {

	if (a != NULL) {
		if (a->e != NULL) {
			free_expression(a->e);
			a->e = NULL;
		}
		free(a);
	}
}
