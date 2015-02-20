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
#include "number.h"

struct number *new_number(int value) {

	struct number *n = NULL;

	n = (struct number *) malloc(sizeof(struct number));
	if (n == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(n, '\0', sizeof(struct number));

	n->value = value;

	return n;
}

int eval_number(struct number *n) {
	if (n == NULL) {
		return 0;
	}

	return n->value;
}

void print_number(struct number *n) {
	if (n == NULL) {
		return;
	}

	printf("%d", n->value);
}

void free_number(struct number *n) {
	if (n != NULL) {
		free(n);
		n = NULL;
	}
}
