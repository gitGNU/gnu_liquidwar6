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

#ifndef LIQUIDWAR6SYS_H
#define LIQUIDWAR6SYS_H

#ifdef HAVE_GETTEXT
#include "sys-gettext.h"
#ifndef _
#define _(string) gettext(string)
#endif
#else
#define _(string) string
#endif

#ifndef bindtextdomain
#include <libintl.h>
#endif

#ifndef setlocale
#include <locale.h>
#endif

/*
 * Those are so common it saves time to always include them
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#ifdef LW6_MS_WINDOWS
#ifndef LW6_SKIP_WINDOWS_H
#include <windows.h>
#endif
#endif

/*
 * Redefining basic types mostly for platforms like MinGW which do
 * not have a complete nice GNU libc 8-(
 */
#ifdef LW6_MS_WINDOWS
#ifndef int8_t
typedef signed char int8_t;
#endif
#ifndef int16_t
typedef signed short int16_t;
#endif
#ifndef int32_t
typedef signed int int32_t;
#endif
#ifndef int64_t
typedef signed long long int64_t;
#endif
#ifndef u_int8_t
typedef unsigned char u_int8_t;
#endif
#ifndef u_int16_t
typedef unsigned short u_int16_t;
#endif
#ifndef u_int32_t
typedef unsigned int u_int32_t;
#endif
#ifndef u_int64_t
typedef unsigned long long u_int64_t;
#endif
#endif // MS_WINDOWS

#ifndef char_ptr_t
typedef char *char_ptr_t;
#endif

#define LW6SYS_QUOTE(STR) #STR
#define LW6SYS_TICKS_PER_SEC 1000L

/*
 * Yet another MS-windows hack, printf %ll doesn't work
 * one needs to use a special "%I64" instead...
 */
#ifdef LW6_MS_WINDOWS
#define LW6SYS_PRINTF_LL "I64"
#else
#define LW6SYS_PRINTF_LL "ll"
#endif

#include "../def/def.h"

#ifdef LW6_PARANOID
#define LW6SYS_BAZOOKA_DEFAULT_SIZE 999983
#else
#define LW6SYS_BAZOOKA_DEFAULT_SIZE 99991
#endif

#define LW6SYS_MAIN_BEGIN { setlocale (LC_ALL, ""); bindtextdomain (lw6sys_build_get_package_tarname (), lw6sys_build_get_localedir ()); textdomain (lw6sys_build_get_package_tarname ()); lw6sys_time_init(); lw6sys_history_init(); lw6sys_default_memory_bazooka(); lw6sys_profiler_check(1); }
#define LW6SYS_MAIN_END { ret = lw6sys_check_thread_count () && ret; ret = lw6sys_memory_bazooka_report() && ret; lw6sys_clear_memory_bazooka(); }

#define LW6SYS_TEST_FUNCTION_BEGIN { lw6sys_log(LW6SYS_LOG_NOTICE,_("running tests in function \"%s\""),__FUNCTION__); }
#define LW6SYS_TEST_FUNCTION_END   if (ret) { lw6sys_log(LW6SYS_LOG_NOTICE,_("tests in \"%s\" OK"),__FUNCTION__); } else { lw6sys_log(LW6SYS_LOG_WARNING,_("tests in \"%s\" failed"),__FUNCTION__); }
#define LW6SYS_BACKEND_FUNCTION_BEGIN { lw6sys_log(LW6SYS_LOG_DEBUG,_("begin backend function \"%s\""),__FUNCTION__); }
#define LW6SYS_BACKEND_FUNCTION_END { lw6sys_log(LW6SYS_LOG_DEBUG,_("end backend function \"%s\""),__FUNCTION__); }

#define LW6SYS_SCRIPT_FUNCTION_BEGIN { lw6sys_log(LW6SYS_LOG_DEBUG,_("begin script function \"%s\""),__FUNCTION__); }
#define LW6SYS_SCRIPT_FUNCTION_END { lw6sys_log(LW6SYS_LOG_DEBUG,_("end script function \"%s\""),__FUNCTION__); }

typedef struct lw6sys_xyz_s
{
  /*
   * All-in 32 bit 3D position, to save memory.
   *
   * It's a deliberate choice in Liquid War to handle "limited size"
   * levels. In fact 14 bits still allows 8000x8000 maps, which are
   * at least 100 times too slow to play now (2008). Should we follow
   * Moore's law we'd have at least 6 years until those are playable,
   * and well, until then, let's wait. The point is that storing this
   * information (x*y) on 4 bytes might be very important in some cases,
   * since it can reduce memory footprint on structs which are stored
   * in numerous quantities, and therefore maximize chances that we
   * use level 1 & 2 caches and other nice things which happen when
   * memory consumption is not too high.
   *
   * Point is: why use INT32 and then limit it to 14 bits instead of
   * using an INT16 or short in the first place? Answer: it's easier
   * to handle INT32 all the time in the rest of the code. Compiler
   * and CPU might even handle that better than short. Then, and only
   * when data will be read/written in the struct will it be truncated.
   * Typical example is: we want to multiplicate y by w (which is a
   * width). Result is beyond INT16/short scope but we want to handle
   * it! Casting everything to INT32/int is a pain. With this int y:14
   * trick, we use y as a "full-featured" INT32/int and well, when it
   * will be read/written we'll loose values over 8191, but we simply
   * do not care.
   */
  int32_t x:14;
  int32_t y:14;
  int32_t z:4;
}
lw6sys_xyz_t;

