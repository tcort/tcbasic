/*
    tcb - a small BASIC Interpreter written in C.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#include "expression.h"
#include "rnd.h"

struct rnd *new_rnd(struct expression *expression) {

	struct rnd *r = NULL;

	r = (struct rnd *) malloc(sizeof(struct rnd));
	if (r == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(r, '\0', sizeof(struct rnd));

	r->expression = expression;

	return r;
}

int eval_rnd(struct rnd *r) {
	int n;

	if (r == NULL) {
		return rand();
	}

	n = eval_expression(r->expression);
	return rand() % n;
}

void print_rnd(struct rnd *r) {
	if (r == NULL) {
		return;
	}

	printf("RND(");
	print_expression(r->expression);
	printf(")");
}

void free_rnd(struct rnd *r) {
	if (r != NULL) {
		if (r->expression != NULL) {
			free_expression(r->expression);
			r->expression = NULL;
		}
		free(r);
		r = NULL;
	}
}
