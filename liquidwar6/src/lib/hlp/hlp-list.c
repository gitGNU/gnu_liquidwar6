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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "hlp.h"

static int
_update_quick (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_HELP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COPYRIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VERSION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DEFAULTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ABOUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DEBUG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PEDIGREE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HOST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_AUDIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MODULES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CREDITS);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_quick
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning quick options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_quick (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_quick (sys_context, &list);
    }

  return list;
}

static int
_update_doc (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_QUICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_DOC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_SHOW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_PLAYERS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_INPUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_GRAPHICS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_SOUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_NETWORK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_MAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_MAP_RULES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_MAP_HINTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_MAP_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_MAP_TEAMS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_FUNCS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_HOOKS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_ADVANCED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_ALIASES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_TEAM_COLORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIST_WEAPONS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXAMPLE_RULES_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXAMPLE_HINTS_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXAMPLE_STYLE_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXAMPLE_TEAMS_XML);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_doc
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning self-documentation system.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_doc (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_doc (sys_context, &list);
    }

  return list;
}

static int
_update_show (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_PACKAGE_TARNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_PACKAGE_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_PACKAGE_STRING);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_PACKAGE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_VERSION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_CODENAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_VERSION_BASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_VERSION_MAJOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_VERSION_MINOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_STAMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_MD5SUM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_COPYRIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_LICENSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_HOME_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_BUGS_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_CONFIGURE_ARGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_GCC_VERSION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_CFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_LDFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_HOSTNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_DATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_TIME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_HOST_CPU);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENDIANNESS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_POINTER_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_X86);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_HOST_OS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_GNU);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_UNIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_MS_WINDOWS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_MAC_OS_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_GP2X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_TOP_SRCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ABS_SRCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_DATADIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_LIBDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_INCLUDEDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_LOCALEDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_DOCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_CONSOLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_GTK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_GL1);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_GLES2);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_SOFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_CACA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_OPENMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_ALLINONE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_PARANOID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_GPROF);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_PROFILER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_GCOV);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_ENABLE_VALGRIND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_BUILD_BIN_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_CONFIG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_MOD_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_DATA_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_MAP_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DEFAULT_SCRIPT_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_CWD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_RUN_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_CONFIG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_MOD_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_DATA_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_MAP_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHOW_SCRIPT_FILE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_show
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the show options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_show (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_show (sys_context, &list);
    }

  return list;
}

static int
_update_path (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CONFIG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MOD_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DATA_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAP_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SCRIPT_FILE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_path
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the path options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_path (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_path (sys_context, &list);
    }

  return list;
}

static int
_update_players (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER1_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER2_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER3_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER4_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER1_STATUS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER2_STATUS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER3_STATUS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER4_STATUS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER1_CONTROL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER2_CONTROL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER3_CONTROL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER4_CONTROL);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_players
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the players options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_players (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_players (sys_context, &list);
    }

  return list;
}

static int
_update_input (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_MOUSE_SENSITIVITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CURSOR_SENSITIVITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_CURSOR_SPEED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_REPEAT_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_REPEAT_INTERVAL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DOUBLE_CLICK_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_AUTO_RELEASE_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_DOUBLE_CLICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_LEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_RIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_ESC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_ENTER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_CTRL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_ALT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_PGUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUSTOM_PGDOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CLICK_TO_FOCUS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_ESC_BUTTON);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ZOOM_STEP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ZOOM_STICK_DELAY);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_input
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the input options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_input (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_input (sys_context, &list);
    }

  return list;
}

static int
_update_graphics (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_WIDTH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HEIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FULLSCREEN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GFX_BACKEND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GFX_QUALITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WINDOWED_MODE_LIMIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CAPTURE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_graphics
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the graphics options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_graphics (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_graphics (sys_context, &list);
    }

  return list;
}

static int
_update_sound (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_FX_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WATER_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SND_BACKEND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_AMBIANCE_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_AMBIANCE_FILTER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_AMBIANCE_EXCLUDE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_sound
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the sound options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_sound (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_sound (sys_context, &list);
    }

  return list;
}

