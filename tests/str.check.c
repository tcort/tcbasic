/*

  tcbasic - an interpreter for the programming language BASIC
  Copyright (C) 2022 Thomas Cort

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "config.h"
#include "check.h"
#include "str.h"
#include "sys.h"

static int check_strlen_null(void) { return tc_strlen(TC_NULL) == 0; }
static int check_strlen_empty(void) { return tc_strlen("") == 0; }
static int check_strlen_one(void) { return tc_strlen("X") == 1; }
static int check_strlen_many(void) { return tc_strlen("foo") == 3; }

static int check_streql_yes(void) { return tc_streql("FoO", "FoO") == 1; }
static int check_streql_no_a(void) { return tc_streql("FoO", "FOO") == 0; }
static int check_streql_no_b(void) { return tc_streql("foo", "foobar") == 0; }
static int check_streql_no_c(void) { return tc_streql("foobar", "foo") == 0; }
static int check_streql_no_d(void) { return tc_streql("foo", TC_NULL) == 0; }
static int check_streql_no_e(void) { return tc_streql(TC_NULL, "foo") == 0; }

static int check_strneql_yes_a(void) { return tc_strneql("--version", "--", 2) == 1; }
static int check_strneql_yes_b(void) { return tc_strneql("--", "--version", 2) == 1; }
static int check_strneql_yes_c(void) { return tc_strneql("--", "--", 2) == 1; }
static int check_strneql_no_a(void) { return tc_strneql("-x", "--", 2) == 0; }
static int check_strneql_no_b(void) { return tc_strneql(TC_NULL, "--", 2) == 0; }
static int check_strneql_no_c(void) { return tc_strneql("--", TC_NULL, 2) == 0; }

static int check_memset(void) {
	int i;
	char foo[16];

	foo[15] = 'A';
	tc_memset(foo, 'X', 15);

	for (i = 0; i < 15; i++) {
		if (foo[i] != 'X') {
			return TC_CHECK_FAIL;
		}
	}

	return foo[15] == 'A' ? TC_CHECK_PASS : TC_CHECK_FAIL;
}

int main(int argc, char *argv[]) {

	static struct check checks[] = {
		{ check_strlen_null,  "length of TC_NULL is 0" },
		{ check_strlen_empty, "length of empty string is 0" },
		{ check_strlen_one,   "length of \"X\" string is 1" },
		{ check_strlen_many,  "length of \"foo\" string is 3" },
		{ check_streql_yes,   "two identical strings are equal" },
		{ check_streql_no_a,   "two non-identical strings are not equal" },
		{ check_streql_no_b,   "two strings of different length are not equal" },
		{ check_streql_no_c,   "two strings of different length are not equal" },
		{ check_streql_no_d,   "a string and TC_NULL are not equal" },
		{ check_streql_no_e,   "a string and TC_NULL are not equal" },
		{ check_strneql_yes_a,	"\"--version\" strneql(2) \"--\"" },
		{ check_strneql_yes_b,	"\"--\" strneql(2) \"--version\"" },
		{ check_strneql_yes_c,	"\"--\" strneql(2) \"--\"" },
		{ check_strneql_no_a,	"\"-x\" !strneql(2) \"--\"" },
		{ check_strneql_no_b,	"TC_NULL !strneql(2) \"--\"" },
		{ check_strneql_no_c,	"\"--\" !strneql(2) TC_NULL" },
		{ check_memset,       "memset sets the desired number of chars to the desired value" },
		{ TC_NULL, TC_NULL }
	};

	return tc_check(checks);
}
