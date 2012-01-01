#!/bin/sh

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>
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
# Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
# Contact author        : ufoot@ufoot.org

# Script used to generate /local dependencies for msys-for-liquidwar6
# Not bullet proof, just a convenience script to waiting for hours
# between two "./configure && make && make install" and also keep
# an accurate, up-to-date track of compilation options. Logs should
# be found in "/local/src/msys-for-liquidwar6-build.log".

# usage:
# ./msys-for-liquidwar6-build.sh > msys-for-liquidwar6-build.log 2>&1

header() {
	echo "msys-for-liquidwar6: $1"
}

header pthreads-w32
cd pthreads-w32-2-8-0-release && make clean GC && cp pthread.h sched.h /usr/local/include/ && cp pthreadGC2.dll /usr/local/bin/ && cp libpthreadGC2.a /usr/local/lib/ && cd ..

header gmp
cd gmp-4.2.2 && ./configure && make && make install && cd ..

header guile
cd guile-1.8.5 && ./configure --disable-nls --disable-rpath --disable-error-on-warning --without-threads && make && make install && cd ..

header expat
cd expat-2.0.1 && ./configure && make && make install && cd ..

header sqlite
cd sqlite-amalgamation-3.5.9 && ./configure && make && make install && cd ..

header libpng
cd libpng-1.2.29 && ./configure && make && make install && cd ..

header libjpeg
cd jpeg-6b && ./configure && make && make install && make install-lib && cd ..

header curl
cd curl-7.18.1 && ./configure --without-ssl && make && make install && cd ..

header freetype
cd freetype-2.3.5 && ./configure && make && make install && cd ..

header libogg
cd libogg-1.1.3 && ./configure && make && make install && cd ..

header libvorbis
LDFLAGS="$LDFLAGS -logg" && cd libvorbis-1.2.0 && ./configure && make && make install && cd ..

header SDL
cd SDL-1.2.13 && ./configure && make && make install && cd ..

header SDL_image
cd SDL_image-1.2.6 && ./configure && make && make install && cd ..

header SDL_mixer
cd SDL_mixer-1.2.8 && ./configure && make && make install && cd ..

header SDL_ttf
cd SDL_ttf-2.0.9 && ./configure && make && make install && cd ..

