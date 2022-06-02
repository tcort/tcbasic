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

AC_PREREQ(2.59)
AC_INIT([tcbasic], [3.0.0], [tcedit@tcort.dev])
AC_DEFINE([PACKAGE_BUILD], [0], [build number])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])

AC_LANG([C])
AC_PROG_CC

AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])
AC_CHECK_FUNCS([close exit free malloc open read write])
AC_CHECK_DECLS([O_RDONLY], [], [], [[#include <fcntl.h>]])


AC_CONFIG_HEADERS([config.h:config.in])

AC_CONFIG_FILES([ Makefile src/Makefile tests/Makefile ])

AC_OUTPUT
