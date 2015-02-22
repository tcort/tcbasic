# tcbasic

tcbasic is a small [BASIC](http://en.wikipedia.org/wiki/BASIC) Interpreter
written in C.

## Current Status

The "basics" are done and working. The interpreter implements the
[Tiny BASIC](http://en.wikipedia.org/wiki/Tiny_BASIC) dialect of BASIC.
Development will continue with the goal of implementing successively more
complete dialects of BASIC.

## Requirements

* [C compiler](http://www.gnu.org/software/gcc/) and standard build tools ([make](http://www.gnu.org/software/make/), [sh](http://www.gnu.org/software/bash/), ...).
* [flex](http://www.gnu.org/software/flex/) - buildtime
* [bison](http://www.gnu.org/software/bison/) - buildtime
* [diff](http://www.gnu.org/software/diffutils/) - buildtime (running unit tests)
* [sed](http://www.gnu.org/software/sed/) - buildtime (running unit tests)
* [autoconf](http://gnu.org/software/autoconf) - development time
* [automake](http://gnu.org/software/makeconf) - development time

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

The following program calculates
[Fibonacci numbers](http://en.wikipedia.org/wiki/Fibonacci_number)
and demonstrates all of the available statements. It's included
in the sources as `sample.bas`:

    10 PRINT "How many fibonacci numbers should I calculate?"
    20 INPUT X
    30 IF X <= 2 THEN GOTO 10
    40 PRINT "Calculating the first ",X," fibonacci numbers: "
    50 PRINT "fib[0] = 0"
    60 PRINT "fib[1] = 1"
    70 LET J = 1

    100 LET J = J + 1
    110 LET A = 0
    120 LET B = 1
    130 LET I = 1
    140 GOSUB 1000
    150 PRINT "fib[",J,"] = ",C
    160 IF J < X-1 THEN GOTO 100
    170 END
    
    1000 LET C = B + A
    1010 LET T = B
    1020 LET B = C
    1030 LET A = T
    1040 LET I = I + 1
    1050 IF I = J THEN RETURN
    1060 GOTO 1000

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
