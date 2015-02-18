# basic

basic is a small BASIC Interpreter written in C.

## Current Status

Under active development.

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [readline](http://www.gnu.org/software/readline/)
* [flex](http://www.gnu.org/software/flex/)
* [bison](http://www.gnu.org/software/bison/)

## Building

Standard autotools build:

    $ autoreconf -i
    $ ./configure
    $ make
    # make install

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
