%{

/*
    basic - a small BASIC Interpreter written in C.
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

#include "ast.h"
#include "runtime.h"
#include "scanner.h"

int yyerror(char *s) {
	fprintf(stdout, "%s\n", s);
	exit(EXIT_FAILURE);
	return -1;
}

%}

%union {
	int number;
	char var;
	char *string;
	struct ast_node *node;
};

%token <number> NUMBER
%token <var> VAR
%token <string> STRING

/* keywords */
%token PRINT IF THEN GOTO INPUT LET GOSUB _RETURN CLEAR LIST RUN END QUIT

/* relational operators */
%token LTEQ LTGT LT GTEQ GTLT GT EQ

/* arithmetic operators */
%token PLUS MINUS TIMES DIVIDE

/* separators */
%token COMMA OPAREN CPAREN

%type <node> expression factor statement term
%type <node> var_list expr_list expr_item
%type <node> number string var
%type <node> addop mulop relop

%%

line	: number statement { struct ast_node *line = new_ast_node(AST_LINE); line->node_leaves.node_line.number = $1->node_leaves.node_number.value; line->node_leaves.node_line.statement = $2; free_ast_node($1); processLine(line); }
	| statement { struct ast_node *line = new_ast_node(AST_LINE); line->node_leaves.node_line.number = -1; line->node_leaves.node_line.statement = $1; processLine(line); }
	;

statement	: PRINT expr_list { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_PRINT; $$->node_leaves.node_statement.ast_statement_value.ast_statement_print.expr_list = $2; }
		| IF expression relop expression THEN statement { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_IF; $$->node_leaves.node_statement.ast_statement_value.ast_statement_if.left = $2; $$->node_leaves.node_statement.ast_statement_value.ast_statement_if.relop = $3; $$->node_leaves.node_statement.ast_statement_value.ast_statement_if.right = $4; $$->node_leaves.node_statement.ast_statement_value.ast_statement_if.statement = $6; }
		| GOTO expression { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_GOTO; $$->node_leaves.node_statement.ast_statement_value.ast_statement_goto.expression = $2; }
		| INPUT var_list { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_INPUT; $$->node_leaves.node_statement.ast_statement_value.ast_statement_input.var_list = $2; }
		| LET var EQ expression { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_LET; $$->node_leaves.node_statement.ast_statement_value.ast_statement_let.var = $2; $$->node_leaves.node_statement.ast_statement_value.ast_statement_let.expression = $4; }
		| GOSUB expression { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_GOSUB; $$->node_leaves.node_statement.ast_statement_value.ast_statement_gosub.expression = $2; }
		| _RETURN { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_RETURN; }
		| CLEAR { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_CLEAR; }
		| LIST { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_LIST; }
		| RUN { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_RUN; }
		| END { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_END; }
		| QUIT { $$ = new_ast_node(AST_STATEMENT); $$->node_leaves.node_statement.statement_type = AST_ST_QUIT; }
		;

expr_list	: expr_list COMMA expr_item { $$ = new_ast_node(AST_EXPR_LIST); $$->node_leaves.node_expr_list.expr_list_type = AST_EL_LIST; $$->node_leaves.node_expr_list.list = $1;  $$->node_leaves.node_expr_list.single = $3; }
		| expr_item { $$ = new_ast_node(AST_EXPR_LIST); $$->node_leaves.node_expr_list.expr_list_type = AST_EL_SINGLE; $$->node_leaves.node_expr_list.single = $1; }
		;

expr_item	: expression { $$ = new_ast_node(AST_EXPR_ITEM); $$->node_leaves.node_expr_item.expr_item_type = AST_EI_EXPRESSION; $$->node_leaves.node_expr_item.expr_item_value.expression = $1; }
		| string { $$ = new_ast_node(AST_EXPR_ITEM); $$->node_leaves.node_expr_item.expr_item_type = AST_EI_STRING; $$->node_leaves.node_expr_item.expr_item_value.string = $1; }
		;

var_list	: var_list COMMA var { $$ = new_ast_node(AST_VAR_LIST); $$->node_leaves.node_var_list.var_list_type = AST_VL_LIST; $$->node_leaves.node_var_list.list = $1;  $$->node_leaves.node_var_list.single = $3; }
		| var { $$ = new_ast_node(AST_VAR_LIST); $$->node_leaves.node_var_list.var_list_type = AST_VL_SINGLE; $$->node_leaves.node_var_list.single = $1; }
		;

addop		: PLUS  { $$ = new_ast_node(AST_ADDOP); $$->node_leaves.node_addop.op = AST_PLUS;  }
		| MINUS { $$ = new_ast_node(AST_ADDOP); $$->node_leaves.node_addop.op = AST_MINUS; }
		;

expression	: addop term addop term { $$ = new_ast_node(AST_EXPRESSION); $$->node_leaves.node_expression.expression_type = AST_ET_ATAT; $$->node_leaves.node_expression.term1_op = $1; $$->node_leaves.node_expression.term1 = $2; $$->node_leaves.node_expression.term2_op = $3; $$->node_leaves.node_expression.term2 = $4; }
		| addop term { $$ = new_ast_node(AST_EXPRESSION); $$->node_leaves.node_expression.expression_type = AST_ET_AT; $$->node_leaves.node_expression.term1_op = $1; $$->node_leaves.node_expression.term1 = $2; }
		| term addop term { $$ = new_ast_node(AST_EXPRESSION); $$->node_leaves.node_expression.expression_type = AST_ET_TAT; $$->node_leaves.node_expression.term1 = $1; $$->node_leaves.node_expression.term2_op = $2; $$->node_leaves.node_expression.term2 = $3; }
		| term { $$ = new_ast_node(AST_EXPRESSION); $$->node_leaves.node_expression.expression_type = AST_ET_T; $$->node_leaves.node_expression.term1 = $1; }
		;

mulop		: TIMES  { $$ = new_ast_node(AST_MULOP); $$->node_leaves.node_mulop.op = AST_TIMES;  }
		| DIVIDE { $$ = new_ast_node(AST_MULOP); $$->node_leaves.node_mulop.op = AST_DIVIDE; }
		;

term		: factor mulop factor { $$ = new_ast_node(AST_TERM); $$->node_leaves.node_term.term_type = AST_TT_FMF; $$->node_leaves.node_term.factor1 = $1; $$->node_leaves.node_term.op = $2;  $$->node_leaves.node_term.factor2 = $3; }
		| factor { $$ = new_ast_node(AST_TERM); $$->node_leaves.node_term.term_type = AST_TT_F; $$->node_leaves.node_term.factor1 = $1; }
		;

factor		: var { $$ = new_ast_node(AST_FACTOR); $$->node_leaves.node_factor.factor_type = AST_FT_VAR; $$->node_leaves.node_factor.factor_value.var = $1; }
		| number { $$ = new_ast_node(AST_FACTOR); $$->node_leaves.node_factor.factor_type = AST_FT_NUMBER; $$->node_leaves.node_factor.factor_value.number = $1; }
		| OPAREN expression CPAREN { $$ = new_ast_node(AST_FACTOR); $$->node_leaves.node_factor.factor_type = AST_FT_EXPRESSION; $$->node_leaves.node_factor.factor_value.expression = $2; }
		;

string		: STRING { $$ = new_ast_node(AST_STRING); $$->node_leaves.node_string.value = $1; }
		;

var		: VAR { $$ = new_ast_node(AST_VAR); $$->node_leaves.node_var.value = $1; }
		;

number		: NUMBER { $$ = new_ast_node(AST_NUMBER); $$->node_leaves.node_number.value = $1; }
		;

relop		: LTEQ { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_LTEQ; }
		| LTGT { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_LTGT; }
		| LT   { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_LT;   }
		| GTEQ { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_GTEQ; }
		| GTLT { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_GTLT; }
		| GT   { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_GT;   }
		| EQ   { $$ = new_ast_node(AST_RELOP); $$->node_leaves.node_relop.op = AST_EQ;   }
		;

%%

void parseLine(char *line) {

	if (line == NULL || strlen(line) == 0) {
		return;
	}

	yy_scan_string(line);
	yyparse();
	yylex_destroy();
}
