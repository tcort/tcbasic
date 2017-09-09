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
#include <time.h>

#include "tokenizer.h"

#include "number.h"
#include "time_.h"

struct time *new_time(void) {

	struct time *t;

	t = (struct time *) malloc(sizeof(struct time));
	if (t == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(t, '\0', sizeof(struct time));

	return t;
}

struct time *parse_time(struct tokenizer *t) {

	token_get(t);
	if (t->token.type != TIME) {
		token_unget(t);
		return NULL;
	}

	return new_time();
}

struct number *eval_time(void) {

	/*
	 * computes seconds since midnight
	 * this should work correctly even on days where the time changes to/from daylight savings time
	 */

	struct tm t;
	time_t now;
	time_t midnight;

	time(&now); /* get current time */
	t = *localtime(&now);

	t.tm_hour = 0; /* rewind to midnight */
	t.tm_min = 0;
	t.tm_sec = 0;

	midnight = mktime(&t); /* get unix timestamp for midnight today */

	return new_number_from_int(now - midnight);
}

void print_time(struct time *t) {
	if (t == NULL) {
		return;
	}

	printf("TIME");
}

void free_time(struct time *t) {
	if (t != NULL) {
		free(t);
	}
}
