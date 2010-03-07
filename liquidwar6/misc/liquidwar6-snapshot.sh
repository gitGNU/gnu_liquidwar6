#!/bin/sh

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009  Christian Mauduit <ufoot@ufoot.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
# Contact author        : ufoot@ufoot.org

# This is the script used to generate snapshot packages

. /etc/profile > /dev/null 2>&1

LW6_SNAPSHOT_ROOT="/var/opt/snapshots/liquidwar6/"
LW6_BIN=${LW6_SNAPSHOT_ROOT}/bin
LW6_LOG=${LW6_SNAPSHOT_ROOT}/log
LW6_PUB=${LW6_SNAPSHOT_ROOT}/pub
LW6_TMP=${LW6_SNAPSHOT_ROOT}/tmp
LW6_BRANCH="liquidwar6--beta"
LW6_BRANCH_EXTRA_MAPS="liquidwar6--extra-maps"
LW6_SNAPSHOT_VERSION=`date +%Y%m%d`"snapshot"
LW6_LOG_FILE=$LW6_LOG/liquidwar6-${LW6_SNAPSHOT_VERSION}
LW6_CTIME=31
LW6_OK=0
LW6_OK_EXTRA_MAPS=0

date > ${LW6_LOG_FILE}.date.txt

install -d $LW6_LOG && cd $LW6_LOG || exit 1
install -d $LW6_PUB || exit 1
install -d $LW6_TMP || exit 1
find $LW6_TMP -type f -exec chmod a+rw "{}" \;
find $LW6_TMP -type d -exec chmod a+rwx "{}" \;
rm -rf $LW6_TMP && mkdir $LW6_TMP && cd $LW6_TMP || exit 1

#tla register-archive "http://arch.savannah.gnu.org/archives/liquidwar6" > /dev/null 2>&1
#tla get --archive "liquidwar6@sv.gnu.org" $LW6_BRANCH > ${LW6_LOG_FILE}.get.log.txt 2>&1
#
#cd liquidwar6--beta* || exit 1
git clone git://git.sv.gnu.org/liquidwar6.git > ${LW6_LOG_FILE}.clone.log.txt 2>&1

cd liquidwar6/liquidwar6 || exit 1

sed -i -e "s/\(^AC_INIT.*War 6\]\,\[\)\(.*\)\(\]\,\[ufoot.*\)/\1${LW6_SNAPSHOT_VERSION}\3/g" configure.ac > ${LW6_LOG_FILE}.sed.log.txt 2>&1
rm -f configure
autoreconf > ${LW6_LOG_FILE}.autoreconf.log.txt 2>&1
#automake > ${LW6_LOG_FILE}.automake.log.txt 2>&1
#autoconf > ${LW6_LOG_FILE}.autoconf.log.txt 2>&1
./configure > ${LW6_LOG_FILE}.configure.log.txt 2>&1
make > ${LW6_LOG_FILE}.make.log.txt 2>&1
#make check > ${LW6_LOG_FILE}.check.log.txt 2>&1
#make dist > ${LW6_LOG_FILE}.dist.log.txt 2>&1
#rm -f liquidwar6-${LW6_SNAPSHOT_VERSION}.tar.gz
make distcheck > ${LW6_LOG_FILE}.distcheck.log.txt 2>&1
make -C doc html > ${LW6_LOG_FILE}.doc.log.txt 2>&1
if test -f liquidwar6-${LW6_SNAPSHOT_VERSION}.tar.gz && test -d doc/liquidwar6.html ; then
    install -d ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}
    mv liquidwar6-${LW6_SNAPSHOT_VERSION}.tar.gz ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/
    cp ChangeLog ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/ChangeLog.txt
    install -d ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/doc
    mv doc/liquidwar6.html/*.html $LW6_PUB/${LW6_SNAPSHOT_VERSION}/doc/
    if test -f ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/liquidwar6-${LW6_SNAPSHOT_VERSION}.tar.gz && test -d ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/doc ; then
	for i in ${LW6_PUB}/???* ; do
	    if test -f "$i"; then
		rm -f "$i"
	    fi
	    if test -d "$i"; then
		find "$i" -ctime +${LW6_CTIME} -exec rm -rf "{}" \;
	    fi
	done
        LW6_OK=1
    fi
fi

cd ..
cd ..

#tla get --archive "liquidwar6@sv.gnu.org" $LW6_BRANCH_EXTRA_MAPS > ${LW6_LOG_FILE}.get-extra-maps.log.txt 2>&1
cd liquidwar6/liquidwar6-extra-maps || exit 1

sed -i -e "s/\(^AC_INIT.*War 6 extra maps\]\,\[\)\(.*\)\(\]\,\[ufoot.*\)/\1${LW6_SNAPSHOT_VERSION}\3/g" configure.ac > ${LW6_LOG_FILE}.sed.log.txt 2>&1
rm -f configure
autoconf > ${LW6_LOG_FILE}.autoconf-extra-maps.log.txt 2>&1
./configure > ${LW6_LOG_FILE}.configure-extra-maps.log.txt 2>&1
make > ${LW6_LOG_FILE}.make-extra-maps.log.txt 2>&1
make dist > ${LW6_LOG_FILE}.dist-extra-maps.log.txt 2>&1
make zip > ${LW6_LOG_FILE}.zip-extra-maps.log.txt 2>&1
if test -f liquidwar6-extra-maps-${LW6_SNAPSHOT_VERSION}.zip ; then
    install -d ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}
    mv liquidwar6-extra-maps-${LW6_SNAPSHOT_VERSION}.zip ${LW6_PUB}/${LW6_SNAPSHOT_VERSION}/
    LW6_OK_EXTRA_MAPS=1
fi
   
cd ..
cd ..

if test x${LW6_OK} = x1 && test x${LW6_OK_EXTRA_MAPS} = x1 ; then
    exit 0
else
    exit 1
fi

