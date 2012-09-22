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
#include <expat.h>

#include "cfg.h"
#include "cfg-internal.h"

/**
 * lw6cfg_xml_element
 *
 * @type: type as an enum
 *
 * Returns the string corresponding to a given type, suitable for
 * use in XML files. For instance if you pass @LW6HLP_TYPE_INT then
 * you will obtain the string int (string of 3 chars containing i, n 
 * and t.
 *
 * Return value: string, must not be freed.
 */
char *
lw6cfg_xml_element (lw6hlp_type_t type)
{
  char *xml_type = "";

  switch (type)
    {
    case LW6HLP_TYPE_INT:
      xml_type = LW6CFG_XML_INT;
      break;
    case LW6HLP_TYPE_BOOL:
      xml_type = LW6CFG_XML_BOOL;
      break;
    case LW6HLP_TYPE_FLOAT:
      xml_type = LW6CFG_XML_FLOAT;
      break;
    case LW6HLP_TYPE_STR:
      xml_type = LW6CFG_XML_STRING;
      break;
    case LW6HLP_TYPE_COLOR:
      xml_type = LW6CFG_XML_COLOR;
      break;
    default:
      xml_type = "";
      break;
    }

  return xml_type;
}

/**
 * lw6cfg_read_xml_int
 *
 * @xml_key: key found in XML file
 * @xml_value: value found in XML file
 * @target_key: key we're searching for
 * @value: the value if found (out param)
 *
 * Tries to find a value in a key/value pair. If @xml_key and @target_key
 * match, then will put the corresponding value (converted to int) in
 * the @value param. Typically, you would call this in a loop on every
 * single entry found in an XML file, in an expat callback.
 *
 * Return value: none.
 */
void
lw6cfg_read_xml_int (const char *xml_key, const char *xml_value,
		     const char *target_key, int *value)
{
  if (!strcasecmp (xml_key, target_key))
    {
      if (value)
	{
	  (*value) = lw6sys_atoi (xml_value);
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("XML int \"%s\"=\"%d\""), xml_key,
		      *value);
	}
    }
}

/**
 * lw6cfg_read_xml_bool
 *
 * @xml_key: key found in XML file
 * @xml_value: value found in XML file
 * @target_key: key we're searching for
 * @value: the value if found (out param)
 *
 * Tries to find a value in a key/value pair. If @xml_key and @target_key
 * match, then will put the corresponding value (converted to boolean) in
 * the @value param. Typically, you would call this in a loop on every
 * single entry found in an XML file, in an expat callback.
 *
 * Return value: none.
 */
void
lw6cfg_read_xml_bool (const char *xml_key, const char *xml_value,
		      const char *target_key, int *value)
{
  if (!strcasecmp (xml_key, target_key))
    {
      if (value)
	{
	  (*value) = lw6sys_atoi (xml_value);
	  if (!(*value))
	    {
	      /*
	       * OK, trying to convert the value to a number gives 0,
	       * we just try if it could be a string like "true", "yes"
	       * on on.
	       * Note that prefered value is "true" 8-)
	       */
	      (*value) = lw6sys_atob (xml_value);
	      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("XML bool \"%s\"=\"%d\""),
			  xml_key, *value);
	    }
	}
    }
}

/**
 * lw6cfg_read_xml_float
 *
 * @xml_key: key found in XML file
 * @xml_value: value found in XML file
 * @target_key: key we're searching for
 * @value: the value if found (out param)
 *
 * Tries to find a value in a key/value pair. If @xml_key and @target_key
 * match, then will put the corresponding value (converted to float) in
 * the @value param. Typically, you would call this in a loop on every
 * single entry found in an XML file, in an expat callback.
 *
 * Return value: none.
 */
void
lw6cfg_read_xml_float (const char *xml_key, const char *xml_value,
		       const char *target_key, float *value)
{
  if (!strcasecmp (xml_key, target_key))
    {
      if (value)
	{
	  (*value) = lw6sys_atof (xml_value);
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("XML float \"%s\"=\"%f\""),
		      xml_key, *value);
	}
    }
}

