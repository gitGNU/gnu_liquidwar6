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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "hlp.h"

static int
update_quick (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_HELP);
  lw6sys_list_push_back (list, LW6DEF_COPYRIGHT);
  lw6sys_list_push_back (list, LW6DEF_VERSION);
  lw6sys_list_push_back (list, LW6DEF_TEST);
  lw6sys_list_push_back (list, LW6DEF_DEFAULTS);
  lw6sys_list_push_back (list, LW6DEF_LIST);
  lw6sys_list_push_back (list, LW6DEF_ABOUT);
  lw6sys_list_push_back (list, LW6DEF_DEBUG);
  lw6sys_list_push_back (list, LW6DEF_PEDIGREE);
  lw6sys_list_push_back (list, LW6DEF_HOST);
  lw6sys_list_push_back (list, LW6DEF_AUDIT);
  lw6sys_list_push_back (list, LW6DEF_MODULES);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_quick ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_quick (&list);
    }

  return list;
}

static int
update_doc (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_LIST_QUICK);
  lw6sys_list_push_back (list, LW6DEF_LIST_DOC);
  lw6sys_list_push_back (list, LW6DEF_LIST_SHOW);
  lw6sys_list_push_back (list, LW6DEF_LIST_PATH);
  lw6sys_list_push_back (list, LW6DEF_LIST_PLAYERS);
  lw6sys_list_push_back (list, LW6DEF_LIST_INPUT);
  lw6sys_list_push_back (list, LW6DEF_LIST_GRAPHICS);
  lw6sys_list_push_back (list, LW6DEF_LIST_SOUND);
  lw6sys_list_push_back (list, LW6DEF_LIST_NETWORK);
  lw6sys_list_push_back (list, LW6DEF_LIST_MAP);
  lw6sys_list_push_back (list, LW6DEF_LIST_MAP_RULES);
  lw6sys_list_push_back (list, LW6DEF_LIST_MAP_HINTS);
  lw6sys_list_push_back (list, LW6DEF_LIST_MAP_STYLE);
  lw6sys_list_push_back (list, LW6DEF_LIST_FUNCS);
  lw6sys_list_push_back (list, LW6DEF_LIST_HOOKS);
  lw6sys_list_push_back (list, LW6DEF_LIST_ADVANCED);
  lw6sys_list_push_back (list, LW6DEF_LIST_ALIASES);
  lw6sys_list_push_back (list, LW6DEF_EXAMPLE_RULES_XML);
  lw6sys_list_push_back (list, LW6DEF_EXAMPLE_HINTS_XML);
  lw6sys_list_push_back (list, LW6DEF_EXAMPLE_STYLE_XML);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_doc ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_doc (&list);
    }

  return list;
}

static int
update_show (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_PACKAGE_TARNAME);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_PACKAGE_NAME);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_PACKAGE_STRING);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_VERSION);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_CODENAME);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_STAMP);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_MD5SUM);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_COPYRIGHT);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_LICENSE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_URL);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_CONFIGURE_ARGS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_GCC_VERSION);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_CFLAGS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_LDFLAGS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_HOSTNAME);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_DATE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_TIME);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_HOST_CPU);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENDIANNESS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_POINTER_SIZE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_X86);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_HOST_OS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_MS_WINDOWS);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_MAC_OS_X);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_GP2X);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_TOP_SRCDIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_PREFIX);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_DATADIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_LIBDIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_INCLUDEDIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_LOCALEDIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_DOCDIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_CONSOLE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_GTK);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_MOD_GL);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_ALLINONE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_PARANOID);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_GPROF);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_PROFILER);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_GCOV);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_ENABLE_VALGRIND);
  lw6sys_list_push_back (list, LW6DEF_SHOW_BUILD_BIN_ID);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_USER_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_CONFIG_FILE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_LOG_FILE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_PREFIX);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_MOD_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_DATA_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_MUSIC_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_MUSIC_PATH);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_MAP_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_MAP_PATH);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DEFAULT_SCRIPT_FILE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_CWD);
  lw6sys_list_push_back (list, LW6DEF_SHOW_RUN_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_USER_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_CONFIG_FILE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_LOG_FILE);
  lw6sys_list_push_back (list, LW6DEF_SHOW_PREFIX);
  lw6sys_list_push_back (list, LW6DEF_SHOW_MOD_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_DATA_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_MUSIC_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_MUSIC_PATH);
  lw6sys_list_push_back (list, LW6DEF_SHOW_MAP_DIR);
  lw6sys_list_push_back (list, LW6DEF_SHOW_MAP_PATH);
  lw6sys_list_push_back (list, LW6DEF_SHOW_SCRIPT_FILE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_show ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_show (&list);
    }

  return list;
}