typedef struct lw6sys_whd_s
{
  /*
   * 3 differences with its "XYZ" equivalent:
   * - sometimes w*h*d reads better than x*y*z.
   * - xyz is signed, whd is unsigned
   * - these are real int32 values, it does not really cost any
   *   memory for it's usually used as a single "shape" attribute
   *   for a whole map. At the same time, it's very often used
   *   as a test value in loops, so it's interesting to have
   *   it in a value that's easy to optimize for the compiler
   *   (exactly one register...)
   */
  u_int32_t w;
  u_int32_t h;
  u_int32_t d;
}
lw6sys_whd_t;

typedef struct lw6sys_color_f_s
{
  float r;			// red   [0 ... 1.0f]
  float g;			// green [0 ... 1.0f]
  float b;			// blue  [0 ... 1.0f]
  float a;			// alpha [0 ... 1.0f]
}
lw6sys_color_f_t;

typedef struct lw6sys_color_8_s
{
  u_int8_t r;			// red   [0 ... 255]
  u_int8_t g;			// green [0 ... 255]
  u_int8_t b;			// blue  [0 ... 255]
  u_int8_t a;			// alpha [0 ... 255]
}
lw6sys_color_8_t;

typedef struct lw6sys_color_hsv_s
{
  float h;			// hue        [0 ... 360.0f]
  float s;			// saturation [0 ... 1.0f]
  float v;			// value      [0 ... 1.0f]
  float a;			// alpha      [0 ... 1.0f]
}
lw6sys_color_hsv_t;

typedef struct lw6sys_progress_s
{
  float min;
  float max;
  float *value;
} lw6sys_progress_t;

typedef struct lw6sys_module_pedigree_s
{
  char *id;
  char *category;
  char *name;
  char *readme;
  char *version;
  char *copyright;
  char *license;
  char *date;
  char *time;
} lw6sys_module_pedigree_t;

#define LW6SYS_SIZEOF_INT8 1
#define LW6SYS_SIZEOF_INT16 2
#define LW6SYS_SIZEOF_INT32 4
#define LW6SYS_SIZEOF_INT64 8

#define LW6SYS_SIZEOF_XYZ 4

typedef void (*lw6sys_free_func_t) (void *data);
typedef void (*lw6sys_assoc_callback_func_t) (void *func_data, char *key,
					      void *value);
typedef void (*lw6sys_list_callback_func_t) (void *func_data, void *data);
typedef int (*lw6sys_list_filter_func_t) (void *func_data, void *data);
typedef void *(*lw6sys_dup_func_t) (void *data);
typedef void (*lw6sys_thread_callback_func_t) (void *callback_data);
typedef int (*lw6sys_dir_list_filter_func_t) (void *func_data, char *file);

typedef struct lw6sys_assoc_s
{
  char *key;
  void *value;
  lw6sys_free_func_t free_func;
  void *next_item;		// lw6sys_assoc_t *
}
lw6sys_assoc_t;

typedef struct lw6sys_hash_s
{
  int size;
  lw6sys_assoc_t **entries;
  lw6sys_free_func_t free_func;
}
lw6sys_hash_t;

typedef struct lw6sys_list_s
{
  void *data;
  lw6sys_free_func_t free_func;
  void *next_item;		// lw6sys_list_t *
}
lw6sys_list_t;

typedef int (*lw6sys_sort_callback_func_t) (lw6sys_list_t ** list_a,
					    lw6sys_list_t ** list_b);

typedef struct lw6sys_hexa_serializer_s
{
  char *buf;
  int buf_size;
  int pos;
}
lw6sys_hexa_serializer_t;

extern lw6sys_color_f_t LW6SYS_COLOR_F_WHITE;
extern lw6sys_color_f_t LW6SYS_COLOR_F_BLACK;
extern lw6sys_color_f_t LW6SYS_COLOR_F_RED;
extern lw6sys_color_f_t LW6SYS_COLOR_F_GREEN;
extern lw6sys_color_f_t LW6SYS_COLOR_F_BLUE;
extern lw6sys_color_8_t LW6SYS_COLOR_8_WHITE;
extern lw6sys_color_8_t LW6SYS_COLOR_8_BLACK;
extern lw6sys_color_8_t LW6SYS_COLOR_8_RED;
extern lw6sys_color_8_t LW6SYS_COLOR_8_GREEN;
extern lw6sys_color_8_t LW6SYS_COLOR_8_BLUE;
extern lw6sys_color_hsv_t LW6SYS_COLOR_HSV_WHITE;
extern lw6sys_color_hsv_t LW6SYS_COLOR_HSV_BLACK;
extern lw6sys_color_hsv_t LW6SYS_COLOR_HSV_RED;
extern lw6sys_color_hsv_t LW6SYS_COLOR_HSV_GREEN;
extern lw6sys_color_hsv_t LW6SYS_COLOR_HSV_BLUE;

/*
 * Usefull macros for lw6sys_log, please do not use
 * lw6sys_log directly, but use constructs like
 * lw6sys_log(LW6SYS_LOG_<LEVEL>,"%s%d",str,num);
 */
