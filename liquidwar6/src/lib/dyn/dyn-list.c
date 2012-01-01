/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "../sys/sys.h"
#include "dyn.h"
#include "dyn-internal.h"

#include <dirent.h>
#include <ctype.h>
#include <limits.h>

#define LIST_DEVEL_DIR1_FORMAT "%ssrc/lib/%s"
#define LIST_DEVEL_DIR3_NAME ".libs"
#define LIST_DEVEL_MOD_PREFIX_STR "mod-"
#define LIST_DEVEL_MOD_PREFIX_SIZE 4

#define LIST_SYSTEM_DIR_FORMAT "%s/%s"
#define LIST_SYSTEM_FILE_MATCH "libmod_x_(.*)-.*." _LW6DYN_SUFFIX

#define LIST_LIBMOD_PREFIX_STR "libmod_"
#define LIST_LIBMOD_PREFIX_SIZE 7
#define LIST_SO_STR "." _LW6DYN_SUFFIX

static int
add_backend (lw6sys_assoc_t ** list, const char *libdir, const char *filename,
	     const char *id)
{
  int ret = 0;
  char *so_file = NULL;
  char *get_pedigree_func_str = NULL;
  lw6sys_module_pedigree_t *(*get_pedigree_func) () = NULL;
  lw6sys_module_pedigree_t *module_pedigree = NULL;
  lw6dyn_dl_handle_t *backend_handle = NULL;

  so_file = lw6sys_path_concat (libdir, filename);
  if (so_file)
    {
      backend_handle = lw6dyn_dlopen_backend_so (so_file);
      if (backend_handle)
	{
	  get_pedigree_func_str =
	    lw6sys_new_sprintf (LW6DYN_GET_PEDIGREE_FUNC_FORMAT, id);
	  if (get_pedigree_func_str)
	    {
	      get_pedigree_func =
		lw6dyn_dlsym (backend_handle, get_pedigree_func_str);
	      if (get_pedigree_func)
		{
		  module_pedigree =
		    (lw6sys_module_pedigree_t *) (get_pedigree_func) ();
		  if (module_pedigree && module_pedigree->id
		      && module_pedigree->name)
		    {
		      lw6sys_assoc_set (list, module_pedigree->id,
					lw6sys_str_copy
					(module_pedigree->name));
		      if (*list)
			{
			  ret = 1;
			}
		      LW6SYS_FREE (module_pedigree);
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _x_
				  ("can't get pedigree for mod_%s in \"%s\""),
				  id, so_file);
		    }
		}
	      LW6SYS_FREE (get_pedigree_func_str);
	    }
	  lw6dyn_dlclose_backend (backend_handle);
	}
      LW6SYS_FREE (so_file);
    }

  return ret;
}

static void
update_list (lw6sys_assoc_t ** list, const char *path)
{
  DIR *dir_handle = NULL;
  struct dirent *dir_entry = NULL;
  struct dirent *dir_entry_result = NULL;
  int dir_entry_size;
  int eod = 0;
  char *id = NULL;
  char *pos = NULL;

  dir_handle = opendir (path);
  if (dir_handle)
    {
      while (!eod)
	{
	  dir_entry_size = sizeof (struct dirent) + NAME_MAX + 1;
	  dir_entry = (struct dirent *) LW6SYS_CALLOC (dir_entry_size);
	  if (dir_entry)
	    {
	      readdir_r (dir_handle, dir_entry, &dir_entry_result);
	      if (dir_entry_result && (dir_entry_result == dir_entry))
		{
		  if (dir_entry->d_name
		      && strstr (dir_entry->d_name, LIST_SO_STR)
		      && !strncmp (LIST_LIBMOD_PREFIX_STR, dir_entry->d_name,
				   LIST_LIBMOD_PREFIX_SIZE))
		    {
		      id =
			lw6sys_str_copy (dir_entry->d_name +
					 LIST_LIBMOD_PREFIX_SIZE);
		      if (id)
			{
			  pos = id;
			  while (isalnum (*pos))
			    {
			      pos++;
			    }
			  (*pos) = '\0';
			  add_backend (list, path, dir_entry->d_name, id);
			  LW6SYS_FREE (id);
			}
		    }
		}
	      else
		{
		  eod = 1;
		}
	      LW6SYS_FREE (dir_entry);
	    }
	}
      closedir (dir_handle);
    }
}

