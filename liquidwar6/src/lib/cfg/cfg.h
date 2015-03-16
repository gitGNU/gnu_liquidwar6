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

#ifndef LIQUIDWAR6CFG_H
#define LIQUIDWAR6CFG_H

#include "../sys/sys.h"
#include "../hlp/hlp.h"

#define LW6CFG_XML_KEY "key"
#define LW6CFG_XML_VALUE "value"

#define LW6CFG_XML_STRING "string"
#define LW6CFG_XML_INT "int"
#define LW6CFG_XML_BOOL "bool"
#define LW6CFG_XML_FLOAT "float"
#define LW6CFG_XML_COLOR "color"

#define LW6CFG_XML_TRUE "true"
#define LW6CFG_XML_FALSE "false"

typedef void (*lw6cfg_read_xml_callback_func_t) (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value);

/* cfg-arg.c */
extern int lw6cfg_parse_command_line (lw6sys_context_t * sys_context, void *cfg_context);

/* cfg-defaults.c */
extern int lw6cfg_defaults (lw6sys_context_t * sys_context, void *cfg_context);

/* cfg-env.c */
extern int lw6cfg_merge_env (lw6sys_context_t * sys_context, void *cfg_context);

/* cfg-exp.c */
extern int lw6cfg_load_exp (lw6sys_context_t * sys_context, const char *user_dir, int *exp);
extern int lw6cfg_save_exp (lw6sys_context_t * sys_context, const char *user_dir, int exp);

/* cfg-format.c */
extern char *lw6cfg_format (lw6sys_context_t * sys_context, const char *key, const char *value, lw6hlp_type_t type);
extern char *lw6cfg_format_guess_type (lw6sys_context_t * sys_context, const char *key, const char *value);

/* cfg-load.c */
extern int lw6cfg_load (lw6sys_context_t * sys_context, void *cfg_context, const char *filename);

/* cfg-option.c */
extern int lw6cfg_option_exists (lw6sys_context_t * sys_context, void *cfg_context, const char *key);
extern char *lw6cfg_get_option (lw6sys_context_t * sys_context, void *cfg_context, const char *key);
extern void lw6cfg_set_option (lw6sys_context_t * sys_context, void *cfg_context, const char *key, const char *value);
extern int lw6cfg_get_option_int (lw6sys_context_t * sys_context, void *cfg_context, const char *key);
extern void lw6cfg_set_option_int (lw6sys_context_t * sys_context, void *cfg_context, const char *key, int value);
extern int lw6cfg_get_option_bool (lw6sys_context_t * sys_context, void *cfg_context, const char *key);
extern void lw6cfg_set_option_bool (lw6sys_context_t * sys_context, void *cfg_context, const char *key, int value);

/* cfg-save.c */
extern int lw6cfg_must_be_saved (lw6sys_context_t * sys_context, const char *option);
extern int lw6cfg_save (lw6sys_context_t * sys_context, void *cfg_context, const char *filename);

/* cfg-setup.c */
extern void *lw6cfg_init (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern void lw6cfg_quit (lw6sys_context_t * sys_context, void *cfg_context);
extern void lw6cfg_reset (lw6sys_context_t * sys_context, int argc, const char *argv[]);

/* cfg-test.c */
extern int lw6cfg_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6cfg_test_run (lw6sys_context_t * sys_context, int mode);

/* cfg-unified.c */
extern char *lw6cfg_unified_get_value (lw6sys_context_t * sys_context, int argc, const char *argv[], char *key);
extern char *lw6cfg_unified_get_user_dir (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern char *lw6cfg_unified_get_log_file (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern char *lw6cfg_unified_get_music_path (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern char *lw6cfg_unified_get_map_path (lw6sys_context_t * sys_context, int argc, const char *argv[]);

/* cfg-xml.c */
extern char *lw6cfg_xml_element (lw6sys_context_t * sys_context, lw6hlp_type_t type);
extern void lw6cfg_read_xml_int (lw6sys_context_t * sys_context, const char *xml_key, const char *xml_value, const char *target_key, int *value);
extern void lw6cfg_read_xml_bool (lw6sys_context_t * sys_context, const char *xml_key, const char *xml_value, const char *target_key, int *value);
extern void lw6cfg_read_xml_float (lw6sys_context_t * sys_context, const char *xml_key, const char *xml_value, const char *target_key, float *value);
extern void lw6cfg_read_xml_string (lw6sys_context_t * sys_context, const char *xml_key, const char *xml_value, const char *target_key, char **value);
extern void lw6cfg_read_xml_color (lw6sys_context_t * sys_context, const char *xml_key, const char *xml_value, const char *target_key,
				   lw6sys_color_8_t * value);
extern int lw6cfg_read_key_value_xml_file (lw6sys_context_t * sys_context, const char *filename, lw6cfg_read_xml_callback_func_t callback_func,
					   void *callback_data);
extern void lw6cfg_write_xml_int (lw6sys_context_t * sys_context, FILE * f, const char *key, int value);
extern void lw6cfg_write_xml_bool (lw6sys_context_t * sys_context, FILE * f, const char *key, int value);
extern void lw6cfg_write_xml_float (lw6sys_context_t * sys_context, FILE * f, const char *key, float value);
extern void lw6cfg_write_xml_string (lw6sys_context_t * sys_context, FILE * f, const char *key, const char *value);
extern void lw6cfg_write_xml_color (lw6sys_context_t * sys_context, FILE * f, const char *key, lw6sys_color_8_t value);
extern void lw6cfg_write_xml_guess_type (lw6sys_context_t * sys_context, FILE * f, const char *key, const char *value);
extern void lw6cfg_write_xml_guess_type_skip_same (lw6sys_context_t * sys_context, FILE * f, const char *key, const char *value);

#endif
