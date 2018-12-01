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

#include "int_.h"
#include "expression.h"
#include "number.h"

struct int_ *new_int(struct expression *e) {

	struct int_ *i;

	i = (struct int_ *) malloc(sizeof(struct int_));
	if (i == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(i, '\0', sizeof(struct int_));

	i->e = e;

	return i;
}

struct int_ *parse_int(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != INT_) {
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

	return new_int(e);
}

struct number *eval_int(struct int_ *i) {
	struct number *n;

	n = eval_expression(i->e);
	if (n->type == FLOAT) {
		n->type = INT;
		n->value.ival = (int) n->value.fval;
	}

	return n;
}

void print_int(struct int_ *i) {
	if (i == NULL) {
		return;
	}

	printf("INT (");
	print_expression(i->e);
	printf(")");
}

void free_int(struct int_ *i) {

	if (i != NULL) {
		if (i->e != NULL) {
			free_expression(i->e);
			i->e = NULL;
		}
		free(i);
	}
}
