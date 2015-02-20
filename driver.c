/*
    tcb - a small BASIC Interpreter written in C.
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

#include "config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "runtime.h"

void parseLine(char *line);

void print_help(char *progname) {

	fprintf(stdout, "%s - a small BASIC Interpreter written in C\n", PACKAGE_NAME);
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage: %s [filename]\n", progname);
	fprintf(stdout, "\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, " -h, -?            --help                  Print a helpful message and exit\n");
	fprintf(stdout, " -v                --version               Print version information and exit\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Report bugs to %s\n", PACKAGE_BUGREPORT);

	exit(0);
}

static void print_version(void) {

	fprintf(stdout, "%s\n", PACKAGE_STRING);
	fprintf(stdout, "\n");
	fprintf(stdout, "Copyright (C) 2015 Thomas Cort <linuxgeek@gmail.com>\n");
	fprintf(stdout, "This is free software; see the source for copying conditions.  There is NO\n");
	fprintf(stdout, "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Written by Thomas Cort <linuxgeek@gmail.com>\n");

	exit(0);
}

int main(int argc, char *argv[]) {

	int optc;
	char *line = NULL;

	runtime_reset();

	const char* const short_options = "h?v";
#if HAVE_GETOPT_LONG
	static const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};
#endif

#if HAVE_GETOPT_LONG
	while ((optc = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
#else
	while ((optc = getopt(argc, argv, short_options)) != -1) {
#endif
		switch (optc) {
			case 'v':
				print_version();
				break;
			case 'h':
			case '?':
				print_help(argv[0]);
				break;
		}
	}

	if (optind != argc && optind + 1 != argc) {
		print_help(argv[0]);
	}


	if (argc == 2) {
		rl_instream = fopen(argv[optind], "r");
		if (rl_instream == NULL) {
			perror("fopen");
			return EXIT_FAILURE;
		}

		rl_outstream = fopen("/dev/null", "r");
		if (rl_outstream == NULL) {
			perror("fopen");
			return EXIT_FAILURE;
		}
	} else {
		using_history();
	}

	do {
		line = readline(NULL);
		if (line == NULL) {
			break;
		}
		parseLine(line);
		if (argc == 1 && strlen(line) > 0) {
			add_history(line);
		}
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
	} else {
		clear_history();
	}

	if (rl_outstream != NULL) {
		fclose(rl_outstream);
		rl_outstream = NULL;
	}

	runtime_reset();

	return EXIT_SUCCESS;
}
