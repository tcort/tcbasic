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

#include "factor.h"
#include "mulop.h"
#include "term.h"

struct term *new_term(struct factor *left, struct mulop *op, struct factor *right) {

	struct term *t = NULL;

	t = (struct term *) malloc(sizeof(struct term));
	if (t == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(t, '\0', sizeof(struct term));

	t->left = left;
	t->op = op;
	t->right = right;

	return t;
}

int eval_term(struct term *t) {

	int l, r;

	if (t == NULL) {
		return 0;
	}

	l = eval_factor(t->left);
	r = eval_factor(t->right);

	return (t->op != NULL && t->op->type == DIVIDE) ? l / r : l * r;
}

void print_term(struct term *t) {

	if (t == NULL) {
		return;
	}

	print_factor(t->left);
	if (t->op != NULL) {
		printf(" ");
		print_mulop(t->op);
		printf(" ");
		print_factor(t->right);
	}
}

void free_term(struct term *t) {
	if (t != NULL) {
		free_factor(t->left);
		t->left = NULL;

		free_mulop(t->op);
		t->op = NULL;

		free_factor(t->right);
		t->right = NULL;

		free(t);
		t = NULL;
	}
}