static int
_update_network (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_SKIP_NETWORK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BROADCAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PASSWORD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BIND_IP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BIND_PORT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CLI_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SRV_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PUBLIC_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NODE_TITLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NODE_DESCRIPTION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_KNOWN_NODES);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_network
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the network options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_network (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_network (sys_context, &list);
    }

  return list;
}

static int
_update_map (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_CHOSEN_MAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_TEXTURE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_CURSOR_TEXTURE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_RULES_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_HINTS_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_STYLE_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_TEAMS_XML);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_MUSIC_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FORCE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_map
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the map options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_map (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_map (sys_context, &list);
    }

  return list;
}

static int
_update_map_rules (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_TOTAL_TIME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESPAWN_TEAM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESPAWN_POSITION_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESPAWN_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MOVES_PER_ROUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SPREADS_PER_ROUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ROUNDS_PER_SEC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIGHTER_ATTACK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIGHTER_DEFENSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIGHTER_NEW_HEALTH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIGHTER_REGENERATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SIDE_ATTACK_FACTOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SIDE_DEFENSE_FACTOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NB_MOVE_TRIES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NB_ATTACK_TRIES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NB_DEFENSE_TRIES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VERTICAL_MOVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SPREAD_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SINGLE_ARMY_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TOTAL_ARMIES_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_NB_TEAMS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_NB_CURSORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_NB_NODES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HIGHEST_TEAM_COLOR_ALLOWED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HIGHEST_WEAPON_ALLOWED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_X_POLARITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_Y_POLARITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_Z_POLARITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_ZONE_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ROUND_DELTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_ROUND_DELTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_CURSOR_POT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CURSOR_POT_INIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_CURSOR_POT_OFFSET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_RED_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_GREEN_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_BLUE_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_YELLOW_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_CYAN_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_MAGENTA_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_ORANGE_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_LIGHTBLUE_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_PURPLE_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_PINK_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_RED_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_GREEN_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_BLUE_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_YELLOW_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_CYAN_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_MAGENTA_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_ORANGE_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_LIGHTBLUE_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_PURPLE_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_PINK_Y);
  lw6sys_list_push_back (sys_context, list, LW6DEF_START_POSITION_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLOR_CONFLICT_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SPREAD_THREAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GLUE_POWER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOOST_POWER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DANGER_POWER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MEDICINE_POWER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FRAGS_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FRAGS_TO_DISTRIBUTE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FRAGS_FADE_OUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_USE_TEAM_PROFILES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_AGGRESSIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_VULNERABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_MOBILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_FAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_HANDICAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_WEAPON_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_WEAPON_ALTERNATE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_RED_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_PROFILE_PINK_WEAPON_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WEAPON_DURATION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WEAPON_CHARGE_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WEAPON_CHARGE_MAX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WEAPON_TUNE_BERZERK_POWER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WEAPON_TUNE_TURBO_POWER);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_rules
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the rules options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_map_rules (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_map_rules (sys_context, &list);
    }

  return list;
}

static int
_update_map_hints (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESAMPLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MIN_MAP_WIDTH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_MAP_WIDTH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MIN_MAP_HEIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_MAP_HEIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MIN_MAP_SURFACE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_MAP_SURFACE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIGHTER_SCALE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_UPSIZE_USING_FIGHTER_SCALE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DOWNSIZE_USING_BENCH_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_UPSIZE_USING_BENCH_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GUESS_COLORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_COLOR_AUTO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_COLOR_AUTO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_AUTO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_COLOR_AUTO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SYSTEM_COLOR_AUTO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WALL_GREASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GUESS_MOVES_PER_SEC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SPEED);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_hints
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the hints options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_map_hints (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_map_hints (sys_context, &list);
    }

  return list;
}

