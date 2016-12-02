/*
    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016  Thomas Cort <linuxgeek@gmail.com>

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "tokenizer.h"

struct token_info {
	const char *regex_text;
	enum token_types type;
};

struct token_info token_defs[] = {
	{ "^PRINT",                                          PRINT      },
	{ "^IF",                                             IF         },
	{ "^THEN",                                           THEN       },
	{ "^GOTO",                                           GOTO       },
	{ "^INPUT",                                          INPUT      },
	{ "^LET",                                            LET        },
	{ "^GOSUB",                                          GOSUB      },
	{ "^RETURN",                                         RETURN     },
	{ "^END",                                            END        },
	{ "^REM.*$",                                         REM        },
	{ "^RANDOMIZE",                                      RANDOMIZE  },
	{ "^CLEAR",                                          CLEAR      },
	{ "^LIST",                                           LIST       },
	{ "^RUN",                                            RUN        },
	{ "^STOP",                                           STOP       },
	{ "^RND",                                            RND        },
	{ "^SIN",                                            SIN        },
	{ "^COS",                                            COS        },
	{ "^TAN",                                            TAN        },
	{ "^COT",                                            COT        },
	{ "^ATN",                                            ATN        },
	{ "^EXP",                                            EXP        },
	{ "^LOG",                                            LOG        },
	{ "^ABS",                                            ABS        },
	{ "^SQR",                                            SQR        },
	{ "^<=",                                             LTEQ       },
	{ "^<>",                                             LTGT       },
	{ "^<",                                              LT         },
	{ "^>=",                                             GTEQ       },
	{ "^><",                                             GTLT       },
	{ "^>",                                              GT         },
	{ "^=",                                              EQ         },
	{ "^\\+",                                            PLUS       },
	{ "^\\-",                                            MINUS      },
	{ "^\\*",                                            TIMES      },
	{ "^\\/",                                            DIVIDE     },
	{ "^MOD",                                            MOD        },
	{ "^,",                                              COMMA      },
	{ "^\\(",                                            OPAREN     },
	{ "^\\)",                                            CPAREN     },
	{ "^\"[^\"]*\"",                                     STR        },
	{ "^[A-Z]",                                          VAR        },
	{ "^(([0-9]+|([0-9]*\\.[0-9]*))([eE][-+]?[0-9]+)?)", NUMBER     },
	{ "^[ \t\f\r\n]+",                                   SPACE      }
};
#define NTOKEN_TYPES (sizeof(token_defs)/sizeof(token_defs[0]))

regex_t regexes[NTOKEN_TYPES];

static struct token nextToken(char **s) {
	size_t i;
	size_t len;
	struct token t;
	int rc;
	regmatch_t matches[1];

	t.type = INVALID;
	t.text = NULL;

	do {
		if (s == NULL || *s == NULL) {
			t.type = INVALID;
			return t;
		} else if (*s[0] == '\0' || strlen(*s) == 0) {
			t.type = EOS;
			return t;
		}
		if (t.text != NULL) { /* free space token */
			free(t.text);
			t.text = NULL;
		}
		for (i = 0; i < NTOKEN_TYPES; i++) {
			rc = regexec(&regexes[i], *s, 1, matches,  0);
			if (rc == 0) {
				len = (size_t) (matches[0].rm_eo - matches[0].rm_so);
				t.type = token_defs[i].type;
				t.text = (char *) malloc(len + 1);
				if (t.text == NULL) {
					perror("malloc");
					exit(EXIT_FAILURE);
				}
				strncpy(t.text, *s, len);
				t.text[len] = '\0';
				*s += len;
				break;
			}
		}
	} while (t.type == SPACE);
	return t;
}

void token_get(struct tokenizer *t) {
	if (t == NULL) {
		return;
	} else if (t->token.text != NULL) {
		free(t->token.text);
		t->token.text = NULL;
	}
	t->token = nextToken(t->s);
}

void token_unget(struct tokenizer *t) {
	char *s;
	if (t->token.text == NULL) {
		return;
	}
	s = *(t->s);
	s -= strlen(t->token.text);
	*(t->s) = s;
	t->token.type = INVALID;
	free(t->token.text);
	t->token.text = NULL;
}


void tokenizer_init() {
	size_t i;
	for (i = 0; i < NTOKEN_TYPES; i++) {
		regcomp(&regexes[i], token_defs[i].regex_text, REG_EXTENDED);
	}
}

void tokenizer_exit() {
	size_t i;
	for (i = 0; i < NTOKEN_TYPES; i++) {
		regfree(&regexes[i]);
	}	
}

