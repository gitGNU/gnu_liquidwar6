#!/bin/sh
#
# This script has been ripped from the Allegro game library.
# It handles the CR/LF in the text files so that there are
# no conflicts between DOS/Windows and UNIX text readers.

proc_utod()
{
   echo "Converting files from Unix to DOS/Win32..."
   for file in $FILELIST; do
      echo "$file"
      cp $file _tmpfile
      perl -p -i -e "s/([^\r]|^)\n/\1\r\n/" _tmpfile
      touch -r $file _tmpfile
      mv _tmpfile $file
   done
}

FILELIST=$*

proc_utod