#define LW6SYS_LOG_ERROR_ID 0
#define LW6SYS_LOG_WARNING_ID 1
#define LW6SYS_LOG_NOTICE_ID 2
#define LW6SYS_LOG_INFO_ID 3
#define LW6SYS_LOG_DEBUG_ID 4
#define LW6SYS_LOG_TMP_ID 5
#define LW6SYS_LOG_ERROR LW6SYS_LOG_ERROR_ID, __FILE__, __LINE__
#define LW6SYS_LOG_WARNING LW6SYS_LOG_WARNING_ID, __FILE__, __LINE__
#define LW6SYS_LOG_NOTICE LW6SYS_LOG_NOTICE_ID, __FILE__, __LINE__
#define LW6SYS_LOG_INFO LW6SYS_LOG_INFO_ID, __FILE__, __LINE__
#define LW6SYS_LOG_DEBUG LW6SYS_LOG_DEBUG_ID, __FILE__, __LINE__
#define LW6SYS_LOG_TMP LW6SYS_LOG_TMP_ID, __FILE__, __LINE__

#define LW6SYS_LOG_DEFAULT_ID LW6SYS_LOG_INFO_ID

/*
 * macro TMP defined for quick-debugging / trace
 */
#define TMP(message) lw6sys_log(LW6SYS_LOG_TMP, message)
#define TMP1(message,arg1) lw6sys_log(LW6SYS_LOG_TMP, message, arg1)
#define TMP2(message,arg1,arg2) lw6sys_log(LW6SYS_LOG_TMP, message, arg1, arg2)
#define TMP3(message,arg1,arg2,arg3) lw6sys_log(LW6SYS_LOG_TMP, message, arg1, arg2, arg3)
#define TMP4(message,arg1,arg2,arg3,arg4) lw6sys_log(LW6SYS_LOG_TMP, message, arg1, arg2, arg3, arg4)
#define TMP5(message,arg1,arg2,arg3,arg4,arg5) lw6sys_log(LW6SYS_LOG_TMP, message, arg1, arg2, arg3, arg4, arg5)

/*
 * Common indenting parameters
 */
#define LW6SYS_REFORMAT_DEFAULT_PREFIX ""
#define LW6SYS_REFORMAT_DEFAULT_NB_COLUMNS 78
#define LW6SYS_REFORMAT_INDENT_PREFIX "        "
#define LW6SYS_REFORMAT_INDENT_NB_COLUMNS 70
#define LW6SYS_REFORMAT_XML_PREFIX "    "
#define LW6SYS_REFORMAT_XML_NB_COLUMNS 74

/* sys-arg.c */
extern int lw6sys_arg_match (char *keyword, char *argv_string);
extern int lw6sys_arg_exists (int argc, char *argv[], char *keyword);
extern char *lw6sys_arg_get_value (int argc, char *argv[], char *keyword);
extern char *lw6sys_arg_get_value_with_env (int argc, char *argv[],
					    char *keyword);
extern int lw6sys_arg_test_mode (int argc, char *argv[]);

/* sys-assoc.c */
extern lw6sys_assoc_t *lw6sys_assoc_new (lw6sys_free_func_t free_func);
extern void lw6sys_assoc_free (lw6sys_assoc_t * assoc);
extern int lw6sys_assoc_has_key (lw6sys_assoc_t * assoc, char *key);
extern void *lw6sys_assoc_get (lw6sys_assoc_t * assoc, char *key);
extern void lw6sys_assoc_set (lw6sys_assoc_t ** assoc, char *key,
			      void *value);
extern void lw6sys_assoc_unset (lw6sys_assoc_t * assoc, char *key);
extern lw6sys_list_t *lw6sys_assoc_keys (lw6sys_assoc_t * assoc);
extern void lw6sys_assoc_map (lw6sys_assoc_t * assoc,
			      lw6sys_assoc_callback_func_t func,
			      void *func_data);
extern void lw6sys_assoc_sort_and_map (lw6sys_assoc_t * assoc,
				       lw6sys_assoc_callback_func_t func,
				       void *func_data);
extern lw6sys_assoc_t *lw6sys_assoc_dup (lw6sys_assoc_t * assoc,
					 lw6sys_dup_func_t dup_func);

/* sys-bazooka.c */
extern int lw6sys_default_memory_bazooka ();
extern void lw6sys_clear_memory_bazooka ();
extern int lw6sys_set_memory_bazooka_size (int size);
extern int lw6sys_get_memory_bazooka_size ();
extern int lw6sys_set_memory_bazooka_eraser (int state);
extern int lw6sys_get_memory_bazooka_eraser ();
extern int lw6sys_get_memory_bazooka_malloc_count ();
extern int lw6sys_get_memory_bazooka_free_count ();
extern int lw6sys_get_memory_bazooka_malloc_max ();
extern int lw6sys_get_memory_bazooka_malloc_count ();
extern int lw6sys_get_memory_bazooka_free_count ();
extern int lw6sys_get_memory_bazooka_malloc_current_count ();
extern int lw6sys_get_memory_bazooka_malloc_max_count ();
extern int lw6sys_get_memory_bazooka_malloc_megabytes ();
extern int lw6sys_get_memory_bazooka_free_megabytes ();
extern int lw6sys_get_memory_bazooka_malloc_current_bytes ();
extern int lw6sys_get_memory_bazooka_malloc_max_bytes ();
extern int lw6sys_memory_bazooka_report ();

