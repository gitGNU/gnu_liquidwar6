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

#include <string.h>
#include <unistd.h>

#include "sys.h"
#include "sys-internal.h"

#ifdef LW6_MS_WINDOWS
#include <winbase.h>
#else
#ifdef LW6_MAC_OS_X
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#else
#ifdef HAVE_SYS_SYSINFO_H
#include <sys/types.h>
#include <sys/sysinfo.h>
#endif // HAVE_SYS_SYSINFO_H
#endif // LW6_MAC_OS_X
#endif // LW6_MS_WINDOWS

#define MEM_DIVIDE 1048576

/**
 * lw6sys_malloc
 *
 * @size: number of bytes to allocate.
 * @file: name of the file calling the function, use @__FILE__x_
 * @line: line in the file calling the function, use @__LINE__x_
 *
 * This is a wrapper over the standard @malloc function. Additionnally
 * it will keep track of the call with an internal program-wide counter,
 * thus enabling memory leak checks. You should not use this function
 * directly but use the macro @LW6SYS_MALLOC which has the same syntax,
 * without the last two parameters, which are automatically provided
 * by macro expansion.
 *
 * Return value: the newly allocated pointer. Data is not initialized.
 */
void *
lw6sys_malloc (int size, const char *file, int line)
{
  void *ptr;

#ifdef LW6_PARANOID
  size = lw6sys_imax (size, _LW6SYS_BAZOOKA_ALLOC_MIN);
#endif

  ptr = malloc (size);

  if (ptr != NULL)
    {
#ifndef LW6_OPTIMIZE
      _lw6sys_bazooka_register_malloc (ptr, size, file, line);
#endif
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("out of memory malloc (%d) failed in %s:%d"), size, file,
		  line);
    }

  return ptr;
}

/**
 * lw6sys_calloc
 *
 * @size: number of bytes to allocate.
 * @file: name of the file calling the function, use @__FILE__x_
 * @line: line in the file calling the function, use @__LINE__x_
 *
 * This is a wrapper over the standard @calloc function. Additionnally
 * it will keep track of the call with an internal program-wide counter,
 * thus enabling memory leak checks. You should not use this function
 * directly but use the macro @LW6SYS_CALLOC which has the same syntax,
 * without the last two parameters, which are automatically provided
 * by macro expansion.
 *
 * Return value: the newly allocated pointer. Data is filled with zeros.
 */
void *
lw6sys_calloc (int size, const char *file, int line)
{
  void *ptr;

#ifdef LW6_PARANOID
  size = lw6sys_imax (size, _LW6SYS_BAZOOKA_ALLOC_MIN);
#endif

  ptr = calloc (size, sizeof (char));

  if (ptr != NULL)
    {
#ifndef LW6_OPTIMIZE
      _lw6sys_bazooka_register_calloc (ptr, size, file, line);
#endif
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("out of memory calloc(%d) failed in %s:%d"), size, file,
		  line);
    }

  return ptr;
}

/**
 * lw6sys_realloc
 *
 * @ptr: the pointer to reallocate.
 * @size: number of bytes to allocate.
 * @file: name of the file calling the function, use @__FILE__x_
 * @line: line in the file calling the function, use @__LINE__x_
 *
 * This is a wrapper over the standard @realloc function.
 * You should not use this function
 * directly but use the macro @LW6SYS_REALLOC which has the same syntax,
 * without the last two parameters, which are automatically provided
 * by macro expansion.
 *
 * Return value: the newly allocated pointer.
 */
void *
lw6sys_realloc (void *ptr, int size, const char *file, int line)
{
  void *ptr2;

#ifdef LW6_PARANOID
  size = lw6sys_imax (size, _LW6SYS_BAZOOKA_ALLOC_MIN);
#endif

#ifndef LW6_OPTIMIZE
  if (!_lw6sys_bazooka_register_realloc_1 (ptr, size, file, line))
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("suspicious realloc (stage 1) at %s:%d"), file, line);
    }

#endif

  ptr2 = realloc (ptr, size);

  if (ptr2)
    {
#ifndef LW6_OPTIMIZE
      if (!_lw6sys_bazooka_register_realloc_2 (ptr, ptr2, size, file, line))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("suspicious realloc (stage 2) at %s:%d"), file,
		      line);
	}
#endif
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("out of memory realloc (%d) failed in %s:%d"), size,
		  file, line);
    }

  return ptr2;
}

/**
 * lw6sys_free
 *
 * @ptr: the pointer to free.
 * @file: name of the file calling the function, use @__FILE__x_
 * @line: line in the file calling the function, use @__LINE__x_
 *
 * This is a wrapper over the standard @free function. Additionnally
 * it will keep track of the call with an internal program-wide counter,
 * thus enabling memory leak checks. You should not use this function
 * directly but use the macro @LW6SYS_FREE which has the same syntax,
 * without the last two parameters, which are automatically provided
 * by macro expansion.
 *
 * Return value: none.
 */
void
lw6sys_free (void *ptr, const char *file, int line)
{
  if (ptr != NULL)
    {
#ifndef LW6_OPTIMIZE
      if (!_lw6sys_bazooka_register_free (ptr))
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("double free suspected at %s:%d"), file, line);
	}
#endif
      /*
       * It's important to free the pointer after registering, since
       * the later might overwrite memory in some cases.
       */
      free (ptr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("trying to free NULL pointer in %s:%d"), file, line);
    }
}

