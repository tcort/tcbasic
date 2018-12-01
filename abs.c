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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "abs.h"
#include "expression.h"
#include "number.h"

struct abs *new_abs(struct expression *e) {

	struct abs *a;

	a = (struct abs *) malloc(sizeof(struct abs));
	if (a == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(a, '\0', sizeof(struct abs));

	a->e = e;

	return a;
}

struct abs *parse_abs(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != ABS) {
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

	return new_abs(e);
}

struct number *eval_abs(struct abs *a) {
	struct number *n;

	n = eval_expression(a->e);
	if (n->type == INT) {
		n->value.ival = abs(n->value.ival);
	} else { /* float */
		n->value.fval = fabsf(n->value.fval);
	}

	return n;
}

void print_abs(struct abs *a) {
	if (a == NULL) {
		return;
	}

	printf("ABS (");
	print_expression(a->e);
	printf(")");
}

void free_abs(struct abs *a) {

	if (a != NULL) {
		if (a->e != NULL) {
			free_expression(a->e);
			a->e = NULL;
		}
		free(a);
	}
}