/* sys-build.c */
extern char *lw6sys_build_get_package_tarname ();
extern char *lw6sys_build_get_package_name ();
extern char *lw6sys_build_get_package_string ();
extern char *lw6sys_build_get_version ();
extern char *lw6sys_build_get_codename ();
extern char *lw6sys_build_get_stamp ();
extern char *lw6sys_build_get_md5sum ();
extern char *lw6sys_build_get_copyright ();
extern char *lw6sys_build_get_license ();
extern char *lw6sys_build_get_url ();
extern char *lw6sys_build_get_configure_args ();
extern char *lw6sys_build_get_gcc_version ();
extern char *lw6sys_build_get_cflags ();
extern char *lw6sys_build_get_ldflags ();
extern char *lw6sys_build_get_hostname ();
extern char *lw6sys_build_get_date ();
extern char *lw6sys_build_get_time ();
extern char *lw6sys_build_get_host_cpu ();
extern char *lw6sys_build_get_endianness ();
extern int lw6sys_build_get_pointer_size ();
extern int lw6sys_build_is_x86 ();
extern char *lw6sys_build_get_host_os ();
extern int lw6sys_build_is_ms_windows ();
extern int lw6sys_build_is_mac_os_x ();
extern int lw6sys_build_is_gp2x ();
extern char *lw6sys_build_get_top_srcdir ();
extern char *lw6sys_build_get_prefix ();
extern char *lw6sys_build_get_datadir ();
extern char *lw6sys_build_get_libdir ();
extern char *lw6sys_build_get_includedir ();
extern char *lw6sys_build_get_localedir ();
extern char *lw6sys_build_get_docdir ();
extern char *lw6sys_build_get_enable_console ();
extern char *lw6sys_build_get_enable_mod_gl ();
extern char *lw6sys_build_get_enable_mod_csound ();
extern char *lw6sys_build_get_enable_mod_ogg ();
extern char *lw6sys_build_get_enable_mod_http ();
extern char *lw6sys_build_get_enable_optimize ();
extern char *lw6sys_build_get_enable_allinone ();
extern char *lw6sys_build_get_enable_fullstatic ();
extern char *lw6sys_build_get_enable_paranoid ();
extern char *lw6sys_build_get_enable_gprof ();
extern char *lw6sys_build_get_enable_instrument ();
extern char *lw6sys_build_get_enable_profiler ();
extern char *lw6sys_build_get_enable_gcov ();
extern char *lw6sys_build_get_enable_valgrind ();
extern int lw6sys_build_get_bin_id ();
extern void lw6sys_build_log_all ();

/* sys-checksum.c */
extern u_int32_t lw6sys_checksum (unsigned char *data, int len);
extern u_int32_t lw6sys_checksum_str (char *value);
extern u_int32_t lw6sys_checksum_int32 (u_int32_t value);
extern u_int32_t lw6sys_checksum_int64 (u_int64_t value);
extern u_int32_t lw6sys_checksum_whd (lw6sys_whd_t * whd);
extern u_int32_t lw6sys_checksum_xyz (lw6sys_xyz_t * xyz);
extern void lw6sys_checksum_update (u_int32_t * checksum,
				    unsigned char *data, int len);
extern void lw6sys_checksum_update_str (u_int32_t * checksum, char *value);
extern void lw6sys_checksum_update_int32 (u_int32_t * checksum,
					  int32_t value);
extern void lw6sys_checksum_update_int64 (u_int32_t * checksum,
					  int64_t value);
extern void lw6sys_checksum_update_whd (u_int32_t * checksum,
					lw6sys_whd_t * whd);
extern void lw6sys_checksum_update_xyz (u_int32_t * checksum,
					lw6sys_xyz_t * xyz);

/* sys-color.c */
extern u_int8_t lw6sys_color_float2char (float f);
extern float lw6sys_color_char2float (u_int8_t i);
extern lw6sys_color_8_t lw6sys_color_f_to_8 (lw6sys_color_f_t * color_f);
extern void lw6sys_color_8_to_f (lw6sys_color_f_t *
				 color_f, lw6sys_color_8_t color_8);
extern u_int32_t lw6sys_color_f_to_i (lw6sys_color_f_t * color_f);
extern u_int32_t lw6sys_color_8_to_i (lw6sys_color_8_t color_8);
extern void lw6sys_color_i_to_f (lw6sys_color_f_t * color_f,
				 u_int32_t color_i);
extern lw6sys_color_8_t lw6sys_color_i_to_8 (u_int32_t color_i);
extern lw6sys_color_8_t lw6sys_color_a_to_8 (char *ascii);
extern void lw6sys_color_a_to_f (lw6sys_color_f_t * color_f, char *ascii);
extern char *lw6sys_color_8_to_a (lw6sys_color_8_t color_8);
extern void lw6sys_color_rgb_to_hsv (lw6sys_color_hsv_t * color_hsv,
				     lw6sys_color_8_t color_8);
extern lw6sys_color_8_t lw6sys_color_hsv_to_rgb (lw6sys_color_hsv_t *
						 color_hsv);
extern int lw6sys_color_is_grey (lw6sys_color_8_t color);
extern lw6sys_color_8_t lw6sys_color_average (int size,
					      lw6sys_color_8_t * colors);
extern lw6sys_color_8_t lw6sys_color_ponderate (lw6sys_color_8_t color1,
						lw6sys_color_8_t color2,
						float coeff);
extern float lw6sys_color_distance (lw6sys_color_8_t color1,
				    lw6sys_color_8_t color2);
extern int lw6sys_color_is_same (lw6sys_color_8_t color1,
				 lw6sys_color_8_t color2);
