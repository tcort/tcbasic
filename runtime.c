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
#include <time.h>

#include "ast.h"
#include "runtime.h"

#define NVARS 26

static struct ast_node * evalLine(struct ast_node *line);
static int evalExpression(struct ast_node *expression);
static int evalTerm(struct ast_node *term);
static int evalFactor(struct ast_node *factor);
static int evalRnd(struct ast_node *rnd);
static void printExprItem(struct ast_node *expr_item);
static void printExprList(struct ast_node *expr_list);
static struct ast_node *getLine(int number);

int done = 0;
int vars[NVARS];
struct ast_node *lines = NULL;
struct stack *call_stack = NULL;

struct stack {
	struct ast_node *val;
	struct stack *next;
};

static void stack_push(struct ast_node *val) {
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

static struct ast_node * stack_pop() {

	struct stack *old;
	struct ast_node *val;

	if (call_stack == NULL) {
		return NULL;
	}

	val = call_stack->val;
	old = call_stack;
	call_stack = call_stack->next;
	free(old);

	return val;
}

void removeLine(int number) {
	lines = remove_line(lines, number);
}

void processLine(struct ast_node *line) {

	struct ast_node *cur;
	struct ast_node *command = line->node_leaves.node_line.command;

	switch (line->node_leaves.node_line.line_type) {
		case AST_LT_COMMAND:
			switch (command->node_leaves.node_command.command_type) {
				case AST_CT_CLEAR:
					runtime_reset();
					break;
				case AST_CT_LIST:
					print_ast_node(lines);
					break;
				case AST_CT_RUN:
					cur = lines;
					while (cur != NULL) {
						cur = evalLine(cur);
					}
					break;
			}
			break;
		case AST_LT_STATEMENT:
			if (line->node_leaves.node_line.number != -1) {
				lines = insert_line(lines, line);
			} else {
				evalLine(line);
				free_ast_node(line);
			}
			break;
	}
}

static struct ast_node * evalLine(struct ast_node *line) {

	if (line == NULL) {
		return NULL;
	}

	int i, j, r;
	struct ast_node *cur;
	struct ast_node *next = line->node_leaves.node_line.next;
	struct ast_node *statement = line->node_leaves.node_line.statement;

	switch (statement->node_leaves.node_statement.statement_type) {
		case AST_ST_PRINT:
			printExprList(statement->node_leaves.node_statement.ast_statement_value.ast_statement_print.expr_list);
			printf("\n");
			break;
		case AST_ST_IF:
			i = evalExpression(statement->node_leaves.node_statement.ast_statement_value.ast_statement_if.left);
			j = evalExpression(statement->node_leaves.node_statement.ast_statement_value.ast_statement_if.right);

			switch (statement->node_leaves.node_statement.ast_statement_value.ast_statement_if.relop->node_leaves.node_relop.op) {
				case AST_LT:
					r = i < j;
					break;
				case AST_LTEQ:
					r = i <= j;
					break;
				case AST_LTGT:
					r = i != j;
					break;
				case AST_GT:
					r = i > j;
					break;
				case AST_GTEQ:
					r = i >= j;
					break;
				case AST_GTLT:
					r = i != j;
					break;
				case AST_EQ:
					r = i == j;
					break;
			}

			if (r) {
				cur = new_ast_node(AST_LINE);
				cur->node_leaves.node_line.next = line->node_leaves.node_line.next;
				cur->node_leaves.node_line.statement = statement->node_leaves.node_statement.ast_statement_value.ast_statement_if.statement;
				cur->node_leaves.node_line.number = line->node_leaves.node_line.number;
				next = evalLine(cur);
				cur->node_leaves.node_line.next = NULL;
				cur->node_leaves.node_line.statement = NULL;
				cur->node_leaves.node_line.number = -1;
				free_ast_node(cur);
			}
			break;
		case AST_ST_GOTO:
			i = evalExpression(statement->node_leaves.node_statement.ast_statement_value.ast_statement_goto.expression);
			next = getLine(i);
			break;
		case AST_ST_INPUT:
/* TODO implement */
			break;
		case AST_ST_LET:
			i = ((statement->node_leaves.node_statement.ast_statement_value.ast_statement_let.var->node_leaves.node_var.value) - 'A') % NVARS;
			vars[i] = evalExpression(statement->node_leaves.node_statement.ast_statement_value.ast_statement_let.expression);
			break;
		case AST_ST_GOSUB:
			i = evalExpression(statement->node_leaves.node_statement.ast_statement_value.ast_statement_gosub.expression);
			stack_push(next);
			next = getLine(i);
			break;
		case AST_ST_RETURN:
			next = stack_pop();
			break;
		case AST_ST_END:
			next = NULL;
			break;
	}

	return next;
}

static int evalExpression(struct ast_node *expression) {

	int left, right, tmp;
	struct ast_node *term1_op = expression->node_leaves.node_expression.term1_op;
	struct ast_node *term1    = expression->node_leaves.node_expression.term1;
	struct ast_node *term2_op = expression->node_leaves.node_expression.term2_op;
	struct ast_node *term2    = expression->node_leaves.node_expression.term2;

	switch (expression->node_leaves.node_expression.expression_type) {
		case AST_ET_ATAT:
			left = evalTerm(term1);
			right = evalTerm(term2);
			tmp = 0;
			tmp = (term1_op->node_leaves.node_addop.op == AST_PLUS) ? (tmp + left) : (tmp - left);
			tmp = (term2_op->node_leaves.node_addop.op == AST_PLUS) ? (tmp + right) : (tmp - right);
			return tmp;
		case AST_ET_AT:
			left = evalTerm(term1);
			return (term1_op->node_leaves.node_addop.op == AST_PLUS) ? (0 + left) : (0 - left);
		case AST_ET_TAT:
			left = evalTerm(term1);
			right = evalTerm(term2);
			tmp = left;
			tmp = (term2_op->node_leaves.node_addop.op == AST_PLUS) ? (tmp + right) : (tmp - right);
			return tmp;
		case AST_ET_T:
			return evalTerm(term1);

	}

}

static int evalTerm(struct ast_node *term) {

	int left, right;
	struct ast_node *factor1 = term->node_leaves.node_term.factor1;
	struct ast_node *op = term->node_leaves.node_term.op;
	struct ast_node *factor2 = term->node_leaves.node_term.factor2;

	switch (term->node_leaves.node_term.term_type) {
		case AST_TT_FMF:
			left = evalFactor(factor1);
			right = evalFactor(factor2);
			return (op->node_leaves.node_mulop.op == AST_TIMES) ? (left * right) : (left / right);
		case AST_TT_F:
			return evalFactor(factor1);
	}
}

static int evalFactor(struct ast_node *factor) {

	struct ast_node *var = factor->node_leaves.node_factor.factor_value.var;
	struct ast_node *number = factor->node_leaves.node_factor.factor_value.number;
	struct ast_node *expression = factor->node_leaves.node_factor.factor_value.expression;
	struct ast_node *rnd = factor->node_leaves.node_factor.factor_value.rnd;

	switch (factor->node_leaves.node_factor.factor_type) {
		case AST_FT_VAR:
			return vars[((var->node_leaves.node_var.value) - 'A') % NVARS];
		case AST_FT_NUMBER:
			return number->node_leaves.node_number.value;
		case AST_FT_EXPRESSION:
			return evalExpression(expression);
		case AST_FT_RND:
			return evalRnd(rnd);
	}
}

static int evalRnd(struct ast_node *rnd) {
	struct ast_node *expression = rnd->node_leaves.node_rnd.expression;
	return rand() % evalExpression(expression); /* TODO avoid rand() and biasing the output, use arc4random_uniform() instead */
}

static void printExprItem(struct ast_node *expr_item) {

	struct ast_node *string = expr_item->node_leaves.node_expr_item.expr_item_value.string;
	struct ast_node *expression = expr_item->node_leaves.node_expr_item.expr_item_value.expression;

	switch (expr_item->node_leaves.node_expr_item.expr_item_type) {
		case AST_EI_STRING:
			printf("%s", string->node_leaves.node_string.value);
			break;
		case AST_EI_EXPRESSION:
			printf("%d", evalExpression(expression));
			break;
	}

}

static void printExprList(struct ast_node *expr_list) {

	struct ast_node *list = expr_list->node_leaves.node_expr_list.list;
	struct ast_node *single = expr_list->node_leaves.node_expr_list.single;

	switch (expr_list->node_leaves.node_expr_list.expr_list_type) {
		case AST_EL_SINGLE:
			printExprItem(single);
			break;
		case AST_EL_LIST:
			printExprList(list);
			printExprItem(single);
			break;
	}
}

void runtime_reset(void) {
	memset(vars, '\0', sizeof(char) * NVARS);
	if (lines != NULL) {
		free_ast_node(lines);
		lines = NULL;
	}
/* TODO clear stack */
}

int runtime_continue(void) {
	return !done;
}

static struct ast_node *getLine(int number) {
	struct ast_node *cur;
	struct ast_node *next;

	for (cur = lines, next = NULL; cur != NULL; cur = cur->node_leaves.node_line.next) {
		if (number == cur->node_leaves.node_line.number) {
			next = cur;
			break;
		}
	}

	return next;
}
