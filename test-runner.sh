#!/bin/sh
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

IN=${TCB_IN_DIR}/${1}.bas
OUT=${TCB_OUT_DIR}/${1}.out
EX=${TCB_EX_DIR}/${1}.ex

if test "x${DIFF}" = "x"; then
	# no diff program, skipping test
	exit 77
fi

${TCB} ${IN} | ${SED} -e 's/\r$//' > ${OUT}
${DIFF} -u ${EX} ${OUT}

RESULT=$?
if [ $RESULT -gt 0 ]
then
	exit 1
else
	exit 0
fi
