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

#ifndef __LOG_H
#define __LOG_H

struct number;
struct tokenizer;

struct log {
	struct expression *e;
};

struct log *new_log(struct expression *e);
struct log *parse_log(struct tokenizer *t);
struct number *eval_log(struct log *l);
void print_log(struct log *l);
void free_log(struct log *l);

#endif
