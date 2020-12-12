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

#ifndef TCBASIC_STATEMENT_H
#define TCBASIC_STATEMENT_H

struct expr_list;
struct expression;
struct statement;
struct relop;
struct rem;

struct statement {
	int type;

	union statements {
		struct print_statement {
			struct expr_list *expr_list;
		} print_stmt;
		struct if_statement {
			struct expression *left;
			struct relop *relop;
			struct expression *right;
			struct statement *statement;
		} if_stmt;
		struct shell_statement {
			struct str *str;
		} shell_stmt;
		struct goto_statement {
			struct expression *expression;
		} goto_stmt;
		struct input_statement {
			struct var_list *var_list;
		} input_stmt;
		struct let_statement {
			struct var *var;
			struct expression *expression;
		} let_stmt;
		struct gosub_statement {
			struct expression *expression;
		} gosub_stmt;
		struct for_statement {
			struct var *var;
			struct expression *initial;
			struct expression *limit;
			struct expression *step;
		} for_stmt;
		struct next_statement {
			struct var *var;
		} next_stmt;
		struct rem_statement {
			struct rem *rem;
		} rem_stmt;
	} u;
};

struct statement *new_statement(int type, void *arg1, void *arg2, void *arg3, void *arg4);
struct statement *parse_statement(struct tokenizer *t);
void print_statement(struct statement *s);
int eval_statement(struct statement *s, int number, int next_number);
void free_statement(struct statement *s);

#endif
