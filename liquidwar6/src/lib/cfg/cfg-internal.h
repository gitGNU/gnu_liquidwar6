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
  char **argv;
  char *config_file;
  lw6sys_hash_t *options;
}
_lw6cfg_context_t;

typedef struct _lw6cfg_xml_callback_s
{
  void (*func) (void *data, char *element, char *key, char *value);
  void *data;
}
_lw6cfg_xml_callback_t;

/*
 * In commandline.c
 */
extern int _lw6cfg_parse_command_line (_lw6cfg_context_t * cfg_context);

// cfg-env.c
extern int _lw6cfg_merge_env (_lw6cfg_context_t * cfg_context);

/* cfg-load.c */
extern int _lw6cfg_load (_lw6cfg_context_t * cfg_context, char *filename);

/*
 * In option.c
 */
extern int _lw6cfg_option_exists (_lw6cfg_context_t * cfg_context, char *key);
extern char *_lw6cfg_get_option (_lw6cfg_context_t * cfg_context, char *key);
extern void _lw6cfg_set_option (_lw6cfg_context_t * cfg_context, char *key,
				char *value);
extern int _lw6cfg_get_option_int (_lw6cfg_context_t * cfg_context,
				   char *key);
extern void _lw6cfg_set_option_int (_lw6cfg_context_t * cfg_context,
				    char *key, int value);
extern int _lw6cfg_get_option_bool (_lw6cfg_context_t * cfg_context,
				    char *key);
extern void _lw6cfg_set_option_bool (_lw6cfg_context_t * cfg_context,
				     char *key, int value);

/* cfg-save.c */
extern int _lw6cfg_save (_lw6cfg_context_t * cfg_context, char *filename);

/* cfg-setup.c */
extern _lw6cfg_context_t *_lw6cfg_init (int argc, char *argv[]);
extern void _lw6cfg_quit (_lw6cfg_context_t * cfg_context);

#endif
