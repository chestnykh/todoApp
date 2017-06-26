#!/bin/bash


libtoolize --force
aclocal --force
automake --force-missing --copy --add-missing
autoconf
./configure --srcdir=./ --bindir=$(pwd)/bin/
