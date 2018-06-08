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

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "atn.h"
#include "expression.h"
#include "number.h"

struct atn *new_atn(struct expression *e) {

	struct atn *a;

	a = (struct atn *) malloc(sizeof(struct atn));
	if (a == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(a, '\0', sizeof(struct atn));

	a->e = e;

	return a;
}

struct atn *parse_atn(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != ATN) {
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

	return new_atn(e);
}

struct number *eval_atn(struct atn *a) {
	struct number *n;

	n = eval_expression(a->e);
	if (n->type == INT) {
		float f;
		f = atanf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = atanf(n->value.fval);
	}

	return n;
}

void print_atn(struct atn *a) {
	if (a == NULL) {
		return;
	}

	printf("ATN (");
	print_expression(a->e);
	printf(")");
}

void free_atn(struct atn *a) {

	if (a != NULL) {
		if (a->e != NULL) {
			free_expression(a->e);
			a->e = NULL;
		}
		free(a);
	}
}