static int
update_path (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_USER_DIR);
  lw6sys_list_push_back (list, LW6DEF_CONFIG_FILE);
  lw6sys_list_push_back (list, LW6DEF_LOG_FILE);
  lw6sys_list_push_back (list, LW6DEF_PREFIX);
  lw6sys_list_push_back (list, LW6DEF_MOD_DIR);
  lw6sys_list_push_back (list, LW6DEF_DATA_DIR);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_DIR);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_PATH);
  lw6sys_list_push_back (list, LW6DEF_MAP_DIR);
  lw6sys_list_push_back (list, LW6DEF_MAP_PATH);
  lw6sys_list_push_back (list, LW6DEF_SCRIPT_FILE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_path ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_path (&list);
    }

  return list;
}

static int
update_players (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_PLAYER1_NAME);
  lw6sys_list_push_back (list, LW6DEF_PLAYER2_NAME);
  lw6sys_list_push_back (list, LW6DEF_PLAYER3_NAME);
  lw6sys_list_push_back (list, LW6DEF_PLAYER4_NAME);
  lw6sys_list_push_back (list, LW6DEF_PLAYER1_STATUS);
  lw6sys_list_push_back (list, LW6DEF_PLAYER2_STATUS);
  lw6sys_list_push_back (list, LW6DEF_PLAYER3_STATUS);
  lw6sys_list_push_back (list, LW6DEF_PLAYER4_STATUS);
  lw6sys_list_push_back (list, LW6DEF_PLAYER1_CONTROL);
  lw6sys_list_push_back (list, LW6DEF_PLAYER2_CONTROL);
  lw6sys_list_push_back (list, LW6DEF_PLAYER3_CONTROL);
  lw6sys_list_push_back (list, LW6DEF_PLAYER4_CONTROL);
  lw6sys_list_push_back (list, LW6DEF_PLAYER1_BOT);
  lw6sys_list_push_back (list, LW6DEF_PLAYER2_BOT);
  lw6sys_list_push_back (list, LW6DEF_PLAYER3_BOT);
  lw6sys_list_push_back (list, LW6DEF_PLAYER4_BOT);
  lw6sys_list_push_back (list, LW6DEF_PLAYER1_COLOR);
  lw6sys_list_push_back (list, LW6DEF_PLAYER2_COLOR);
  lw6sys_list_push_back (list, LW6DEF_PLAYER3_COLOR);
  lw6sys_list_push_back (list, LW6DEF_PLAYER4_COLOR);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_players ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_players (&list);
    }

  return list;
}

static int
update_input (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_MOUSE_SENSITIVITY);
  lw6sys_list_push_back (list, LW6DEF_CURSOR_SENSITIVITY);
  lw6sys_list_push_back (list, LW6DEF_MAX_CURSOR_SPEED);
  lw6sys_list_push_back (list, LW6DEF_REPEAT_DELAY);
  lw6sys_list_push_back (list, LW6DEF_REPEAT_INTERVAL);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_UP);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_DOWN);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_LEFT);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_RIGHT);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_ESC);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_ENTER);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_PGUP);
  lw6sys_list_push_back (list, LW6DEF_CUSTOM_PGDOWN);
  lw6sys_list_push_back (list, LW6DEF_CLICK_TO_FOCUS);
  lw6sys_list_push_back (list, LW6DEF_USE_ESC_BUTTON);
  lw6sys_list_push_back (list, LW6DEF_ZOOM_STEP);
  lw6sys_list_push_back (list, LW6DEF_ZOOM_STICK_DELAY);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_input ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_input (&list);
    }

  return list;
}

static int
update_graphics (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_WIDTH);
  lw6sys_list_push_back (list, LW6DEF_HEIGHT);
  lw6sys_list_push_back (list, LW6DEF_FULLSCREEN);
  lw6sys_list_push_back (list, LW6DEF_GFX_BACKEND);
  lw6sys_list_push_back (list, LW6DEF_GFX_QUALITY);
  lw6sys_list_push_back (list, LW6DEF_WINDOWED_MODE_LIMIT);
  lw6sys_list_push_back (list, LW6DEF_CAPTURE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_graphics ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_graphics (&list);
    }

  return list;
}

