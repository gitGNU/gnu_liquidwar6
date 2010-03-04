#!/bin/sh

find . -type f | xargs grep "#include" | grep "<" | cut -d "#" -f 2 | cut -d " " -f 2 | sort -u | sed "s/[<>]//g"

