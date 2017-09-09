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

`tcbasic` doesn't have any external dependencies. To compile it,
one simply needs a standard C toolchain.

The test suite requires `diff` and `sed`. The test suite optionally
requires `cppcheck`.

## Building

Standard autotools build:

    $ ./configure
    $ make
    $ make check
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

### Statements

#### REM remark

Remark has no effect on the execution of the program and acts as a comment to the developer. The remark can contain any character.

Example:

    10 REM anything after "REM" is considered a comment and ignored by the interpreter.

#### PRINT list

Prints the contents of 'list' to the console where 'list' is a comma separated list of one or more quoted strings, expressions, and/or variables.

Example:

    10 PRINT 2,"+",2,"=",2+2

#### IF expression relop expression THEN statement

Evaluates both 'expression's and compares them using relational operator 'relop'. If the relation is true, then the statement will be executed. Otherwise, the statement is skipped.

Example:

    1000 PRINT "Enter a number:"
    1010 INPUT A
    1020 REM Absolute value of A
    1030 IF A < 0 THEN LET A = -A
    1040 PRINT "The absolute value of the number entered is: ",A

#### GOTO expression

Evaluates 'expression' and jumps to the corresponding line number.

Example:

    10 PRINT "How old are you?"
    20 INPUT A
    30 IF A >= 40 THEN GOTO 60
    40 PRINT "You are too young!"
    50 GOTO 70
    60 PRINT "You are over the hill!"
    70 END

#### INPUT var_list

Given a comma separated list of 1 or more variables, this command will prompt the user to enter a comma separated list of expressions.  The value of each expression will be assigned to each corresponding variable.

Example:

    10 PRINT "Enter 2 numbers to multiply:"
    20 INPUT A,B
    30 PRINT "The product of ",A," and ",B," is ",A*B

#### GOSUB expression

Evaluates 'expression' and jumps to the corresponding line number after saving the current line number.

Example:

    10 PRINT "Enter 2 numbers to multiply:"
    20 INPUT A,B
    30 GOSUB 100
    40 PRINT "The product of ",A," and ",B," is ",C
    50 END

    100 REM Multiply Subroutine
    110 LET C = A*B
    120 RETURN

#### RETURN

Returns control to the line after the last 'GOSUB' call.

Example:

    10 PRINT "Enter 2 numbers to multiply:"
    20 INPUT A,B
    30 GOSUB 100
    40 PRINT "The product of ",A," and ",B," is ",C
    50 END

    100 REM Multiply Subroutine
    110 LET C = A*B
    120 RETURN

#### LET var = expression

Assigns the value of expression to variable 'var' where 'var' is a single character variable name in the range from A to Z.

Example:

    10 LET A = 2
    20 LET B = 2
    30 LET C = A + B
    40 PRINT A,"+",B,"=",C

#### END

Ends the execution of the program.

Example:

    10 PRINT "Hello, World"
    20 END
    30 REM Nothing is executed after the 'END' statement is executed.
    40 PRINT "Nothing to see here"

#### STOP

Exits the interpreter.

Example:

    STOP

#### RANDOMIZE

Re-seeds the random number generator, causing RND to return a different sequence of random numbers.

Example:

    10 REM Flip a coin and print HEADS or TAILS
    15 RANDOMIZE
    20 LET C = RND
    30 IF C < 0.5 THEN PRINT "HEADS"
    40 IF C >= 0.5 THEN PRINT "TAILS"

### Commands

#### CLEAR

Clears the program from memory and sets all variables to 0.

Example:

    CLEAR

#### LIST

Prints the current program.

Example:

    LIST

#### RENUM

Renumber lines in memory.

Example:

    1 REM FOO
    2 REM BAR
    5 REM BAZ
    RENUM
    LIST
    10 REM FOO
    20 REM BAR
    30 REM BAZ

#### RUN

Executes the current program.

Example:

    RUN

#### TROFF

Turn off debug tracing.

    TROFF

#### TRON

Turn on debug tracing. When on, debug tracing prints "TR> " followed by the line that is about to be executed. Use `TROFF` to turn tracing off.

    10 PRINT "HELLO"
    20 PRINT "GOODBYE"
    TRON
    RUN
    TR> 10 PRINT "HELLO"
    HELLO
    TR> 20 PRINT "GOODBYE"
    GOODBYE

### Built-in Functions

#### RND

Returns a random number in the range [0,1). The same sequence of numbers is returned in each program run. Use the RANDOMIZE statement to generate different sequences in each program run.

Example:

    10 REM Flip a coin and print HEADS or TAILS
    20 LET C = RND
    30 IF C < 0.5 THEN PRINT "HEADS"
    40 IF C >= 0.5 THEN PRINT "TAILS"

#### TIME

Returns the number of seconds since midnight.

Example:

    10 PRINT TIME

#### SIN (expression)

Returns the sine of the given expression (measured in radians).

Example:

    110 LET A = SIN((0*3.1415926)/2)

#### COS (expression)

Returns the cosine of the given expression (measured in radians).

Example:

    210 LET A = COS((0*3.1415926)/2)

#### TAN (expression)

Returns the tangent of the given expression (measured in radians).

Example:

    320 LET B = TAN(-3)

#### COT (expression)

Returns the cotangent of the given expression (measured in radians).

Example:

    420 LET B = COT(-0.01)

#### ATN (expression)

Returns the arctangent of the given expression (measured in radians).

Example:

    530 LET C = ATN(1.0)

#### EXP (expression)

Returns `e` to the power of the given expression.

Example:

    610 LET A = EXP(2.0)

#### LOG (expression)

Returns the natural logarithm of of the given expression.

Example:

    720 LET B = LOG(2)

#### ABS (expression)

Returns the absolute value of the given expression.

Example:

    840 LET D = ABS(-2.0)

#### SGN (expression)

Returns the sign of the given expression. `1` if `expression > 0`, `0` if `expression = 0`, `-1` if `expression < 0`.

    40 LET S = SGN(3.14159)

#### SQR (expression)

Returns the square root of the given expression.

Example:

    920 LET B = SQR(2)

## License

    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015, 2016, 2017  Thomas Cort <linuxgeek@gmail.com>
    
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