/**
 * lw6cfg_read_xml_string
 *
 * @xml_key: key found in XML file
 * @xml_value: value found in XML file
 * @target_key: key we're searching for
 * @value: the value if found (out param)
 *
 * Tries to find a value in a key/value pair. If @xml_key and @target_key
 * match, then will put the corresponding value (as a string) in
 * the @value param. Typically, you would call this in a loop on every
 * single entry found in an XML file, in an expat callback.
 *
 * Return value: none.
 */
void
lw6cfg_read_xml_string (const char *xml_key, const char *xml_value,
			const char *target_key, char **value)
{
  if (!strcasecmp (xml_key, target_key))
    {
      if (value)
	{
	  if (*value)
	    {
	      /*
	       * This implies that *value is zeroed at initialisation,
	       * or filled with a dynamically allocated string.
	       */
	      LW6SYS_FREE (*value);
	      (*value) = NULL;
	    }
	  (*value) = lw6sys_str_copy (xml_value);
	  if (*value)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("XML string \"%s\"=\"%s\""),
			  xml_key, *value);
	    }
	}
    }
}

/**
 * lw6cfg_read_xml_color
 *
 * @xml_key: key found in XML file
 * @xml_value: value found in XML file
 * @target_key: key we're searching for
 * @value: the value if found (out param)
 *
 * Tries to find a value in a key/value pair. If @xml_key and @target_key
 * match, then will put the corresponding value (converted to a color) in
 * the @value param. Typically, you would call this in a loop on every
 * single entry found in an XML file, in an expat callback.
 *
 * Return value: none.
 */
void
lw6cfg_read_xml_color (const char *xml_key, const char *xml_value,
		       const char *target_key, lw6sys_color_8_t * value)
{
  if (!strcasecmp (xml_key, target_key))
    {
      (*value) = lw6sys_color_a_to_8 (xml_value);
      lw6sys_log (LW6SYS_LOG_INFO,
		  _x_ ("XML color \"%s\"=\"#%02x%02x%02x%02x\""), xml_key,
		  value->r, value->g, value->b, value->a);
    }
}

static void XMLCALL
element_start (void *data, const char *element, const char **attr)
{
  _lw6cfg_xml_callback_t *callback;
  char *key = NULL;
  char *value = NULL;
  char **at;

  callback = (_lw6cfg_xml_callback_t *) data;

  if (callback && callback->func)
    {
      for (at = (char **) attr; (*at) != NULL; at += 2)
	{
	  if (strcasecmp (LW6CFG_XML_KEY, (*at)) == 0)
	    {
	      key = *(at + 1);
	    }
	  if (strcasecmp (LW6CFG_XML_VALUE, (*at)) == 0)
	    {
	      value = *(at + 1);
	    }
	}
      if (key && value)
	{
	  callback->func (callback->data, (char *) element, key, value);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("XML callback function is NULL"));
    }
}

static void XMLCALL
element_end (void *data, const char *el)
{

}