extern void lw6sys_color_8_solid (lw6sys_color_8_t * color);
extern void lw6sys_color_f_solid (lw6sys_color_f_t * color);

/* sys-convert.c */
extern int lw6sys_atoi (char *str);
extern int lw6sys_atob (char *str);
extern float lw6sys_atof (char *str);
extern char *lw6sys_itoa (int value);
extern char *lw6sys_btoa (int value);
extern char *lw6sys_ftoa (float value);

/* sys-debug.c */
extern int lw6sys_debug_get ();
extern void lw6sys_debug_set (int mode);

/* sys-dump.c */
extern void lw6sys_dump_clear (char *user_dir);
extern int lw6sys_dump (char *user_dir, char *content);

/* sys-env.c */
extern char lw6sys_env_separator_char ();
extern char *lw6sys_env_separator_str ();
extern char *lw6sys_env_concat (char *value1, char *value2);
extern int lw6sys_env_exists_prefixed (char *keyword);
extern char *lw6sys_getenv (char *key);
extern char *lw6sys_getenv_prefixed (char *keyword);
extern int lw6sys_setenv (char *keyword, char *value);
extern int lw6sys_setenv_prefixed (char *keyword, char *value);
extern lw6sys_list_t *lw6sys_env_split (char *value);
extern char *lw6sys_get_home ();
extern char *lw6sys_get_username ();
extern char *lw6sys_get_hostname ();

/* sys-exec.c */
extern char *lw6sys_exec_find_myself (int argc, char *argv[]);
extern int lw6sys_is_executed_again (int argc, char *argv[]);
extern int lw6sys_exec_again (int argc, char *argv[]);

/* sys-file.c */
extern int lw6sys_clear_file (char *filename);
extern char *lw6sys_read_file_content (char *filename);
extern int lw6sys_write_file_content (char *filename, char *content);

/* sys-profiler.c */
extern int lw6sys_profiler_check (int verbose);

/* sys-hash.c */
extern lw6sys_hash_t *lw6sys_hash_new (lw6sys_free_func_t free_func,
				       int size);
extern void lw6sys_hash_free (lw6sys_hash_t * hash);
extern int lw6sys_hash_has_key (lw6sys_hash_t * hash, char *key);
extern void *lw6sys_hash_get (lw6sys_hash_t * hash, char *key);
extern void lw6sys_hash_set (lw6sys_hash_t * hash, char *key, void *value);
extern void lw6sys_hash_unset (lw6sys_hash_t * hash, char *key);
extern lw6sys_list_t *lw6sys_hash_keys (lw6sys_hash_t * hash);
extern void lw6sys_hash_map (lw6sys_hash_t * hash,
			     lw6sys_assoc_callback_func_t func,
			     void *func_data);
extern void lw6sys_hash_sort_and_map (lw6sys_hash_t * hash,
				      lw6sys_assoc_callback_func_t func,
				      void *func_data);
extern lw6sys_hash_t *lw6sys_hash_dup (lw6sys_hash_t * hash,
				       lw6sys_dup_func_t dup_func);

/* sys-hexa.c */
extern lw6sys_hexa_serializer_t *lw6sys_hexa_serializer_new (char
							     *hexa_string);
extern void lw6sys_hexa_serializer_free (lw6sys_hexa_serializer_t *
					 hexa_serializer);
extern void lw6sys_hexa_serializer_rewind (lw6sys_hexa_serializer_t *
					   hexa_serializer);
extern int lw6sys_hexa_serializer_eof (lw6sys_hexa_serializer_t *
				       hexa_serializer);
extern char *lw6sys_hexa_serializer_as_string (lw6sys_hexa_serializer_t *
					       hexa_serializer);
extern int lw6sys_hexa_serializer_push_int64 (lw6sys_hexa_serializer_t *
					      hexa_serializer, int64_t value);
extern int lw6sys_hexa_serializer_push_int32 (lw6sys_hexa_serializer_t *
					      hexa_serializer, int32_t value);
extern int lw6sys_hexa_serializer_push_int16 (lw6sys_hexa_serializer_t *
					      hexa_serializer, int16_t value);
extern int lw6sys_hexa_serializer_push_int8 (lw6sys_hexa_serializer_t *
					     hexa_serializer, int8_t value);
extern int lw6sys_hexa_serializer_push_float (lw6sys_hexa_serializer_t *
					      hexa_serializer, float value);
extern int lw6sys_hexa_serializer_push_str (lw6sys_hexa_serializer_t *
					    hexa_serializer, char *value);
extern int lw6sys_hexa_serializer_push_xyz (lw6sys_hexa_serializer_t *
					    hexa_serializer,
					    lw6sys_xyz_t value);
extern int lw6sys_hexa_serializer_push_whd (lw6sys_hexa_serializer_t *
					    hexa_serializer,
					    lw6sys_whd_t value);
extern int lw6sys_hexa_serializer_push_color (lw6sys_hexa_serializer_t *
					      hexa_serializer,
					      lw6sys_color_8_t value);
extern int lw6sys_hexa_serializer_pop_int64 (lw6sys_hexa_serializer_t *
					     hexa_serializer,
					     int64_t * value);
extern int lw6sys_hexa_serializer_pop_int32 (lw6sys_hexa_serializer_t *
					     hexa_serializer,
					     int32_t * value);
extern int lw6sys_hexa_serializer_pop_int16 (lw6sys_hexa_serializer_t *
					     hexa_serializer,
					     int16_t * value);
