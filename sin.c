/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017  Thomas Cort <linuxgeek@gmail.com>

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

#include "sin.h"
#include "expression.h"
#include "number.h"

struct sin *new_sin(struct expression *e) {

	struct sin *s = NULL;

	s = (struct sin *) malloc(sizeof(struct sin));
	if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(s, '\0', sizeof(struct sin));

	s->e = e;

	return s;
}

struct sin *parse_sin(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != SIN) {
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

	return new_sin(e);
}

struct number *eval_sin(struct sin *s) {
	struct number *n;

	n = eval_expression(s->e);
	if (n->type == INT) {
		float f;
		f = sinf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = sinf(n->value.fval);
	}

	return n;
}

void print_sin(struct sin *s) {
	if (s == NULL) {
		return;
	}

	printf("SIN (");
	print_expression(s->e);
	printf(")");
}

void free_sin(struct sin *s) {

	if (s != NULL) {
		if (s->e != NULL) {
			free_expression(s->e);
			s->e = NULL;
		}
		free(s);
	}
}
