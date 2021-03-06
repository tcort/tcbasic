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
#include "shell.h"
#include "str.h"
#include "var_list.h"
#include "var.h"

#include "statement.h"

struct statement *new_statement(int type, void *arg1, void *arg2, void *arg3, void *arg4) {

	struct statement *s;

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
		case SHELL:
			s->u.shell_stmt.str = (struct str *) arg1;
			break;
		case BEEP:
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
		case FOR:
			s->u.for_stmt.var = (struct var *) arg1;
			s->u.for_stmt.initial = (struct expression *) arg2;
			s->u.for_stmt.limit = (struct expression *) arg3;
			s->u.for_stmt.step = (struct expression *) arg4;
			break;
		case NEXT:
			s->u.next_stmt.var = (struct var *) arg1;
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
		case CLS:
			break;
	}

	return s;
}

struct statement *parse_statement(struct tokenizer *t) {

	struct expr_list *el;
	struct expression *expr;
	struct expression *expr2;
	struct expression *expr3;
	struct relop *op;
	struct rem *r;
	struct statement *stmt;
	struct str *str;
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
		case BEEP:
			return new_statement(BEEP, NULL, NULL, NULL, NULL);
		case SHELL:
			str = parse_str(t);
			if (str == NULL) {
				return NULL;
			}
			return new_statement(SHELL, str, NULL, NULL, NULL);
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
		case FOR:
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
			token_get(t);
			if (t->token.type != TO) {
				token_unget(t);
				free_expression(expr);
				expr = NULL;
				free_var(v);
				v = NULL;
				return NULL;
			}
			expr2 = parse_expression(t);
			if (expr2 == NULL) {
				free_expression(expr);
				expr = NULL;
				free_var(v);
				v = NULL;
				return NULL;
			}
			token_get(t);
			if (t->token.type == STEP) {
				expr3 = parse_expression(t);
				if (expr3 == NULL) {
					free_expression(expr2);
					expr2 = NULL;
					free_expression(expr);
					expr = NULL;
					free_var(v);
					v = NULL;
					return NULL;
				}
			} else {
				token_unget(t);
				expr3 = NULL;
			}
			return new_statement(FOR, v, expr, expr2, expr3);
		case NEXT:
			v = parse_var(t);
			if (v == NULL) {
				return NULL;
			}
			return new_statement(NEXT, v, NULL, NULL, NULL);
		case REM:
			token_unget(t);
			r = parse_rem(t);
			if (r == NULL) {
				return NULL;
			}
			return new_statement(REM, r, NULL, NULL, NULL);
		case RANDOMIZE:
			return new_statement(RANDOMIZE, NULL, NULL, NULL, NULL);
		case CLS:
			return new_statement(CLS, NULL, NULL, NULL, NULL);
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
	struct for_state *fs;

	if (s == NULL) {
		return -1;
	}

	switch (s->type) {
		case PRINT:
			eval_expr_list(s->u.print_stmt.expr_list);
			printf("\n");
			break;
		case SHELL:
			doshell(s->u.shell_stmt.str->value);
			break;
		case BEEP:
			putchar(7);
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
				default:
					fprintf(stdout, "Runtime error: Unknown relational operator type: %d", s->u.if_stmt.relop->type);
					exit(EXIT_FAILURE);
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
		case FOR:

			e1 = eval_expression(s->u.for_stmt.limit);
			if (e1 == NULL) {
				next = -1;
				break;
			}

			if (s->u.for_stmt.step == NULL) {
				e2 = new_number_from_int(1);
			} else {
				e2 = eval_expression(s->u.for_stmt.step);
			}

			if (e2 == NULL) {
				free_number(e2);
				next = -1;
				break;
			}

			runtime_set_for_state(s->u.for_stmt.var->value, e1, e2, next_number);

			free_number(e2);
			free_number(e1);

			/* initialize index variable */
			n = eval_expression(s->u.for_stmt.initial);
			runtime_set_var(s->u.for_stmt.var->value, n);
			free_number(n);

			/* eval limit */
			fs = runtime_get_for_state(s->u.next_stmt.var->value);
			e1 = subtract_number(
				runtime_get_var(s->u.for_stmt.var->value),
				fs->limit
			);

			e2 = new_number_from_int(FLOAT_VALUE(fs->step) >= 0.0 ? 1 : -1);

			n = multiply_number(e1, e2);
			if (FLOAT_VALUE(n) <= 0.0) {
				next = -1;
			} else {
				next = runtime_get_line_after_nearest_next(number, s->u.for_stmt.var->value);
			}

			free_number(n);
			free_number(e2);
			free_number(e1);

			break;
		case NEXT:
			fs = runtime_get_for_state(s->u.next_stmt.var->value);
			e1 = runtime_get_var(s->u.next_stmt.var->value);
			e2 = fs->step;
			n = add_number(e1, e2);
			runtime_set_var(s->u.next_stmt.var->value, n);
			free_number(n);

			/* eval limit */
			e1 = subtract_number(
				runtime_get_var(s->u.next_stmt.var->value),
				fs->limit
			);

			e2 = new_number_from_int(FLOAT_VALUE(fs->step) >= 0.0 ? 1 : -1);

			n = multiply_number(e1, e2);
			if (FLOAT_VALUE(n) <= 0.0) {
				next = fs->target;
			} else {
				next = next_number;
			}

			free_number(n);
			free_number(e2);
			free_number(e1);

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
			tcb_randomize();
			break;
		case CLS:
			puts("\x1b[2J");
			puts("\x1b[H");
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
		case SHELL:
			printf("SHELL ");
			print_str(s->u.shell_stmt.str);
			break;
		case BEEP:
			printf("BEEP");
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
		case FOR:
			printf("FOR ");
			print_var(s->u.for_stmt.var);
			printf(" = ");
			print_expression(s->u.for_stmt.initial);
			printf(" TO ");
			print_expression(s->u.for_stmt.limit);
			if (s->u.for_stmt.step != NULL) {
				printf(" STEP ");
				print_expression(s->u.for_stmt.step);
			}
			break;
		case NEXT:
			printf("NEXT ");
			print_var(s->u.next_stmt.var);
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
			break;
		case REM:
			print_rem(s->u.rem_stmt.rem);
			break;
		case RANDOMIZE:
			printf("RANDOMIZE");
			break;
		case CLS:
			printf("CLS");
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
			case BEEP:
				break;
			case SHELL:
				free_str(s->u.shell_stmt.str);
				s->u.shell_stmt.str = NULL;
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
			case FOR:
				free_var(s->u.for_stmt.var);
				s->u.for_stmt.var = NULL;
				free_expression(s->u.for_stmt.initial);
				s->u.for_stmt.initial = NULL;
				free_expression(s->u.for_stmt.limit);
				s->u.for_stmt.limit = NULL;
				free_expression(s->u.for_stmt.step);
				s->u.for_stmt.step = NULL;
				break;
			case NEXT:
				free_var(s->u.next_stmt.var);
				s->u.next_stmt.var = NULL;
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
			case CLS:
				break;
		}
		free(s);
	}
}
