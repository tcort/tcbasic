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

#include "cot.h"
#include "expression.h"
#include "number.h"

struct cot *new_cot(struct expression *e) {

	struct cot *c = NULL;

	c = (struct cot *) malloc(sizeof(struct cot));
	if (c == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(c, '\0', sizeof(struct cot));

	c->e = e;

	return c;
}

struct cot *parse_cot(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != COT) {
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

	return new_cot(e);
}

struct number *eval_cot(struct cot *c) {
	struct number *n;

	n = eval_expression(c->e);
	if (n->type == INT) {
		float f;
		f = 1.0f / tanf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = 1.0f / tanf(n->value.fval);
	}

	return n;
}

void print_cot(struct cot *c) {
	if (c == NULL) {
		return;
	}

	printf("COT (");
	print_expression(c->e);
	printf(")");
}

void free_cot(struct cot *c) {

	if (c != NULL) {
		if (c->e != NULL) {
			free_expression(c->e);
			c->e = NULL;
		}
		free(c);
	}
}
