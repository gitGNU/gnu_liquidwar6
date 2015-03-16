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

#include "msg.h"

/**
 * lw6msg_envelope_generate
 *
 * @sys_context: global system context
 * @mode: mode to use (a la TELNET or URL compatible)
 * @version: the program version to use (note: can be changed when testing)
 * @password_checksum: the password string to send
 * @physical_ticket_sig: the signature of the message, calculated with ticket + physical from/to
 * @logical_ticket_sig: the signature of the message, calculated with ticket + logical from/to
 * @physical_from_id: the sender id
 * @physical_to_id: the receiver id
 * @logical_from_id: the message creator id
 * @logical_to_id: the message final destination id
 * @msg: the body of the message
 *
 * Generate an envelope, that is, the complete message sendable on the
 * network.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_envelope_generate (lw6sys_context_t * sys_context, lw6msg_envelope_mode_t mode, const char *version,
			  const char *password_checksum,
			  u_int32_t physical_ticket_sig,
			  u_int32_t logical_ticket_sig,
			  u_int64_t physical_from_id, u_int64_t physical_to_id, u_int64_t logical_from_id, u_int64_t logical_to_id, const char *msg)
{
  char *ret = NULL;
  char sep = '\0';
  char *lw6 = NULL;
  int need_base64 = 0;
  const char *body = NULL;
  char *base64_str = NULL;
  char *physical_ticket_sig_str = NULL;
  char *logical_ticket_sig_str = NULL;
  char *logical_from_id_str = NULL;
  char *logical_to_id_str = NULL;
  char *msg_undef = LW6MSG_UNDEF;

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
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknown mode %d"), (int) mode);
      sep = LW6MSG_TELNET_SEP;
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
    }

  body = msg;
  if (need_base64)
    {
      base64_str = lw6glb_base64_encode_str (sys_context, msg);
      if (base64_str)
	{
	  body = base64_str;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to base64 encode, sending as is"));
	}
    }

  if (physical_ticket_sig == 0)
    {
      physical_ticket_sig_str = msg_undef;
    }
  else
    {
      physical_ticket_sig_str = lw6sys_new_sprintf (sys_context, "%08x", physical_ticket_sig);
    }

  if (logical_ticket_sig == 0 || logical_ticket_sig == physical_ticket_sig)
    {
      logical_ticket_sig_str = msg_undef;
    }
  else
    {
      logical_ticket_sig_str = lw6sys_new_sprintf (sys_context, "%08x", logical_ticket_sig);
    }

  if (physical_from_id == logical_from_id)
    {
      logical_from_id_str = msg_undef;
    }
  else
    {
      logical_from_id_str = lw6sys_id_ltoa (sys_context, logical_from_id);
    }

  if (physical_to_id == logical_to_id)
    {
      logical_to_id_str = msg_undef;
    }
  else
    {
      logical_to_id_str = lw6sys_id_ltoa (sys_context, logical_to_id);
    }

  if (logical_from_id_str && logical_to_id_str)
    {
      ret =
	lw6sys_new_sprintf (sys_context, "%s%c%s%c%s%c%s%c%s%c%" LW6SYS_PRINTF_LL "x%c%"
			    LW6SYS_PRINTF_LL "x%c%s%c%s%c%s", lw6, sep,
			    version, sep, password_checksum, sep,
			    physical_ticket_sig_str, sep,
			    logical_ticket_sig_str, sep,
			    (long long) physical_from_id, sep, (long long) physical_to_id, sep, logical_from_id_str, sep, logical_to_id_str, sep, body);
    }

  if (base64_str)
    {
      LW6SYS_FREE (sys_context, base64_str);
    }

  if (physical_ticket_sig_str && physical_ticket_sig_str != msg_undef)
    {
      LW6SYS_FREE (sys_context, physical_ticket_sig_str);
    }

  if (logical_ticket_sig_str && logical_ticket_sig_str != msg_undef)
    {
      LW6SYS_FREE (sys_context, logical_ticket_sig_str);
    }

  if (logical_from_id_str && logical_from_id_str != msg_undef)
    {
      LW6SYS_FREE (sys_context, logical_from_id_str);
    }

  if (logical_to_id_str && logical_to_id_str != msg_undef)
    {
      LW6SYS_FREE (sys_context, logical_to_id_str);
    }

  return ret;
}

/**
 * lw6msg_envelope_analyse
 *
 * @sys_context: global system context
 * @envelope: the envelope to analyse
 * @mode: mode to use (a la TELNET or URL compatible)
 * @local_url: the url of local server (usefull for password)
 * @password: the password to check against
 * @expected_physical_from_id: the sender id, if NULL, no check performed
 * @expected_physical_to_id: the receiver id, if NULL, no check performed
 * @msg: if not NULL, will contain body of the message
 * @physical_ticket_sig: if not NULL, will contain signature of message, calculated with ticket
 * @logical_ticket_sig: if not NULL, will contain signature of message, calculated with ticket
 * @physical_from_id: if not NULL, will contain sender id
 * @physical_to_id: if not NULL, will contain receiver id
 * @logical_from_id: if not NULL, will contain message creator id
 * @logical_to_id: if not NULL, will contain message final destination id
 * @physical_from_url: if not NULL and if message allows, will contain sender public URL
 *
 * Generate an envelope, that is, the complete message sendable on the
 * network.
 *
 * Return value: newly allocated string.
 */
