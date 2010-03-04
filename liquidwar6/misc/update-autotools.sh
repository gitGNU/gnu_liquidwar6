#!/bin/sh

echo "Running automake"
automake -a -f -c
echo "Running aclocal"
aclocal -I m4
echo "Running autoconf"
autoconf -f

