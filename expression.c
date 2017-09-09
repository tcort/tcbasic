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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "addop.h"
#include "expression.h"
#include "number.h"
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

struct expression *parse_expression(struct tokenizer *t) {

	struct addop *t1_op;
	struct term *t1;
	struct addop *t2_op;
	struct term *t2;

	t1_op = parse_addop(t);
	if (t1_op == NULL) {
		t1 = parse_term(t);
		if (t1 == NULL) {			/* not an expression */
			return NULL;
		}
		t2_op = parse_addop(t);
		if (t2_op == NULL) {					/* X */
			return new_expression(NULL, t1, NULL, NULL);
		}
		t2 = parse_term(t);
		if (t2 == NULL) {
			free_term(t1);
			free_addop(t2_op);
			return NULL;
		}
		return new_expression(NULL, t1, t2_op, t2);		/* X+Y */
	}

	t1 = parse_term(t);
	if (t1 == NULL) {				/* + but no term */
		free_addop(t1_op);
		return NULL;
	}

	t2_op = parse_addop(t);
	if (t2_op == NULL) {						/* +X */
		return new_expression(t1_op, t1, NULL, NULL);
	}

	t2 = parse_term(t);						/* +X+ no term */
	if (t2 == NULL) {
		free_addop(t1_op);
		free_term(t1);
		free_addop(t2_op);
		return NULL;
	}
	return new_expression(t1_op, t1, t2_op, t2);
}

struct number *eval_expression(struct expression *e) {


	struct number *o1;
	struct number *t1;
	struct number *o2;
	struct number *t2;
	struct number *l;
	struct number *r;
	struct number *result;

	if (e == NULL) {
		return 0;
	}

	if (e->term1_op != NULL) {
		o1 = new_number_from_int(eval_addop(e->term1_op));
	} else {
		o1 = new_number_from_int(1);
	}

	if (e->term1 != NULL) {
		t1 = eval_term(e->term1);
	} else {
		t1 = new_number_from_int(0);
	}

	if (e->term2_op != NULL) {
		o2 = new_number_from_int(eval_addop(e->term2_op));
	} else {
		o2 = new_number_from_int(1);
	}

	if (e->term2 != NULL) {
		t2 = eval_term(e->term2);
	} else {
		t2 = new_number_from_int(0);
	}

	l = multiply_number(o1, t1);
	r = multiply_number(o2, t2);

	result = add_number(l, r);

	free_number(l);
	free_number(r);
	free_number(o1);
	free_number(t1);
	free_number(o2);
	free_number(t2);

	return result;
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
		free_term(e->term1);
		free_addop(e->term2_op);
		free_term(e->term2);
		free(e);
	}
}
