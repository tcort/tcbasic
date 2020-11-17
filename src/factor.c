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

#include "factor.h"
#include "number.h"
#include "primary.h"

struct factor *new_factor(struct primary *p1, struct primary *p2) {

	struct factor *f;

	f = (struct factor *) malloc(sizeof(struct factor));
	if (f == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(f, '\0', sizeof(struct factor));

	f->p1 = p1;
	f->p2 = p2;

	return f;
}

struct factor *parse_factor(struct tokenizer *t) {

	struct primary *p1;
	struct primary *p2;

	p1 = parse_primary(t);
	if (p1 == NULL) {
		return NULL;
	}

	token_get(t);
	if (t->token.type == CIRCUMFLEX) {
		p2 = parse_primary(t);
		if (p2 == NULL) {
			free_primary(p1);
			return NULL;
		}
	} else {
		p2 = NULL;
		token_unget(t);
	}

	return new_factor(p1, p2);
}

struct number * eval_factor(struct factor *f) {

	struct number *n1 = NULL;
	struct number *n2 = NULL;
	struct number *r = NULL;

	if (f == NULL) {
		return new_number_from_int(1);
	}

	n1 = eval_primary(f->p1);
	if (f->p2 == NULL) {
		r = clone_number(n1);
	} else {
		n2 = eval_primary(f->p2);
		r = pow_number(n1, n2);
	}

	free_number(n1);
	free_number(n2);

	return r;
}

void print_factor(struct factor *f) {

	if (f == NULL) {
		return;
	}

	print_primary(f->p1);
	if (f->p2 != NULL) {
		printf("^");
		print_primary(f->p1);
	}
}

void free_factor(struct factor *f) {
	if (f != NULL) {
		if (f->p1 != NULL) {
			free_primary(f->p1);
			f->p1 = NULL;
		}
		if (f->p2 != NULL) {
			free_primary(f->p2);
			f->p2 = NULL;
		}
		free(f);
	}
}