static int
_update_map_style (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_KEEP_RATIO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ZOOM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ZOOM_MIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ZOOM_MAX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_X_WRAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_Y_WRAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ANIMATION_DENSITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ANIMATION_SPEED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CURSOR_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLORIZE_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BLINK_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HIDDEN_LAYER_ALPHA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLORIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PIXELIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLOR_BASE_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLOR_BASE_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLOR_ALTERNATE_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COLOR_ALTERNATE_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_COLOR_ROOT_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_COLOR_ROOT_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_COLOR_STUFF_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BACKGROUND_COLOR_STUFF_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_COLOR_FRAME_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_COLOR_FRAME_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_COLOR_TEXT_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_HUD_COLOR_TEXT_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_DEFAULT_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_DEFAULT_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_SELECTED_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_SELECTED_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_DISABLED_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MENU_COLOR_DISABLED_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_COLOR_CURSOR_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_COLOR_CURSOR_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_COLOR_MAP_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_VIEW_COLOR_MAP_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SYSTEM_COLOR_BG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SYSTEM_COLOR_FG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_DEAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_RED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_GREEN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_BLUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_YELLOW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_CYAN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_MAGENTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_ORANGE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_LIGHTBLUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_PURPLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TEAM_COLOR_PINK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_FILTER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MUSIC_EXCLUDE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_WAVES);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_style
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the style options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_map_style (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_map_style (sys_context, &list);
    }

  return list;
}

static int
_update_map_teams (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER1_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER2_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER3_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAYER4_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NB_BOTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT_SPEED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT_IQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT1_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT2_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT3_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT4_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT5_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT6_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT7_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT8_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT9_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT1_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT2_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT3_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT4_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT5_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT6_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT7_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT8_AI);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BOT9_AI);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_teams
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning the teams options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_map_teams (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_map_teams (sys_context, &list);
    }

  return list;
}

