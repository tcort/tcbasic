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

#include "ast.h"

struct ast_node *new_ast_node(enum ast_node_types node_type) {

	struct ast_node *node = NULL;

	node = (struct ast_node *) malloc(sizeof(struct ast_node));
	if (node == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(node, '\0', sizeof(struct ast_node));

	node->node_type = node_type;

	return node;
}

struct ast_node * remove_line(struct ast_node *list, int number) {
	struct ast_node *head = list;
	struct ast_node *cur = list;
	struct ast_node *prev = list;

	if (head == NULL) { /* empty list */
		/* nothing to do */;
	} else if (head->node_leaves.node_line.number == number) { /* first element in list */
		head = head->node_leaves.node_line.next;
		cur->node_leaves.node_line.next = NULL;
		free_ast_node(cur);
	} else {
		for (prev = cur, cur = cur->node_leaves.node_line.next; cur != NULL; prev = cur, cur = cur->node_leaves.node_line.next) {
			if (cur->node_leaves.node_line.number == number) {
				prev->node_leaves.node_line.next = cur->node_leaves.node_line.next;
				cur->node_leaves.node_line.next = NULL;
				free_ast_node(cur);
			}
		}
	}

	return head;
}

struct ast_node * insert_line(struct ast_node *list, struct ast_node *item) {
	struct ast_node *head = list;
	struct ast_node *cur = list;
	struct ast_node *prev = list;

	if (head == NULL) { /* empty list, make item 1st element */
		head = item;
	} else if (list->node_leaves.node_line.number > item->node_leaves.node_line.number) { /* item comes before list, make item 1st element */
		item->node_leaves.node_line.next = head;
		head = item;
	} else if (list->node_leaves.node_line.number == item->node_leaves.node_line.number) { /* item replaces 1st element */
		item->node_leaves.node_line.next = cur->node_leaves.node_line.next;
		head = item;
		cur->node_leaves.node_line.next = NULL;
		free_ast_node(cur);
	} else { /* item comes after 1st element */
		for (prev = cur, cur = cur->node_leaves.node_line.next; cur != NULL; prev = cur, cur = cur->node_leaves.node_line.next) {
			if (cur->node_leaves.node_line.number == item->node_leaves.node_line.number) { /* item replaces current element */
				prev->node_leaves.node_line.next = item;
				item->node_leaves.node_line.next = cur->node_leaves.node_line.next;
				cur->node_leaves.node_line.next = NULL;
				free_ast_node(cur);
				break;
			} else if (prev->node_leaves.node_line.number < item->node_leaves.node_line.number && item->node_leaves.node_line.number < cur->node_leaves.node_line.number) { /* item goes between prev and cur */
				prev->node_leaves.node_line.next = item;
				item->node_leaves.node_line.next = cur;
				break;
			}
		}
		if (cur == NULL) { /* insert at end of list */
			prev->node_leaves.node_line.next = item;
		}
	}

