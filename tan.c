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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "tan.h"
#include "expression.h"
#include "number.h"

struct tan *new_tan(struct expression *e) {

	struct tan *t = NULL;

	t = (struct tan *) malloc(sizeof(struct tan));
	if (t == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(t, '\0', sizeof(struct tan));

	t->e = e;

	return t;
}

struct tan *parse_tan(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != TAN) {
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

	return new_tan(e);
}

struct number *eval_tan(struct tan *t) {
	struct number *n;

	n = eval_expression(t->e);
	if (n->type == INT) {
		float f;
		f = tanf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = tanf(n->value.fval);
	}

	return n;
}

void print_tan(struct tan *t) {
	if (t == NULL) {
		return;
	}

	printf("TAN (");
	print_expression(t->e);
	printf(")");
}

void free_tan(struct tan *t) {

	if (t != NULL) {
		if (t->e != NULL) {
			free_expression(t->e);
			t->e = NULL;
		}
		free(t);
	}
}
