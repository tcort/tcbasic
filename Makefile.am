# tcb - a small BASIC Interpreter written in C.
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

BUILT_SOURCES = scanner.c scanner.h parser.c parser.h
AM_YFLAGS = -d

scanner.h: scanner.l
	$(LEX) --header-file=$@ -o /dev/null $(top_srcdir)/scanner.l

bin_PROGRAMS = tcb

tcb_SOURCES = \
			scanner.l \
			parser.y \
			driver.c \
	runtime.h	runtime.c \
	addop.h		addop.c \
	command.h	command.c \
	expr_item.h	expr_item.c \
	expr_list.h	expr_list.c \
	expression.h	expression.c \
	factor.h	factor.c \
	line.h		line.c \
	mulop.h		mulop.c \
	number.h	number.c \
	relop.h		relop.c \
	rnd.h		rnd.c \
	statement.h	statement.c \
	str.h		str.c \
	term.h		term.c \
	var.h		var.c \
	var_list.h	var_list.c

tcb_LDADD = @LEXLIB@

man_MANS = tcb.1

TESTS = \
	./testsuite/abs.sh \
	./testsuite/fib.sh \
	./testsuite/hello.sh \
	./testsuite/remainder.sh

TESTS_ENVIRONMENT = \
	DIFF=$(DIFF)  \
	SED=$(SED)  \
	TCB=$(top_builddir)/tcb    \
	TOP_SRCDIR=$(top_srcdir)    \
	TCB_IN_DIR=$(top_srcdir)/testsuite   \
	TCB_OUT_DIR=$(top_builddir)/testsuite \
	TCB_EX_DIR=$(top_srcdir)/testsuite   \
	TEST_RUNNER=$(top_srcdir)/test-runner.sh

CLEANFILES = $(BUILT_SOURCES) \
	$(top_builddir)/testsuite/abs.out \
	$(top_builddir)/testsuite/fib.out \
	$(top_builddir)/testsuite/hello.out \
	$(top_builddir)/testsuite/remainder.out

EXTRA_DIST = $(man_MANS) $(top_srcdir)/test-runner.sh \
	$(top_srcdir)/testsuite/abs.sh		$(top_srcdir)/testsuite/abs.bas		$(top_srcdir)/testsuite/abs.ex	\
	$(top_srcdir)/testsuite/fib.sh		$(top_srcdir)/testsuite/fib.bas		$(top_srcdir)/testsuite/fib.ex	\
	$(top_srcdir)/testsuite/hello.sh	$(top_srcdir)/testsuite/hello.bas	$(top_srcdir)/testsuite/hello.ex \
	$(top_srcdir)/testsuite/remainder.sh	$(top_srcdir)/testsuite/remainder.bas	$(top_srcdir)/testsuite/remainder.ex

