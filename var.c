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
#include "runtime.h"
#include "var.h"

struct var *new_var(int value) {

	struct var *v = NULL;

	v = (struct var *) malloc(sizeof(struct var));
	if (v == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(v, '\0', sizeof(struct var));

	v->value = value;

	return v;
}

int eval_var(struct var *v) {
	if (v == NULL) {
		return 0;
	}

	return runtime_get_var(v->value);
}

void print_var(struct var *v) {
	if (v == NULL) {
		return;
	}

	printf("%c", v->value);
}

void free_var(struct var *v) {
	if (v != NULL) {
		free(v);
		v = NULL;
	}
}
