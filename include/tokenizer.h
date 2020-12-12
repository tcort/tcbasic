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

#ifndef TCBASIC_TOKENIZER_H
#define TCBASIC_TOKENIZER_H

enum token_types {
	PRINT, IF, THEN, GOTO, INPUT, LET, GOSUB, RETURN, END, REM, RANDOMIZE,
	CLEAR, LIST, RENUM, RUN, STOP, TROFF, TRON, CLS, SHELL, BEEP,
	RND, TIME, FOR, TO, STEP, NEXT,
	SIN, COS, TAN, COT, ATN, EXP, LOG, ABS, SGN, SQR, INT_,
	LTEQ, LTGT, LT, GTEQ, GTLT, GT, EQ,
	PLUS, MINUS, TIMES, DIVIDE, IDIVIDE, MOD,
	CIRCUMFLEX, COMMA, OPAREN, CPAREN,
	STR, VAR, NUMBER, PI,
	SPACE, INVALID, EOS
};

enum lhs {
	EXPRESSION
};

struct token {
	enum token_types type;
	char *text;
};

struct tokenizer {
	char **s;
	struct token token;
};

void token_get(struct tokenizer *t);
void token_unget(struct tokenizer *t);

void tokenizer_init(void);
void tokenizer_exit(void);

#endif
