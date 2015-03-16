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

#include "../cli.h"
#include "mod-http-internal.h"

static lw6sys_context_t *_my_realloc_sys_context = NULL;

/*
 * Inspired from example getinmemory.c from libcurl documentation
 */

typedef struct _memory_struct_s
{
  char *memory;
  size_t size;
} _memory_struct_t;

static void *
_my_realloc (void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if (ptr)
    return LW6SYS_REALLOC (_my_realloc_sys_context, ptr, size);
  else
    return LW6SYS_MALLOC (_my_realloc_sys_context, size);
}

static size_t
_write_memory_callback (void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  _memory_struct_t *mem = (_memory_struct_t *) data;

  mem->memory = _my_realloc (mem->memory, mem->size + realsize + 1);
  if (mem->memory)
    {
      memcpy (&(mem->memory[mem->size]), ptr, realsize);
      mem->size += realsize;
      mem->memory[mem->size] = 0;
    }
  return realsize;
}

static void
_print_error (lw6sys_context_t * sys_context, char *function, CURLcode res)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("call to curl function \"%s\" failed with code %d \"%s\""), function, res, curl_easy_strerror (res));
}

char *
_mod_http_get (lw6sys_context_t * sys_context, _mod_http_context_t * http_context, const char *url, const char *password, const char *ip, int port)
{
  char *ret = NULL;
  CURL *curl_handle = NULL;
  CURLcode res = 0;
  _memory_struct_t chunk;
  char *authorization = NULL;
  int64_t origin = lw6sys_get_timestamp (sys_context);
  int test_sock = LW6NET_SOCKET_INVALID;
  int http_ok = 0;

  chunk.memory = NULL;		/* we expect realloc(NULL, size) to work */
  chunk.size = 0;		/* no data at this point */

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("GET \"%s\""), url);

  if (lw6net_is_connectable (sys_context, ip, port))
    {
      /*
       * We use this lock because CURL might use gethostbyname internally.
       * Not locking gives segfault error, typically:
       * "0x0077502f in Curl_getaddrinfo_ex () from /usr/lib/libcurl.so.4"
       * Note that we could lock *only* the curl_easy_perform function,
       * but in practice we would barely gain any time, and the safety
       * of being sure all CURL functions are serialized has some value.
       */
      if (lw6net_dns_lock (sys_context))
	{
	  /*
	   * Check if timeout is OK for if we had many locks, we can have
	   * a timeout between now and the end of the call
	   */
	  if (_mod_http_timeout_ok (sys_context, http_context, origin))
	    {
	      /* init the curl session */
	      curl_handle = curl_easy_init ();
	      if (curl_handle)
		{
		  /*
		   * Complicated, not sure this is 100% mandatory, but
		   * reading http://horstr.blogspot.com/2008/04/on-libcurl-openssl-and-thread-safety.html
		   * I came to the conclusion it might not harm
		   */
		  res = curl_easy_setopt (curl_handle, CURLOPT_NOSIGNAL, 1);
		  if (res == CURLE_OK)
		    {
		      /* specify URL to get */
		      res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
		      if (res == CURLE_OK)
			{
			  /*
			   * OK, before we ever use _write_memory_callback,
			   * we set the global (static) sys_context.
			   * This makes this lib perfectly non-reentrant,
			   * but this is not our fault, there's no real
			   * way to pass an extra pointer to these funcs.
			   */
			  _my_realloc_sys_context = sys_context;
			  res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, _write_memory_callback);
			  if (res == CURLE_OK)
			    {
			      /* we pass our 'chunk' struct to the callback function */
			      res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);
			      if (res == CURLE_OK)
				{
				  /*
				   * Don't waist a full minute when
				   * firewall is in DROP mode
				   */
				  res = curl_easy_setopt (curl_handle, CURLOPT_CONNECTTIMEOUT, http_context->data.consts.connect_timeout);
				  if (res == CURLE_OK)
				    {
				      /*
				       * Don't wait forever is server doesn't answer,
				       * this only happens if server is buggy, but
				       * it *can* be buggy. Or malicious.
				       */
				      res = curl_easy_setopt (curl_handle, CURLOPT_TIMEOUT, http_context->data.consts.global_timeout);
				      if (res == CURLE_OK)
					{
					  /* some servers don't like requests that are made without a user-agent
					     field, so we provide one */
					  res = curl_easy_setopt (curl_handle, CURLOPT_USERAGENT, lw6sys_build_get_package_tarname ());
					  if (res == CURLE_OK)
					    {
					      if (password)
						{
						  authorization = lw6sys_new_sprintf (sys_context, "%s:%s", lw6sys_build_get_package_tarname (), password);
						  if (authorization)
						    {
						      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("using authorization \"%s\""), authorization);
						      /* tell libcurl we can use "any" auth, which lets the lib pick one, but it
						         also costs one extra round-trip and possibly sending of all the PUT
						         data twice!!! */
						      res = curl_easy_setopt (curl_handle, CURLOPT_HTTPAUTH, (long) CURLAUTH_ANY);
						      if (res != CURLE_OK)
							{
							  _print_error (sys_context, "curl_easy_setopt(CURLOPT_HTTPAUTH)", res);
							}
						      /* set user name and password for the authentication */
						      res = curl_easy_setopt (curl_handle, CURLOPT_USERPWD, authorization);
						      if (res != CURLE_OK)
							{
							  _print_error (sys_context, "curl_easy_setopt(CURLOPT_USERPWD)", res);
							}
						      LW6SYS_FREE (sys_context, authorization);
						    }
						}
					      /* get it! */
					      res = curl_easy_perform (curl_handle);
					      if (res == CURLE_OK)
						{
						  /* should be OK */
						  http_ok = 1;
						}
					      else
						{
						  _print_error (sys_context, "curl_easy_perform", res);
						}
					    }
					  else
					    {
					      _print_error (sys_context, "curl_easy_setopt(CURLOPT_USERAGENT)", res);
					    }
					}
				      else
					{
					  _print_error (sys_context, "curl_easy_setopt(CURLOPT_TIMEOUT)", res);
					}
				    }
				  else
				    {
				      _print_error (sys_context, "curl_easy_setopt(CURLOPT_CONNECTTIMEOUT)", res);
				    }
				}
			      else
				{
				  _print_error (sys_context, "curl_easy_setopt(CURLOPT_WRITEDATA)", res);
				}
			    }
			  else
			    {
			      _print_error (sys_context, "curl_easy_setopt(CURLOPT_WRITEFUNCTION)", res);
			    }
			}
		      else
			{
			  _print_error (sys_context, "curl_easy_setopt(CURLOPT_URL)", res);
			}
		    }
		  else
		    {
		      _print_error (sys_context, "curl_easy_setopt(CURLOPT_NOSIGNAL)", res);
		    }
		  /* cleanup curl stuff */
		  curl_easy_cleanup (curl_handle);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to initialize CURL handle"));
		}

	      if (chunk.memory)
		{
		  if (http_ok && chunk.size > 0)
		    {
		      ret = chunk.memory;
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("CURL HTTP response=\"%s\""), ret);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("ignoring CURL HTTP response, http_ok=%d res=%d size=%d"), http_ok, (int) res, (int) chunk.size);
		      LW6SYS_FREE (sys_context, chunk.memory);
		      chunk.memory = NULL;
		    }
		}
	    }
	  lw6net_dns_unlock (sys_context);
	}
      if (!ret)
	{
	  /*
	   * Try a simple connect if we were in failure, this is just to set
	   * the "not connectable" state if needed.
	   */
	  test_sock = lw6net_tcp_connect (sys_context, ip, port, http_context->data.consts.connect_timeout);
	  if (lw6net_socket_is_valid (sys_context, test_sock))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("destination %s:%d is reachable by plain TCP/IP, but unable to serve correct LW6 HTTP stuff on url \"%s\""), ip, port, url);
	      lw6net_socket_close (sys_context, &test_sock);
	    }
	  else
	    {
	      lw6net_set_connectable (sys_context, ip, port, 0);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("destination %s:%d already marked as not connectable, not even trying to connect to it"), ip, port);
    }

  return ret;
}
