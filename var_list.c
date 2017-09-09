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

#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "tokenizer.h"

#include "var_list.h"
#include "var.h"

struct var_list *new_var_list(struct var *var, struct var_list *list) {

	struct var_list *vl;

	vl = (struct var_list *) malloc(sizeof(struct var_list));
	if (vl == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(vl, '\0', sizeof(struct var_list));

	vl->var = var;
	vl->list = list;

	return vl;
}

struct var_list *parse_var_list(struct tokenizer *t) {

	struct var *v;

	v = parse_var(t);
	if (v == NULL) {
		return NULL;
	}

	token_get(t);
	if (t->token.type == COMMA) {
		return new_var_list(v, parse_var_list(t));
	} else {
		token_unget(t);
		return new_var_list(v, NULL);
	}
}

void eval_var_list(struct var_list *vl, char *line) {
	char *let;
	struct var_list *cur;
	size_t len;

	if (vl == NULL || line == NULL) {
		return;
	}

	len = strlen("LET X = ") + strlen(line) + 8;
	let = (char *) malloc(len*sizeof(char));
	if (let == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (cur = vl; cur != NULL; cur = cur->list) {
		char *s;
		struct var *v;
		v = cur->var;
		s = strsep(&line, ",");
		snprintf(let, len, "LET %c = %s", v->value, (s != NULL) ? s : "0");
		eval(let);
	}

	free(let);
	let = NULL;
}

void print_var_list(struct var_list *vl) {

	if (vl == NULL) {
		return;
	}

	print_var(vl->var);
	if (vl->list != NULL) {
		printf(", ");
		print_var_list(vl->list);
	}
}

void free_var_list(struct var_list *vl) {
	if (vl != NULL) {
		free_var(vl->var);
		free_var_list(vl->list);
		free(vl);
	}
}
