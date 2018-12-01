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

#include "exp.h"
#include "expression.h"
#include "number.h"

struct exp *new_exp(struct expression *e) {

	struct exp *ex;

	ex = (struct exp *) malloc(sizeof(struct exp));
	if (ex == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(ex, '\0', sizeof(struct exp));

	ex->e = e;

	return ex;
}

struct exp *parse_exp(struct tokenizer *t) {

	struct expression *e;

	token_get(t);
	if (t->token.type != EXP) {
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

	return new_exp(e);
}

struct number *eval_exp(struct exp *ex) {
	struct number *n;

	n = eval_expression(ex->e);
	if (n->type == INT) {
		float f;
		f = expf((1.0f) * n->value.ival);
		n->type = FLOAT;
		n->value.fval = f;
	} else { /* float */
		n->value.fval = expf(n->value.fval);
	}

	return n;
}

void print_exp(struct exp *ex) {
	if (ex == NULL) {
		return;
	}

	printf("EXP (");
	print_expression(ex->e);
	printf(")");
}

void free_exp(struct exp *ex) {

	if (ex != NULL) {
		if (ex->e != NULL) {
			free_expression(ex->e);
			ex->e = NULL;
		}
		free(ex);
	}
}