/**
 * lw6cfg_read_key_value_xml_file
 *
 * @filename: full path of file to read
 * @callback_func: callback function to be called on each element
 * @callback_data: additionnal pointer passed to callback function
 *
 * Will parse a file and call the given callback on each element.
 * This is an over-simplified way to read XML file, in fact we just
 * explain plain non-nested simple elements but this is exactly what
 * LW config files are made of.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6cfg_read_key_value_xml_file (const char *filename,
				lw6cfg_read_xml_callback_func_t callback_func,
				void *callback_data)
{
  int ret = 0;
  char *content = NULL;
  _lw6cfg_xml_callback_t *callback;

  callback = LW6SYS_MALLOC (sizeof (_lw6cfg_xml_callback_t));
  if (callback)
    {
      callback->func = callback_func;
      callback->data = callback_data;

      content = lw6sys_read_file_content (filename);
      if (content)
	{
	  int length = strlen (content);
	  XML_Parser parser;

	  parser = XML_ParserCreate (NULL);
	  if (parser)
	    {
	      XML_SetElementHandler (parser, element_start, element_end);
	      XML_SetUserData (parser, (void *) callback);

	      if (XML_Parse (parser, content, length, 1) != XML_STATUS_ERROR)
		{
		  ret = 1;
		}
	      else
		{
#ifdef LW6_MS_WINDOWS
		  /*
		   * For some reason there's an error ad end-of-file because
		   * of a ^M after </liquidwar6>. This message is only
		   * INFO and not warning on MS-Windows, this avoids
		   * stupid warning at startup.
		   */
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _x_
			      ("parse error reading XML file \"%s\" at line %d: \"%s\""),
			      filename,
			      XML_GetCurrentLineNumber (parser),
			      XML_ErrorString (XML_GetErrorCode (parser)));
#else
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("parse error reading XML file \"%s\" at line %d: \"%s\""),
			      filename,
			      (int) XML_GetCurrentLineNumber (parser),
			      XML_ErrorString (XML_GetErrorCode (parser)));
#endif
		}
	      XML_ParserFree (parser);
	    }
	  LW6SYS_FREE (content);
	}
      LW6SYS_FREE (callback);
    }

  return ret;
}

// if type is NULL, will be guessed automatically
static void
write_xml (FILE * f, const char *type, const char *key, const char *value)
{
  char *tmp = NULL;
  int pos = 0;
  const char *guessed_type = NULL;
  char *help_string = NULL;
  const char *hlp_about = NULL;
  lw6hlp_type_t hlp_type = LW6HLP_TYPE_STR;
  const char *hlp_default_value = NULL;
  int hlp_min_value = 0;
  int hlp_max_value = 0;

  if (lw6cfg_must_be_saved (key))
    {
      hlp_about =
	lw6hlp_about (&hlp_type, &hlp_default_value, &hlp_min_value,
		      &hlp_max_value, key);
      if (hlp_about)
	{
	  if (hlp_default_value)
	    {
	      if (hlp_min_value != 0 || hlp_max_value != 0)
		{
		  tmp =
		    lw6sys_new_sprintf
		    ("\"%s\" (default=\"%s\", min=\"%d\", max=\"%d\"): %s",
		     key, hlp_default_value, hlp_min_value, hlp_max_value,
		     hlp_about);
		}
	      else
		{
		  tmp =
		    lw6sys_new_sprintf ("\"%s\" (default=\"%s\"): %s", key,
					hlp_default_value, hlp_about);
		}
	    }
	  else
	    {
	      if (hlp_min_value != 0 || hlp_max_value != 0)
		{
		  tmp =
		    lw6sys_new_sprintf ("\"%s\" (min=\"%d\", max=\"%d\"): %s",
					key, hlp_min_value, hlp_max_value,
					hlp_about);
		}
	      else
		{
		  tmp = lw6sys_new_sprintf ("\"%s\": %s", key, hlp_about);
		}
	    }
	}
      if (tmp)
	{
	  help_string =
	    lw6sys_str_reformat (tmp, LW6SYS_REFORMAT_XML_PREFIX,
				 LW6SYS_REFORMAT_XML_NB_COLUMNS);
	  LW6SYS_FREE (tmp);
	}
      switch (hlp_type)
	{
	case LW6HLP_TYPE_STR:
	  guessed_type = LW6CFG_XML_STRING;
	  break;
	case LW6HLP_TYPE_INT:
	  guessed_type = LW6CFG_XML_INT;
	  break;
	case LW6HLP_TYPE_BOOL:
	  guessed_type = LW6CFG_XML_BOOL;
	  break;
	case LW6HLP_TYPE_FLOAT:
	  guessed_type = LW6CFG_XML_FLOAT;
	  break;
	case LW6HLP_TYPE_COLOR:
	  guessed_type = LW6CFG_XML_COLOR;
	  break;
	default:
	  guessed_type = NULL;	// LW6HLP_TYPE_VOID
	}
      if (guessed_type)
	{
	  if (help_string)
	    {
	      fprintf (f, "  <!--%s", lw6sys_eol ());
	      fprintf (f, "%s", help_string);
	      fprintf (f, "  -->%s", lw6sys_eol ());
	    }
	  if (type && strcasecmp (type, guessed_type))
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  ("inconsistency in XML entry type (\"%s\": \"%s!=%s\""),
			  key, type, guessed_type);
	    }
	  if (!type)
	    {
	      type = guessed_type;
	    }
	  fprintf (f, "  <%s %s=\"%s\" %s=\"", type, LW6CFG_XML_KEY,
		   key, LW6CFG_XML_VALUE);
	  for (pos = 0; value[pos] != '\0'; ++pos)
	    {
	      switch (value[pos])
		{
		case '<':
		  fprintf (f, "&lt;");
		  break;
		case '>':
		  fprintf (f, "&gt;");
		  break;
		case '&':
		  fprintf (f, "&amp;");
		  break;
		case '"':
		  fprintf (f, "&quot;");
		  break;
		case '\'':
		  fprintf (f, "&apos;");
		  break;
		default:
		  fprintf (f, "%c", value[pos]);
		}
	    }
	  fprintf (f, "\"/>%s", lw6sys_eol ());
	}
      if (help_string)
	{
	  LW6SYS_FREE (help_string);
	}
    }
}