static void
update_devel_list (lw6sys_assoc_t ** list, const char *top_level_lib,
		   int depth)
{
  char *dir1 = NULL;
  char *dir2 = NULL;
  char *dir3 = NULL;
  DIR *dir_handle = NULL;
  struct dirent *dir_entry = NULL;
  struct dirent *dir_entry_result = NULL;
  int dir_entry_size;
  int eod = 0;

  if (depth >= 0 && depth < _LW6DYN_DEVEL_NB_DEPTHS)
    {
      dir1 =
	lw6sys_new_sprintf (LIST_DEVEL_DIR1_FORMAT,
			    _LW6DYN_DEVEL_DEPTH_STRINGS
			    [depth], top_level_lib);
      if (dir1)
	{
	  if (lw6sys_dir_exists (dir1))
	    {
	      dir_handle = opendir (dir1);
	      if (dir_handle)
		{
		  while (!eod)
		    {
		      dir_entry_size = sizeof (struct dirent) + NAME_MAX + 1;
		      dir_entry =
			(struct dirent *) LW6SYS_CALLOC (dir_entry_size);
		      if (dir_entry)
			{
			  readdir_r (dir_handle, dir_entry,
				     &dir_entry_result);
			  if (dir_entry_result
			      && (dir_entry_result == dir_entry))
			    {
			      if (dir_entry->d_name
				  && !strncmp (LIST_DEVEL_MOD_PREFIX_STR,
					       dir_entry->d_name,
					       LIST_DEVEL_MOD_PREFIX_SIZE))
				{
				  dir2 =
				    lw6sys_path_concat (dir1,
							dir_entry->d_name);
				  if (dir2)
				    {
				      dir3 = lw6sys_path_concat (dir2,
								 LIST_DEVEL_DIR3_NAME);
				      if (dir3)
					{
					  if (lw6sys_dir_exists (dir3))
					    {
					      update_list (list, dir3);
					    }
					  LW6SYS_FREE (dir3);
					}
				      LW6SYS_FREE (dir2);
				    }
				}
			    }
			  else
			    {
			      eod = 1;
			    }
			  LW6SYS_FREE (dir_entry);
			}
		    }
		  closedir (dir_handle);
		}
	    }
	  LW6SYS_FREE (dir1);
	}
    }
}

static void
update_system_list (lw6sys_assoc_t ** list, int argc,
		    const char *argv[], const char *top_level_lib)
{
  char *mod_dir = NULL;
  char *libdir = NULL;

  mod_dir = lw6sys_get_mod_dir (argc, argv);
  if (mod_dir)
    {
      libdir = lw6sys_path_concat (mod_dir, top_level_lib);
      if (libdir)
	{
	  update_list (list, libdir);
	  LW6SYS_FREE (libdir);
	}
      LW6SYS_FREE (mod_dir);
    }
}

/**
 * lw6dyn_list_backends:
 *
 * @argc: the number of command line args, as passed to main
 * @argv: the commind line args, as passed to main
 * @top_level_lib: the library category to query (gfx, snd, cli, srv ...)
 *
 * Returns an assoc which lists all the available modules. The key
 * of the assoc entries in the module internal name such as 'gl'
 * and the value associated is a NULL terminated string describing
 * the module, for instance 'OpenGL'.
 *
 * Return value: an assoc object containing key/label pairs.
 */
lw6sys_assoc_t *
lw6dyn_list_backends (int argc, const char *argv[], const char *top_level_lib)
{
  lw6sys_assoc_t *ret = NULL;
  int depth = 0;

  ret = lw6sys_assoc_new (lw6sys_free_callback);
  if (ret)
    {
      for (depth = 0; depth < _LW6DYN_DEVEL_NB_DEPTHS; ++depth)
	{
	  if (ret)
	    {
	      update_devel_list (&ret, top_level_lib, depth);
	    }
	}
      if (ret)
	{
	  update_system_list (&ret, argc, argv, top_level_lib);
	}
      if (ret)
	{
	  lw6sys_assoc_sort_and_map (ret, NULL, NULL);
	}
    }

  return ret;
}
