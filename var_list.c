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

#include "var_list.h"
#include "var.h"

void parseLine(char *line);

struct var_list *new_var_list(struct var *var, struct var_list *list) {

	struct var_list *vl = NULL;

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

void eval_var_list(struct var_list *vl, char *line) {
	char *s;
	char *let;
	struct var *v;
	struct var_list *cur;
	size_t len;

	if (vl == NULL || line == NULL) {
		return;
	}

	len = strlen("LET X = ") + strlen(line) + 8;
	let = (char *) malloc(len*sizeof(char));

	for (cur = vl; cur; cur = cur->list) {
		v = cur->var;
		s = strsep(&line, ",");
		snprintf(let, len, "LET %c = %s", v->value, (s != NULL) ? s : "0");
		parseLine(let);
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
		printf(",");
		print_var_list(vl->list);
	}
}

void free_var_list(struct var_list *vl) {
	if (vl != NULL) {
		free_var(vl->var);
		vl->var = NULL;

		free_var_list(vl->list);
		vl->list = NULL;

		free(vl);
		vl = NULL;
	}
}