static int
update_sound (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_SOUND_VOLUME);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_VOLUME);
  lw6sys_list_push_back (list, LW6DEF_SND_BACKEND);
  lw6sys_list_push_back (list, LW6DEF_AMBIANCE_FILE);
  lw6sys_list_push_back (list, LW6DEF_AMBIANCE_FILTER);
  lw6sys_list_push_back (list, LW6DEF_AMBIANCE_EXCLUDE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_sound ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_sound (&list);
    }

  return list;
}

static int
update_network (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_SKIP_NETWORK);
  lw6sys_list_push_back (list, LW6DEF_BROADCAST);
  lw6sys_list_push_back (list, LW6DEF_PASSWORD);
  lw6sys_list_push_back (list, LW6DEF_BIND_IP);
  lw6sys_list_push_back (list, LW6DEF_BIND_PORT);
  lw6sys_list_push_back (list, LW6DEF_CLI_BACKENDS);
  lw6sys_list_push_back (list, LW6DEF_SRV_BACKENDS);
  lw6sys_list_push_back (list, LW6DEF_PUBLIC_URL);
  lw6sys_list_push_back (list, LW6DEF_NODE_TITLE);
  lw6sys_list_push_back (list, LW6DEF_NODE_DESCRIPTION);
  lw6sys_list_push_back (list, LW6DEF_KNOWN_NODES);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_network ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_network (&list);
    }

  return list;
}

static int
update_map (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_CHOSEN_MAP);
  lw6sys_list_push_back (list, LW6DEF_USE_TEXTURE);
  lw6sys_list_push_back (list, LW6DEF_USE_RULES_XML);
  lw6sys_list_push_back (list, LW6DEF_USE_HINTS_XML);
  lw6sys_list_push_back (list, LW6DEF_USE_STYLE_XML);
  lw6sys_list_push_back (list, LW6DEF_USE_MUSIC_FILE);
  lw6sys_list_push_back (list, LW6DEF_FORCE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_map ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_map (&list);
    }

  return list;
}

