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

#ifndef TCBASIC_EXPR_LIST_H
#define TCBASIC_EXPR_LIST_H

struct expr_item;
struct tokenizer;

struct expr_list {
	struct expr_item *expr_item;
	struct expr_list *list;
};

struct expr_list *new_expr_list(struct expr_item *expr_item, struct expr_list *list);
struct expr_list *parse_expr_list(struct tokenizer *t);
void eval_expr_list(struct expr_list *el);
void print_expr_list(struct expr_list *el);
void free_expr_list(struct expr_list *el);

#endif
