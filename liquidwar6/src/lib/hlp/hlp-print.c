/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "hlp.h"

static void
print_keyword_callback (void *func_data, void *data)
{
  FILE *f;
  char *entry = NULL;

  f = (FILE *) func_data;
  entry = (char *) data;
  if (entry)
    {
      fprintf (f, "%s\n", entry);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to print NULL help keyword"));
    }
}

static void
print_content_callback (void *func_data, void *data)
{
  FILE *f;
  char *entry = NULL;
  char *desc = NULL;
  const char *about;
  lw6hlp_type_t type;

  f = (FILE *) func_data;
  entry = (char *) data;
  if (entry)
    {
      about = lw6hlp_about (&type, NULL, NULL, NULL, entry);
      if (lw6hlp_match (entry, LW6DEF_ABOUT))
	{
	  /*
	   * OK this is ugly, but so much less complicated than handling
	   * a "does this have a parameter?" flag, especially when this
	   * is the only exception. On the other side that's typically
	   * the option that requires a *precise* help entry, and
	   * moreover this is displayed by '--help', so, I hacked it.
	   * Note that ./doc/selfdoc.pl does the same.
	   */
	  fprintf (f, "--about=<keyword>\n");
	}
      else
	{
	  fprintf (f, "--%s\n", entry);
	}
      desc =
	lw6sys_str_reformat (about, LW6SYS_REFORMAT_INDENT_PREFIX,
			     LW6SYS_REFORMAT_INDENT_NB_COLUMNS);
      if (desc)
	{
	  fprintf (f, "%s", desc);
	  LW6SYS_FREE (desc);
	}
      fprintf (f, "\n");
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("trying to print NULL help keyword"));
    }
}

/**
 * lw6hlp_print_keyword
 *
 * @list: a pointer to a list of keywords
 * @f: the file to print the content to
 *
 * Prints all the keywords from the list. One keyword per line.
 *
 * Return value: none.
 */
void
lw6hlp_print_keyword (lw6sys_list_t ** list, FILE * f)
{
  lw6sys_sort (list, lw6sys_sort_str_callback);
  lw6sys_list_map (*list, print_keyword_callback, (void *) f);
}

/**
 * lw6hlp_print_content
 *
 * @list: a pointer to a list of keywords
 * @f: the file to print the content to
 *
 * Prints all the keywords from the list, with the associated
 * keyword help, to the given file. Output is formatted to fit
 * on the standard terminal/console.
 *
 * Return value: none.
 */
void
lw6hlp_print_content (lw6sys_list_t ** list, FILE * f)
{
  lw6sys_sort (list, lw6sys_sort_str_callback);
  lw6sys_list_map (*list, print_content_callback, (void *) f);
}

/**
 * lw6hlp_print_about
 *
 * @keyword: the keyword to print help about
 * @f: the file to print the content to
 *
 * Displays the help about a keyword, to a file, directly.
 * It's formatted for the purpose of the --about=<value> option.
 *
 * Return value: none
 */
void
lw6hlp_print_about (const char *keyword, FILE * f)
{
  char *doc_str = NULL;
  lw6hlp_type_t type = LW6HLP_TYPE_VOID;
  const char *default_value = NULL;
  int min_value = 0;
  int max_value = 0;
  char *type_str = NULL;
  char *tmp = NULL;
  int values_set = 0;

  doc_str =
    lw6sys_str_reformat (lw6hlp_about
			 (&type, &default_value, &min_value, &max_value,
			  keyword), LW6SYS_REFORMAT_DEFAULT_PREFIX,
			 LW6SYS_REFORMAT_DEFAULT_NB_COLUMNS);
  if (doc_str)
    {
      switch (type)
	{
	case LW6HLP_TYPE_STR:
	  type_str = _x_ ("string");
	  break;
	case LW6HLP_TYPE_INT:
	  type_str = _x_ ("integer");
	  break;
	case LW6HLP_TYPE_FLOAT:
	  type_str = _x_ ("float");
	  break;
	case LW6HLP_TYPE_BOOL:
	  type_str = _x_ ("boolean");
	  break;
	case LW6HLP_TYPE_COLOR:
	  type_str = _x_ ("color");
	  break;
	default:
	  // stay NULL
	  break;
	}
      if (type_str && type != LW6HLP_TYPE_VOID)
	{
	  tmp = lw6sys_new_sprintf (_x_ ("Type: %s"), type_str);
	  if (tmp)
	    {
	      fprintf (f, "%s\n\n", tmp);
	      LW6SYS_FREE (tmp);
	    }
	}
      if (default_value)
	{
	  /*
	   * Previous versions of the game used to print a dot "."
	   * after the default value but this is confusing when
	   * the default is "." or "" ...
	   */
	  tmp = lw6sys_new_sprintf (_x_ ("Default value: %s"), default_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (tmp);
	    }
	}
      if (min_value != 0 || max_value != 0)
	{
	  tmp = lw6sys_new_sprintf (_x_ ("Min value: %d"), min_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (tmp);
	    }
	  tmp = lw6sys_new_sprintf (_x_ ("Max value: %d"), max_value);
	  if (tmp)
	    {
	      values_set = 1;
	      fprintf (f, "%s\n", tmp);
	      LW6SYS_FREE (tmp);
	    }
	}
      if (values_set)
	{
	  fprintf (f, "\n");
	}
      fprintf (f, "%s", doc_str);
      fflush (stdout);
      LW6SYS_FREE (doc_str);
    }
}
