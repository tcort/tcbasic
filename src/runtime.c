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
#include <time.h>

#include "line.h"
#include "number.h"
#include "statement.h"
#include "tokenizer.h"
#include "var.h"

#include "runtime.h"

struct line *lines = NULL;

struct line *runtime_get_first_line(void) {
	return lines;
}

struct line *runtime_get_line(int number) {
	struct line *cur;

	if (number <= 0) {
		return NULL;
	}

	for (cur = lines; cur != NULL; cur = cur->next) {
		if (number == cur->number) {
			return cur;
		}
	}

	return NULL;
}

int runtime_get_line_after_nearest_next(int for_line_number, char var) {

	struct line *cur;

	for (cur = runtime_get_line(for_line_number); cur != NULL; cur = cur->next) {
		if (cur->statement->type == NEXT && cur->statement->u.next_stmt.var->value == var) {
			return cur->next == NULL ? -1 : cur->next->number;
		}

	}

	return -1;

}

void runtime_rm_line(int number) {
	struct line *list = lines;
	struct line *head = list;
	struct line *cur = list;
	struct line *prev;

	if (number <= 0) {
		return;
	}

	if (head == NULL) { /* empty list */
		/* nothing to do */;
	} else if (head->number == number) { /* first element in list */
		head = head->next;
		cur->next = NULL;
		free_line(cur);
	} else {
		for (prev = cur, cur = cur->next; cur != NULL; prev = cur, cur = cur->next) {
			if (cur->number == number) {
				prev->next = cur->next;
				cur->next = NULL;
				free_line(cur);
				cur = prev;
			}
		}
	}

	lines = head;
}

void runtime_set_line(struct line *item) {
	struct line *list = lines;
	struct line *head = list;
	struct line *cur = list;

	if (item == NULL || item->number <= 0) {
		return;
	}

	if (head == NULL) { /* empty list, make item 1st element */
		head = item;
	} else if (list->number > item->number) { /* item comes before list, make item 1st element */
		item->next = head;
		head = item;
	} else if (list->number == item->number) { /* item replaces 1st element */
		item->next = cur->next;
		head = item;
		cur->next = NULL;
		free_line(cur);
	} else { /* item comes after 1st element */
    	struct line *prev;

		for (prev = cur, cur = cur->next; cur != NULL; prev = cur, cur = cur->next) {
			if (cur->number == item->number) { /* item replaces current element */
				prev->next = item;
				item->next = cur->next;
				cur->next = NULL;
				free_line(cur);
				break;
			} else if (prev->number < item->number && item->number < cur->number) { /* item goes between prev and cur */
				prev->next = item;
				item->next = cur;
				break;
			}
		}
		if (cur == NULL) { /* insert at end of list */
			prev->next = item;
		}
	}

	lines = head;
}


/* *** *** *** */

#define NVARS 26
struct number *vars[NVARS] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL
};

void runtime_set_var(char var, struct number *value) {
	int i = (var - 'A') % NVARS;
	if (vars[i] != NULL) {
		free_number(vars[i]);
	}
	vars[(var - 'A') % NVARS] = clone_number(value);
}

struct number *runtime_get_var(char var) {
	return vars[(var - 'A') % NVARS];
}

/* *** *** *** */

#define NFORSTATES 26
struct for_state for_states[NFORSTATES] = {
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 }, { NULL, NULL, 0 },
	{ NULL, NULL, 0 }, { NULL, NULL, 0 }
};

void runtime_set_for_state(char var, struct number *limit, struct number *step, int target) {
	int i = (var - 'A') % NFORSTATES;

	free_number(for_states[i].limit);
	for_states[i].limit = NULL;
	free_number(for_states[i].step);
	for_states[i].step = NULL;

	for_states[i].limit = clone_number(limit);
	for_states[i].step = clone_number(step);
	for_states[i].target = target;
}

struct for_state *runtime_get_for_state(char var) {
	return &for_states[(var - 'A') % NFORSTATES];
}

/* *** *** *** */

int done = 0;

void runtime_stop(void) {
	done = 1;
}

int runtime_continue(void) {
	return !done;
}

/* *** *** *** */

static int trace = 0;

void runtime_trace_on(void) {
	trace = 1;
}

void runtime_trace_off(void) {
	trace = 0;
}

int runtime_is_tracing(void) {
	return trace;
}

/* *** *** *** */

struct stack *call_stack = NULL;

struct stack {
	int val;
	struct stack *next;
};

void runtime_callstack_push(int val) {
	struct stack *s;
	s = (struct stack *) malloc(sizeof(struct stack));
	if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(s, '\0', sizeof(struct stack));

	s->val = val;
	s->next = call_stack;

	call_stack = s;
}

int runtime_callstack_pop() {

	struct stack *old;
	int val;

	if (call_stack == NULL) {
		return -3;
	}

	val = call_stack->val;
	old = call_stack;
	call_stack = call_stack->next;
	free(old);

	return val;
}

/* *** *** *** */

void runtime_reset(void) {
	int i;
	for (i = 0; i < NVARS; i++) {
		free_number(vars[i]);
		vars[i] = NULL;
	}

	for (i = 0; i < NFORSTATES; i++) {
		free_number(for_states[i].limit);
		for_states[i].limit = NULL;
		free_number(for_states[i].step);
		for_states[i].step = NULL;
		for_states[i].target = 0;
	}

	if (lines != NULL) {
		free_line(lines);
		lines = NULL;
	}
	while (runtime_callstack_pop() != -3) {
		/* do nothing */;
	}
}

/* *** *** *** */


