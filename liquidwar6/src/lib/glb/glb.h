/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
  Contact author        : ufoot@ufoot.org
*/

#ifndef LIQUIDWAR6GLB_H
#define LIQUIDWAR6GLB_H

#include "../sys/sys.h"

/* glb-base64.c */
extern char *lw6glb_base64_encode_bin (char *buf, int size);
extern char *lw6glb_base64_decode_bin (int *size, char *base64_str);
extern char *lw6glb_base64_encode_str (char *str);
extern char *lw6glb_base64_decode_str (char *str);
extern char *lw6glb_base64_encode_bin_prefix (char *buf, int size,
					      char *prefix);
extern char *lw6glb_base64_decode_bin_prefix (int *size, char *base64_str,
					      char *prefix);
extern char *lw6glb_base64_encode_str_prefix (char *str, char *prefix);
extern char *lw6glb_base64_decode_str_prefix (char *str, char *prefix);

extern char *lw6glb_base64_encode (char *str);
extern char *lw6glb_base64_decode (char *str);

/* glb-test.c */
extern int lw6glb_test (int mode);

#endif
