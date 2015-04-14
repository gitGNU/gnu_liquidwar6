# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
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

pkgdata_DATA = liquidwar6.desktop
pkgdatadir = $(datadir)/applications
EXTRA_DIST =  $(pkgdata_DATA) liquidwar6.spec liquidwar6d 406FFCAB.gnupg.txt FD409E94.gnupg.txt

distclean-local:
	rm -f @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -f liquidwar6.exe

clean-local:
	rm -f @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip
	rm -f @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows-installer.exe
	rm -f @PACKAGE_TARNAME@-@PACKAGE_VERSION@-mac-os-x.dmg
	rm -f *vendor*

# Rules below concern packaging, do not use them on untrusted
# computers, as they use /tmp and assume they are the only scripts
# doing things in /tmp/liquidwar6. Use at your own risk.

TMP=/tmp/@PACKAGE_TARNAME@
PACKAGE_TMP=/tmp/@PACKAGE_TARNAME@/@PACKAGE_TARNAME@-@PACKAGE_VERSION@
PACKAGE_TMP_EXTRA=/tmp/@PACKAGE_TARNAME@/liquidwar6-extra-maps-@PACKAGE_VERSION@
DLL_ZIP=/storage/1-platinum/download/liquidwar/v6/mingw32/dll-for-liquidwar6-20100823.zip

targz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz

@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz:
	echo "Run make dist in parent directory and/or copy @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz here."

liquidwar6.exe:
	echo "Copy src/.libs/liquidwar6.exe here" && false

zip: ms-windows-zip

ms-windows-zip: @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip

@PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip: liquidwar6.exe @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz
	rm -rf ${TMP}
	install -d ${TMP}
	cp @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz ${TMP}
	cp liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz ${TMP}
	cd ${TMP} && tar xzf @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	cd ${TMP} && tar xzf liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz
	mv ${PACKAGE_TMP}/NEWS ${PACKAGE_TMP}/news.txt
	mv ${PACKAGE_TMP}/ChangeLog ${PACKAGE_TMP}/changes.txt
	mv ${PACKAGE_TMP}/COPYING ${PACKAGE_TMP}/license.txt
	mv ${PACKAGE_TMP}/README ${PACKAGE_TMP}/readme.txt
	mv ${PACKAGE_TMP}/data/README ${PACKAGE_TMP}/data/readme.txt
	mv ${PACKAGE_TMP}/map/README ${PACKAGE_TMP}/map/readme.txt
	mv ${PACKAGE_TMP}/music/README ${PACKAGE_TMP}/music/readme.txt
	mv ${PACKAGE_TMP}/script/README ${PACKAGE_TMP}/script/readme.txt
	rm -rf ${PACKAGE_TMP}-2
	mv ${PACKAGE_TMP} ${PACKAGE_TMP}-2
	install -d ${PACKAGE_TMP}
	mv ${PACKAGE_TMP}-2/data ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/music ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/map ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/script ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/news.txt ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/changes.txt ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/license.txt ${PACKAGE_TMP}/
	mv ${PACKAGE_TMP}-2/readme.txt ${PACKAGE_TMP}/
	rm -rf ${PACKAGE_TMP}-2
	mv ${PACKAGE_TMP_EXTRA}/map ${PACKAGE_TMP}/map/extra
	for i in `find ${PACKAGE_TMP}/ -name README`; do j=`echo $$i | sed s/README/readme.txt/g`; mv $$i $$j ; done
	find ${PACKAGE_TMP} -name "Makefile*" | xargs rm -f
	install -d ${PACKAGE_TMP}/doc
	$(MAKE) -C ../doc liquidwar6.html liquidwar6.pdf
	cp -r ../doc/liquidwar6.html ${PACKAGE_TMP}/doc/html
	${top_srcdir}/src/utod.sh `find ${PACKAGE_TMP} -name "*.txt"`
	${top_srcdir}/src/utod.sh `find ${PACKAGE_TMP} -name "*.html"`
	cp ../doc/liquidwar6.pdf ${PACKAGE_TMP}/doc/
	cp liquidwar6.exe ${PACKAGE_TMP}
	cd ${PACKAGE_TMP} && unzip ${DLL_ZIP}
	cd ${TMP} && zip -r @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip @PACKAGE_TARNAME@-@PACKAGE_VERSION@
	mv ${TMP}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip .
	rm -rf ${TMP}

installer: ms-windows-installer

ms-windows-installer: @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows-installer.exe

@PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows-installer.exe: @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip liquidwar6.nsi
	rm -rf ${TMP}
	install -d ${TMP}
	cp @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip ${TMP}
	cd ${TMP} && unzip @PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows.zip
	cp liquidwar6.nsi ${PACKAGE_TMP}
	cd ${PACKAGE_TMP} && makensis liquidwar6.nsi
	cp ${PACKAGE_TMP}/@PACKAGE_TARNAME@-@PACKAGE_VERSION@-ms-windows-installer.exe .
	rm -rf ${TMP}

