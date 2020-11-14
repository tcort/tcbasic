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

#include "config.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "cos.h"
#include "expression.h"
#include "number.h"

struct cos *new_cos(struct expression *e) {

	struct cos *s;

	s = (struct cos *) malloc(sizeof(struct cos));
	if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(s, '\0', sizeof(struct cos));

	s->e = e;

	return s;
}

struct cos *parse_cos(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != COS) {
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

	return new_cos(e);
}

struct number *eval_cos(struct cos *c) {
	struct number *n;

	n = eval_expression(c->e);
	if (n->type == INT) {
		float f;
		f = cosf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = cosf(n->value.fval);
	}

	return n;
}

void print_cos(struct cos *c) {
	if (c == NULL) {
		return;
	}

	printf("COS (");
	print_expression(c->e);
	printf(")");
}

void free_cos(struct cos *c) {

	if (c != NULL) {
		if (c->e != NULL) {
			free_expression(c->e);
			c->e = NULL;
		}
		free(c);
	}
}
