/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

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
extern char *lw6glb_base64_encode_bin (lw6sys_context_t * sys_context, const char *buf, int size);
extern char *lw6glb_base64_decode_bin (lw6sys_context_t * sys_context, int *size, const char *base64_str);
extern char *lw6glb_base64_encode_str (lw6sys_context_t * sys_context, const char *str);
extern char *lw6glb_base64_decode_str (lw6sys_context_t * sys_context, const char *str);
extern char *lw6glb_base64_encode_bin_prefix (lw6sys_context_t * sys_context, const char *buf, int size, const char *prefix);
extern char *lw6glb_base64_decode_bin_prefix (lw6sys_context_t * sys_context, int *size, const char *base64_str, const char *prefix);
extern char *lw6glb_base64_encode_str_prefix (lw6sys_context_t * sys_context, const char *str, const char *prefix);
extern char *lw6glb_base64_decode_str_prefix (lw6sys_context_t * sys_context, const char *str, const char *prefix);

extern char *lw6glb_base64_encode (lw6sys_context_t * sys_context, const char *str);
extern char *lw6glb_base64_decode (lw6sys_context_t * sys_context, const char *str);

/* glb-sha1.c */
char *lw6glb_sha1_hmac_80_bin (lw6sys_context_t * sys_context, const char *key, int key_size, const char *buf, int buf_size);
char *lw6glb_sha1_hmac_80_str (lw6sys_context_t * sys_context, const char *key, const char *str);
u_int32_t lw6glb_sha1_hmac_32_bin (lw6sys_context_t * sys_context, const char *key, int key_size, const char *buf, int buf_size);
u_int32_t lw6glb_sha1_hmac_32_str (lw6sys_context_t * sys_context, const char *key, const char *str);

/* glb-test.c */
extern int lw6glb_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6glb_test_run (lw6sys_context_t * sys_context, int mode);

#endif
