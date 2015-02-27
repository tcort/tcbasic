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


static unsigned long tcb_seed = 1;

#define A 16807
#define M 2147483647

void tcb_srand(unsigned long seed) {
	tcb_seed = (seed == 0) ? 1 : seed;
}

float tcb_rand(void) {
	tcb_seed = (A * tcb_seed) % M;
	return tcb_seed / (float) M;
}
