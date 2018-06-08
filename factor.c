/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017, 2018  Thomas Cort <linuxgeek@gmail.com>

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

#include "tokenizer.h"

#include "expression.h"
#include "factor.h"
#include "number.h"
#include "rnd.h"
#include "time_.h"
#include "sin.h"
#include "cos.h"
#include "tan.h"
#include "cot.h"
#include "atn.h"
#include "exp.h"
#include "log.h"
#include "abs.h"
#include "sgn.h"
#include "sqr.h"
#include "var.h"

struct factor *new_factor(int type, void *value) {

	struct factor *f;

	f = (struct factor *) malloc(sizeof(struct factor));
	if (f == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(f, '\0', sizeof(struct factor));

	f->type = type;
	switch (f->type) {
		case EXPRESSION:
			f->u.e = (struct expression *) value;
			break;
		case NUMBER:
			f->u.n = (struct number *) value;
			break;
		case RND:
			f->u.r = (struct rnd *) value;
			break;
		case TIME:
			f->u.time = (struct time *) value;
			break;
		case SIN:
			f->u.sin = (struct sin *) value;
			break;
		case COS:
			f->u.cos = (struct cos *) value;
			break;
		case TAN:
			f->u.tan = (struct tan *) value;
			break;
		case COT:
			f->u.cot = (struct cot *) value;
			break;
		case ATN:
			f->u.atn = (struct atn *) value;
			break;
		case EXP:
			f->u.exp = (struct exp *) value;
			break;
		case LOG:
			f->u.log = (struct log *) value;
			break;
		case ABS:
			f->u.abs = (struct abs *) value;
			break;
		case SGN:
			f->u.sgn = (struct sgn *) value;
			break;
		case SQR:
			f->u.sqr = (struct sqr *) value;
			break;
		case VAR:
			f->u.v = (struct var *) value;
			break;
	}

	return f;
}

struct factor *parse_factor(struct tokenizer *t) {

	struct var *v;
	struct number *num;
	struct rnd *r;
	struct time *time;
	struct sin *sin;
	struct cos *cos;
	struct tan *tan;
	struct cot *cot;
	struct atn *atn;
	struct exp *exp;
	struct log *log;
	struct abs *abs;
	struct sgn *sgn;
	struct sqr *sqr;

	v = parse_var(t);
	if (v != NULL) {
		return new_factor(VAR, v);
	}

	num = parse_number(t);
	if (num != NULL) {
		return new_factor(NUMBER, num);
	}

	token_get(t);
	if (t->token.type == OPAREN) {
		struct expression *expr;
		expr = parse_expression(t);
		if (expr == NULL) {
			return NULL;
		}
		token_get(t);
		if (t->token.type == CPAREN) {
			return new_factor(EXPRESSION, expr);
		}
		token_unget(t);
		free_expression(expr);
		return NULL;
	}
	token_unget(t);

	r = parse_rnd(t);
	if (r != NULL) {
		return new_factor(RND, r);
	}

	time = parse_time(t);
	if (time != NULL) {
		return new_factor(TIME, time);
	}

	sin = parse_sin(t);
	if (sin != NULL) {
		return new_factor(SIN, sin);
	}

	cos = parse_cos(t);
	if (cos != NULL) {
		return new_factor(COS, cos);
	}

	tan = parse_tan(t);
	if (tan != NULL) {
		return new_factor(TAN, tan);
	}

	cot = parse_cot(t);
	if (cot != NULL) {
		return new_factor(COT, cot);
	}

	atn = parse_atn(t);
	if (atn != NULL) {
		return new_factor(ATN, atn);
	}

	exp = parse_exp(t);
	if (exp != NULL) {
		return new_factor(EXP, exp);
	}

	log = parse_log(t);
	if (log != NULL) {
		return new_factor(LOG, log);
	}

	abs = parse_abs(t);
	if (abs != NULL) {
		return new_factor(ABS, abs);
	}

	sgn = parse_sgn(t);
	if (sgn != NULL) {
		return new_factor(SGN, sgn);
	}

