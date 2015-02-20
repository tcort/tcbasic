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

#include "runtime.h"

#include "expr_list.h"
#include "expression.h"
#include "relop.h"
#include "var_list.h"
#include "var.h"

#include "statement.h"

struct statement *new_statement(int type, void *arg1, void *arg2, void *arg3, void *arg4) {

	struct statement *s = NULL;

	s = (struct statement *) malloc(sizeof(struct statement));
	if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(s, '\0', sizeof(struct statement));

	s->type = type;
	switch (s->type) {
		case PRINT:
			s->u.print_stmt.expr_list = (struct expr_list *) arg1;
			break;
		case IF:
			s->u.if_stmt.left = (struct expression *) arg1;
			s->u.if_stmt.relop = (struct relop *) arg2;
			s->u.if_stmt.right = (struct expression *) arg3;
			s->u.if_stmt.statement = (struct statement *) arg4;
			break;
		case GOTO:
			s->u.goto_stmt.expression = (struct expression *) arg1;
			break;
		case INPUT:
			s->u.input_stmt.var_list = (struct var_list *) arg1;
			break;
		case LET:
			s->u.let_stmt.var = (struct var *) arg1;
			s->u.let_stmt.expression = (struct expression *) arg2;
			break;
		case GOSUB:
			s->u.gosub_stmt.expression = (struct expression *) arg1;
			break;
		case _RETURN:
			break;
		case END:
			break;
	}

	return s;
}

/*
  returns:
    >0 --> jump to specified line number
    -1 --> next line number
    -2 --> end
    -3 --> invalid (returned by callstack pop is empty)
*/
int eval_statement(struct statement *s, int number, int next_number) {

	int next = -1;
	int e1, e2, r;

	if (s == NULL) {
		return -1;
	}

	switch (s->type) {
		case PRINT:
			eval_expr_list(s->u.print_stmt.expr_list);
			printf("\n");
			break;
		case IF:
			e1 = eval_expression(s->u.if_stmt.left);
			e2 = eval_expression(s->u.if_stmt.right);
			switch (s->u.if_stmt.relop->type) {
				case LT:
					r = e1 < e2;
					break;
				case LTEQ:
					r = e1 <= e2;
					break;
				case LTGT:
					r = e1 != e2;
					break;
				case GT:
					r = e1 > e2;
					break;
				case GTEQ:
					r = e1 >= e2;
					break;
				case GTLT:
					r = e1 != e2;
					break;
				case EQ:
					r = e1 == e2;
					break;
			}
			if (r) {
				next = eval_statement(s->u.if_stmt.statement, number, next_number);
			}
			break;
		case GOTO:
			next = eval_expression(s->u.goto_stmt.expression);
			break;
		case INPUT:
			/* TODO implement */
			break;
		case LET:
			runtime_set_var(s->u.let_stmt.var->value, eval_expression(s->u.let_stmt.expression));
			break;
		case GOSUB:
			if (next_number > 0) {
				runtime_callstack_push(next_number);
			}
			next = eval_expression(s->u.goto_stmt.expression);
			break;
		case _RETURN:
			next = runtime_callstack_pop();
			if (next < 0) {
				next = -1;
			}
			break;
		case END:
			next = -2;
			break;
	}

	return next;
}

void print_statement(struct statement *s) {

	if (s == NULL) {
		return;
	}

	switch (s->type) {
		case PRINT:
			printf("PRINT ");
			print_expr_list(s->u.print_stmt.expr_list);
			break;
		case IF:
			printf("IF ");
			print_expression(s->u.if_stmt.left);
			print_relop(s->u.if_stmt.relop);
			print_expression(s->u.if_stmt.right);
			printf(" THEN ");
			print_statement(s->u.if_stmt.statement);
			break;
		case GOTO:
			printf("GOTO ");
			print_expression(s->u.goto_stmt.expression);
			break;
		case INPUT:
			printf("INPUT ");
			print_var_list(s->u.input_stmt.var_list);
			break;
		case LET:
			printf("LET ");
			print_var(s->u.let_stmt.var);
			printf(" = ");
			print_expression(s->u.let_stmt.expression);
			break;
		case GOSUB:
			printf("GOSUB ");
			print_expression(s->u.gosub_stmt.expression);
			break;
		case _RETURN:
			printf("RETURN");
			break;
		case END:
			printf("END");
			break;
	}

}

void free_statement(struct statement *s) {
	if (s != NULL) {
		switch (s->type) {
			case PRINT:
				free_expr_list(s->u.print_stmt.expr_list);
				s->u.print_stmt.expr_list = NULL;
				break;
			case IF:
				free_expression(s->u.if_stmt.left);
				s->u.if_stmt.left = NULL;

				free_relop(s->u.if_stmt.relop);
				s->u.if_stmt.relop = NULL;

				free_expression(s->u.if_stmt.right);
				s->u.if_stmt.right = NULL;

				free_statement(s->u.if_stmt.statement);
				s->u.if_stmt.statement = NULL;
				break;
			case GOTO:
				free_expression(s->u.goto_stmt.expression);
				s->u.goto_stmt.expression = NULL;
				break;
			case INPUT:
				free_var_list(s->u.input_stmt.var_list);
				s->u.input_stmt.var_list = NULL;
				break;
			case LET:
				free_var(s->u.let_stmt.var);
				s->u.let_stmt.var = NULL;
				free_expression(s->u.let_stmt.expression);
				s->u.let_stmt.expression = NULL;
				break;
			case GOSUB:
				free_expression(s->u.gosub_stmt.expression);
				s->u.gosub_stmt.expression = NULL;
				break;
			case _RETURN:
				break;
			case END:
				break;
		}
		free(s);
		s = NULL;
	}
}
