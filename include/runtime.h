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

#ifndef TCBASIC_RUNTIME_H
#define TCBASIC_RUNTIME_H

struct number;
struct line;

struct for_state {
	struct number *limit;
	struct number *step;
	int target;
};

struct line *runtime_get_first_line(void);
struct line *runtime_get_line(int number);
void runtime_set_line(struct line *item);
void runtime_rm_line(int number);
int runtime_get_line_after_nearest_next(int for_line_number, char var);

void runtime_set_for_state(char v, struct number *limit, struct number *step, int target);
struct for_state *runtime_get_for_state(char v);

void runtime_reset(void);
int runtime_continue(void);
void runtime_stop(void);

void runtime_trace_on(void);
void runtime_trace_off(void);
int runtime_is_tracing(void);

void runtime_callstack_push(int number);
int runtime_callstack_pop(void);

void runtime_set_var(char var, struct number *value);
struct number *runtime_get_var(char var);

#endif