extern int lw6sys_hexa_serializer_pop_int8 (lw6sys_hexa_serializer_t *
					    hexa_serializer, int8_t * value);
extern int lw6sys_hexa_serializer_pop_float (lw6sys_hexa_serializer_t *
					     hexa_serializer, float *value);
extern int lw6sys_hexa_serializer_pop_str (lw6sys_hexa_serializer_t *
					   hexa_serializer, char **value);
extern int lw6sys_hexa_serializer_pop_xyz (lw6sys_hexa_serializer_t *
					   hexa_serializer,
					   lw6sys_xyz_t * value);
extern int lw6sys_hexa_serializer_pop_whd (lw6sys_hexa_serializer_t *
					   hexa_serializer,
					   lw6sys_whd_t * value);
extern int lw6sys_hexa_serializer_pop_color (lw6sys_hexa_serializer_t *
					     hexa_serializer,
					     lw6sys_color_8_t * value);
/* sys-history.c */
extern void lw6sys_history_init ();
extern void lw6sys_history_register (char *msg);
extern char **lw6sys_history_get (int64_t timeout);
extern void lw6sys_history_free (char **history);

/* sys-i18n.c */
extern char *lw6sys_locale_to_utf8 (char *string);

/* sys-id.c */
extern u_int16_t lw6sys_generate_id_16 ();
extern u_int32_t lw6sys_generate_id_32 ();
extern u_int64_t lw6sys_generate_id_64 ();
extern int lw6sys_check_id_16 (u_int16_t id_16);
extern int lw6sys_check_id_32 (u_int32_t id_32);
extern int lw6sys_check_id_64 (u_int64_t id_64);
extern int lw6sys_check_id (u_int64_t id_64);
extern char *lw6sys_id_ltoa (u_int64_t id);
extern u_int64_t lw6sys_id_atol (char *id);

/* sys-keyword.c */
extern char *lw6sys_keyword_as_key (char *keyword);
extern char *lw6sys_keyword_as_arg (char *keyword);
extern char *lw6sys_keyword_as_env (char *keyword);
extern char *lw6sys_keyword_as_xml (char *keyword);

/* sys-list.c */
extern lw6sys_list_t *lw6sys_list_new (lw6sys_free_func_t free_func);
extern void lw6sys_list_free (lw6sys_list_t * list);
extern lw6sys_list_t *lw6sys_list_next (lw6sys_list_t * list);
extern int lw6sys_list_is_empty (lw6sys_list_t * list);
extern int lw6sys_list_length (lw6sys_list_t * list);
extern void lw6sys_list_map (lw6sys_list_t * list,
			     lw6sys_list_callback_func_t func,
			     void *func_data);
extern void lw6sys_list_filter (lw6sys_list_t ** list,
				lw6sys_list_filter_func_t func,
				void *func_data);
extern void lw6sys_list_push_front (lw6sys_list_t ** list, void *data);
extern void *lw6sys_list_pop_front (lw6sys_list_t ** list);
extern void lw6sys_list_push_back (lw6sys_list_t ** list, void *data);
extern void *lw6sys_list_pop_back (lw6sys_list_t ** list);
extern void lw6sys_lifo_push (lw6sys_list_t ** list, void *data);
extern void *lw6sys_lifo_pop (lw6sys_list_t ** list);
extern void lw6sys_fifo_push (lw6sys_list_t ** list, void *data);
extern void *lw6sys_fifo_pop (lw6sys_list_t ** list);
extern lw6sys_list_t *lw6sys_list_dup (lw6sys_list_t * list,
				       lw6sys_dup_func_t dup_func);

/* sys-log.c */
extern void lw6sys_log_set_file (char *filename);
extern void lw6sys_log_clear (char *filename);
extern void lw6sys_log (int level_id, char *file, int line, char *fmt, ...);
extern void lw6sys_log_critical (char *fmt, ...);
extern int lw6sys_log_get_level ();
extern void lw6sys_log_set_level (int level);

/* sys-math.c */
extern void lw6sys_math_poly_wy1y2s1 (float *y, float *s, float x, float w,
				      float y1, float y2, float s1);
extern float lw6sys_math_angle_360 (int x, int y);
extern float lw6sys_math_heartbeat (int64_t x, int period, float y1,
				    float y2);
extern int lw6sys_math_blink (int64_t x, int period);

/* sys-mem.c */
#ifdef LW6_OPTIMIZE
#define LW6SYS_MALLOC(SIZE) malloc((SIZE))
#define LW6SYS_CALLOC(SIZE) calloc((SIZE),1)
#define LW6SYS_REALLOC(PTR,SIZE) realloc((PTR),(SIZE))
#define LW6SYS_FREE(PTR) free((PTR))
#else
#define LW6SYS_MALLOC(SIZE) lw6sys_malloc((SIZE),__FILE__,__LINE__)
#define LW6SYS_CALLOC(SIZE) lw6sys_calloc((SIZE),__FILE__,__LINE__)
#define LW6SYS_REALLOC(PTR,SIZE) lw6sys_realloc((PTR),(SIZE),__FILE__,__LINE__)
#define LW6SYS_FREE(PTR) lw6sys_free((PTR),__FILE__,__LINE__)
#endif

