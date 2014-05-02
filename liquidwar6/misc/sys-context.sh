#!/bin/sh

# examples of typical replaces

exit 1

sed -i "s/\(SYS_FREE\) (\([^s][^y][^s][^_][^c]\)/\1 (sys_context,\2/g" src/lib/???/???-*.c
sed "s/\(SYS_FREE\) (\([^s][^y][^s][^_][^c]\)/\1 (sys_context,\2/g" lib/sys/sys-print.c
sed "s/\(sys_get_timestamp\) ()/\1 (sys_context)/g" lib/sys/sys-test.c
cat src/lib/map/map.h  | grep extern | cut -d " " -f 3 | sed s/*//g | grep lw6map | sort > map.list

