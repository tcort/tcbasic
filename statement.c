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
#include <time.h>

#include "buffer.h"
#include "prng.h"
#include "readaline.h"
#include "runtime.h"
#include "tokenizer.h"

#include "expr_list.h"
#include "expression.h"
#include "number.h"
#include "relop.h"
#include "rem.h"
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
		case RETURN:
			break;
		case END:
			break;
		case STOP:
			break;
		case REM:
			s->u.rem_stmt.rem = (struct rem *) arg1;
			break;
		case RANDOMIZE:
			break;
	}

	return s;
}

struct statement *parse_statement(struct tokenizer *t) {

	struct expr_list *el;
	struct expression *expr;
	struct expression *expr2;
	struct relop *op;
	struct rem *r;
	struct statement *stmt;
	struct var *v;
	struct var_list *vl;

	token_get(t);
	switch (t->token.type) {
		case PRINT:
			el = parse_expr_list(t);
			if (el == NULL) {
				return NULL;
			}
			return new_statement(PRINT, el, NULL, NULL, NULL);
		case IF:
			expr = parse_expression(t);
			if (expr == NULL) {
				return NULL;
			}
			op = parse_relop(t);
			if (op == NULL) {
				free_expression(expr);
				expr = NULL;
				return NULL;
			}
			expr2 = parse_expression(t);
			if (expr2 == NULL) {
				free_expression(expr);
				expr = NULL;
				free_relop(op);
				op = NULL;
				return NULL;
			}
			token_get(t);
			if (t->token.type != THEN) {
				token_unget(t);
				free_expression(expr);
				expr = NULL;
				free_relop(op);
				op = NULL;
				free_expression(expr2);
				expr2 = NULL;
				return NULL;
			}
			stmt = parse_statement(t);
			if (stmt == NULL) {
				free_expression(expr);
				expr = NULL;
				free_relop(op);
				op = NULL;
				free_expression(expr2);
				expr2 = NULL;
				return NULL;
			}
			return new_statement(IF, expr, op, expr2, stmt);
		case GOTO:
			expr = parse_expression(t);
			if (expr == NULL) {
				return NULL;
			}
			return new_statement(GOTO, expr, NULL, NULL, NULL);
		case INPUT:
			vl = parse_var_list(t);
			if (vl == NULL) {
				return NULL;
			}
			return new_statement(INPUT, vl, NULL, NULL, NULL);
		case LET:
			v = parse_var(t);
			if (v == NULL) {
				return NULL;
			}
			token_get(t);
			if (t->token.type != EQ) {
				token_unget(t);
				free_var(v);
				v = NULL;
				return NULL;
			}
			expr = parse_expression(t);
			if (expr == NULL) {
				free_var(v);
				v = NULL;
				return NULL;
			}
			return new_statement(LET, v, expr, NULL, NULL);
		case GOSUB:
			expr = parse_expression(t);
			if (expr == NULL) {
				return NULL;
			}
			return new_statement(GOSUB, expr, NULL, NULL, NULL);
		case RETURN:
			return new_statement(RETURN, NULL, NULL, NULL, NULL);
		case END:
			return new_statement(END, NULL, NULL, NULL, NULL);
		case STOP:
			return new_statement(STOP, NULL, NULL, NULL, NULL);
		case REM:
			token_unget(t);
			r = parse_rem(t);
			if (r == NULL) {
				return NULL;
			}
			return new_statement(REM, r, NULL, NULL, NULL);
		case RANDOMIZE:
			return new_statement(RANDOMIZE, NULL, NULL, NULL, NULL);
		default:
			token_unget(t);
			return NULL;
	}
}

/*
  returns:
    >0 --> jump to specified line number
    -1 --> next line number
    -2 --> end
    -3 --> invalid (returned by callstack pop is empty)
*/
int eval_statement(struct statement *s, int number, int next_number) {

	Buffer *buf;
	int next = -1;
	int r;
	float f1, f2;
	struct number *e1;
	struct number *e2;
	struct number *n;

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
			f1 = FLOAT_VALUE(e1);
			f2 = FLOAT_VALUE(e2);
			switch (s->u.if_stmt.relop->type) {
				case LT:
					r = f1 < f2;
					break;
				case LTEQ:
					r = f1 <= f2;
					break;
				case LTGT:
					r = f1 != f2;
					break;
				case GT:
					r = f1 > f2;
					break;
				case GTEQ:
					r = f1 >= f2;
					break;
				case GTLT:
					r = f1 != f2;
					break;
				case EQ:
					r = f1 == f2;
					break;
			}
			free_number(e1);
			free_number(e2);
			if (r) {
				next = eval_statement(s->u.if_stmt.statement, number, next_number);
			}
			break;
		case GOTO:
			n = eval_expression(s->u.goto_stmt.expression);
			next = INT_VALUE(n);
			free_number(n);
			break;
		case INPUT:
			buf = bf_alloc(32, 16);
			readaline(stdin, "? ", buf);
			eval_var_list(s->u.input_stmt.var_list, buf->buf);
			bf_free(buf);
			buf = NULL;
			break;
		case LET:
			n = eval_expression(s->u.let_stmt.expression);
			runtime_set_var(s->u.let_stmt.var->value, n);
			free_number(n);
			break;
		case GOSUB:
			if (next_number > 0) {
				runtime_callstack_push(next_number);
			}
			n = eval_expression(s->u.goto_stmt.expression);
			next = INT_VALUE(n);
			free_number(n);
			break;
		case RETURN:
			next = runtime_callstack_pop();
			if (next < 0) {
				next = -1;
			}
			break;
		case END:
			next = -2;
			break;
		case STOP:
			runtime_stop();
			break;
		case REM:
			break;
		case RANDOMIZE:
			tcb_srand(time(NULL));
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
		case RETURN:
			printf("RETURN");
			break;
		case END:
			printf("END");
			break;
		case STOP:
			printf("STOP");
		case REM:
			print_rem(s->u.rem_stmt.rem);
			break;
		case RANDOMIZE:
			printf("RANDOMIZE");
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
			case RETURN:
				break;
			case END:
				break;
			case STOP:
				break;
			case REM:
				free_rem(s->u.rem_stmt.rem);
				s->u.rem_stmt.rem = NULL;
				break;
			case RANDOMIZE:
				break;
		}
		free(s);
	}
}