extern void *lw6sys_malloc (int size, char *file, int line);
extern void *lw6sys_calloc (int size, char *file, int line);
extern void *lw6sys_realloc (void *ptr, int size, char *file, int line);
extern void lw6sys_free (void *ptr, char *file, int line);
extern void lw6sys_free_callback (void *ptr);
extern int lw6sys_megabytes_available ();
extern int lw6sys_is_big_endian ();
extern int lw6sys_is_little_endian ();
extern int lw6sys_check_types_size ();

/* sys-mutex.c */
extern void *lw6sys_mutex_create ();
extern void lw6sys_mutex_destroy (void *mutex);
extern int lw6sys_mutex_lock (void *mutex);
extern int lw6sys_mutex_trylock (void *mutex);
extern int lw6sys_mutex_unlock (void *mutex);
extern int lw6sys_get_mutex_lock_count ();
extern int lw6sys_get_mutex_unlock_count ();
extern int lw6sys_check_mutex_count ();

/* sys-nop.c */
extern int lw6sys_true ();
extern int lw6sys_false ();

/* sys-options.c */
extern char *lw6sys_get_cwd ();
extern char *lw6sys_get_default_user_dir ();
extern char *lw6sys_get_default_config_file ();
extern char *lw6sys_get_default_log_file ();
extern char *lw6sys_get_default_prefix ();
extern char *lw6sys_get_default_mod_dir ();
extern char *lw6sys_get_default_data_dir ();
extern char *lw6sys_get_default_music_dir ();
extern char *lw6sys_get_default_music_path ();
extern char *lw6sys_get_default_map_dir ();
extern char *lw6sys_get_default_map_path ();
extern char *lw6sys_get_default_script_file ();
extern void lw6sys_options_log_defaults ();
extern char *lw6sys_get_run_dir (int argc, char *argv[]);
extern char *lw6sys_get_user_dir (int argc, char *argv[]);
extern char *lw6sys_get_config_file (int argc, char *argv[]);
extern char *lw6sys_get_log_file (int argc, char *argv[]);
extern char *lw6sys_get_prefix (int argc, char *argv[]);
extern char *lw6sys_get_mod_dir (int argc, char *argv[]);
extern char *lw6sys_get_data_dir (int argc, char *argv[]);
extern char *lw6sys_get_music_dir (int argc, char *argv[]);
extern char *lw6sys_get_music_path (int argc, char *argv[]);
extern char *lw6sys_get_map_dir (int argc, char *argv[]);
extern char *lw6sys_get_map_path (int argc, char *argv[]);
extern char *lw6sys_get_script_file (int argc, char *argv[]);
extern void lw6sys_options_log (int argc, char *argv[]);

/* sys-path.c */
extern int lw6sys_file_exists (char *filename);
extern int lw6sys_dir_exists (char *dirname);
extern int lw6sys_create_dir (char *dirname);
extern int lw6sys_create_dir_silent (char *dirname);
extern char *lw6sys_path_add_slash (char *path);
extern char *lw6sys_path_strip_slash (char *path);
extern char *lw6sys_path_concat (char *path1, char *path2);
extern lw6sys_list_t *lw6sys_path_split (char *path);
extern int lw6sys_path_is_relative (char *path);
extern int lw6sys_path_is_cwd (char *path);
extern char *lw6sys_path_parent (char *path);
extern char *lw6sys_path_unparent (char *path);
extern char *lw6sys_path_unparent_no_malloc (char *path);
extern lw6sys_list_t *lw6sys_dir_list (char *dir,
				       lw6sys_dir_list_filter_func_t
				       filter_func, void *func_data, int *n);
extern lw6sys_list_t *lw6sys_path_list (char *path,
					lw6sys_dir_list_filter_func_t
					filter_func, void *func_data, int *n);
extern char *lw6sys_find_in_dir_and_path (char *dir, char *path, char *file);

/* sys-print.c */
extern void lw6sys_print_xml_header (FILE * f, char *comment);
extern void lw6sys_print_xml_footer (FILE * f);

/* sys-progress.c */
extern void lw6sys_progress_default (lw6sys_progress_t * progress,
				     float *value);
extern void lw6sys_progress_update (lw6sys_progress_t * progress, int min,
				    int max, int current);
extern void lw6sys_progress_split (lw6sys_progress_t * progress1,
				   lw6sys_progress_t * progress2,
				   lw6sys_progress_t * progress_src);
extern void lw6sys_progress_split_here (lw6sys_progress_t * progress1,
					lw6sys_progress_t * progress2,
					lw6sys_progress_t * progress_src,
					float here);
extern void lw6sys_progress_split3 (lw6sys_progress_t * progress1,
				    lw6sys_progress_t * progress2,
				    lw6sys_progress_t * progress3,
				    lw6sys_progress_t * progress_src);
extern void lw6sys_progress_split4 (lw6sys_progress_t * progress1,
				    lw6sys_progress_t * progress2,
				    lw6sys_progress_t * progress3,
				    lw6sys_progress_t * progress4,
				    lw6sys_progress_t * progress_src);
extern void lw6sys_progress_split5 (lw6sys_progress_t * progress1,
				    lw6sys_progress_t * progress2,
				    lw6sys_progress_t * progress3,
				    lw6sys_progress_t * progress4,
				    lw6sys_progress_t * progress5,
				    lw6sys_progress_t * progress_src);
extern void lw6sys_progress_begin (lw6sys_progress_t * progress);
extern void lw6sys_progress_half (lw6sys_progress_t * progress);
extern void lw6sys_progress_end (lw6sys_progress_t * progress);

