#!/bin/sh

gtkdocize
aclocal
libtoolize -c -f --automake
autoheader 
autoconf
automake --add-missing
automake