int
lw6msg_envelope_analyse (lw6sys_context_t * sys_context, const char *envelope, lw6msg_envelope_mode_t mode,
			 const char *local_url, const char *password,
			 u_int64_t expected_physical_from_id,
			 u_int64_t expected_physical_to_id, char **msg,
			 u_int32_t * physical_ticket_sig,
			 u_int32_t * logical_ticket_sig,
			 u_int64_t * physical_from_id,
			 u_int64_t * physical_to_id, u_int64_t * logical_from_id, u_int64_t * logical_to_id, char **physical_from_url)
{
  int ret = 0;
  char *lw6 = NULL;
  int need_base64 = 0;
  const char *pos = NULL;
  char *seek = NULL;
  char *version = NULL;
  lw6msg_word_t received_lw6;
  lw6msg_word_t received_version;
  lw6msg_word_t received_password;
  u_int32_t received_physical_ticket_sig_int = 0;
  u_int32_t received_logical_ticket_sig_int = 0;
  u_int64_t received_physical_from_id = 0;
  u_int64_t received_physical_to_id = 0;
  u_int64_t received_logical_from_id = 0;
  u_int64_t received_logical_to_id = 0;
  lw6msg_word_t tmp_word;

  switch (mode)
    {
    case LW6MSG_ENVELOPE_MODE_TELNET:
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
      break;
    case LW6MSG_ENVELOPE_MODE_URL:
      lw6 = LW6MSG_LW6_LC;
      need_base64 = 1;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknown mode %d"), (int) mode);
      lw6 = LW6MSG_LW6;
      need_base64 = 0;
    }

  version = lw6sys_build_get_version ();

  if (msg)
    {
      (*msg) = NULL;
    }
  if (physical_ticket_sig)
    {
      (*physical_ticket_sig) = 0;
    }
  if (logical_ticket_sig)
    {
      (*logical_ticket_sig) = 0;
    }
  if (physical_from_id)
    {
      (*physical_from_id) = 0;
    }
  if (physical_to_id)
    {
      (*physical_to_id) = 0;
    }
  if (logical_from_id)
    {
      (*logical_from_id) = 0;
    }
  if (logical_to_id)
    {
      (*logical_to_id) = 0;
    }
  if (physical_from_url)
    {
      (*physical_from_url) = NULL;
    }

  pos = envelope;
  seek = (char *) pos;
  if (lw6msg_word_first_x (sys_context, &received_lw6, &seek, pos))
    {
      if (lw6sys_str_is_same (sys_context, received_lw6.buf, lw6))
	{
	  pos = seek;
	  if (lw6msg_word_first_x (sys_context, &received_version, &seek, pos))
	    {
	      if (lw6sys_version_is_compatible (sys_context, received_version.buf, version))
		{
		  pos = seek;
		  if (lw6msg_word_first_x (sys_context, &received_password, &seek, pos))
		    {
		      if (lw6cnx_password_verify (sys_context, local_url, password, received_password.buf))
			{
			  pos = seek;
			  if (lw6msg_word_first_x (sys_context, &tmp_word, &seek, pos))
			    {
			      pos = seek;
			      /*
			       * Bad and/or 0 ticket accepted at this stage
			       */
			      sscanf (tmp_word.buf, "%x", &received_physical_ticket_sig_int);
			      if (lw6msg_word_first_x (sys_context, &tmp_word, &seek, pos))
				{
				  pos = seek;
				  /*
				   * Bad and/or 0 ticket accepted at this stage
				   */
				  sscanf (tmp_word.buf, "%x", &received_logical_ticket_sig_int);
				  if (!received_logical_ticket_sig_int)
				    {
				      received_logical_ticket_sig_int = received_physical_ticket_sig_int;
				    }
				  if (lw6msg_word_first_id_64 (sys_context, &received_physical_from_id, &seek, pos))
				    {
				      if (expected_physical_from_id == 0LL || (expected_physical_from_id == received_physical_from_id))
					{
					  pos = seek;
					  if (lw6msg_word_first_id_64 (sys_context, &received_physical_to_id, &seek, pos))
					    {
					      if (expected_physical_from_id == 0LL || (expected_physical_to_id == received_physical_to_id))
						{
						  pos = seek;
						  if (lw6msg_word_first_id_64
						      (sys_context, &received_logical_from_id, &seek, pos)
						      || lw6msg_word_first_x (sys_context, &tmp_word, &seek, pos))
						    {
						      pos = seek;
						      if (!received_logical_from_id)
							{
							  received_logical_from_id = received_physical_from_id;
							}
						      if (lw6msg_word_first_id_64 (sys_context, &received_logical_to_id, &seek, pos)
							  || lw6msg_word_first_x (sys_context, &tmp_word, &seek, pos))
							{
							  pos = seek;
							  if (!received_logical_to_id)
							    {
							      received_logical_to_id = received_physical_to_id;
							    }
							  ret = 1;
							  if (msg)
							    {
							      if (need_base64)
								{
								  (*msg) = lw6glb_base64_decode_str (sys_context, pos);
								  if (!(*msg))
								    {
								      /*
								       * message isn't base64 but
								       * we still return it as is, probably
								       * it's truncated, but we must return
								       * it anyway in case this function would
								       *  be called just to check the protocol
								       */
								      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("forcing clear text instead of base64"));
								      (*msg) = lw6sys_str_copy (sys_context, pos);
								    }
								}
							      else
								{
								  (*msg) = lw6sys_str_copy (sys_context, pos);
								}
							    }
							  if (physical_ticket_sig)
							    {
							      (*physical_ticket_sig) = received_physical_ticket_sig_int;
							    }
							  if (logical_ticket_sig)
							    {
							      (*logical_ticket_sig) = received_logical_ticket_sig_int;
							    }
							  if (physical_from_id)
							    {
							      (*physical_from_id) = received_physical_from_id;
							    }
							  if (physical_to_id)
							    {
							      (*physical_to_id) = received_physical_to_id;
							    }
							  if (logical_from_id)
							    {
							      (*logical_from_id) = received_logical_from_id;
							    }
							  if (logical_to_id)
							    {
							      (*logical_to_id) = received_logical_to_id;
							    }
							  if (physical_from_url && msg && (*msg))
							    {
							      (*physical_from_url) = lw6msg_cmd_guess_from_url (sys_context, *msg);
							    }
							}
						      else
							{
							  lw6sys_log
							    (sys_context, LW6SYS_LOG_INFO,
							     _x_ ("can't parse \"logical_to\" (final destination) id in envelope \"%s\""), envelope);
							}
						    }
						  else
						    {
						      lw6sys_log
							(sys_context, LW6SYS_LOG_INFO, _x_ ("can't parse \"logical_from\" (creator) id in envelope \"%s\""),
							 envelope);
						    }
						}
					      else
						{
						  lw6sys_log
						    (sys_context, LW6SYS_LOG_INFO,
						     _x_
						     ("wrong \"physical_to\" (receiver) id expected %"
						      LW6SYS_PRINTF_LL
						      "x received %"
						      LW6SYS_PRINTF_LL
						      "x in envelope \"%s\""),
						     (long long) expected_physical_to_id, (long long) received_physical_to_id, envelope);
						}
					    }
					  else
					    {
					      lw6sys_log (sys_context,
							  LW6SYS_LOG_INFO, _x_ ("can't parse \"physical_to\" (receiver) id in envelope \"%s\""), envelope);
					    }
					}
				      else
					{
					  lw6sys_log (sys_context,
						      LW6SYS_LOG_INFO,
						      _x_
						      ("wrong \"physical_from\" (sender) id expected %"
						       LW6SYS_PRINTF_LL
						       "x received %"
						       LW6SYS_PRINTF_LL
						       "x in envelope \"%s\""),
						      (long long) expected_physical_from_id, (long long) received_physical_from_id, envelope);
					}
				    }
				  else
				    {
				      lw6sys_log (sys_context,
						  LW6SYS_LOG_INFO, _x_ ("can't parse \"physical_from\" (sender) id in envelope \"%s\""), envelope);
				    }
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't parse logical ticket sig in envelope \"%s\""), envelope);
				}
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't parse physical ticket sig in envelope \"%s\""), envelope);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad password in envelope \"%s\""), envelope);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't parse password in envelope \"%s\""), envelope);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("bad version, received \"%s\", need \"%s\" in envelope \"%s\""), received_version.buf, version, envelope);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("can't parse version in envelope \"%s\""), envelope);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad lw6 key, received \"%s\", need \"%s\" in envelope \"%s\""), received_lw6.buf, lw6, envelope);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("message does not start with \"%s\" in envelope \"%s\""), lw6, envelope);
    }

  return ret;
}
