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

#ifndef TCBASIC_ABS_H
#define TCBASIC_ABS_H

struct number;
struct tokenizer;

struct abs {
	struct expression *e;
};

struct abs *new_abs(struct expression *e);
struct abs *parse_abs(struct tokenizer *t);
struct number *eval_abs(struct abs *a);
void print_abs(struct abs *a);
void free_abs(struct abs *a);

#endif