static int
update_map_rules (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_TOTAL_TIME);
  lw6sys_list_push_back (list, LW6DEF_RESPAWN_TEAM);
  lw6sys_list_push_back (list, LW6DEF_RESPAWN_POSITION_MODE);
  lw6sys_list_push_back (list, LW6DEF_MOVES_PER_ROUND);
  lw6sys_list_push_back (list, LW6DEF_SPREADS_PER_ROUND);
  lw6sys_list_push_back (list, LW6DEF_ROUNDS_PER_SEC);
  lw6sys_list_push_back (list, LW6DEF_FIGHTER_ATTACK);
  lw6sys_list_push_back (list, LW6DEF_FIGHTER_DEFENSE);
  lw6sys_list_push_back (list, LW6DEF_FIGHTER_NEW_HEALTH);
  lw6sys_list_push_back (list, LW6DEF_FIGHTER_REGENERATE);
  lw6sys_list_push_back (list, LW6DEF_SIDE_ATTACK_FACTOR);
  lw6sys_list_push_back (list, LW6DEF_SIDE_DEFENSE_FACTOR);
  lw6sys_list_push_back (list, LW6DEF_NB_MOVE_TRIES);
  lw6sys_list_push_back (list, LW6DEF_NB_ATTACK_TRIES);
  lw6sys_list_push_back (list, LW6DEF_NB_DEFENSE_TRIES);
  lw6sys_list_push_back (list, LW6DEF_VERTICAL_MOVE);
  lw6sys_list_push_back (list, LW6DEF_SINGLE_ARMY_SIZE);
  lw6sys_list_push_back (list, LW6DEF_TOTAL_ARMIES_SIZE);
  lw6sys_list_push_back (list, LW6DEF_MAX_NB_TEAMS);
  lw6sys_list_push_back (list, LW6DEF_MAX_NB_CURSORS);
  lw6sys_list_push_back (list, LW6DEF_MAX_NB_NODES);
  lw6sys_list_push_back (list, LW6DEF_X_POLARITY);
  lw6sys_list_push_back (list, LW6DEF_Y_POLARITY);
  lw6sys_list_push_back (list, LW6DEF_Z_POLARITY);
  lw6sys_list_push_back (list, LW6DEF_MAX_ZONE_SIZE);
  lw6sys_list_push_back (list, LW6DEF_ROUND_DELTA);
  lw6sys_list_push_back (list, LW6DEF_MAX_ROUND_DELTA);
  lw6sys_list_push_back (list, LW6DEF_MAX_CURSOR_POT);
  lw6sys_list_push_back (list, LW6DEF_CURSOR_POT_INIT);
  lw6sys_list_push_back (list, LW6DEF_MAX_CURSOR_POT_OFFSET);
  lw6sys_list_push_back (list, LW6DEF_START_RED_X);
  lw6sys_list_push_back (list, LW6DEF_START_RED_Y);
  lw6sys_list_push_back (list, LW6DEF_START_GREEN_X);
  lw6sys_list_push_back (list, LW6DEF_START_GREEN_Y);
  lw6sys_list_push_back (list, LW6DEF_START_BLUE_X);
  lw6sys_list_push_back (list, LW6DEF_START_BLUE_Y);
  lw6sys_list_push_back (list, LW6DEF_START_YELLOW_X);
  lw6sys_list_push_back (list, LW6DEF_START_YELLOW_Y);
  lw6sys_list_push_back (list, LW6DEF_START_CYAN_X);
  lw6sys_list_push_back (list, LW6DEF_START_CYAN_Y);
  lw6sys_list_push_back (list, LW6DEF_START_MAGENTA_X);
  lw6sys_list_push_back (list, LW6DEF_START_MAGENTA_Y);
  lw6sys_list_push_back (list, LW6DEF_START_ORANGE_X);
  lw6sys_list_push_back (list, LW6DEF_START_ORANGE_Y);
  lw6sys_list_push_back (list, LW6DEF_START_LIGHTBLUE_X);
  lw6sys_list_push_back (list, LW6DEF_START_LIGHTBLUE_Y);
  lw6sys_list_push_back (list, LW6DEF_START_PURPLE_X);
  lw6sys_list_push_back (list, LW6DEF_START_PURPLE_Y);
  lw6sys_list_push_back (list, LW6DEF_START_PINK_X);
  lw6sys_list_push_back (list, LW6DEF_START_PINK_Y);
  lw6sys_list_push_back (list, LW6DEF_START_POSITION_MODE);
  lw6sys_list_push_back (list, LW6DEF_COLOR_CONFLICT_MODE);
  lw6sys_list_push_back (list, LW6DEF_SPREAD_THREAD);
  lw6sys_list_push_back (list, LW6DEF_GLUE_POWER);
  lw6sys_list_push_back (list, LW6DEF_BOOST_POWER);
  lw6sys_list_push_back (list, LW6DEF_DANGER_POWER);
  lw6sys_list_push_back (list, LW6DEF_MEDICINE_POWER);
  lw6sys_list_push_back (list, LW6DEF_FRAGS_MODE);
  lw6sys_list_push_back (list, LW6DEF_FRAGS_TO_DISTRIBUTE);
  lw6sys_list_push_back (list, LW6DEF_FRAGS_FADE_OUT);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_map_rules ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_map_rules (&list);
    }

  return list;
}

static int
update_map_hints (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_RESAMPLE);
  lw6sys_list_push_back (list, LW6DEF_MIN_MAP_WIDTH);
  lw6sys_list_push_back (list, LW6DEF_MAX_MAP_WIDTH);
  lw6sys_list_push_back (list, LW6DEF_MIN_MAP_HEIGHT);
  lw6sys_list_push_back (list, LW6DEF_MAX_MAP_HEIGHT);
  lw6sys_list_push_back (list, LW6DEF_MIN_MAP_SURFACE);
  lw6sys_list_push_back (list, LW6DEF_MAX_MAP_SURFACE);
  lw6sys_list_push_back (list, LW6DEF_FIGHTER_SCALE);
  lw6sys_list_push_back (list, LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE);
  lw6sys_list_push_back (list, LW6DEF_UPSIZE_USING_FIGHTER_SCALE);
  lw6sys_list_push_back (list, LW6DEF_DOWNSIZE_USING_BENCH_VALUE);
  lw6sys_list_push_back (list, LW6DEF_UPSIZE_USING_BENCH_VALUE);
  lw6sys_list_push_back (list, LW6DEF_GUESS_COLORS);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_COLOR_AUTO);
  lw6sys_list_push_back (list, LW6DEF_HUD_COLOR_AUTO);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_AUTO);
  lw6sys_list_push_back (list, LW6DEF_VIEW_COLOR_AUTO);
  lw6sys_list_push_back (list, LW6DEF_SYSTEM_COLOR_AUTO);
  lw6sys_list_push_back (list, LW6DEF_WALL_GREASE);
  lw6sys_list_push_back (list, LW6DEF_GUESS_MOVES_PER_SEC);
  lw6sys_list_push_back (list, LW6DEF_SPEED);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_map_hints ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_map_hints (&list);
    }

  return list;
}

