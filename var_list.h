/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017  Thomas Cort <linuxgeek@gmail.com>

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

#ifndef __VAR_LIST_H
#define __VAR_LIST_H

struct var;

struct var_list {
	struct var *var;
	struct var_list *list;
};

struct var_list *new_var_list(struct var *var, struct var_list *list);
struct var_list *parse_var_list(struct tokenizer *t);
void eval_var_list(struct var_list *vl, char *line);
void print_var_list(struct var_list *vl);
void free_var_list(struct var_list *vl);

#endif
