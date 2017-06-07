#!/bin/bash


libtoolize --force
aclocal --force
automake --force-missing --add-missing
autoconf
./configure --srcdir=./ --bindir=$(pwd)/bin/