static int
update_map_style (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_KEEP_RATIO);
  lw6sys_list_push_back (list, LW6DEF_ZOOM);
  lw6sys_list_push_back (list, LW6DEF_ZOOM_MIN);
  lw6sys_list_push_back (list, LW6DEF_ZOOM_MAX);
  lw6sys_list_push_back (list, LW6DEF_X_WRAP);
  lw6sys_list_push_back (list, LW6DEF_Y_WRAP);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_STYLE);
  lw6sys_list_push_back (list, LW6DEF_HUD_STYLE);
  lw6sys_list_push_back (list, LW6DEF_MENU_STYLE);
  lw6sys_list_push_back (list, LW6DEF_VIEW_STYLE);
  lw6sys_list_push_back (list, LW6DEF_ANIMATION_DENSITY);
  lw6sys_list_push_back (list, LW6DEF_ANIMATION_SPEED);
  lw6sys_list_push_back (list, LW6DEF_CURSOR_SIZE);
  lw6sys_list_push_back (list, LW6DEF_HIDDEN_LAYER_ALPHA);
  lw6sys_list_push_back (list, LW6DEF_COLORIZE);
  lw6sys_list_push_back (list, LW6DEF_PIXELIZE);
  lw6sys_list_push_back (list, LW6DEF_COLOR_BASE_BG);
  lw6sys_list_push_back (list, LW6DEF_COLOR_BASE_FG);
  lw6sys_list_push_back (list, LW6DEF_COLOR_ALTERNATE_BG);
  lw6sys_list_push_back (list, LW6DEF_COLOR_ALTERNATE_FG);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_COLOR_ROOT_BG);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_COLOR_ROOT_FG);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_COLOR_STUFF_BG);
  lw6sys_list_push_back (list, LW6DEF_BACKGROUND_COLOR_STUFF_FG);
  lw6sys_list_push_back (list, LW6DEF_HUD_COLOR_FRAME_BG);
  lw6sys_list_push_back (list, LW6DEF_HUD_COLOR_FRAME_FG);
  lw6sys_list_push_back (list, LW6DEF_HUD_COLOR_TEXT_BG);
  lw6sys_list_push_back (list, LW6DEF_HUD_COLOR_TEXT_FG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_DEFAULT_BG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_DEFAULT_FG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_SELECTED_BG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_SELECTED_FG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_DISABLED_BG);
  lw6sys_list_push_back (list, LW6DEF_MENU_COLOR_DISABLED_FG);
  lw6sys_list_push_back (list, LW6DEF_VIEW_COLOR_CURSOR_BG);
  lw6sys_list_push_back (list, LW6DEF_VIEW_COLOR_CURSOR_FG);
  lw6sys_list_push_back (list, LW6DEF_VIEW_COLOR_MAP_BG);
  lw6sys_list_push_back (list, LW6DEF_VIEW_COLOR_MAP_FG);
  lw6sys_list_push_back (list, LW6DEF_SYSTEM_COLOR_BG);
  lw6sys_list_push_back (list, LW6DEF_SYSTEM_COLOR_FG);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_DEAD);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_RED);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_GREEN);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_BLUE);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_YELLOW);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_CYAN);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_MAGENTA);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_ORANGE);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_LIGHTBLUE);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_PURPLE);
  lw6sys_list_push_back (list, LW6DEF_TEAM_COLOR_PINK);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_FILE);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_FILTER);
  lw6sys_list_push_back (list, LW6DEF_MUSIC_EXCLUDE);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_map_style ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_map_style (&list);
    }

  return list;
}

static int
update_funcs (lw6sys_list_t ** list)
{
  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_funcs ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_funcs (&list);
    }

  return list;
}

static int
update_hooks (lw6sys_list_t ** list)
{
  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_hooks ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_hooks (&list);
    }

  return list;
}

