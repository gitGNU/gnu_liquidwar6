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
#endif // HAVE_CONFIG_H

#include <errno.h>

#include "sys.h"
#include "sys-internal.h"

/*
 * This function is one of the rare ones *NOT* to use sys_context
 * as it is really low-level and should not log anything and/or
 * call malloc-style funcs.
 */
void
_lw6sys_caller_info_set (_lw6sys_caller_info_t * caller_info, const char *file, int line, const char *func)
{
  const char *file_only;

  file_only = _lw6sys_path_file_only_raw (file);

  strncpy (caller_info->file, file_only, _LW6SYS_CALLER_INFO_FILE_SIZE - 1);
  caller_info->file[_LW6SYS_CALLER_INFO_FILE_SIZE - 1] = '\0';
  caller_info->line = line;
  strncpy (caller_info->func, func, _LW6SYS_CALLER_INFO_FUNC_SIZE - 1);
  caller_info->func[_LW6SYS_CALLER_INFO_FUNC_SIZE - 1] = '\0';
}

/*
 * This function is one of the rare ones *NOT* to use sys_context
 * as it is really low-level and should not log anything and/or
 * call malloc-style funcs.
 */
void
_lw6sys_caller_info_clear (_lw6sys_caller_info_t * caller_info)
{
  memset (caller_info, 0, sizeof (_lw6sys_caller_info_t));
}