static int
_update_funcs (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_GETTEXT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SET_MEMORY_BAZOOKA_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MEMORY_BAZOOKA_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SET_MEMORY_BAZOOKA_ERASER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MEMORY_BAZOOKA_ERASER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_TARNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_STRING);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_VERSION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_CODENAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_BASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_MAJOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_MINOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_STAMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_MD5SUM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_COPYRIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_LICENSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_HOME_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_BUGS_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_CONFIGURE_ARGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_GCC_VERSION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_CFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_LDFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_HOSTNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_DATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_TIME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_HOST_CPU);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENDIANNESS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_POINTER_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_X86);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_HOST_OS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_GNU);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_UNIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_MS_WINDOWS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_MAC_OS_X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_IS_GP2X);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_TOP_SRCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ABS_SRCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_DATADIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_LIBDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_INCLUDEDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_LOCALEDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_DOCDIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_CONSOLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GTK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_GL1);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_GLES2);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_SOFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_CACA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_CSOUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_OGG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_HTTP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_OPENMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_OPTIMIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_ALLINONE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_FULLSTATIC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_PARANOID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GPROF);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_INSTRUMENT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_PROFILER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GCOV);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_VALGRIND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_BUILD_GET_BIN_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_DEBUG_GET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_DEBUG_SET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_DUMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_DUMP_CLEAR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_USERNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_HOSTNAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GETENV);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GETENV_PREFIXED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GENERATE_ID_16);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GENERATE_ID_32);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GENERATE_ID_64);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG_SET_DIALOG_TIMEOUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG_GET_LEVEL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG_SET_LEVEL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG_GET_BACKTRACE_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_LOG_SET_BACKTRACE_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_MEGABYTES_AVAILABLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_OPENMP_GET_NUM_PROCS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_CONFIG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_MOD_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_DATA_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_MAP_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DEFAULT_SCRIPT_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_CWD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_RUN_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_CONFIG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_PREFIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MOD_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_DATA_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MAP_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_SCRIPT_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_PATH_CONCAT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_PATH_FILE_ONLY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_PATH_PARENT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_PATH_SPLIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SIGNAL_CUSTOM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SIGNAL_DEFAULT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SIGNAL_SEND_QUIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SIGNAL_POLL_QUIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_TIMESTAMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_UPTIME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_GET_CYCLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SLEEP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_DELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_IDLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_SNOOZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SYS_URL_CANONIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_ABOUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_GET_DEFAULT_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_QUICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_DOC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_SHOW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_PLAYERS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_INPUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_GRAPHICS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_SOUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_NETWORK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_MAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_MAP_RULES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_MAP_HINTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_MAP_STYLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_MAP_TEAMS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_FUNCS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_HOOKS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_ADVANCED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_ALIASES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_TEAM_COLORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST_WEAPONS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6HLP_LIST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_DEFAULTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_LOAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_OPTION_EXISTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_GET_OPTION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_SET_OPTION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_SAVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_INIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_QUIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_UNIFIED_GET_USER_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_UNIFIED_GET_LOG_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_UNIFIED_GET_MUSIC_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CFG_UNIFIED_GET_MAP_PATH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_APPEND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_REMOVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_REMOVE_ALL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SYNC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SELECT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SELECT_ESC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_ENABLE_ESC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SCROLL_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SCROLL_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_SET_BREADCRUMBS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_CLOSE_POPUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MENU_HAS_POPUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_DEFAULT_LOOK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_LOOK_SET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_LOOK_GET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_LOOK_ZOOM_IN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_LOOK_ZOOM_OUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_INPUT_RESET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POLL_MOVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_GET_STATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_IS_PRESSED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_GET_MOVE_PAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_GET_MOVE_PAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_GET_MOVE_PAD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_LEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_RIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_MIDDLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_SIMPLE_CLICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_DOUBLE_CLICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_TRIPLE_CLICK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_WHEEL_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_MOUSE_POP_WHEEL_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_LEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_RIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ENTER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ESC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_CTRL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ALT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_PGUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_PGDOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_LEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_RIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_A);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_B);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_C);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_D);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_E);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_F);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_LEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_RIGHT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_A);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_B);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_C);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_D);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_E);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_F);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GFX_GET_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_RELEASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_UPDATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_NB_FRAMES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_LAST_FRAME_RENDERING_TIME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_INSTANT_FPS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_AVERAGE_FPS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_VIDEO_MODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6DSP_GET_FULLSCREEN_MODES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_GET_ENTRIES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_READ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_READ_RELATIVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_PRINT_EXAMPLES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_HINTS_GET_DEFAULT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_EXP_VALIDATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6LDR_CHAIN_ENTRY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_LOOK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_PARAM_GET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_TITLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_MUSIC_DIR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_TEAM_COLOR_INDEX_TO_KEY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_TEAM_COLOR_KEY_TO_INDEX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_TEAM_COLOR_INDEX_TO_LABEL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_TEAM_COLOR_LIST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_WEAPON_INDEX_TO_KEY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_WEAPON_KEY_TO_INDEX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_WEAPON_INDEX_TO_LABEL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_WEAPON_LIST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_RULES_GET_DEFAULT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_RULES_GET_MIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_RULES_GET_MAX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_RULES_GET_INT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_STYLE_GET_DEFAULT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_TEAMS_GET_DEFAULT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_EXP_IS_TEAM_COLOR_ALLOWED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_EXP_IS_WEAPON_ALLOWED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_EXP_GET_UNLOCKED_TEAM_COLOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_EXP_GET_UNLOCKED_WEAPON);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_MAX_NB_COLORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_MAX_NB_CURSORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6MAP_GET_MAX_NB_NODES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_BUILD_GAME_STRUCT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_BUILD_GAME_STATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_SYNC_GAME_STATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_DUP_GAME_STATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GAME_STRUCT_CHECKSUM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GAME_STATE_CHECKSUM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_REGISTER_NODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_UNREGISTER_NODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_NODE_EXISTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_ADD_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_REMOVE_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_CURSOR_EXISTS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_SET_CURSOR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_DO_ROUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_MOVES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_SPREADS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_ROUNDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_IS_OVER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_DID_CURSOR_WIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_NB_COLORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_NB_CURSORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6KER_GET_NB_NODES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GEN_CREATE_FROM_SEED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GEN_SEED_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6GEN_SEED_NORMALIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_BENCH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SEED_COMMAND_GENERATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_DUMP_COMMAND_GENERATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_POLL_DUMP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_BUILD_PILOT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SEND_COMMAND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_LOCAL_COMMAND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_COMMIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_FIX_COORDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_FIX_COORDS_X10);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_EXECUTE_COMMAND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MAIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_LOCAL_CURSORS_SET_MOUSE_CONTROLLED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_MAKE_BACKUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SYNC_FROM_BACKUP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SYNC_FROM_REFERENCE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SYNC_FROM_DRAFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_CALIBRATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SPEED_UP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SLOW_DOWN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_ROUND_0);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_SEQ_0);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SEQ2ROUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_ROUND2SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_NEXT_SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_LAST_COMMIT_SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_REFERENCE_CURRENT_SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_REFERENCE_TARGET_SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_MAX_SEQ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_IS_OVER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_DID_CURSOR_WIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_WINNER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_GET_LOOSER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SEQ_RANDOM_0);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_INIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_GET_SEQ_0);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_GET_NODE_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_GET_COMMANDS_BY_NODE_INDEX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_GET_COMMANDS_BY_STAGE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6PIL_SUITE_GET_CHECKPOINT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_GET_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_POLL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_RELEASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_PLAY_FX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_SET_FX_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_SET_WATER_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_IS_MUSIC_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_PLAY_MUSIC_FILE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_PLAY_MUSIC_RANDOM);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_STOP_MUSIC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SND_SET_MUSIC_VOLUME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CNS_CONSOLE_SUPPORT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CNS_TERM_SUPPORT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CNS_INIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CNS_QUIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CNS_POLL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6TSK_LOADER_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6TSK_LOADER_PUSH_GEN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6TSK_LOADER_PUSH_LDR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6TSK_LOADER_POP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6TSK_LOADER_GET_STAGE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6IMG_SCREENSHOT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6NET_INIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6NET_QUIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6CLI_GET_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6SRV_GET_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_DB_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_DB_RESET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_DB_DEFAULT_NAME);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_POLL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_CLOSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_ENTRIES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_SERVER_START);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_CLIENT_JOIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_REFRESH_PEER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_DISCONNECT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_UPDATE_INFO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_CALIBRATE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_LOCAL_SEQ_0);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_LOCAL_SEQ_LAST);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_SEQ_MIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_SEQ_MAX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_SEQ_DRAFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_SEQ_REFERENCE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_IS_PEER_CONNECTED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_IS_PEER_REGISTERED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_IS_SEED_NEEDED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_IS_DUMP_NEEDED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_PUT_LOCAL_MSG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_NEXT_REFERENCE_MSG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6P2P_NODE_GET_NEXT_DRAFT_MSG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6BOT_GET_BACKENDS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6BOT_NEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6BOT_NEXT_MOVE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6_RELEASE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6_EXIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6_SET_RET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_C_LW6_GET_RET);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_funcs
 *
 * @sys_context: global system context
 *
 * Returns the list of C-function exported to Guile.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_funcs (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_funcs (sys_context, &list);
    }

  return list;
}