	return head;
}

void free_ast_node(struct ast_node *node) {

	if (node == NULL) {
		return;
	}

	switch (node->node_type) {
		case AST_LINE:
			free_ast_node(node->node_leaves.node_line.statement);
			free_ast_node(node->node_leaves.node_line.next);
			break;
		case AST_STATEMENT:
			switch (node->node_leaves.node_statement.statement_type) {
				case AST_ST_PRINT:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_print.expr_list);
					break;
				case AST_ST_IF:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.left);
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.relop);
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.right);
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.statement);
					break;
				case AST_ST_GOTO:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_goto.expression);
					break;
				case AST_ST_INPUT:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_input.var_list);
					break;
				case AST_ST_LET:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_let.var);
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_let.expression);
					break;
				case AST_ST_GOSUB:
					free_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_gosub.expression);
					break;
				case AST_ST_RETURN:
					// nothing to free here
					break;
				case AST_ST_END:
					// nothing to free here
					break;
			}
			break;
		case AST_COMMAND:
			switch (node->node_leaves.node_command.command_type) {
				case AST_CT_CLEAR:
					// nothing to free here
					break;
				case AST_CT_LIST:
					// nothing to free here
					break;
				case AST_CT_RUN:
					// nothing to free here
					break;
			}
			break;
		case AST_EXPR_LIST:
			switch (node->node_leaves.node_expr_list.expr_list_type) {
				case AST_EL_SINGLE:
					free_ast_node(node->node_leaves.node_expr_list.single);
					break;
				case AST_EL_LIST:
					free_ast_node(node->node_leaves.node_expr_list.list);
					free_ast_node(node->node_leaves.node_expr_list.single);
					break;
			}
			break;
		case AST_EXPR_ITEM:
			switch (node->node_leaves.node_expr_item.expr_item_type) {
				case AST_EI_STRING:
					free_ast_node(node->node_leaves.node_expr_item.expr_item_value.string);
					break;
				case AST_EI_EXPRESSION:
					free_ast_node(node->node_leaves.node_expr_item.expr_item_value.expression);
					break;
			}
			break;
		case AST_VAR_LIST:
			switch (node->node_leaves.node_var_list.var_list_type) {
				case AST_VL_SINGLE:
					free_ast_node(node->node_leaves.node_var_list.single);
					break;
				case AST_VL_LIST:
					free_ast_node(node->node_leaves.node_var_list.list);
					free_ast_node(node->node_leaves.node_var_list.single);
					break;
			}
			break;
		case AST_EXPRESSION:
			switch (node->node_leaves.node_expression.expression_type) {
				case AST_ET_ATAT:
					free_ast_node(node->node_leaves.node_expression.term1_op);
					free_ast_node(node->node_leaves.node_expression.term1);
					free_ast_node(node->node_leaves.node_expression.term2_op);
					free_ast_node(node->node_leaves.node_expression.term2);
					break;
				case AST_ET_AT:
					free_ast_node(node->node_leaves.node_expression.term1_op);
					free_ast_node(node->node_leaves.node_expression.term1);
					break;
				case AST_ET_TAT:
					free_ast_node(node->node_leaves.node_expression.term1);
					free_ast_node(node->node_leaves.node_expression.term2_op);
					free_ast_node(node->node_leaves.node_expression.term2);
					break;
				case AST_ET_T:
					free_ast_node(node->node_leaves.node_expression.term1);
					break;
			}
			break;
		case AST_TERM:
			switch (node->node_leaves.node_term.term_type) {
				case AST_TT_FMF:
					free_ast_node(node->node_leaves.node_term.factor1);
					free_ast_node(node->node_leaves.node_term.op);
					free_ast_node(node->node_leaves.node_term.factor2);
					break;
				case AST_TT_F:
					free_ast_node(node->node_leaves.node_term.factor1);
					break;
			}
			break;
		case AST_FACTOR:
			switch (node->node_leaves.node_factor.factor_type) {
				case AST_FT_VAR:
					free_ast_node(node->node_leaves.node_factor.factor_value.var);
					break;
				case AST_FT_NUMBER:
					free_ast_node(node->node_leaves.node_factor.factor_value.number);
					break;
				case AST_FT_EXPRESSION:
					free_ast_node(node->node_leaves.node_factor.factor_value.expression);
					break;
				case AST_FT_RND:
					free_ast_node(node->node_leaves.node_factor.factor_value.rnd);
					break;
			}
			break;
		case AST_RELOP:
			// nothing to free here
			break;
		case AST_ADDOP:
			// nothing to free here
			break;
		case AST_MULOP:
			// nothing to free here
			break;
		case AST_VAR:
			// nothing to free here
			break;
		case AST_RND:
			free_ast_node(node->node_leaves.node_rnd.expression);
			break;
		case AST_STRING:
			free(node->node_leaves.node_string.value);
			node->node_leaves.node_string.value = NULL;
			break;
		case AST_NUMBER:
			// nothing to free here
			break;
	}

	memset(node, '\0', sizeof(struct ast_node));
	free(node);
	node = NULL;
}

