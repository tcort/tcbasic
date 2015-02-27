# tcbasic - a small BASIC Interpreter written in C.
# Copyright (C) 2015  Thomas Cort <linuxgeek@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS = tcbasic

tcbasic_SOURCES = \
			driver.c \
	buffer.h	buffer.c \
	compat.h	compat.c \
	readaline.h	readaline.c \
	runtime.h	runtime.c \
	addop.h		addop.c \
	command.h	command.c \
	expr_item.h	expr_item.c \
	expr_list.h	expr_list.c \
	expression.h	expression.c \
	eval.h		eval.c \
	factor.h	factor.c \
	line.h		line.c \
	mulop.h		mulop.c \
	number.h	number.c \
	prng.h		prng.c \
	rem.h		rem.c \
	relop.h		relop.c \
	rnd.h		rnd.c \
	statement.h	statement.c \
	str.h		str.c \
	term.h		term.c \
	tokenizer.h	tokenizer.c \
	var.h		var.c \
	var_list.h	var_list.c

man_MANS = tcbasic.1

TESTS = \
	abs.sh \
	fib.sh \
	hello.sh \
	newton.sh \
	remainder.sh

TESTS_ENVIRONMENT = \
	DIFF=$(DIFF)  \
	SED=$(SED)  \
	TCB=$(top_builddir)/tcbasic    \
	TOP_SRCDIR=$(top_srcdir)    \
	TCB_IN_DIR=$(top_srcdir)   \
	TCB_OUT_DIR=$(top_builddir) \
	TCB_EX_DIR=$(top_srcdir)   \
	TEST_RUNNER=$(top_srcdir)/test-runner.sh

CLEANFILES = $(BUILT_SOURCES) \
	abs.out \
	fib.out \
	hello.out \
	newton.out \
	remainder.out

EXTRA_DIST = \
	$(man_MANS) \
	test-runner.sh \
	README.md \
	abs.sh		abs.bas		abs.ex	\
	fib.sh		fib.bas		fib.ex	\
	hello.sh	hello.bas	hello.ex \
	newton.sh	newton.bas	newton.ex \
	remainder.sh	remainder.bas	remainder.ex

