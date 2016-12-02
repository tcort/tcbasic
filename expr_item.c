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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr_item.h"
#include "expression.h"
#include "number.h"
#include "str.h"

struct expr_item *new_expr_item(struct expression *e, struct str *str) {

	struct expr_item *ei = NULL;

	ei = (struct expr_item *) malloc(sizeof(struct expr_item));
	if (ei == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(ei, '\0', sizeof(struct expr_item));

	ei->e = e;
	ei->str = str;

	return ei;
}

struct expr_item *parse_expr_item(struct tokenizer *t) {

	struct expression *expr;
	struct str *s;

	expr = parse_expression(t);
	if (expr != NULL) {
		return new_expr_item(expr, NULL);
	}

	s = parse_str(t);
	if (s != NULL) {
		return new_expr_item(NULL, s);
	}

	return NULL;
}

void eval_expr_item(struct expr_item *ei) {

	if (ei == NULL) {
		return;
	}

	if (ei->str != NULL) {
		printf("%s", eval_str(ei->str));
	} else {
		struct number *n = eval_expression(ei->e);
		print_number(n);
		free_number(n);
	}
}

void print_expr_item(struct expr_item *ei) {

	if (ei == NULL) {
		return;
	}

	print_expression(ei->e);
	print_str(ei->str);
}

void free_expr_item(struct expr_item *ei) {
	if (ei != NULL) {
		free_str(ei->str);
		free_expression(ei->e);
		free(ei);
	}
}
