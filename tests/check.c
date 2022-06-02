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
#include "io.h"
#include "str.h"
#include "sys.h"

/*
 * Execute a series of checks in 'checks'
 *
 * Parameters:
 *  checks - a list of checks to perform
 *
 * Returns TC_CHECK_PASS if all return it, otherwise return on first fail/skip
 */
int tc_check(struct check *checks) {
	int i;

	if (checks == TC_NULL) {
		tc_puts(TC_STDERR, "list of checks is NULL\n");
		return TC_CHECK_FAIL;
	}

	for (i = 0; checks[i].message != TC_NULL; i++) {
		int rc;

		rc = checks[i].fn();
		if (rc == 0) {
			tc_puts(TC_STDERR, "check \"");
			tc_puts(TC_STDERR, checks[i].message);
			tc_puts(TC_STDERR, "\" failed\n");
			return rc;
		}
	}

	return TC_CHECK_PASS;
}