/**
 * lw6sys_free_callback
 *
 * @ptr: the pointer to free.
 *
 * This is a callback to be used when the @lw6sys_free does not fit.
 * A good example is a list, which, to free its elements, requires
 * you to provide a callback that only takes 1 arg, the pointer
 * to free. Problem, @lw6sys_free takes 3 args. And the @LW6SYS_FREE
 * macro is not usable in such a context. And you can't use standard @free
 * either for it would mess up the @malloc / @free automatical count
 * which is so convenient to track memory leaks. So this callback is
 * here, it's only drawback is that in case of an error, the error
 * will not be reported with the real file and line parameters. It's
 * still better than nothing.
 *
 * Return value: none.
 */
void
lw6sys_free_callback (void *ptr)
{
  LW6SYS_FREE (ptr);
}

/**
 * lw6sys_megabytes_available
 *
 * Gives a raw approximation of available memory, in megabytes.
 * Value is to be taken with distance, but it can give good hints
 * when system is running short of ressources.
 *
 * Return value: number of megabytes (physical memory) available.
 */
int
lw6sys_megabytes_available ()
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  MEMORYSTATUS status;

  memset (&status, 0, sizeof (status));
  status.dwLength = sizeof (status);

  GlobalMemoryStatus (&status);
  ret = status.dwAvailPhys / MEM_DIVIDE;
#else
#ifdef LW6_MAC_OS_X
  vm_size_t page_size;
  mach_port_t mach_port;
  mach_msg_type_number_t count;
  vm_statistics_data_t vm_stats;
  int64_t freeram = 0;

  mach_port = mach_host_self ();
  count = sizeof (vm_stats) / sizeof (natural_t);
  if (KERN_SUCCESS == host_page_size (mach_port, &page_size) &&
      KERN_SUCCESS == host_statistics (mach_port, HOST_VM_INFO,
				       (host_info_t) & vm_stats, &count))
    {
      freeram = (int64_t) vm_stats.free_count * (int64_t) page_size;
    }
  freeram /= MEM_DIVIDE;
  ret = freeram;
#else
#ifdef HAVE_SYS_SYSINFO_H
  /*
   * sys/sysinfo.h is Linux specific
   */
  struct sysinfo meminfo;
  int64_t freeram = 0;

  memset (&meminfo, 0, sizeof (struct sysinfo));
  sysinfo (&meminfo);
  freeram = meminfo.freeram;
  freeram *= meminfo.mem_unit;
  freeram /= MEM_DIVIDE;
  ret = freeram;
#else
#ifdef _SC_AVPHYS_PAGES
  /*
   * Fallback, seems to return low values, dunno why
   */
  ret = (sysconf (_SC_PAGESIZE) * sysconf (_SC_AVPHYS_PAGES)) / MEM_DIVIDE;
#else
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _x_ ("unable to guess how much free memory is available"));
#endif // _SC_AVPHYS_PAGES
#endif // HAVE_SYS_SYSINFO_H
#endif // LW6_MAC_OS_X
#endif // LW6_MS_WINDOWS

  return ret;
}

/**
 * lw6sys_is_big_endian
 *
 * Checks the endianess of the machine. PPC is big endian, for instance.
 *
 * Return value: 1 if system is big endian, 0 if little endian.
 */
int
lw6sys_is_big_endian ()
{
  return !lw6sys_is_little_endian ();
}

/**
 * lw6sys_is_little_endian
 *
 * Checks the endianess of the machine. x86 is little endian, for instance.
 *
 * Return value: 1 if system is little endian, 0 if big endian.
 */
int
lw6sys_is_little_endian ()
{
  int int_test = 1;
  char *char_test = (char *) &int_test;
  int ret = ((*char_test) != 0);

  return ret;
}

/**
 * lw6sys_check_types_size
 *
 * Checks of common types and usefull structures, this is a debugging
 * function which helps finding compiler strange behaviors and 
 * programmer's bad intuitions.
 *
 * Return value: 1 if everything is OK, 0 if error.
 */
int
lw6sys_check_types_size ()
{
  int ret = 1;

  if (sizeof (int8_t) != LW6SYS_SIZEOF_INT8)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("int8_t is %d bytes, should be %d"), sizeof (int8_t),
		  LW6SYS_SIZEOF_INT8);
      ret = 0;
    }
  if (sizeof (int16_t) != LW6SYS_SIZEOF_INT16)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("int16_t is %d bytes, should be %d"), sizeof (int16_t),
		  LW6SYS_SIZEOF_INT16);
      ret = 0;
    }
  if (sizeof (int32_t) != LW6SYS_SIZEOF_INT32)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("int32_t is %d bytes, should be %d"), sizeof (int32_t),
		  LW6SYS_SIZEOF_INT32);
      ret = 0;
    }
  if (sizeof (int64_t) != LW6SYS_SIZEOF_INT64)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("int64_t is %d bytes, should be %d"), sizeof (int64_t),
		  LW6SYS_SIZEOF_INT64);
      ret = 0;
    }
  if (sizeof (lw6sys_xyz_t) != LW6SYS_SIZEOF_XYZ)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("lw6sys_xyz_t is %d bytes, should be %d"),
		  sizeof (lw6sys_xyz_t), LW6SYS_SIZEOF_XYZ);
      ret = 0;
    }

  return ret;
}
