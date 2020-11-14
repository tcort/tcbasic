Enhancements:

* PRINT separators (';' vs ','), print zone, etc

Implement additional statements:

* FOR/NEXT
* READ/DATA/RESTORE
* ON .. GOTO
* DEF
* DIM

Implement additional operators:

* ^ operator (x^y means x to the power of y) -- remember to get order of operations right.
* Document all operators (+,-,*,/,MOD,etc)

Implement language fundamentals:

* Strings
* Arrays (test with quicksort)

Implement language standards / defacto standards:

* Dartmouth BASIC
* ECMA-55 Minimal BASIC
* ECMA-116 Level 1 and 2

Internals:

* Better representation of lines.

Testing:

* write more tests
* add a preflight test to run cppcheck, valgrind, etc.

Other changes:

* Better error handling / reporting.

Report and Fix Issues:

* https://github.com/tcort/tcbasic/issues