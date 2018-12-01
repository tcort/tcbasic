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

#include "rem.h"

#include "tokenizer.h"

struct rem *new_rem(char *value) {

	struct rem *r;

	r = (struct rem *) malloc(sizeof(struct rem));
	if (r == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(r, '\0', sizeof(struct rem));

	r->value = value;

	return r;
}

struct rem *parse_rem(struct tokenizer *t) {
	token_get(t);
	if (t->token.type == REM) {
		char *s = strdup(t->token.text);
		return new_rem(s);
	} else {
		token_unget(t);
		return NULL;
	}
}

void print_rem(struct rem *r) {
	if (r == NULL) {
		return;
	}

	printf("%s", r->value);
}

void free_rem(struct rem *r) {
	if (r != NULL) {
		if (r->value != NULL) {
			free(r->value);
		}
		free(r);
	}
}
