# tcbasic - a small BASIC Interpreter written in C.
# Copyright (C) 2015, 2016, 2017, 2018  Thomas Cort <linuxgeek@gmail.com>
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
	abs.h		abs.c \
	addop.h		addop.c \
	atn.h		atn.c \
	buffer.h	buffer.c \
	command.h	command.c \
	cos.h		cos.c \
	cot.h		cot.c \
			driver.c \
	exp.h		exp.c \
	expr_item.h	expr_item.c \
	expr_list.h	expr_list.c \
	expression.h	expression.c \
	eval.h		eval.c \
	factor.h	factor.c \
	int_.h		int_.c \
	line.h		line.c \
	log.h		log.c \
	mulop.h		mulop.c \
	number.h	number.c \
	prng.h		prng.c \
	readaline.h	readaline.c \
	rem.h		rem.c \
	relop.h		relop.c \
	rnd.h		rnd.c \
	runtime.h	runtime.c \
	sgn.h		sgn.c \
	sin.h		sin.c \
	statement.h	statement.c \
	str.h		str.c \
	sqr.h		sqr.c \
	tan.h		tan.c \
	term.h		term.c \
	time_.h		time_.c \
	tokenizer.h	tokenizer.c \
	var.h		var.c \
	var_list.h	var_list.c

man_MANS = tcbasic.1

TESTS = \
	abs.sh \
	coinflip.sh \
	cppcheck.sh \
	fib.sh \
	functions.sh \
	hello.sh \
	int_.sh \
	mod.sh \
	newton.sh \
	remainder.sh \
	squares.sh

TESTS_ENVIRONMENT = \
	CPPCHECK=$(CPPCHECK) \
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
	coinflip.out \
	fib.out \
	functions.out \
	hello.out \
	int_.out \
	mod.out \
	newton.out \
	remainder.out \
	squares.out

EXTRA_DIST = \
	$(man_MANS) \
	test-runner.sh	cppcheck.sh \
	README.md 	CONTRIBUTING.md	HACKING.md \
	abs.sh		abs.bas		abs.ex	\
	coinflip.sh	coinflip.bas	coinflip.ex \
	fib.sh		fib.bas		fib.ex	\
	functions.sh	functions.bas	functions.ex \
	hello.sh	hello.bas	hello.ex \
	int_.sh		int_.bas	int_.ex \
	mod.sh		mod.bas		mod.ex \
	newton.sh	newton.bas	newton.ex \
	remainder.sh	remainder.bas	remainder.ex \
	squares.sh	squares.bas	squares.ex

