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

#include "msg.h"

/**
 * lw6msg_envelope_generate
 *
 * @mode: mode to use (a la TELNET or URL compatible)
 * @version: the program version to use (note: can be changed when testing)
 * @password_checksum: the password string to send
 * @physical_from_id: the sender id
 * @physical_to_id: the receiver id
 * @msg: the body of the message
 *
 * Generate an envelope, that is, the complete message sendable on the
 * network.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_envelope_generate (lw6msg_envelope_mode_t mode, char *version,
			  char *password_checksum, char *physical_from_id,
			  char *physical_to_id, char *msg)
{
  char *ret = NULL;
  char sep = '\0';
  char *lw6 = NULL;
  int need_base64 = 0;
  char *body = NULL;
  char *base64_str = NULL;

  switch (mode)
    {
    case LW6MSG_ENVELOPE_MODE_TELNET:
      sep = LW6MSG_TELNET_SEP;
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
      break;
    case LW6MSG_ENVELOPE_MODE_URL:
      sep = LW6MSG_URL_SEP;
      lw6 = LW6MSG_LW6_LC;
      need_base64 = 1;
      break;
    default:
      lw6sys_log (LW6SYS_LOG_WARNING, _("unknown mode %d"), (int) mode);
      sep = LW6MSG_TELNET_SEP;
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
    }

  body = msg;
  if (need_base64)
    {
      base64_str = lw6glb_base64_encode_str (msg);
      if (base64_str)
	{
	  body = base64_str;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to base64 encode, sending as is"));
	}
    }


  ret = lw6sys_new_sprintf ("%s%c%s%c%s%c%s%c%s%c%s",
			    lw6,
			    sep,
			    version,
			    sep,
			    password_checksum,
			    sep,
			    physical_from_id, sep, physical_to_id, sep, body);

  if (base64_str)
    {
      LW6SYS_FREE (base64_str);
    }

  return ret;
}

/**
 * lw6msg_envelope_analyse
 *
 * @envelope: the envelope to analyse
 * @mode: mode to use (a la TELNET or URL compatible)
 * @local_url: the url of local server (usefull for password)
 * @password: the password to check against
 * @expected_physical_from_id: the sender id, if NULL, no check performed
 * @expected_physical_to_id: the receiver id, if NULL, no check performed
 * @msg: if not NULL, will contain body of the message
 * @physical_from_id: if not NULL, will contain sender id
 * @physical_from_url: if not NULL and if message allows, will contain sender public URL
 *
 * Generate an envelope, that is, the complete message sendable on the
 * network.
 *
 * Return value: newly allocated string.
 */
int
lw6msg_envelope_analyse (char *envelope, lw6msg_envelope_mode_t mode,
			 char *local_url, char *password,
			 char *expected_physical_from_id,
			 char *expected_physical_to_id, char **msg,
			 u_int64_t * physical_from_id,
			 char **physical_from_url)
{
  int ret = 0;
  char sep = '\0';
  char *lw6 = NULL;
  int need_base64 = 0;
  char *pos = NULL;
  char *seek = NULL;
  char *version = NULL;
  lw6msg_word_t received_lw6;
  lw6msg_word_t received_version;
  lw6msg_word_t received_password;
  u_int64_t received_physical_from_id = 0;
  u_int64_t received_physical_to_id = 0;

  switch (mode)
    {
    case LW6MSG_ENVELOPE_MODE_TELNET:
      sep = LW6MSG_TELNET_SEP;
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
      break;
    case LW6MSG_ENVELOPE_MODE_URL:
      sep = LW6MSG_URL_SEP;
      lw6 = LW6MSG_LW6_LC;
      need_base64 = 1;
      break;
    default:
      lw6sys_log (LW6SYS_LOG_WARNING, _("unknown mode %d"), (int) mode);
      sep = LW6MSG_TELNET_SEP;
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
    }

  pos = seek = envelope;
  if (lw6msg_word_first_x (&received_lw6, &seek, pos))
    {
      if (lw6sys_str_is_same (received_lw6.buf, lw6))
	{
	  pos = seek;
	  if (lw6msg_word_first_x (&received_version, &seek, pos))
	    {
	      if (lw6sys_str_is_same (received_version.buf, version))
		{
		  pos = seek;
		  if (lw6msg_word_first_x (&received_password, &seek, pos))
		    {
		      if (lw6sys_password_verify
			  (local_url, password, received_password.buf))
			{
			  pos = seek;
			  if (lw6msg_word_first_id_64
			      (&received_physical_from_id, &seek, pos))
			    {
			      if (expected_physical_from_id == NULL
				  ||
				  (lw6sys_id_atol (expected_physical_from_id)
				   == received_physical_from_id))
				{
				  pos = seek;
				  if (lw6msg_word_first_id_64
				      (&received_physical_to_id, &seek, pos))
				    {
				      if (expected_physical_from_id == NULL
					  ||
					  (lw6sys_id_atol
					   (expected_physical_to_id) ==
					   received_physical_to_id))
					{
					  pos = seek;
					  ret = 1;
					  if (msg)
					    {
					      if (need_base64)
						{
						  (*msg) =
						    lw6glb_base64_decode_str
						    (pos);
						}
					      else
						{
						  (*msg) =
						    lw6sys_str_copy (pos);
						}
					    }
					  if (physical_from_id)
					    {
					      (*physical_from_id) =
						received_physical_from_id;
					    }
					  if (physical_from_url)
					    {
					      (*physical_from_url) =
						lw6msg_cmd_guess_from_url
						(pos);
					    }
					}
				      else
					{
					  lw6sys_log (LW6SYS_LOG_DEBUG,
						      _
						      ("wrong \"physical_to\" (receiver) id"));
					}
				    }
				  else
				    {
				      lw6sys_log (LW6SYS_LOG_DEBUG,
						  _
						  ("can't parse receiver id"));
				    }
				}
			      else
				{
				  lw6sys_log (LW6SYS_LOG_DEBUG,
					      _
					      ("wrong \"physical_from\" (sender) id"));
				}
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_DEBUG,
					  _("can't parse sender id"));
			    }
			}
		      else
			{
			  lw6sys_log (LW6SYS_LOG_DEBUG, _("bad password"));
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _("can't parse password"));
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _("bad version, received \"%s\", need \"%s\""),
			      received_version.buf, version);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("can't parse version"));
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("bad lw6 key, received \"%s\", need \"%s\""),
		      received_lw6.buf, lw6);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("message does not start with \"%s\""),
		  lw6);
    }

  return ret;
}
