/*
    basic - a small BASIC Interpreter written in C.
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
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>

#include "runtime.h"

void parseLine(char *line);

int main(int argc, char *argv[]) {

	char *line = NULL;

	runtime_reset();

	if (argc == 2) {
		rl_instream = fopen(argv[1], "r");
		if (rl_instream == NULL) {
			perror("fopen");
			return EXIT_FAILURE;
		}

		rl_outstream = fopen("/dev/null", "r");
		if (rl_outstream == NULL) {
			perror("fopen");
			return EXIT_FAILURE;
		}
	}

	do {
		line = readline(NULL);
		if (line == NULL) {
			break;
		}
		parseLine(line);	/* TODO readline history? */
		free(line);
	} while (runtime_continue());

	if (argc == 2) {
		line = strdup("RUN");
		parseLine(line);
		free(line);
	}

	if (rl_instream != NULL) {
		fclose(rl_instream);
		rl_instream = NULL;
	}

	if (rl_outstream != NULL) {
		fclose(rl_outstream);
		rl_outstream = NULL;
	}

	runtime_reset();

	return EXIT_SUCCESS;
}