/* sys-random.c */
extern u_int32_t lw6sys_random (u_int32_t range);
extern float lw6sys_random_float (float min, float max);

/* sys-sdl.c */
extern int lw6sys_sdl_register ();
extern int lw6sys_sdl_unregister ();

/* sys-serial.c */
extern void lw6sys_serialize_int64 (unsigned char *data, int64_t value);
extern int64_t lw6sys_unserialize_int64 (unsigned char *data);
extern void lw6sys_serialize_int32 (unsigned char *data, int32_t value);
extern int32_t lw6sys_unserialize_int32 (unsigned char *data);
extern void lw6sys_serialize_int16 (unsigned char *data, int16_t value);
extern int16_t lw6sys_unserialize_int16 (unsigned char *data);

/* sys-shape.c */
extern int lw6sys_shape_check_min_max_whd (lw6sys_whd_t * shape, int min_wh,
					   int max_wh, int max_d);;
extern int lw6sys_shape_check_pos (lw6sys_whd_t * shape, lw6sys_xyz_t * pos);
extern int lw6sys_shape_is_same (lw6sys_whd_t * shape_a,
				 lw6sys_whd_t * shape_b);
extern int lw6sys_shape_is_same_xy (lw6sys_whd_t * shape_a,
				    lw6sys_whd_t * shape_b);

/* sys-sort.c */
extern int lw6sys_sort_int_callback (lw6sys_list_t ** list_a,
				     lw6sys_list_t ** list_b);
extern int lw6sys_sort_int_desc_callback (lw6sys_list_t ** list_a,
					  lw6sys_list_t ** list_b);
extern int lw6sys_sort_float_callback (lw6sys_list_t ** list_a,
				       lw6sys_list_t ** list_b);
extern int lw6sys_sort_float_desc_callback (lw6sys_list_t ** list_a,
					    lw6sys_list_t ** list_b);
extern int lw6sys_sort_str_callback (lw6sys_list_t ** list_a,
				     lw6sys_list_t ** list_b);
extern int lw6sys_sort_str_desc_callback (lw6sys_list_t ** list_a,
					  lw6sys_list_t ** list_b);
extern void lw6sys_sort (lw6sys_list_t ** list,
			 lw6sys_sort_callback_func_t sort_func);

/* sys-spinlock.c */
extern void *lw6sys_spinlock_create ();
extern void lw6sys_spinlock_destroy (void *spinlock);
extern int lw6sys_spinlock_lock (void *spinlock);
extern int lw6sys_spinlock_trylock (void *spinlock);
extern int lw6sys_spinlock_unlock (void *spinlock);

/* sys-str.c */
extern char *lw6sys_str_copy (char *src);
extern char *lw6sys_str_concat (char *str1, char *str2);
extern char *lw6sys_new_sprintf (char *fmt, ...);
extern int lw6sys_str_is_blank (char *str);
extern int lw6sys_str_is_same (char *str_a, char *str_b);
extern int lw6sys_skip_blanks (char **str_ptr);
extern void lw6sys_str_cleanup (char *str);
extern char *lw6sys_str_reformat (char *str, char *prefix, int nb_columns);
extern char *lw6sys_eol ();
extern lw6sys_list_t *lw6sys_str_split (char *str, char c);
extern lw6sys_list_t *lw6sys_str_split_no_0 (char *str, char c);
extern lw6sys_list_t *lw6sys_str_split_config_item (char *str);
extern void lw6sys_str_toupper (char *str);
extern void lw6sys_str_tolower (char *str);

/* sys-test.c */
extern int lw6sys_test_exec (int argc, char *argv[], int mode);
extern int lw6sys_test (int mode);

/* sys-testandset.s */
extern int lw6sys_test_and_set (int *spinlock);

/* sys-thread.c */
extern void
  *lw6sys_thread_create (lw6sys_thread_callback_func_t callback_func,
			 lw6sys_thread_callback_func_t callback_join,
			 void *callback_data);
extern int lw6sys_thread_is_callback_done (void *thread_handler);
extern int lw6sys_thread_get_id (void *thread_handler);
extern void *lw6sys_thread_get_data (void *thread_handler);
extern void lw6sys_thread_join (void *thread_handler);
extern int lw6sys_get_thread_create_count ();
extern int lw6sys_get_thread_join_count ();
extern int lw6sys_check_thread_count ();

/* sys-time.c */
extern int64_t lw6sys_get_timestamp ();
extern int64_t lw6sys_get_uptime ();
extern int32_t lw6sys_get_cycle ();
extern void lw6sys_timer_update (int64_t * timestamp, int64_t * uptime,
				 int32_t * cycle);
extern void lw6sys_sleep (float seconds);
extern void lw6sys_delay (int msec);
extern void lw6sys_idle ();
extern void lw6sys_time_init ();

/* sys-vthread.c */
extern int lw6sys_vthread_run (lw6sys_thread_callback_func_t callback_func,
			       lw6sys_thread_callback_func_t callback_join,
			       void *callback_data);
extern int lw6sys_vthread_is_running ();
extern int lw6sys_vthread_create (lw6sys_thread_callback_func_t callback_func,
				  lw6sys_thread_callback_func_t callback_join,
				  void *callback_data);
extern void lw6sys_vthread_join ();


/* inline utils */
static inline int
lw6sys_min (int a, int b)
{
  return (a < b) ? a : b;
}

static inline int
lw6sys_max (int a, int b)
{
  return (a > b) ? a : b;
}

#endif
