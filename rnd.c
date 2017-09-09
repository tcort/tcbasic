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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prng.h"
#include "tokenizer.h"

#include "number.h"
#include "rnd.h"

struct rnd *new_rnd() {

	struct rnd *r;

	r = (struct rnd *) malloc(sizeof(struct rnd));
	if (r == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(r, '\0', sizeof(struct rnd));

	return r;
}

struct rnd *parse_rnd(struct tokenizer *t) {

	token_get(t);
	if (t->token.type != RND) {
		token_unget(t);
		return NULL;
	}

	return new_rnd();
}

struct number *eval_rnd(void) {
	return new_number_from_float(tcb_rand());
}

void print_rnd(struct rnd *r) {
	if (r == NULL) {
		return;
	}

	printf("RND");
}

void free_rnd(struct rnd *r) {
	if (r != NULL) {
		free(r);
	}
}
