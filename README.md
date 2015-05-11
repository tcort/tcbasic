# tcbasic

tcbasic is a small [BASIC](http://en.wikipedia.org/wiki/BASIC) Interpreter
written in [C](http://en.wikipedia.org/wiki/C_%28programming_language%29).

## Current Status

The "basics" are done and working. The interpreter implements the
[Tiny BASIC](http://en.wikipedia.org/wiki/Tiny_BASIC) dialect of BASIC
with added support for floating point numbers. Development will continue
with the goal of implementing successively more complete dialects of BASIC.

## Requirements

The following software is used for developing tcbasic:

* [cc](http://www.gnu.org/software/gcc/)
* [make](http://www.gnu.org/software/make/)
* [sh](http://www.gnu.org/software/bash/)
* [sed](http://www.gnu.org/software/sed/)
* [diff](http://www.gnu.org/software/diffutils/)
* [autoconf](http://gnu.org/software/autoconf)
* [automake](http://gnu.org/software/makeconf)

tcbasic does not depend on any external libraries other than
[libc](http://www.gnu.org/software/libc/)

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

#### RUN

Executes the current program.

Example:

    RUN

### Built-in Functions

#### RND

Returns a random number in the range [0,1). The same sequence of numbers is returned in each program run. Use the RANDOMIZE statement to generate different sequences in each program run.

Example:

    10 REM Flip a coin and print HEADS or TAILS
    20 LET C = RND
    30 IF C < 0.5 THEN PRINT "HEADS"
    40 IF C >= 0.5 THEN PRINT "TAILS"

## License

    tcbasic - a small BASIC Interpreter written in C.
    Copyright (C) 2015  Thomas Cort <linuxgeek@gmail.com>
    
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

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
