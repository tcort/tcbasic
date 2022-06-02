#  tcbasic - an interpreter for the programming language BASIC
#  Copyright (C) 2022 Thomas Cort
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = str.check

str_check_SOURCES = check.c \
	$(top_srcdir)/src/io.c $(top_srcdir)/src/io.h \
	$(top_srcdir)/src/str.c $(top_srcdir)/src/str.h \
	$(top_srcdir)/src/sys.c $(top_srcdir)/src/sys.h \
	str.check.c

TESTS = $(check_PROGRAMS)
