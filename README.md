# tcbasic

tcbasic is a small [BASIC](http://en.wikipedia.org/wiki/BASIC) Interpreter
written in [C](http://en.wikipedia.org/wiki/C_%28programming_language%29).

## Current Status

The "basics" are done and working. The interpreter implements the
[Tiny BASIC](http://en.wikipedia.org/wiki/Tiny_BASIC) dialect of BASIC
with added support for floating point numbers and many of the built-in mathematical
functions in [Dartmouth BASIC](http://en.wikipedia.org/wiki/Dartmouth_BASIC).
Development will continue with the goal of implementing successively more
complete dialects of BASIC.

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [cmake](https://cmake.org/)

## Building

Standard cmake build (`make test` runs the test suite):

    $ cd bin
    $ cmake ..
    $ make
    $ make test
    # make install

## Using

Start the interpreter in interactive mode:

    $ tcbasic

Execute a program from a file in batch mode:

    $ tcbasic sample.bas

## Example Program

    10 REM Square Root Calculator using Newton's Method
    
    100 LET X = 1337
    400 GOSUB 1000
    500 PRINT "The square root of ",X," is approximately ",Y
    600 END
    
    1000 REM Calculate the square root of a given Number
    1001 REM Expects input to be in X. Output will be in Y.
    1002 REM Uses Z as a temporary variable.
    1050 LET Y = 0.5 * X
    1100 LET Z = Y
    1200 LET Y = Y-(((Y*Y)-X)/(2*Y))
    1300 IF Z <> Y THEN GOTO 1100
    1400 RETURN

## Language Reference

See `LANG.md`

## License

See `COPYING` for the full license. Here's a summary:

    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017, 2018, 2020  Thomas Cort <linuxgeek@gmail.com>
    
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

