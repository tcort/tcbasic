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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr_list.h"
#include "expr_item.h"

#include "tokenizer.h"

struct expr_list *new_expr_list(struct expr_item *expr_item, struct expr_list *list) {

	struct expr_list *el;

	el = (struct expr_list *) malloc(sizeof(struct expr_list));
	if (el == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(el, '\0', sizeof(struct expr_list));

	el->expr_item = expr_item;
	el->list = list;

	return el;
}

struct expr_list *parse_expr_list(struct tokenizer *t) {

	struct expr_item *i;

	i = parse_expr_item(t);
	if (i == NULL) {
		return NULL;
	}

	token_get(t);
	if (t->token.type == COMMA) {
		return new_expr_list(i, parse_expr_list(t));
	} else {
		token_unget(t);
		return new_expr_list(i, NULL);
	}
}

void eval_expr_list(struct expr_list *el) {

	if (el == NULL) {
		return;
	}

	eval_expr_item(el->expr_item);
	eval_expr_list(el->list);
}

void print_expr_list(struct expr_list *el) {

	if (el == NULL) {
		return;
	}

	print_expr_item(el->expr_item);
	if (el->list != NULL) {
		printf(", ");
		print_expr_list(el->list);
	}
}

void free_expr_list(struct expr_list *el) {
	if (el != NULL) {
		free_expr_item(el->expr_item);
		free_expr_list(el->list);
		free(el);
	}
}
