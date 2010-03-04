#!/bin/sh
#
# This script has been ripped from the Allegro game library.
# It handles the CR/LF in the text files so that there are
# no conflicts between DOS/Windows and UNIX text readers.

proc_dtou()
{
   echo "Converting files from DOS/Win32 to Unix..."
   for file in $FILELIST; do
      echo "$file"
      mv $file _tmpfile
      tr -d '\015' < _tmpfile > $file
      touch -r _tmpfile $file
      rm _tmpfile
   done
}

FILELIST=$*

proc_dtou