	sqr = parse_sqr(t);
	if (sqr != NULL) {
		return new_factor(SQR, sqr);
	}

	return NULL;
}

struct number * eval_factor(struct factor *f) {

	struct number *n = NULL;

	if (f == NULL) {
		return new_number_from_int(1);
	}

	switch (f->type) {
		case EXPRESSION:
			n = eval_expression(f->u.e);
			break;
		case NUMBER:
			n = clone_number(f->u.n);
			break;
		case RND:
			n = eval_rnd();
			break;
		case TIME:
			n = eval_time();
			break;
		case SIN:
			n = eval_sin(f->u.sin);
			break;
		case COS:
			n = eval_cos(f->u.cos);
			break;
		case TAN:
			n = eval_tan(f->u.tan);
			break;
		case COT:
			n = eval_cot(f->u.cot);
			break;
		case ATN:
			n = eval_atn(f->u.atn);
			break;
		case EXP:
			n = eval_exp(f->u.exp);
			break;
		case LOG:
			n = eval_log(f->u.log);
			break;
		case ABS:
			n = eval_abs(f->u.abs);
			break;
		case SGN:
			n = eval_sgn(f->u.sgn);
			break;
		case SQR:
			n = eval_sqr(f->u.sqr);
			break;
		case VAR:
			n = eval_var(f->u.v);
			break;
	}

	return n;
}

void print_factor(struct factor *f) {

	if (f == NULL) {
		return;
	}

	switch (f->type) {
		case EXPRESSION:
			printf("(");
			print_expression(f->u.e);
			printf(")");
			break;
		case NUMBER:
			print_number(f->u.n);
			break;
		case RND:
			print_rnd(f->u.r);
			break;
		case TIME:
			print_time(f->u.time);
			break;
		case SIN:
			print_sin(f->u.sin);
			break;
		case COS:
			print_cos(f->u.cos);
			break;
		case TAN:
			print_tan(f->u.tan);
			break;
		case COT:
			print_cot(f->u.cot);
			break;
		case ATN:
			print_atn(f->u.atn);
			break;
		case EXP:
			print_exp(f->u.exp);
			break;
		case LOG:
			print_log(f->u.log);
			break;
		case ABS:
			print_abs(f->u.abs);
			break;
		case SGN:
			print_sgn(f->u.sgn);
			break;
		case SQR:
			print_sqr(f->u.sqr);
			break;
		case VAR:
			print_var(f->u.v);
			break;
	}
}

void free_factor(struct factor *f) {
	if (f != NULL) {
		switch (f->type) {
			case EXPRESSION:
				free_expression(f->u.e);
				f->u.e = NULL;
				break;
			case NUMBER:
				free_number(f->u.n);
				f->u.n = NULL;
				break;
			case RND:
				free_rnd(f->u.r);
				f->u.r = NULL;
				break;
			case TIME:
				free_time(f->u.time);
				f->u.time = NULL;
				break;
			case SIN:
				free_sin(f->u.sin);
				f->u.sin = NULL;
				break;
			case COS:
				free_cos(f->u.cos);
				f->u.cos = NULL;
				break;
			case TAN:
				free_tan(f->u.tan);
				f->u.tan = NULL;
				break;
			case COT:
				free_cot(f->u.cot);
				f->u.cot = NULL;
				break;
			case ATN:
				free_atn(f->u.atn);
				f->u.atn = NULL;
				break;
			case EXP:
				free_exp(f->u.exp);
				f->u.exp = NULL;
				break;
			case LOG:
				free_log(f->u.log);
				f->u.log = NULL;
				break;
			case ABS:
				free_abs(f->u.abs);
				f->u.abs = NULL;
				break;
			case SGN:
				free_sgn(f->u.sgn);
				f->u.sgn = NULL;
				break;
			case SQR:
				free_sqr(f->u.sqr);
				f->u.sqr = NULL;
				break;
			case VAR:
				free_var(f->u.v);
				f->u.v = NULL;
				break;
		}
		free(f);
	}
}
