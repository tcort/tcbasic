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

#include "log.h"
#include "expression.h"
#include "number.h"

struct log *new_log(struct expression *e) {

	struct log *l = NULL;

	l = (struct log *) malloc(sizeof(struct log));
	if (l == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(l, '\0', sizeof(struct log));

	l->e = e;

	return l;
}

struct log *parse_log(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != LOG) {
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

	return new_log(e);
}

struct number *eval_log(struct log *l) {
	struct number *n;

	n = eval_expression(l->e);
	if (n->type == INT) {
		float f;
		f = logf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = logf(n->value.fval);
	}

	return n;
}

void print_log(struct log *l) {
	if (l == NULL) {
		return;
	}

	printf("LOG (");
	print_expression(l->e);
	printf(")");
}

void free_log(struct log *l) {

	if (l != NULL) {
		if (l->e != NULL) {
			free_expression(l->e);
			l->e = NULL;
		}
		free(l);
	}
}
