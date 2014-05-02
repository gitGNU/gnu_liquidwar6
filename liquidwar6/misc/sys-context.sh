#!/bin/sh

# examples of typical replaces

sed -i "s/\(SYS_FREE\) (\([^s][^y][^s][^_][^c]\)/\1 (sys_context,\2/g" src/lib/???/???-*.c
sed "s/\(SYS_FREE\) (\([^s][^y][^s][^_][^c]\)/\1 (sys_context,\2/g" lib/sys/sys-print.c
sed "s/\(sys_get_timestamp\) ()/\1 (sys_context)/g" lib/sys/sys-test.c