static int
update_advanced (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, LW6DEF_RESET);
  lw6sys_list_push_back (list, LW6DEF_RESET_CONFIG_ON_UPGRADE);
  lw6sys_list_push_back (list, LW6DEF_LOG_LEVEL);
  lw6sys_list_push_back (list, LW6DEF_TARGET_FPS);
  lw6sys_list_push_back (list, LW6DEF_IO_PER_SEC);
  lw6sys_list_push_back (list, LW6DEF_COMMANDS_PER_SEC);
  lw6sys_list_push_back (list, LW6DEF_LOADER_SLEEP);
  lw6sys_list_push_back (list, LW6DEF_PILOT_LAG);
  lw6sys_list_push_back (list, LW6DEF_MEMORY_BAZOOKA_SIZE);
  lw6sys_list_push_back (list, LW6DEF_MEMORY_BAZOOKA_ERASER);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_SPLASH);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_BACKGROUND);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_PREVIEW);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_MAP);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_FIGHTERS);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_CURSORS);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_HUD);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_SCORE);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_MENU);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_PROGRESS);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_LOG);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_FPS);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_MPS);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_URL);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_MOUSE);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_DEBUG_ZONES);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_DEBUG_GRADIENT);
  lw6sys_list_push_back (list, LW6DEF_DISPLAY_CONSOLE);
  lw6sys_list_push_back (list, LW6DEF_SERVER);
  lw6sys_list_push_back (list, LW6DEF_DAEMON);
  lw6sys_list_push_back (list, LW6DEF_QUICK_START);
  lw6sys_list_push_back (list, LW6DEF_DEMO);
  lw6sys_list_push_back (list, LW6DEF_BENCH);
  lw6sys_list_push_back (list, LW6DEF_BENCH_VALUE);
  lw6sys_list_push_back (list, LW6DEF_MAGIC_NUMBER);
  lw6sys_list_push_back (list, LW6DEF_BIN_ID);
  lw6sys_list_push_back (list, LW6DEF_CHECK);
  lw6sys_list_push_back (list, LW6DEF_BOT_SPEED);
  lw6sys_list_push_back (list, LW6DEF_BOT_IQ);
  lw6sys_list_push_back (list, LW6DEF_DEBUG_TEAM_ID);
  lw6sys_list_push_back (list, LW6DEF_DEBUG_LAYER_ID);
  lw6sys_list_push_back (list, LW6DEF_LOG_TIMEOUT);
  lw6sys_list_push_back (list, LW6DEF_GFX_CPU_USAGE);
  lw6sys_list_push_back (list, LW6DEF_DIRTY_READ);
  lw6sys_list_push_back (list, LW6DEF_GFX_DEBUG);
  lw6sys_list_push_back (list, LW6DEF_EXECUTED_AGAIN);
  lw6sys_list_push_back (list, LW6DEF_BASE64_ENCODE);
  lw6sys_list_push_back (list, LW6DEF_BASE64_DECODE);
  lw6sys_list_push_back (list, LW6DEF_Z_ENCODE);
  lw6sys_list_push_back (list, LW6DEF_Z_DECODE);
  lw6sys_list_push_back (list, LW6DEF_NET_LOG);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_advanced ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_advanced (&list);
    }

  return list;
}

static int
update_aliases (lw6sys_list_t ** list)
{
  lw6sys_list_push_back (list, "h");
  lw6sys_list_push_back (list, "?");
  lw6sys_list_push_back (list, "v");
  lw6sys_list_push_back (list, LW6DEF_COPYLEFT);
  lw6sys_list_push_back (list, LW6DEF_COPY);
  lw6sys_list_push_back (list, "c");
  lw6sys_list_push_back (list, LW6DEF_CFLAGS);
  lw6sys_list_push_back (list, LW6DEF_LDFLAGS);
  lw6sys_list_push_back (list, LW6DEF_CONFIGURE_ARGS);

  return ((*list) != NULL);
}

lw6sys_list_t *
lw6hlp_list_aliases ()
{
  lw6sys_list_t *list = NULL;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      update_aliases (&list);
    }

  return list;
}

/**
 * lw6hlp_list
 *
 * Returns a list of all available keywords.
 *
 * Return value: a list containing all the keywords. Strings are not
 *   dynamically allocated, you can't modify them.
 */
lw6sys_list_t *
lw6hlp_list ()
{
  lw6sys_list_t *list = NULL;
  int ok = 0;

  list = lw6sys_list_new (NULL);
  if (list)
    {
      ok = update_quick (&list) && update_doc (&list) && update_show (&list)
	&& update_path (&list) && update_players (&list)
	&& update_input (&list) && update_graphics (&list)
	&& update_sound (&list) && update_network (&list)
	&& update_map (&list) && update_map_rules (&list)
	&& update_map_hints (&list) && update_map_style (&list)
	&& update_funcs (&list) && update_hooks (&list)
	&& update_advanced (&list) && update_aliases (&list);
    }

  if (!ok)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("problem generating help keywords list"));
      if (list)
	{
	  lw6sys_list_free (list);
	  list = NULL;
	}
    }

  return list;
}
