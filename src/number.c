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

#include "config.h"

#include <errno.h>
#include <regex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "number.h"

#include "tokenizer.h"

struct number *new_number(char *s) {

	int is_int = 0;
	regex_t ireg;
	regmatch_t matches[1];
	struct number *n;

	n = (struct number *) malloc(sizeof(struct number));
	if (n == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memset(n, '\0', sizeof(struct number));

	regcomp(&ireg, "^[0-9]+$", REG_EXTENDED);
	is_int = !regexec(&ireg, s, 1, matches,  0);
	regfree(&ireg);

	if (is_int) {
		n->type = INT;
		n->value.ival = atoi(s);
	} else {
		n->type = FLOAT;
		n->value.fval = (float) atof(s);
	}

	return n;
}

struct number *new_number_from_int(int i) {

	struct number r;

	r.type = INT;
	r.value.ival = i;

	return clone_number(&r);
}

struct number *new_number_from_float(float f) {

	struct number r;

	r.type = FLOAT;
	r.value.fval = f;

	return clone_number(&r);
}

struct number *parse_number(struct tokenizer *t) {

	token_get(t);
	if (t->token.type == NUMBER) {
		return new_number(t->token.text);
	} else {
		token_unget(t);
		return NULL;
	}
}

struct number *clone_number(struct number *n) {
	struct number *clone;

	if (n == NULL) {
		return NULL;
	}

	clone = (struct number *) malloc(sizeof(struct number));
	if (clone == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	memcpy(clone, n, sizeof(struct number));
	return clone;
}

struct number *add_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}


	if (x->type == INT) {
		if (y->type == INT) {
			r.type = INT;
			r.value.ival = x->value.ival + y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.ival + y->value.fval;
		}
	} else {
		if (y->type == INT) {
			r.type = FLOAT;
			r.value.fval = x->value.fval + y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.fval + y->value.fval;
		}
	}

	return clone_number(&r);
}

struct number *subtract_number(struct number *x, struct number *y) {

	struct number *neg;
	struct number *yneg;
	struct number *r;

	if (x == NULL || y == NULL) {
		return NULL;
	}

	neg = new_number_from_int(-1);
	if (neg == NULL) {
		return NULL;
	}

	yneg = multiply_number(neg, y);
	if (yneg == NULL) {
		free_number(neg);
		return NULL;
	}

	free_number(neg);

	r = add_number(x, yneg);
	if (r == NULL) {
		free_number(yneg);
		return NULL;
	}

	free_number(yneg);
	return r;

}

struct number *multiply_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}


	if (x->type == INT) {
		if (y->type == INT) {
			r.type = INT;
			r.value.ival = x->value.ival * y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.ival * y->value.fval;
		}
	} else {
		if (y->type == INT) {
			r.type = FLOAT;
			r.value.fval = x->value.fval * y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.fval * y->value.fval;
		}
	}

	return clone_number(&r);
}

struct number *idivide_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}

	r.type = INT;
	r.value.ival = INT_VALUE(x) / INT_VALUE(y);

	return clone_number(&r);
}

struct number *divide_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}


	if (x->type == INT) {
		if (y->type == INT) {
			r.type = INT;
			r.value.ival = x->value.ival / y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.ival / y->value.fval;
		}
	} else {
		if (y->type == INT) {
			r.type = FLOAT;
			r.value.fval = x->value.fval / y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = x->value.fval / y->value.fval;
		}
	}

	return clone_number(&r);
}

struct number *modulus_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}


	if (x->type == INT) {
		if (y->type == INT) {
			r.type = INT;
			r.value.ival = x->value.ival % y->value.ival;
		} else {
			r.type = FLOAT;
			r.value.fval = fmod(x->value.ival, y->value.fval);
		}
	} else {
		if (y->type == INT) {
			r.type = FLOAT;
			r.value.fval = fmod(x->value.fval, y->value.ival);
		} else {
			r.type = FLOAT;
			r.value.fval = fmod(x->value.fval, y->value.fval);
		}
	}

	return clone_number(&r);
}

struct number *pow_number(struct number *x, struct number *y) {
	struct number r;

	if (x == NULL || y == NULL) {
		return NULL;
	}


	if (x->type == INT) {
		if (y->type == INT) {
			r.type = INT;
			r.value.ival = (int) pow(x->value.ival, y->value.ival);
		} else {
			r.type = FLOAT;
			r.value.fval = pow(x->value.ival, y->value.fval);
		}
	} else {
		if (y->type == INT) {
			r.type = FLOAT;
			r.value.fval = pow(x->value.fval, y->value.ival);
		} else {
			r.type = FLOAT;
			r.value.fval = pow(x->value.fval, y->value.fval);
		}
	}

	return clone_number(&r);
}

void print_number(struct number *n) {
	if (n == NULL) {
		return;
	}

	if (n->type == INT) {
		printf("%d", n->value.ival);
	} else {
		printf("%f", n->value.fval);
	}
}

void free_number(struct number *n) {
	if (n != NULL) {
		free(n);
	}
}

