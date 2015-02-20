# tcb

tcb is a small BASIC Interpreter written in C.

## Current Status

Under active development.

## Road to v1.0.0

The following components are complete:

* Scanner
* Parser
* Abstract Syntax Tree
* Man page

The following components are in progress:

* Evaluator (`INPUT` command)
* Unit Tests

The following components remain:

* Refactor/Optimize AST
* i18n/l10n

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [readline](http://www.gnu.org/software/readline/) - runtime and buildtime
* [flex](http://www.gnu.org/software/flex/) - buildtime
* [bison](http://www.gnu.org/software/bison/) - buildtime
* [diff](http://www.gnu.org/software/diffutils/) - buildtime (running unit tests)
* [sed](https://www.gnu.org/software/sed/) - buildtime (running unit tests)


## Building

Standard autotools build:

    $ autoreconf -i
    $ ./configure
    $ make
    # make install

## Using

Start the interpreter in interactive mode:

    $ tcb

Execute a program from a file in batch mode:

    $ tcb sample.bas

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