deb: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz ${top_srcdir}/debian/changelog
	rm -rf ${TMP}
	install -d ${TMP}
	cp @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz ${TMP}
	cd ${TMP} && tar xzf @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	cd ${PACKAGE_TMP} && ./configure
	cd ${PACKAGE_TMP} && case $$(whoami) in ufoot) dpkg-buildpackage -kFD409E94 -rfakeroot ;; jenkins) dpkg-buildpackage -k406FFCAB -rfakeroot ;; *) dpkg-buildpackage -rfakeroot ;; esac || true
	cp ${TMP}/@PACKAGE_TARNAME@*@PACKAGE_VERSION@*vendor* .
	rm -rf ${TMP}

rpm: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz liquidwar6.spec
	rm -rf ${TMP}
	install -d ${TMP}
	cp @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz ${TMP}
	cd ${TMP} && fakeroot rpmbuild -ta --sign @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	cp $$HOME/rpmbuild/SRPMS/@PACKAGE_TARNAME@*@PACKAGE_VERSION@*vendor*.src.rpm .
	cp $$HOME/rpmbuild/RPMS/*/@PACKAGE_TARNAME@*@PACKAGE_VERSION@*vendor*.rpm .
	rm -rf ${TMP}
	rm -f $$HOME/rpmbuild/SRPMS/@PACKAGE_TARNAME@*@PACKAGE_VERSION@*vendor*.src.rpm
	rm -f $$HOME/rpmbuild/RPMS/*/@PACKAGE_TARNAME@*@PACKAGE_VERSION@*vendor*.rpm

dmg: @PACKAGE_TARNAME@-@PACKAGE_VERSION@-mac-os-x.dmg

# Packaging for Mac OS X isn't that easy. Following links can help:
# http://qin.laya.com/tech_coding_help/dylib_linking.html
# http://doc.qt.nokia.com/qq/qq09-mac-deployment.html

@PACKAGE_TARNAME@-@PACKAGE_VERSION@-mac-os-x.dmg: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz Info.plist
	rm -rf ${TMP}
	install -d ${TMP}
	cp @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz ${TMP}
	cp liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz ${TMP}
	cd ${TMP} && tar xzf @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	cd ${TMP} && tar xzf liquidwar6-extra-maps-@PACKAGE_VERSION@.tar.gz
	install -d "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources"
	cp -r ${PACKAGE_TMP}/data "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp -r ${PACKAGE_TMP}/music "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp -r ${PACKAGE_TMP}/map "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	mv ${PACKAGE_TMP_EXTRA}/map "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/map/extra"
	cp -r ${PACKAGE_TMP}/script "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp ${PACKAGE_TMP}/NEWS "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp ${PACKAGE_TMP}/ChangeLog "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp ${PACKAGE_TMP}/COPYING "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp ${PACKAGE_TMP}/COPYING "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/License.txt"
	cp ${PACKAGE_TMP}/README "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cp ${PACKAGE_TMP}/data/icon/liquidwar6.icns "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	install -d ${TMP}/dll
	cd ${TMP}/dll && unzip ${DLL_ZIP}
	cp -r ${TMP}/dll/guile "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/"
	cd ${PACKAGE_TMP} && ./configure --prefix=/opt && $(MAKE)
	install -d "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS"
	cp ${PACKAGE_TMP}/src/liquidwar6 "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/"
	for i in `otool -L ${PACKAGE_TMP}/src/liquidwar6 | grep dylib | grep -v /usr | grep /opt | cut -d "(" -f 1` ; do j=$${i##*/}; cp $$i "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/"; chmod 644 "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/$$j"; install_name_tool -id @executable_path/$$j "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/$$j"; install_name_tool -change $$i @executable_path/$$j "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/liquidwar6"; done # yes, this is complicated, the idea is to run otool -L to find every library dependency, keep only those in /opt installed by macports or from source, then run install_name_tool to change references to a path relative to the executable path, so that it's findable within the package and does not rely on some developper's local install. One more point: we can't put libs outsite @executable_path (./MacOS) because if path is too long install_name_path won't work... See -headerpad_max_install_names option in LDFLAGS, might help too.
	for i in `otool -L ${PACKAGE_TMP}/src/liquidwar6 | grep dylib | grep -v /usr | grep /opt | cut -d "(" -f 1` ; do j=$${i##*/}; for k in `otool -L "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/$$j" | grep dylib | grep -v /usr | grep /opt | cut -d "(" -f 1` ; do l=$${k##*/}; install_name_tool -change $$k @executable_path/$$l "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/$$j"; done; done # 2nd pass, we track the references inside every .dylib so that dylibs reference each other correctly
	ln -s "libpng12.0.dylib" "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/libpng12.dylib" # needed by SDL_image
	ln -s "libjpeg.62.dylib" "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/MacOS/libjpeg.dylib" # needed by SDL_image
	cp ${PACKAGE_TMP}/pkg/Info.plist "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/"
	install -d "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/doc"
	$(MAKE) -C ../doc liquidwar6.html liquidwar6.pdf
	cp -r ../doc/liquidwar6.html "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/doc/html"
	cp ../doc/liquidwar6.pdf "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.app/Contents/Resources/doc"
	cp ../doc/liquidwar6.pdf "${TMP}/@PACKAGE_STRING@/@PACKAGE_NAME@.pdf"
	find ${TMP} -name "Makefile*" | xargs rm -f
	rm -f $@ && hdiutil create -srcfolder "${TMP}/@PACKAGE_STRING@" $@
	rm -rf ${TMP}
