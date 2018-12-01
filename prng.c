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

#include "config.h"

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "prng.h"

static unsigned long tcb_seed = 1;

#define A 16807
#define M 2147483647

static int _seed_arc4random(void) {

	return 0;
}

void tcb_randomize(void) {

#if HAVE_CLOCK_GETTIME == 1 || HAVE_GETTIMEOFDAY == 1
	int rc;
#endif

#if HAVE_CLOCK_GETTIME == 1
	struct timespec ts;
#endif

#if HAVE_GETTIMEOFDAY == 1
	struct timeval tv;
#endif

	tcb_srand((unsigned long) time(NULL));

#if HAVE_GETTIMEOFDAY == 1
	rc = gettimeofday(&tv, NULL);
	if (rc == 0) {
		tcb_srand(tv.tv_usec);
	}
#endif

#if HAVE_CLOCK_GETTIME == 1
	rc = clock_gettime(CLOCK_REALTIME, &ts);
	if (rc == 0) {
		tcb_srand(ts.tv_nsec);
	}
#endif

#if HAVE_ARC4RANDOM == 1
	tcb_srand(arc4random());
#endif
}

void tcb_srand(unsigned long seed) {
	tcb_seed = (seed == 0) ? 1 : seed;
}

float tcb_rand(void) {
	tcb_seed = (A * tcb_seed) % M;
	return tcb_seed / (float) M;
}
