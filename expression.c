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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#include "addop.h"
#include "expression.h"
#include "term.h"

struct expression *new_expression(struct addop *term1_op, struct term *term1, struct addop *term2_op, struct term *term2) {

	struct expression *e = NULL;

	e = (struct expression *) malloc(sizeof(struct expression));
	if (e == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(e, '\0', sizeof(struct expression));

	e->term1_op = term1_op;
	e->term1    = term1;
	e->term2_op = term2_op;
	e->term2    = term2;

	return e;
}

int eval_expression(struct expression *e) {

	int t1, t2, o1, o2;

	if (e == NULL) {
		return 0;
	}

	o1 = (e->term1_op != NULL) ? eval_addop(e->term1_op) : 1;
	t1 = (e->term1    != NULL) ? eval_term( e->term1   ) : 0;
	o2 = (e->term2_op != NULL) ? eval_addop(e->term2_op) : 1;
	t2 = (e->term2    != NULL) ? eval_term( e->term2   ) : 0;

	return (o1 * t1) + (o2 * t2);
}

void print_expression(struct expression *e) {

	if (e == NULL) {
		return;
	}

	print_addop(e->term1_op);
	print_term(e->term1);
	print_addop(e->term2_op);
	print_term(e->term2);
}

void free_expression(struct expression *e) {
	if (e != NULL) {
		free_addop(e->term1_op);
		e->term1_op = NULL;

		free_term(e->term1);
		e->term1 = NULL;

		free_addop(e->term2_op);
		e->term2_op = NULL;

		free_term(e->term2);
		e->term2 = NULL;

		free(e);
		e = NULL;
	}
}
