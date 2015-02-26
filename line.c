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

#include "runtime.h"
#include "tokenizer.h"

#include "command.h"
#include "line.h"
#include "number.h"
#include "statement.h"

struct line *new_line(struct statement *statement, struct number *number) {

	struct line *l = NULL;

	l = (struct line *) malloc(sizeof(struct line));
	if (l == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(l, '\0', sizeof(struct line));

	l->statement = statement;
	l->number = INT_VALUE(number);

	free_number(number);

	return l;
}

void parse_line(struct tokenizer *t) {

	struct command *cmd;
	struct statement *stmt;
	struct number *num;

	cmd = parse_command(t);
	if (cmd != NULL) {
		exec_command(cmd);
		free_command(cmd);
		cmd = NULL;
		token_get(t);
		return;
	}

	stmt = parse_statement(t);
	if (stmt != NULL) {
		eval_statement(stmt, -1, -1);
		free_statement(stmt);
		stmt = NULL;
		token_get(t);
		return;
	}

	num = parse_number(t);
	if (num != NULL) {
		stmt = parse_statement(t);
		if (stmt != NULL) {
			runtime_set_line(new_line(stmt, num));
			token_get(t);
			return;
		} else {
			runtime_rm_line(INT_VALUE(num));
			free_number(num);
			num = NULL;
			token_get(t);
			return;
		}
	}
}

int eval_line(struct line *l) {

	if (l == NULL) {
		return -1;
	}

	return eval_statement(l->statement, l->number, (l->next != NULL) ? l->next->number : -1);
}

void print_line(struct line *l) {

	if (l == NULL) {
		return;
	}

	printf("%d ", l->number);
	print_statement(l->statement);
	printf("\n");

	print_line(l->next);
}

void free_line(struct line *l) {
	if (l != NULL) {
		free_statement(l->statement);
		free_line(l->next);
		free(l);
	}
}
