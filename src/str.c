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

#include "str.h"

#include "tokenizer.h"

struct str *new_str(char *value) {

	struct str *s;

	s = (struct str *) malloc(sizeof(struct str));
	if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(s, '\0', sizeof(struct str));

	s->value = value;

	return s;
}

struct str *parse_str(struct tokenizer *t) {
	token_get(t);
	if (t->token.type == STR) {
		size_t len = strlen(t->token.text+1);
		char *s = strdup(t->token.text+1);
		s[len - 1] = '\0'; /* chomp ending double quote */
		return new_str(s);
	} else {
		token_unget(t);
		return NULL;
	}
}

char *eval_str(struct str *s) {
	if (s == NULL) {
		return NULL;
	}

	return s->value;
}

void print_str(struct str *s) {
	if (s == NULL) {
		return;
	}

	printf("\"%s\"", s->value);
}

void free_str(struct str *s) {
	if (s != NULL) {
		if (s->value != NULL) {
			free(s->value);
		}
		free(s);
	}
}
