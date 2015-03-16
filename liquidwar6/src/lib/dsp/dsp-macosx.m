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

#ifdef LW6_MAC_OS_X

#include "dsp-internal.h"

#import <Cocoa/Cocoa.h>

void *
_lw6dsp_macosx_thread_init (lw6sys_context_t * sys_context)
{
  void *ret;
  NSAutoreleasePool *autoreleasepool = NULL;

  /*
   * Quoting http://www.idevgames.com/forum/archive/index.php/t-7710.html
   *
   * Each thread that uses Obj-C objects (appkit, foundation etc, directly or
   * indirectly) must have an autorelease pool in place. This is explained in
   * the documentation for NSAutoReleasePool.
   *
   * Just wrap your thread with
   *
   * NSAutoreleasePool   *autoreleasepool = [[NSAutoreleasePool alloc] init];
   * // do thread work
   * [autoreleasepool release];
   *
   * NSApplication does this for you automatically in the main thread.
   * 
   * SDL's OS X specific routines are implemented in Obj.C. Its threading 
   * implementation, however, is *not*. This explains why you can do
   * multithreading, and not get any warnings as long as your non-main
   * thread(s) do(es)n't call any Obj.C code. Whether or not that means you
   * also should not do IO, for instance, without wrapping remains to be seen and
   * is not relevant now.
   *
   * I got the warnings because it turned out that I changed my window title
   * from within the new thread, with SDL_WM_SetTitle. This was easy enough to
   * trace with a gdb breakpoint on _NSAutoReleaseNoPool.
   */

  autoreleasepool =[[NSAutoreleasePool alloc] init];
  ret = (void *) autoreleasepool;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _("Cocoa release pool alloc at address %p"), ret);

  return ret;
}

void
_lw6dsp_macosx_thread_quit (lw6sys_context_t * sys_context, void *ptr)
{
  NSAutoreleasePool *autoreleasepool = NULL;

  autoreleasepool = (NSAutoreleasePool *) ptr;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _("Cocoa release pool release at address %p"), ptr);
  [autoreleasepool release];
}
#endif
