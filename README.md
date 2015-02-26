# tcbasic

tcbasic is a small [BASIC](http://en.wikipedia.org/wiki/BASIC) Interpreter
written in [C](http://en.wikipedia.org/wiki/C_%28programming_language%29).

## Current Status

The "basics" are done and working. The interpreter implements the
[Tiny BASIC](http://en.wikipedia.org/wiki/Tiny_BASIC) dialect of BASIC.
Development will continue with the goal of implementing successively more
complete dialects of BASIC.

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

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
