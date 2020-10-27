/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017, 2018, 2020  Thomas Cort <linuxgeek@gmail.com>

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#include "mulop.h"

struct mulop *new_mulop(int type) {

	struct mulop *op;

	op = (struct mulop *) malloc(sizeof(struct mulop));
	if (op == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(op, '\0', sizeof(struct mulop));

	op->type = type;

	return op;
}

struct mulop *parse_mulop(struct tokenizer *t) {

	token_get(t);
	switch (t->token.type) {
		case TIMES:
			return new_mulop(TIMES);
		case DIVIDE:
			return new_mulop(DIVIDE);
		case IDIVIDE:
			return new_mulop(IDIVIDE);
		case MOD:
			return new_mulop(MOD);
		default:
			token_unget(t);
			return NULL;
	}
}

void print_mulop(struct mulop *op) {

	if (op == NULL) {
		return;
	}

	switch (op->type) {
		case TIMES:
			printf("*");
			break;
		case DIVIDE:
			printf("/");
			break;
		case IDIVIDE:
			printf("\\");
			break;
		case MOD:
			printf("MOD");
			break;
		default:
			printf("?");
			break;
	}
}

void free_mulop(struct mulop *op) {
	if (op != NULL) {
		free(op);
	}
}
