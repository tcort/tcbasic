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

struct addop *new_addop(int type) {

	struct addop *op = NULL;

	op = (struct addop *) malloc(sizeof(struct addop));
	if (op == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(op, '\0', sizeof(struct addop));

	op->type = type;

	return op;
}

int eval_addop(struct addop *op) {

	if (op == NULL) {
		return 1;
	}

	return (op->type == PLUS) ? 1 : -1;
}

void print_addop(struct addop *op) {

	if (op == NULL) {
		return;
	}

	switch (op->type) {
		case PLUS:
			printf("+");
			break;
		case MINUS:
			printf("-");
			break;
		default:
			printf("?");
			break;
	}
}

void free_addop(struct addop *op) {
	if (op != NULL) {
		free(op);
		op = NULL;
	}
}
