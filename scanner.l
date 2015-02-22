%{

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

#include <stdio.h>
#include <string.h>

#include "parser.h"

int yyerror(char *);
%}

%option reentrant

%%

"PRINT"		{ return PRINT; }
"IF"		{ return IF; }
"THEN"		{ return THEN; }
"GOTO"		{ return GOTO; }
"INPUT"		{ return INPUT; }
"LET"		{ return LET; }
"GOSUB"		{ return GOSUB; }
"RETURN"	{ return _RETURN; }
"CLEAR"		{ return CLEAR; }
"LIST"		{ return LIST; }
"RUN"		{ return RUN; }
"END"		{ return END; }

"RND"		{ return RND; }

"<="		{ return LTEQ; }
"<>"		{ return LTGT; }
"<"		{ return LT; }
">="		{ return GTEQ; }
"><"		{ return GTLT; }
">"		{ return GT; }
"="		{ return EQ; }

"+"		{ return PLUS; }
"-"		{ return MINUS; }
"*"		{ return TIMES; }
"/"		{ return DIVIDE; }

","		{ return COMMA; }
"("		{ return OPAREN; }
")"		{ return CPAREN; }

\"[^"]*\"	{
			yylval.string_literal = strdup(yytext + 1);
			yylval.string_literal[strlen(yylval.string_literal) - 1] = '\0';
			return STRING;
		}

[A-Z]		{
			yylval.var_name = yytext[0];
			return VAR;
		}

[0-9]+		{
			yylval.number_literal = atoi(yytext);
			return NUMBER;
		}

[ \t\f\r\n]+	{
			;       /* ignore whitespace */
		}

.	{
		yyerror("Unknown character");
	}

%%

int yywrap(yyscan_t yyscanner) {
	return 1;
}
