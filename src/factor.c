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

struct factor *new_factor(struct primary *p, struct factor *f) {

	struct factor *r;

	r = (struct factor *) malloc(sizeof(struct factor));
	if (r == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(r, '\0', sizeof(struct factor));

	r->p = p;
	r->f = f;

	return r;
}

struct factor *parse_factor(struct tokenizer *t) {

	struct primary *p;
	struct factor *f;

	p = parse_primary(t);
	if (p == NULL) {
		return NULL;
	}

	token_get(t);
	if (t->token.type == CIRCUMFLEX) {
		f = parse_factor(t);
		if (f == NULL) {
			free_primary(p);
			return NULL;
		}
	} else {
		f = NULL;
		token_unget(t);
	}

	return new_factor(p, f);
}

struct number * eval_factor(struct factor *f) {

	struct number *n1 = NULL;
	struct number *n2 = NULL;
	struct number *r = NULL;

	if (f == NULL) {
		return new_number_from_int(1);
	}

	n1 = eval_primary(f->p);
	if (f->f == NULL) {
		r = clone_number(n1);
	} else {
		n2 = eval_factor(f->f);
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

	print_primary(f->p);
	if (f->f != NULL) {
		printf("^");
		print_factor(f->f);
	}
}

void free_factor(struct factor *f) {
	if (f != NULL) {
		if (f->p != NULL) {
			free_primary(f->p);
			f->p = NULL;
		}
		if (f->f != NULL) {
			free_factor(f->f);
			f->f = NULL;
		}
		free(f);
	}
}