static int
_update_hooks (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  return ((*list) != NULL);
}

/**
 * lw6hlp_list_hooks
 *
 * @sys_context: global system context
 *
 * Returns the list of hooks.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_hooks (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_hooks (sys_context, &list);
    }

  return list;
}

static int
_update_advanced (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESET);
  lw6sys_list_push_back (sys_context, list, LW6DEF_RESET_CONFIG_ON_UPGRADE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LOG_LEVEL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TARGET_FPS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_IO_PER_SEC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NET_PER_SEC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COMMANDS_PER_SEC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SCREENSHOTS_PER_MIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LOADER_SLEEP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PILOT_LAG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MEMORY_BAZOOKA_SIZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MEMORY_BAZOOKA_ERASER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_SPLASH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_BACKGROUND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_PREVIEW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_MAP);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_FIGHTERS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_CURSORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_HUD);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_SCORE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_MENU);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_META);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_PROGRESS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_LOG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_FPS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_MPS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_URL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_MOUSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_DEBUG_ZONES);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_DEBUG_GRADIENT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISPLAY_CONSOLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SERVER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DAEMON);
  lw6sys_list_push_back (sys_context, list, LW6DEF_QUICK_START);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DEMO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BENCH);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BENCH_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LOCAL_BENCH_DELTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NETWORK_BENCH_DELTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_LOCAL_BENCH_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAX_NETWORK_BENCH_VALUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAGIC_NUMBER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BIN_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CHECK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CUNIT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DEBUG_TEAM_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DEBUG_LAYER_ID);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LOG_TIMEOUT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GFX_CPU_USAGE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DIRTY_READ);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GFX_DEBUG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_EXECUTED_AGAIN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BASE64_ENCODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BASE64_DECODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_Z_ENCODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_Z_DECODE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NET_LOG);
  lw6sys_list_push_back (sys_context, list, LW6DEF_OPEN_RELAY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_NETWORK_RELIABILITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TROJAN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SIMULATE_BASIC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SIMULATE_FULL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TRAP_ERRORS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_JPEG_QUALITY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DIALOG_TIMEOUT);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_advanced
 *
 * @sys_context: global system context
 *
 * Returns the list of keywords concerning advanced options.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_advanced (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_advanced (sys_context, &list);
    }

  return list;
}

static int
_update_aliases (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, "h");
  lw6sys_list_push_back (sys_context, list, "?");
  lw6sys_list_push_back (sys_context, list, "v");
  lw6sys_list_push_back (sys_context, list, LW6DEF_COPYLEFT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_COPY);
  lw6sys_list_push_back (sys_context, list, "c");
  lw6sys_list_push_back (sys_context, list, LW6DEF_CFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LDFLAGS);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CONFIGURE_ARGS);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_aliases
 *
 * @sys_context: global system context
 *
 * Returns the list of command-line aliases.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_aliases (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_aliases (sys_context, &list);
    }

  return list;
}

static int
_update_team_colors (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_RED);
  lw6sys_list_push_back (sys_context, list, LW6DEF_GREEN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_BLUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_YELLOW);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CYAN);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MAGENTA);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ORANGE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_LIGHTBLUE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PURPLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PINK);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_team_colors
 *
 * @sys_context: global system context
 *
 * Returns the list of team_colors.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_team_colors (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_team_colors (sys_context, &list);
    }

  return list;
}

static int
_update_weapons (lw6sys_context_t * sys_context, lw6sys_list_t ** list)
{
  lw6sys_list_push_back (sys_context, list, LW6DEF_BERZERK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_INVINCIBLE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ESCAPE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TURBO);
  lw6sys_list_push_back (sys_context, list, LW6DEF_TELEPORT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SCATTER);
  lw6sys_list_push_back (sys_context, list, LW6DEF_FIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_MIX);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CONTROL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PERMUTATION);
  lw6sys_list_push_back (sys_context, list, LW6DEF_STEAL);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ATOMIC);
  lw6sys_list_push_back (sys_context, list, LW6DEF_REVERSE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_CRAZY);
  lw6sys_list_push_back (sys_context, list, LW6DEF_REWIND);
  lw6sys_list_push_back (sys_context, list, LW6DEF_ATTRACT);
  lw6sys_list_push_back (sys_context, list, LW6DEF_SHRINK);
  lw6sys_list_push_back (sys_context, list, LW6DEF_KAMIKAZE);
  lw6sys_list_push_back (sys_context, list, LW6DEF_DISAPPEAR);
  lw6sys_list_push_back (sys_context, list, LW6DEF_PLAGUE);

  return ((*list) != NULL);
}

/**
 * lw6hlp_list_weapons
 *
 * @sys_context: global system context
 *
 * Returns the list of weapons.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list_weapons (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      _update_weapons (sys_context, &list);
    }

  return list;
}

/**
 * lw6hlp_list
 *
 * @sys_context: global system context
 *
 * Returns the list of all available keywords.
 *
 * Return value: list of static strings (can't modify them)
 */
lw6sys_list_t *
lw6hlp_list (lw6sys_context_t * sys_context)
{
  lw6sys_list_t *list = NULL;
  int ok = 0;

  list = lw6sys_list_new (sys_context, NULL);
  if (list)
    {
      ok = _update_quick (sys_context, &list) && _update_doc (sys_context, &list) && _update_show (sys_context, &list)
	&& _update_path (sys_context, &list) && _update_players (sys_context, &list)
	&& _update_input (sys_context, &list) && _update_graphics (sys_context, &list)
	&& _update_sound (sys_context, &list) && _update_network (sys_context, &list)
	&& _update_map (sys_context, &list) && _update_map_rules (sys_context, &list)
	&& _update_map_hints (sys_context, &list) && _update_map_style (sys_context, &list)
	&& _update_map_teams (sys_context, &list) && _update_funcs (sys_context, &list)
	&& _update_hooks (sys_context, &list) && _update_advanced (sys_context, &list) && _update_aliases (sys_context, &list)
	&& _update_team_colors (sys_context, &list) && _update_weapons (sys_context, &list);
    }

  if (!ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("problem generating help keywords list"));
      if (list)
	{
	  lw6sys_list_free (sys_context, list);
	  list = NULL;
	}
    }

  return list;
}
