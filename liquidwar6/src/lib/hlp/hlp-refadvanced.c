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

#include "../map/map.h"
#include "../pil/pil.h"
#include "../img/img.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_advanced (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_RESET,
			 _x_
			 ("Clears the config file so that the game will run with defaults next time. The idea is to get rid of traces of previous executions. The difference with '--defaults' is that '--reset' does not run the game, while '--defaults' does."));
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_RESET_CONFIG_ON_UPGRADE,
			 _x_ ("If set, then a reset (config file set to defaults) is run every time you upgrade the game."), 1);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_LOG_LEVEL,
			_x_
			("Defines the log level, that is, how verbose the program will be regarding logs and console output. 0 (ERROR) is the minimum, only errors are reported. 1 (WARNING) means errors + warnings. 2 (NOTICE) displays most important messages. 3 (INFO) is the default, the log file will contain all messages but debug stuff. 4 (DEBUG) logs everything, including debug informations."),
			LW6SYS_LOG_DEFAULT_ID, LW6SYS_LOG_ERROR_ID, LW6SYS_LOG_DEBUG_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TARGET_FPS,
			_x_
			("Defines how many frames will be displayed per second. Of course this is a maximum value, if your hardware can't keep up with this value, display will just be slow, no matter what value you define here. Note that you might really wish to have something rather low here, to keep network and 'logic' function responsiveness. Passed 60 frames per second, speed is really only for visual comfort, as Liquid War 6 is now so fast-paced that it requires 200 frames/sec to outperform opponents."),
			60, 1, 1000);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_IO_PER_SEC,
			_x_
			("Defines the number of calls to input/output functions per second. This can affect speed of menus but also cursors, but won't change the speed of the game itself. It's a cosmectic, comfort option."),
			20, 1, 1000);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NET_PER_SEC,
			_x_
			("Defines the number of calls to network functions per second. This can technically change the network transfers speed, the higher the number, the faster it should be, but at the same time it can technically be more CPU greedy."),
			500, 1, 1000);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_COMMANDS_PER_SEC,
			_x_
			("Defines the number of commands per second. When a command is generated, orders are actually sent to the game engine, for instance, 'this cursor moved there'. So this option will affect game responsiveness, setting this to a high value will make the game more responsive but consume bandwidth on network games."),
			10, 1, 1000);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SCREENSHOTS_PER_MIN,
			_x_
			("Defines the number of screenshots / node info per minute. This can a quite costly operation, but still it must not be too low else screenshots are too outdated."),
			12, 0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_LOADER_SLEEP,
			  _x_ ("Defines how long the loader thread should wait between two polls. Default value should fit in most cases."), 0.5, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_PILOT_LAG,
			_x_
			("Maximum lag, in rounds, until the game engine is slowed down. This will typically be usefull if your computer is too slow for the map resolution and the game speed you set up."),
			10, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MEMORY_BAZOOKA_SIZE,
			_x_
			("The memory bazooka is a brute-force tool, conceived after a full night spent tracking some memory leak. The idea is to keep a track of all allocated pointers, when the data was allocated (timestamp), where in the code (file, line), and even point out what data there is in that place. A memory bazooka report at the end of the game will just show what's left. There should be nothing. This parameter is here to avoid wasting CPU cycles on a feature which is very debug-oriented and does not really make sense for the casual user. Set it to 0 for best performance, something like 100 might just be helpfull, but 1000000 is the right way to seriously debug code."),
			LW6SYS_BAZOOKA_DEFAULT_SIZE, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_MEMORY_BAZOOKA_ERASER,
			 _x_
			 ("The memory eraser is a tool which will systematically fill allocated memory with 'M', and overwrite all allocated bytes with 'F' before freeing memory. It will even handle realloc calls. This is usefull to track bugs. Indeed, with this option enabled, freshly allocated memory will never contain zeroes unless one calls calloc, and if you ever free some memory zone before being done with it, it will be filled with junk and therefore not be usable. The memory bazooka must be big enough if you want this feature to actually work."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_SPLASH,
			 _x_ ("Set this to 'false' to disable the display of the splash screen at game startup."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_BACKGROUND, _x_ ("Decides wether the background animation/image should be displayed at all."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_PREVIEW, _x_ ("Decides wether a map preview should be displayed when choosing a level."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_MAP,
			 _x_ ("Debugging option which can be set to 'false' to disable map (level) display when playing."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_FIGHTERS,
			 _x_ ("Debugging option which can be set to 'false' to disable the display of fighters when playing."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_CURSORS,
			 _x_ ("Debugging option which can be set to 'false' to disable the display of cursors when playing."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_HUD, _x_ ("Decides wether the hud (informations while playing) should be displayed."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_SCORE, _x_ ("Decides wether the score screen should be displayed."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_MENU, _x_ ("Debugging option which can be set to 'false' to disable the display of menus."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_META,
			 _x_ ("Set to 'false' to disable the display of meta information, this includes the help, tootips and breadcrumbs in menus."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_PROGRESS,
			 _x_ ("Decides wether a progress bar should be displayed when a long operation is realized as a background task."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_LOG,
			 _x_ ("Set this to 'false' to disable the display of error messages on the screen. Mote that you can miss valuable informations."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_FPS,
			 _x_
			 ("Set this to 'true' to display the number of frames per second. When this gets too low... play a smaller map, buy a new computer or contribute and hack Liquid War 6 so that it runs faster!"),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_MPS,
			 _x_
			 ("Set this to 'true' to display the number of moves per second. In theory the game should maintain this constant but in practise it can get low if your computer is too slow or too busy."),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_URL,
			 _x_
			 ("Set this to 'true' to display the URL (homepage) of the game. This is mostly used when doing screenshots, so that generated images contain a link to the homepage."),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_MOUSE, _x_ ("Set this to 'false' to always hide the mouse pointer."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_DEBUG_ZONES,
			 _x_ ("Set this to 'true' to display the zones, this is usefull to debug the core algorithm or understand how it works."), 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_DEBUG_GRADIENT,
			 _x_ ("Set this to 'true' to display the gradient, this is usefull to debug the core algorithm or understand how it works."), 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_DISPLAY_CONSOLE,
			 _x_
			 ("Defines wether the interactive system console must be displayed. Note that console support must have been enabled at compilation time. It might not be available on your computer, for instance if you are running a system such as Microsoft Windows."),
			 0);
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SERVER,
			 _x_
			 ("Start the game in server mode, without requiring any graphics backend. Server mode is usefull if you just want to start a network node without hosting any real game on it. It can be used to list existing nodes and sessions or as a bounce server in case some clients can't contact each other because firewalled. If you only want to start a server game on your computer, don't use this option, just start the game normally and start a game server by clicking on the GUI buttons."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_DAEMON,
			 _x_
			 ("Start the game in daemon mode, this is typically used with the server mode, if you want the process to be detached from the console and executed in the background."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_QUICK_START,
			 _x_ ("Start the game just like if the player had requested a quick start, without showing any menu."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_DEMO, _x_ ("Start the game in demo mode. 2 bots play against each other forever."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_BENCH,
			 _x_
			 ("Runs a benchmarking test which will report an approximative performance estimation of the game on your computer. The result is in an arbitrary unit, but it is logarithmic, and works the way the audio decibels do. That is, 30 is 10 times greater than 20. 10 is supposed to be a reference of a computer that can reasonnably run the game. So if you get 40, you are 1000 times powerfull enough. Negative values can technically show up on very slow computers."));
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_BENCH_VALUE,
			_x_
			("Contains the current bench value of the computer running the game. This is used internally to choose the right map settings. You can override this value and use your own but... use at your own risk. Pretending you have a faster computer than what you really have can lead to confusion."),
			LW6LDR_DEFAULT_BENCH_VALUE, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_LOCAL_BENCH_DELTA,
			_x_
			("A value which is added to bench before starting a local game. This is typically zero or negative, as adding to bench is like pretending your computer is faster than it really is."),
			0, -70, 20);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NETWORK_BENCH_DELTA,
			_x_
			("A value which is added to bench before starting a network game. This is typically a negative value, lower than the one added to local game. This is because network games can be more CPU greedy."),
			-5, -70, 20);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_LOCAL_BENCH_VALUE,
			_x_
			("Even if your computer is very fast, this parameter will be used to tame the optimism of the test, and do not load maps in very high detail. It's believed at some point, it's best to keep your extra power to deal with unordinary situations rather than waste it on useless details. Game should be fun with that setting, but if you really want to use your shiny CPU at its maximum, raise this."),
			LW6LDR_DEFAULT_MAX_LOCAL_BENCH_VALUE, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_NETWORK_BENCH_VALUE,
			_x_
			("On network games, we need to be sure everyone can play in correct conditions, therefore maps won't be loaded with more details than this, by default. You're free to increase this parameter but it can cause your games to be unjoignable by some people."),
			LW6LDR_DEFAULT_MAX_NETWORK_BENCH_VALUE, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAGIC_NUMBER,
			_x_
			("This 'magic' number probably requires an explanation. It's used to estimate how big a map can be built. The calculus is very approximative, basically bench_value*magic_number=total_fighters_on_map*rounds_per_sec*moves_per_round with total_fighters_on_map depending on various parameters such as map size but also how many fighters are on the map. The map loader will try and adjust the map size so that it is just big enough not to saturate your CPU while being as high-res as possible. The magic number in itself has no real meaning, the higher it gets, the more optimized it means the game is. Normally you shouldn't change this but if you find the map resizing is too agressively pessimistic, or if for some reason bench returns bogus values, you can modify it."),
			LW6LDR_DEFAULT_MAGIC_NUMBER, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_BIN_ID,
			_x_
			("The internal 'bin-id' value. Note that this is not necessarly equal to the value returned by 'show-build-bin-id'. When they are different, it is assumed this is because of a software upgrade."),
			0, 0, 0);
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CHECK,
			 _x_
			 ("Running the game with '--check' is almost like running '--test', the difference is that '--check' will not run tests which involve graphics or sound backends, so it's adapted to pure console mode. This can be usefull for automated checks on a build farm, or if you want to check things in a headless (pure console) environment."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CUNIT,
			 _x_
			 ("Running the game with '--cunit' is almost like running '--test', the difference is that '--cunit' will use CUnit interactive interface, allowing the user to cherry-pick some tests, and avoid running the whole suite just for one test. This can be usefull for debugging, when individual test binaries are not available."));
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DEBUG_TEAM_ID,
			_x_ ("A team id which will be used for debugging purposes, for instance when displaying gradient."), 0, 0, LW6MAP_MAX_NB_TEAMS - 1);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DEBUG_LAYER_ID,
			_x_ ("A team id which will be used for debugging purposes, for instance when displaying gradient."), 0, 0, LW6MAP_MAX_BODY_DEPTH - 1);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_LOG_TIMEOUT, _x_ ("Delay, in msec, for which a log message will stay displayed on the screen."), 5000,
			0, 0);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_GFX_CPU_USAGE,
			  _x_
			  ("Percentage of the CPU which will be used by the display thread. It's wise to leave some time to other threads to execute. The OS does it naturally, but setting this helps the whole process by explicitely pausing (sleep call) the display thread. You could change this to a low value if you have lagging games but smooth display."),
			  0.75, 0, 1);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DIRTY_READ,
			_x_
			("How to handle dirty reads and locks when displaying stuff. If set to 0, there will be no dirty reads at all, a lock (mutex) will be set whenever it's needed. If set to 1, display might be done with inconsistent data, however the data itself won't be modified while displaying. If set to 2, displayed data can (and will) be modified while the rendering thread is running."),
			LW6PIL_DIRTY_READ_ALWAYS, LW6PIL_DIRTY_READ_NEVER, LW6PIL_DIRTY_READ_ALWAYS);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_GFX_DEBUG,
			 _x_
			 ("Enables dedicated graphics debugging tools. This is different from 'debug' mode which is global, this one is really graphics specific."),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_EXECUTED_AGAIN,
			 _x_
			 ("This environment variable/keyword is used to detect wether the program has been launched by itself with an internal execv call. This is used as a workarround to set some environment variables (DYLD_LIBRARY_PATH on Mac OS X for instance) before the program is run, as sometimes using setenv() inside the program does not work."),
			 0);
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_BASE64_ENCODE,
			 _x_
			 ("If specified, program will take stdin and base64 encode it to stdout. This is for testing purpose (for network messages for instance). Will *not* use standard base64 encoding using characters + and / but - and _ instead to be url-compliant, see RFC 4648 for details."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_BASE64_DECODE,
			 _x_
			 ("If specified, program will take stdin and base64 decode it to stdout. This is for testing purpose (for network messages for instance). Will decode in standard base64 encoding using characters + and / but also the url-compliant version using - and /, see RFC 4648 for details."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_Z_ENCODE,
			 _x_
			 ("If specified, program will take stdin and z-encode it to stdout. This is for testing purpose (for network messages for instance). Z-encoding, here means passing the message through Zlib deflating then base64 encoding and prefix it with a Z."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_Z_DECODE,
			 _x_
			 ("If specified, program will take stdin and z-decode it to stdout. This is for testing purpose (for network messages for instance). Z-decoding, here means verifying there a Z at the beginning, base64 decode and pass the content through Zlib inflating. I content is not Z-prefixed, will be returned as is."));
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_NET_LOG,
			 _x_
			 ("Activates network log, that is, logs everything sent/received over the network, except data which is sent through a third party library such as libCurl. This is mostly for debugging purpose, it can lead to rather big log files."),
			 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_OPEN_RELAY,
			 _x_
			 ("Enables forwarding of abritrary network messages. If open relay is forbidden, the game will only forward messages when physical sender and logical sender are the same. This is to say if messages come from A for C and is sent by A to B, B will forward it to C. But if message comes from X to C and is sent by A to B, then B won't forward it. In practice, it means without open relay, messages can only be forwarded once."),
			 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NETWORK_RELIABILITY,
			_x_
			("The program assumes network is non-reliable, however the problem with those assumptions is that when you test, network is always reliable, even with non-garanteed protocols like UDP. This option will force the program to actually ignore some calls to send or recv functions, simulating a network disfunction. This is to ensure the internal mecanisms correcting network problems do work for good, on daily regular use. It's not possible to set it to a perfect behavior, never dropping any packet, however using the default settings you probably won't even notice the performance drop induced by having to fix problems. The highest the number is, the most reliable network will look, the algorithm is simply to drop one message out of X."),
			1000, 1, 1000000000);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_TROJAN,
			 _x_
			 ("Make the program act like a (stupid) trojan horse, trying to fake messages, sending various inconsistent informations. This is to check the normal version of the program is able to detect such a fake and kick it out of the game. It's of no use for regular players, be sure to unset this if you want to play for good."),
			 0);
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SIMULATE_BASIC,
			 _x_
			 ("Simulates some fights using the basic colors red, green, yellow and blue. Will output on the console a percentage based on scores obtained by the teams. This is typically for map designers and/or people who want to fiddle with team profiles, if some team is really stronger than another one, it should appear in these percentages."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SIMULATE_FULL,
			 _x_
			 ("Simulates some fights using all available colors. This can be very long, it will run approximatively 1000 games consecutively, you can look in the log file to see the progress. Will output on the console a percentage based on scores obtained by the teams. This is typically for map designers and/or people who want to fiddle with team profiles, if some team is really stronger than another one, it should appear in these percentages."));
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_TRAP_ERRORS,
			 _x_
			 ("If set to true, will trap segmentation fault and floating point errors, and display messages about those in a custom box instead of the default one"),
			 LW6SYS_TRAP_ERRORS_DEFAULT);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_JPEG_QUALITY,
			_x_
			("Quality used by libjpeg when creating screenshot images. The same value you would give to Gimp before exporting an image as a JPEG."),
			LW6IMG_JPEG_QUALITY_DEFAULT, LW6IMG_JPEG_QUALITY_MIN, LW6IMG_JPEG_QUALITY_DEFAULT);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DIALOG_TIMEOUT,
			_x_
			("Timeout, in seconds, after which a dialog will automatically be closed, wether user clicked on it or not. Mostly used for testing, to avoid program being stall on a visual prompt. 0 will simply disable this feature and wait forever. Note that some platforms might not support this. Interfaces using Gtk do support it."),
			LW6SYS_DIALOG_TIMEOUT_DEFAULT, LW6SYS_DIALOG_TIMEOUT_MIN, LW6SYS_DIALOG_TIMEOUT_MAX);

  ret = 1;

  return ret;
}