/**
 * lw6cfg_write_xml_int
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes an int entry into an opened XML file.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_int (FILE * f, const char *key, int value)
{
  char *str_value = NULL;

  str_value = lw6sys_itoa (value);
  if (str_value)
    {
      write_xml (f, LW6CFG_XML_INT, key, str_value);
      LW6SYS_FREE (str_value);
    }
}

/**
 * lw6cfg_write_xml_bool
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes a boolean entry into an opened XML file.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_bool (FILE * f, const char *key, int value)
{
  char *str_value = NULL;

  str_value = lw6sys_btoa (value);
  if (str_value)
    {
      write_xml (f, LW6CFG_XML_BOOL, key, str_value);
      LW6SYS_FREE (str_value);
    }
}

/**
 * lw6cfg_write_xml_float
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes a float entry into an opened XML file.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_float (FILE * f, const char *key, float value)
{
  char *str_value = NULL;

  str_value = lw6sys_ftoa (value);
  if (str_value)
    {
      write_xml (f, LW6CFG_XML_FLOAT, key, str_value);
      LW6SYS_FREE (str_value);
    }
}

/**
 * lw6cfg_write_xml_string
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes a string entry into an opened XML file.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_string (FILE * f, const char *key, const char *value)
{
  write_xml (f, LW6CFG_XML_STRING, key, value);
}

/**
 * lw6cfg_write_xml_color
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes a color entry into an opened XML file.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_color (FILE * f, const char *key, lw6sys_color_8_t value)
{
  char *str_value = NULL;

  str_value = lw6sys_color_8_to_a (value);
  if (str_value)
    {
      write_xml (f, LW6CFG_XML_COLOR, key, str_value);
      LW6SYS_FREE (str_value);
    }
}

/**
 * lw6cfg_write_xml_guess_type
 *
 * @f: file to write data to (append mode)
 * @key: key to write
 * @value: value to write
 *
 * Writes an entry into an opened XML file, will try and guess
 * type from the internal help system, typically, if this is a
 * standard config file entry (the one documented by the about
 * command line function) it will pick up the right type. The
 * reason not to use this all the times is that sometimes, one
 * might to to store non-standard options, and additionnally,
 * guessing the type does consume some CPU.
 *
 * Return value: none.
 */
void
lw6cfg_write_xml_guess_type (FILE * f, const char *key, const char *value)
{
  write_xml (f, NULL, key, value);
}
