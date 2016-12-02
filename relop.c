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

#include "tokenizer.h"

#include "relop.h"

struct relop *new_relop(int type) {

	struct relop *op = NULL;

	op = (struct relop *) malloc(sizeof(struct relop));
	if (op == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(op, '\0', sizeof(struct relop));

	op->type = type;

	return op;
}

struct relop *parse_relop(struct tokenizer *t) {

	token_get(t);
	switch (t->token.type) {
		case LTEQ:
			return new_relop(LTEQ);
		case LTGT:
			return new_relop(LTGT);
		case LT:
			return new_relop(LT);
		case GTEQ:
			return new_relop(GTEQ);
		case GTLT:
			return new_relop(GTLT);
		case GT:
			return new_relop(GT);
		case EQ:
			return new_relop(EQ);
		default:
			token_unget(t);
			return NULL;
	}
}

void print_relop(struct relop *op) {

	if (op == NULL) {
		return;
	}

	switch (op->type) {
		case LTEQ:
			printf("<=");
			break;
		case LTGT:
			printf("<>");
			break;
		case LT:
			printf("<");
			break;
		case GTEQ:
			printf(">=");
			break;
		case GTLT:
			printf("><");
			break;
		case GT:
			printf(">");
			break;
		case EQ:
			printf("=");
			break;
		default:
			printf("?");
			break;
	}
}

void free_relop(struct relop *op) {
	if (op != NULL) {
		free(op);
	}
}