void print_ast_node(struct ast_node *node) {

	if (node == NULL) {
		return;
	}

	switch (node->node_type) {
		case AST_LINE:
			if (node->node_leaves.node_line.number != -1) {
				printf("%d ", node->node_leaves.node_line.number);
			}
			print_ast_node(node->node_leaves.node_line.statement);
			printf("\n");
			print_ast_node(node->node_leaves.node_line.next);
			break;
		case AST_STATEMENT:
			switch (node->node_leaves.node_statement.statement_type) {
				case AST_ST_PRINT:
					printf("PRINT ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_print.expr_list);
					break;
				case AST_ST_IF:
					printf("IF ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.left);
					printf(" ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.relop);
					printf(" ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.right);
					printf(" THEN ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_if.statement);
					break;
				case AST_ST_GOTO:
					printf("GOTO ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_goto.expression);
					break;
				case AST_ST_INPUT:
					printf("INPUT ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_input.var_list);
					break;
				case AST_ST_LET:
					printf("LET ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_let.var);
					printf(" = ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_let.expression);
					break;
				case AST_ST_GOSUB:
					printf("GOSUB ");
					print_ast_node(node->node_leaves.node_statement.ast_statement_value.ast_statement_gosub.expression);
					break;
				case AST_ST_RETURN:
					printf("RETURN");
					break;
				case AST_ST_END:
					printf("END");
					break;
			}
			break;
		case AST_COMMAND:
			switch (node->node_leaves.node_command.command_type) {
				case AST_CT_CLEAR:
					printf("CLEAR");
					break;
				case AST_CT_LIST:
					printf("LIST");
					break;
				case AST_CT_RUN:
					printf("RUN");
					break;
			}
			break;
		case AST_EXPR_LIST:
			switch (node->node_leaves.node_expr_list.expr_list_type) {
				case AST_EL_SINGLE:
					print_ast_node(node->node_leaves.node_expr_list.single);
					break;
				case AST_EL_LIST:
					print_ast_node(node->node_leaves.node_expr_list.list);
					printf(", ");
					print_ast_node(node->node_leaves.node_expr_list.single);
					break;
			}
			break;
		case AST_EXPR_ITEM:
			switch (node->node_leaves.node_expr_item.expr_item_type) {
				case AST_EI_STRING:
					print_ast_node(node->node_leaves.node_expr_item.expr_item_value.string);
					break;
				case AST_EI_EXPRESSION:
					print_ast_node(node->node_leaves.node_expr_item.expr_item_value.expression);
					break;
			}
			break;
		case AST_VAR_LIST:
			switch (node->node_leaves.node_var_list.var_list_type) {
				case AST_VL_SINGLE:
					print_ast_node(node->node_leaves.node_var_list.single);
					break;
				case AST_VL_LIST:
					print_ast_node(node->node_leaves.node_var_list.list);
					printf(", ");
					print_ast_node(node->node_leaves.node_var_list.single);
					break;
			}
			break;
		case AST_EXPRESSION:
			switch (node->node_leaves.node_expression.expression_type) {
				case AST_ET_ATAT:
					print_ast_node(node->node_leaves.node_expression.term1_op);
					print_ast_node(node->node_leaves.node_expression.term1);
					printf(" ");
					print_ast_node(node->node_leaves.node_expression.term2_op);
					printf(" ");
					print_ast_node(node->node_leaves.node_expression.term2);
					break;
				case AST_ET_AT:
					print_ast_node(node->node_leaves.node_expression.term1_op);
					print_ast_node(node->node_leaves.node_expression.term1);
					break;
				case AST_ET_TAT:
					print_ast_node(node->node_leaves.node_expression.term1);
					printf(" ");
					print_ast_node(node->node_leaves.node_expression.term2_op);
					printf(" ");
					print_ast_node(node->node_leaves.node_expression.term2);
					break;
				case AST_ET_T:
					print_ast_node(node->node_leaves.node_expression.term1);
					break;
			}
			break;
		case AST_TERM:
			switch (node->node_leaves.node_term.term_type) {
				case AST_TT_FMF:
					print_ast_node(node->node_leaves.node_term.factor1);
					printf(" ");
					print_ast_node(node->node_leaves.node_term.op);
					printf(" ");
					print_ast_node(node->node_leaves.node_term.factor2);
					break;
				case AST_TT_F:
					print_ast_node(node->node_leaves.node_term.factor1);
					break;
			}
			break;
		case AST_FACTOR:
			switch (node->node_leaves.node_factor.factor_type) {
				case AST_FT_VAR:
					print_ast_node(node->node_leaves.node_factor.factor_value.var);
					break;
				case AST_FT_NUMBER:
					print_ast_node(node->node_leaves.node_factor.factor_value.number);
					break;
				case AST_FT_EXPRESSION:
					printf("(");
					print_ast_node(node->node_leaves.node_factor.factor_value.expression);
					printf(")");
					break;
				case AST_FT_RND:
					print_ast_node(node->node_leaves.node_factor.factor_value.rnd);
					break;
			}
			break;
		case AST_RELOP:
			switch (node->node_leaves.node_relop.op) {
				case AST_LT:
					printf("<");
					break;
				case AST_LTEQ:
					printf("<=");
					break;
				case AST_LTGT:
					printf("<>");
					break;
				case AST_GT:
					printf(">");
					break;
				case AST_GTEQ:
					printf(">=");
					break;
				case AST_GTLT:
					printf("><");
					break;
				case AST_EQ:
					printf("=");
					break;
			}
			break;
		case AST_ADDOP:
			printf("%c", node->node_leaves.node_addop.op == AST_PLUS ? '+' : '-');
			break;
		case AST_MULOP:
			printf("%c", node->node_leaves.node_mulop.op == AST_TIMES ? '*' : '/');
			break;
		case AST_VAR:
			printf("%c", node->node_leaves.node_var.value);
			break;
		case AST_STRING:
			printf("\"%s\"", node->node_leaves.node_string.value);
			break;
		case AST_NUMBER:
			printf("%d", node->node_leaves.node_number.value);
			break;
		case AST_RND:
			printf("RND(");
			print_ast_node(node->node_leaves.node_rnd.expression);
			printf(")");
			break;
	}

}
