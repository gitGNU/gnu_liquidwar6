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

#ifndef LIQUIDWAR6CFG_INTERNAL_H
#define LIQUIDWAR6CFG_INTERNAL_H

#include "../sys/sys.h"

typedef struct _lw6cfg_option_s
{
  int from_command_line;
  char *key;
  char *value;
}
_lw6cfg_option_t;

typedef struct _lw6cfg_context_s
{
  int argc;
  const char **argv;
  char *config_file;
  lw6sys_hash_t *options;
  lw6sys_spinlock_t *spinlock;
}
_lw6cfg_context_t;

typedef struct _lw6cfg_xml_callback_info_s
{
  lw6sys_context_t *sys_context;
  lw6cfg_read_xml_callback_func_t func;
  void *data;
}
_lw6cfg_xml_callback_info_t;

/* cfg-commandline.c */
extern int _lw6cfg_parse_command_line (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context);

/* cfg-env.c */
extern int _lw6cfg_merge_env (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context);

/* cfg-load.c */
extern int _lw6cfg_load (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *filename);

/* cfg-option.c */
extern int _lw6cfg_option_exists (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key);
extern char *_lw6cfg_get_option (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key);
extern void _lw6cfg_set_option (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, const char *value);
extern int _lw6cfg_get_option_int (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key);
extern void _lw6cfg_set_option_int (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, int value);
extern int _lw6cfg_get_option_bool (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key);
extern void _lw6cfg_set_option_bool (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *key, int value);

/* cfg-save.c */
extern int _lw6cfg_save (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *filename);

/* cfg-setup.c */
extern _lw6cfg_context_t *_lw6cfg_init (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern void _lw6cfg_quit (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context);

#endif
