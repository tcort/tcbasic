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

#ifndef __AST_H
#define __AST_H

struct ast_node {
	enum ast_node_types {
		AST_LINE,
		AST_STATEMENT,
		AST_EXPR_LIST,
		AST_EXPR_ITEM,
		AST_VAR_LIST,
		AST_EXPRESSION,
		AST_TERM,
		AST_FACTOR,
		AST_RELOP,
		AST_ADDOP,
		AST_MULOP,
		AST_VAR,
		AST_STRING,
		AST_NUMBER
	} node_type;

	union ast_leaves {
		struct ast_line {
			int number;
			struct ast_node *statement;
			struct ast_node *next;
		} node_line;
		struct ast_statement {
			enum statement_type {
				AST_ST_PRINT,
				AST_ST_IF,
				AST_ST_GOTO,
				AST_ST_INPUT,
				AST_ST_LET,
				AST_ST_GOSUB,
				AST_ST_RETURN,
				AST_ST_CLEAR,
				AST_ST_LIST,
				AST_ST_RUN,
				AST_ST_END,
				AST_ST_QUIT
			} statement_type;
			union ast_statement_values {
				struct ast_statement_print {
					struct ast_node *expr_list;
				} ast_statement_print;
				struct ast_statement_if {
					struct ast_node *left;
					struct ast_node *relop;
					struct ast_node *right;
					struct ast_node *statement;
				} ast_statement_if;
				struct ast_statement_goto {
					struct ast_node *expression;
				} ast_statement_goto;
				struct ast_statement_input {
					struct ast_node *var_list;
				} ast_statement_input;
				struct ast_statement_let {
					struct ast_node *var;
					struct ast_node *expression;
				} ast_statement_let;
				struct ast_statement_gosub {
					struct ast_node *expression;
				} ast_statement_gosub;
			} ast_statement_value;
		} node_statement;
		struct ast_expr_list {
			enum ast_expr_list_types {
				AST_EL_SINGLE,
				AST_EL_LIST
			} expr_list_type;
			struct ast_node *list;
			struct ast_node *single;
		} node_expr_list;
		struct ast_expr_item {
			enum ast_expr_item_types {
				AST_EI_STRING,
				AST_EI_EXPRESSION
			} expr_item_type;
			union ast_expr_item_values {
				struct ast_node *string;
				struct ast_node *expression;
			} expr_item_value;
		} node_expr_item;
		struct ast_var_list {
			enum ast_var_list_types {
				AST_VL_SINGLE,
				AST_VL_LIST
			} var_list_type;
			struct ast_node *list;
			struct ast_node *single;
		} node_var_list;
		struct ast_addop {
			enum ast_addops {
				AST_PLUS,
				AST_MINUS
			} op;
		} node_addop;
		struct ast_expression {
			enum ast_expression_types {
				AST_ET_ATAT,
				AST_ET_AT,
				AST_ET_TAT,
				AST_ET_T
			} expression_type;
			struct ast_node *term1_op;
			struct ast_node *term1;
			struct ast_node *term2_op;
			struct ast_node *term2;
		} node_expression;
		struct ast_mulop {
			enum ast_mulops {
				AST_TIMES,
				AST_DIVIDE
			} op;
		} node_mulop;
		struct ast_term {
			enum ast_term_types {
				AST_TT_FMF,
				AST_TT_F
			} term_type;
			struct ast_node *factor1;
			struct ast_node *op;
			struct ast_node *factor2;
		} node_term;
		struct ast_factor {
			enum ast_factor_types {
				AST_FT_VAR,
				AST_FT_NUMBER,
				AST_FT_EXPRESSION
			} factor_type;
			union ast_factor_values {
				struct ast_node *var;
				struct ast_node *number;
				struct ast_node *expression;
			} factor_value;
		} node_factor;
		struct ast_relop {
			enum ast_relops {
				AST_LT,
				AST_LTEQ,
				AST_LTGT,
				AST_GT,
				AST_GTEQ,
				AST_GTLT,
				AST_EQ
			} op;
		} node_relop;
		struct ast_var {
			char value;
		} node_var;
		struct ast_string {
			char *value;
		} node_string;
		struct ast_number {
			int value;
		} node_number;
	} node_leaves;
};

struct ast_node *new_ast_node(enum ast_node_types node_type);
struct ast_node * insert_line(struct ast_node *list, struct ast_node *item);
void free_ast_node(struct ast_node *node);
void print_ast_node(struct ast_node *node);

#endif
