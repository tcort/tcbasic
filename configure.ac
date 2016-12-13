
dnl    tcbasic - a small BASIC Interpreter written in C.
dnl    Copyright (C) 2015, 2016  Thomas Cort <linuxgeek@gmail.com>
dnl
dnl    This program is free software: you can redistribute it and/or modify
dnl    it under the terms of the GNU General Public License as published by
dnl    the Free Software Foundation, either version 3 of the License, or
dnl    (at your option) any later version.
dnl
dnl    This program is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl    GNU General Public License for more details.
dnl
dnl    You should have received a copy of the GNU General Public License
dnl    along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ(2.59)
AC_INIT([tcbasic], [2.1.0], [linuxgeek@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_SILENT_RULES([yes])

AC_LANG([C])
AC_PROG_CC

AC_HEADER_STDC
AC_CHECK_LIB([m],[cos])
AC_CHECK_HEADERS([getopt.h])
AC_CHECK_FUNCS([getopt_long strsep])

AC_CONFIG_HEADERS([config.h:config.in])

AC_ARG_VAR([DIFF],[the 'diff' program to use for test output comparison])
AC_PATH_PROG([DIFF],[diff],[])
if test "x$DIFF" = "x"; then
	AC_MSG_NOTICE([==> diff command not found!])
	AC_MSG_NOTICE([==> Set DIFF variable if present in non-standard path!])
	AC_MSG_NOTICE([diff is mandatory to run the tests])
fi

AC_ARG_VAR([SED],[the 'sed' program to use for test output massaging])
AC_PATH_PROG([SED],[sed],[])
if test "x$SED" = "x"; then
	AC_MSG_NOTICE([==> sed command not found!])
	AC_MSG_NOTICE([==> Set SED variable if present in non-standard path!])
	AC_MSG_NOTICE([sed is mandatory to run the tests])
fi

AC_ARG_VAR([CPPCHECK],[the 'cppcheck' program is used for some static analysis])
AC_PATH_PROG([CPPCHECK],[cppcheck],[])
if test "x$CPPCHECK" = "x"; then
	AC_MSG_NOTICE([==> cppcheck command not found! See http://cppcheck.sourceforge.net/ ])
	AC_MSG_NOTICE([==> Set CPPCHECK variable if present in non-standard path!])
	AC_MSG_NOTICE([cppcheck static analysis will be skipped.])
fi

AC_CONFIG_FILES([Makefile tcbasic.1])
AC_OUTPUT
