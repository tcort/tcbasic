# Hacking tcbasic

This file contains some notes for the maintainer.

## Release Tasks

The following set of notes covers the final testing and release process.
The idea is to consistently release great software.

### Semi-Automated Testing

- Build with as many compile time checks as possible: `CFLAGS="-Wall -Wpedantic -Wextra -Wpointer-arith -Wcast-qual -Wmissing-prototypes -Wunreachable-code -Wconversion -Waggregate-return -Wwrite-strings -Wstrict-overflow=5 -Wstrict-prototypes" ./configure && make clean && make`
- Build with `scan-build`: `./configure && make clean && scan-build make`
- Check the distribution `make distcheck`: `./configure && make distcheck`
- Run `cppcheck` on the codebase: `cppcheck -I. --enable=all --error-exitcode=1 *.c`
- Submit to coverity.

**TODO** valgrind.

### Manual Testing

Create a release candidate tarball:

    git clone git://github.com/tcort/tcbasic
    cd tcbasic
    ./autogen
    ./configure
    make distcheck

Build, test, and install on as many operating systems as possible:

    tar zvxf /path/to/release-candidate.tar.gz && cd release-candidate
    ./configure && make && make check && sudo make install

### Version Bump

Bump version in `configure.ac`

### Tag

    git tag -a v2.1.0 -m "Version 2.1.0"
    git push && git push --tags

### Build Release Tarball

    git clone git://github.com/tcort/tcbasic
    cd tcbasic
    ./autogen
    ./configure
    make distcheck

### Create release on github

[Draft a new release](https://github.com/tcort/tcbasic/releases/new)

### Upload

[Upload release tarball](https://github.com/tcort/tcbasic/releases)
