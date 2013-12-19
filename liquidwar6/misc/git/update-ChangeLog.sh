#!/bin/sh

NEW_CHANGELOG=ChangeLog
OLD_CHANGELOG=misc/git/ChangeLog-before-git.txt
EXTRA_CHANGELOG=$(dirname $(dirname $(readlink -f $NEW_CHANGELOG)))/liquidwar6-extra-maps/ChangeLog

echo "updating ChangeLog"
echo NEW_CHANGELOG=$NEW_CHANGELOG
echo OLD_CHANGELOG=$OLD_CHANGELOG
echo EXTRA_CHANGELOG=$EXTRA_CHANGELOG
if which git2cl && git status && test -f $NEW_CHANGELOG && test -f $OLD_CHANGELOG && test -f $EXTRA_CHANGELOG ; then
	git2cl > $NEW_CHANGELOG	
	cat $OLD_CHANGELOG >> $NEW_CHANGELOG
	cp $NEW_CHANGELOG $EXTRA_CHANGELOG
else
	echo "ERROR, doing nothing"
	exit 1
fi

exit 0

