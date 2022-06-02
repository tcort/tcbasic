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
#include "args.h"
#include "sys.h"

/*
 * Main Program Entry Point
 *
 * Parameters:
 *  - argc denotes the length of argv[]
 *  - argv[] contains a list of command line fragments
 *
 * Return: exit code
 */
int main(int argc, char *argv[]) {

	struct tc_prog_arg *arg;

	static struct tc_prog_arg args[] = {
		TC_PROG_ARG_HELP,
		TC_PROG_ARG_VERSION,
		TC_PROG_ARG_END
	};

	static struct tc_prog_example examples[] = {
		{ .command = "tcbasic foo.bas", .description = "execute foo.bas" },
		TC_PROG_EXAMPLE_END
	};

	static struct tc_prog prog = {
		.program = "tcbasic",
		.usage = "[OPTIONS] FILE.BAS",
		.description = "an interpreter for the programming language BASIC",
		.build = PACKAGE_BUILD,
		.version = PACKAGE_VERSION,
		.copyright = PACKAGE_COPYRIGHT,
		.license = PACKAGE_LICENSE,
		.author =  PACKAGE_AUTHOR,
		.args = args,
		.examples = examples
	};

	while ((arg = tc_args_process(&prog, argc, argv)) != TC_NULL) {
		switch (arg->arg) {
			case 'h':
				tc_args_show_help(&prog);
				break;
			case 'V':
				tc_args_show_version(&prog);
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (argc != 1) {
		tc_args_show_usage(&prog);
	}

	return TC_EXIT_SUCCESS;
}
