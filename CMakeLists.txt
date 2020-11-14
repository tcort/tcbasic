# tcbasic - a small BASIC Interpreter written in C.
# Copyright (C) 2015, 2016, 2017, 2018, 2020  Thomas Cort <linuxgeek@gmail.com>
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

cmake_minimum_required(VERSION 3.10)
project(tcbasic VERSION 0.0.0 LANGUAGES C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -Wpedantic")

configure_file (
    "${PROJECT_SOURCE_DIR}/include/config.h.in"
    "${PROJECT_BINARY_DIR}/include/config.h"
)

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}/include"
)

configure_file (
    "${PROJECT_SOURCE_DIR}/man/tcbasic.1.in"
    "${PROJECT_BINARY_DIR}/man/tcbasic.1"
)

file(GLOB SOURCES "src/*.c")
add_executable(tcbasic ${SOURCES})
target_link_libraries(tcbasic m)
install(TARGETS tcbasic DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/man/tcbasic.1 DESTINATION man/man1)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/examples DESTINATION share/${PROJECT_NAME})

include(CTest)
enable_testing()

set (functional_tests abs coinflip division fib functions hello int_ mod newton pemdas remainder shell squares)
foreach(test ${functional_tests})
    add_test(NAME ${test}_exec COMMAND sh -c "${PROJECT_BINARY_DIR}/${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/tests/${test}.bas > ${PROJECT_BINARY_DIR}/tests_${test}.out")
    add_test(NAME ${test}_check COMMAND ${CMAKE_COMMAND} -E compare_files ${PROJECT_SOURCE_DIR}/tests/${test}.ex ${PROJECT_BINARY_DIR}/tests_${test}.out)
endforeach()
