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

#include "prng.h"
#include "tokenizer.h"

#include "number.h"
#include "pi.h"

struct pi *new_pi(void) {

	struct pi *r;

	r = (struct pi *) malloc(sizeof(struct pi));
	if (r == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(r, '\0', sizeof(struct pi));

	return r;
}

struct pi *parse_pi(struct tokenizer *t) {

	token_get(t);
	if (t->token.type != PI) {
		token_unget(t);
		return NULL;
	}

	return new_pi();
}

struct number *eval_pi(void) {
	return new_number_from_float(3.14159265);
}

void print_pi(struct pi *r) {
	if (r == NULL) {
		return;
	}

	printf("π");
}

void free_pi(struct pi *r) {
	if (r != NULL) {
		free(r);
	}
}
