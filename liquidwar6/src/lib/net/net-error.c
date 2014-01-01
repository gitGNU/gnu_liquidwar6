/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LW6_MS_WINDOWS
#include <errno.h>
#endif

#include "net.h"
#include "net-internal.h"

#define _ERRBUFFER_LEN 1000

#ifdef LW6_MS_WINDOWS
/*
 * Convenience fonction which returns the readable name of a
 * winsock error.
 */
char *
_lw6net_wsa_str (int wsa_int)
{
  char *ret = "WSA_UNKNOWN";

  switch (wsa_int)
    {
    case 0:
      ret = "WSA_OK";
      break;
#ifdef WSA_INVALID_HANDLE
    case WSA_INVALID_HANDLE:
      ret = "WSA_INVALID_HANDLE";
      break;
#endif
#ifdef WSA_NOT_ENOUGH_MEMORY
    case WSA_NOT_ENOUGH_MEMORY:
      ret = "WSA_NOT_ENOUGH_MEMORY";
      break;
#endif
#ifdef WSA_INVALID_PARAMETER
    case WSA_INVALID_PARAMETER:
      ret = "WSA_INVALID_PARAMETER";
      break;
#endif
#ifdef WSA_OPERATION_ABORTED
    case WSA_OPERATION_ABORTED:
      ret = "WSA_OPERATION_ABORTED";
      break;
#endif
#ifdef WSA_IO_INCOMPLETE
    case WSA_IO_INCOMPLETE:
      ret = "WSA_IO_INCOMPLETE";
      break;
#endif
#ifdef WSA_IO_PENDING
    case WSA_IO_PENDING:
      ret = "WSA_IO_PENDING";
      break;
#endif
#ifdef WSAEINTR
    case WSAEINTR:
      ret = "WSAEINTR";
      break;
#endif
#ifdef WSAEBADF
    case WSAEBADF:
      ret = "WSAEBADF";
      break;
#endif
#ifdef WSAEACCES
    case WSAEACCES:
      ret = "WSAEACCES";
      break;
#endif
#ifdef WSAEFAULT
    case WSAEFAULT:
      ret = "WSAEFAULT";
      break;
#endif
#ifdef WSAEINVAL
    case WSAEINVAL:
      ret = "WSAEINVAL";
      break;
#endif
#ifdef WSAEMFILE
    case WSAEMFILE:
      ret = "WSAEMFILE";
      break;
#endif
#ifdef WSAEWOULDBLOCK
    case WSAEWOULDBLOCK:
      ret = "WSAEWOULDBLOCK";
      break;
#endif
#ifdef WSAEINPROGRESS
    case WSAEINPROGRESS:
      ret = "WSAEINPROGRESS";
      break;
#endif
#ifdef WSAEALREADY
    case WSAEALREADY:
      ret = "WSAEALREADY";
      break;
#endif
#ifdef WSAENOTSOCK
    case WSAENOTSOCK:
      ret = "WSAENOTSOCK";
      break;
#endif
#ifdef WSAEDESTADDRREQ
    case WSAEDESTADDRREQ:
      ret = "WSAEDESTADDRREQ";
      break;
#endif
#ifdef WSAEMSGSIZE
    case WSAEMSGSIZE:
      ret = "WSAEMSGSIZE";
      break;
#endif
#ifdef WSAEPROTOTYPE
    case WSAEPROTOTYPE:
      ret = "WSAEPROTOTYPE";
      break;
#endif
#ifdef WSAENOPROTOOPT
    case WSAENOPROTOOPT:
      ret = "WSAENOPROTOOPT";
      break;
#endif
#ifdef WSAEPROTONOSUPPORT
    case WSAEPROTONOSUPPORT:
      ret = "WSAEPROTONOSUPPORT";
      break;
#endif
#ifdef WSAESOCKTNOSUPPORT
    case WSAESOCKTNOSUPPORT:
      ret = "WSAESOCKTNOSUPPORT";
      break;
#endif
#ifdef WSAEOPNOTSUPP
    case WSAEOPNOTSUPP:
      ret = "WSAEOPNOTSUPP";
      break;
#endif
#ifdef WSAEPFNOSUPPORT
    case WSAEPFNOSUPPORT:
      ret = "WSAEPFNOSUPPORT";
      break;
#endif
#ifdef WSAEAFNOSUPPORT
    case WSAEAFNOSUPPORT:
      ret = "WSAEAFNOSUPPORT";
      break;
#endif
#ifdef WSAEADDRINUSE
    case WSAEADDRINUSE:
      ret = "WSAEADDRINUSE";
      break;
#endif
#ifdef WSAEADDRNOTAVAIL
    case WSAEADDRNOTAVAIL:
      ret = "WSAEADDRNOTAVAIL";
      break;
#endif
#ifdef WSAENETDOWN
    case WSAENETDOWN:
      ret = "WSAENETDOWN";
      break;
#endif
#ifdef WSAENETUNREACH
    case WSAENETUNREACH:
      ret = "WSAENETUNREACH";
      break;
#endif
#ifdef WSAENETRESET
    case WSAENETRESET:
      ret = "WSAENETRESET";
      break;
#endif
#ifdef WSAECONNABORTED
    case WSAECONNABORTED:
      ret = "WSAECONNABORTED";
      break;
#endif
#ifdef WSAECONNRESET
    case WSAECONNRESET:
      ret = "WSAECONNRESET";
      break;
#endif
#ifdef WSAENOBUFS
    case WSAENOBUFS:
      ret = "WSAENOBUFS";
      break;
#endif
#ifdef WSAEISCONN
    case WSAEISCONN:
      ret = "WSAEISCONN";
      break;
#endif
#ifdef WSAENOTCONN
    case WSAENOTCONN:
      ret = "WSAENOTCONN";
      break;
#endif
#ifdef WSAESHUTDOWN
    case WSAESHUTDOWN:
      ret = "WSAESHUTDOWN";
      break;
#endif
#ifdef WSAETOOMANYREFS
    case WSAETOOMANYREFS:
      ret = "WSAETOOMANYREFS";
      break;
#endif
#ifdef WSAETIMEDOUT
    case WSAETIMEDOUT:
      ret = "WSAETIMEDOUT";
      break;
#endif
#ifdef WSAECONNREFUSED
    case WSAECONNREFUSED:
      ret = "WSAECONNREFUSED";
      break;
#endif
#ifdef WSAELOOP
    case WSAELOOP:
      ret = "WSAELOOP";
      break;
#endif
#ifdef WSAENAMETOOLONG
    case WSAENAMETOOLONG:
      ret = "WSAENAMETOOLONG";
      break;
#endif
#ifdef WSAEHOSTDOWN
    case WSAEHOSTDOWN:
      ret = "WSAEHOSTDOWN";
      break;
#endif
#ifdef WSAEHOSTUNREACH
    case WSAEHOSTUNREACH:
      ret = "WSAEHOSTUNREACH";
      break;
#endif
#ifdef WSAENOTEMPTY
    case WSAENOTEMPTY:
      ret = "WSAENOTEMPTY";
      break;
#endif
#ifdef WSAEPROCLIM
    case WSAEPROCLIM:
      ret = "WSAEPROCLIM";
      break;
#endif
#ifdef WSAEUSERS
    case WSAEUSERS:
      ret = "WSAEUSERS";
      break;
#endif
#ifdef WSAEDQUOT
    case WSAEDQUOT:
      ret = "WSAEDQUOT";
      break;
#endif
#ifdef WSAESTALE
    case WSAESTALE:
      ret = "WSAESTALE";
      break;
#endif
#ifdef WSAEREMOTE
    case WSAEREMOTE:
      ret = "WSAEREMOTE";
      break;
#endif
#ifdef WSASYSNOTREADY
    case WSASYSNOTREADY:
      ret = "WSASYSNOTREADY";
      break;
#endif
#ifdef WSAVERNOTSUPPORTED
    case WSAVERNOTSUPPORTED:
      ret = "WSAVERNOTSUPPORTED";
      break;
#endif
#ifdef WSANOTINITIALISED
    case WSANOTINITIALISED:
      ret = "WSANOTINITIALISED";
      break;
#endif
#ifdef WSAEDISCON
    case WSAEDISCON:
      ret = "WSAEDISCON";
      break;
#endif
#ifdef WSAENOMORE
    case WSAENOMORE:
      ret = "WSAENOMORE";
      break;
#endif
#ifdef WSAECANCELLED
    case WSAECANCELLED:
      ret = "WSAECANCELLED";
      break;
#endif
#ifdef WSAEINVALIDPROCTABLE
    case WSAEINVALIDPROCTABLE:
      ret = "WSAEINVALIDPROCTABLE";
      break;
#endif
#ifdef WSAEINVALIDPROVIDER
    case WSAEINVALIDPROVIDER:
      ret = "WSAEINVALIDPROVIDER";
      break;
#endif
#ifdef WSAEPROVIDERFAILEDINIT
    case WSAEPROVIDERFAILEDINIT:
      ret = "WSAEPROVIDERFAILEDINIT";
      break;
#endif
#ifdef WSASYSCALLFAILURE
    case WSASYSCALLFAILURE:
      ret = "WSASYSCALLFAILURE";
      break;
#endif
#ifdef WSASERVICE_NOT_FOUND
    case WSASERVICE_NOT_FOUND:
      ret = "WSASERVICE_NOT_FOUND";
      break;
#endif
#ifdef WSATYPE_NOT_FOUND
    case WSATYPE_NOT_FOUND:
      ret = "WSATYPE_NOT_FOUND";
      break;
#endif
#ifdef WSA_E_NO_MORE
    case WSA_E_NO_MORE:
      ret = "WSA_E_NO_MORE";
      break;
#endif
#ifdef WSA_E_CANCELLED
    case WSA_E_CANCELLED:
      ret = "WSA_E_CANCELLED";
      break;
#endif
#ifdef WSAEREFUSED
    case WSAEREFUSED:
      ret = "WSAEREFUSED";
      break;
#endif
#ifdef WSAHOST_NOT_FOUND
    case WSAHOST_NOT_FOUND:
      ret = "WSAHOST_NOT_FOUND";
      break;
#endif
#ifdef WSATRY_AGAIN
    case WSATRY_AGAIN:
      ret = "WSATRY_AGAIN";
      break;
#endif
#ifdef WSANO_RECOVERY
    case WSANO_RECOVERY:
      ret = "WSANO_RECOVERY";
      break;
#endif
#ifdef WSANO_DATA
    case WSANO_DATA:
      ret = "WSANO_DATA";
      break;
#endif
#ifdef WSA_QOS_RECEIVERS
    case WSA_QOS_RECEIVERS:
      ret = "WSA_QOS_RECEIVERS";
      break;
#endif
#ifdef WSA_QOS_SENDERS
    case WSA_QOS_SENDERS:
      ret = "WSA_QOS_SENDERS";
      break;
#endif
#ifdef WSA_QOS_NO_SENDERS
    case WSA_QOS_NO_SENDERS:
      ret = "WSA_QOS_NO_SENDERS";
      break;
#endif
#ifdef WSA_QOS_NO_RECEIVERS
    case WSA_QOS_NO_RECEIVERS:
      ret = "WSA_QOS_NO_RECEIVERS";
      break;
#endif
#ifdef WSA_QOS_REQUEST_CONFIRMED
    case WSA_QOS_REQUEST_CONFIRMED:
      ret = "WSA_QOS_REQUEST_CONFIRMED";
      break;
#endif
#ifdef WSA_QOS_ADMISSION_FAILURE
    case WSA_QOS_ADMISSION_FAILURE:
      ret = "WSA_QOS_ADMISSION_FAILURE";
      break;
#endif
#ifdef WSA_QOS_POLICY_FAILURE
    case WSA_QOS_POLICY_FAILURE:
      ret = "WSA_QOS_POLICY_FAILURE";
      break;
#endif
#ifdef WSA_QOS_BAD_STYLE
    case WSA_QOS_BAD_STYLE:
      ret = "WSA_QOS_BAD_STYLE";
      break;
#endif
#ifdef WSA_QOS_BAD_OBJECT
    case WSA_QOS_BAD_OBJECT:
      ret = "WSA_QOS_BAD_OBJECT";
      break;
#endif
#ifdef WSA_QOS_TRAFFIC_CTRL_ERROR
    case WSA_QOS_TRAFFIC_CTRL_ERROR:
      ret = "WSA_QOS_TRAFFIC_CTRL_ERROR";
      break;
#endif
#ifdef WSA_QOS_GENERIC_ERROR
    case WSA_QOS_GENERIC_ERROR:
      ret = "WSA_QOS_GENERIC_ERROR";
      break;
#endif
#ifdef WSA_QOS_ESERVICETYPE
    case WSA_QOS_ESERVICETYPE:
      ret = "WSA_QOS_ESERVICETYPE";
      break;
#endif
#ifdef WSA_QOS_EFLOWSPEC
    case WSA_QOS_EFLOWSPEC:
      ret = "WSA_QOS_EFLOWSPEC";
      break;
#endif
#ifdef WSA_QOS_EPROVSPECBUF
    case WSA_QOS_EPROVSPECBUF:
      ret = "WSA_QOS_EPROVSPECBUF";
      break;
#endif
#ifdef WSA_QOS_EFILTERSTYLE
    case WSA_QOS_EFILTERSTYLE:
      ret = "WSA_QOS_EFILTERSTYLE";
      break;
#endif
#ifdef WSA_QOS_EFILTERTYPE
    case WSA_QOS_EFILTERTYPE:
      ret = "WSA_QOS_EFILTERTYPE";
      break;
#endif
#ifdef WSA_QOS_EFILTERCOUNT
    case WSA_QOS_EFILTERCOUNT:
      ret = "WSA_QOS_EFILTERCOUNT";
      break;
#endif
#ifdef WSA_QOS_EOBJLENGTH
    case WSA_QOS_EOBJLENGTH:
      ret = "WSA_QOS_EOBJLENGTH";
      break;
#endif
#ifdef WSA_QOS_EFLOWCOUNT
    case WSA_QOS_EFLOWCOUNT:
      ret = "WSA_QOS_EFLOWCOUNT";
      break;
#endif
#ifdef WSA_QOS_EUNKOWNPSOBJ
    case WSA_QOS_EUNKOWNPSOBJ:
      ret = "WSA_QOS_EUNKOWNPSOBJ";
      break;
#endif
#ifdef WSA_QOS_EPOLICYOBJ
    case WSA_QOS_EPOLICYOBJ:
      ret = "WSA_QOS_EPOLICYOBJ";
      break;
#endif
#ifdef WSA_QOS_EFLOWDESC
    case WSA_QOS_EFLOWDESC:
      ret = "WSA_QOS_EFLOWDESC";
      break;
#endif
#ifdef WSA_QOS_EPSFLOWSPEC
    case WSA_QOS_EPSFLOWSPEC:
      ret = "WSA_QOS_EPSFLOWSPEC";
      break;
#endif
#ifdef WSA_QOS_EPSFILTERSPEC
    case WSA_QOS_EPSFILTERSPEC:
      ret = "WSA_QOS_EPSFILTERSPEC";
      break;
#endif
#ifdef WSA_QOS_ESDMODEOBJ
    case WSA_QOS_ESDMODEOBJ:
      ret = "WSA_QOS_ESDMODEOBJ";
      break;
#endif
#ifdef WSA_QOS_ESHAPERATEOBJ
    case WSA_QOS_ESHAPERATEOBJ:
      ret = "WSA_QOS_ESHAPERATEOBJ";
      break;
#endif
#ifdef WSA_QOS_RESERVED_PETYPE
    case WSA_QOS_RESERVED_PETYPE:
      ret = "WSA_QOS_RESERVED_PETYPE";
      break;
#endif
    }

  return ret;
}
#endif

/**
 * lw6net_last_error:
 * 
 * Reports the last network error. This is basically a debug function,
 * designed mostly for Microsoft Winsock API, but can be safely called
 * on any platform.
 *
 * Return value: the last error code, has no universal meaning, depends
 *   on the platform you're working on.
 */
int
lw6net_last_error ()
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  ret = WSAGetLastError ();
  if (ret || errno)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("winsock error %d \"%s\""), ret,
		  _lw6net_wsa_str (ret));
    }
#else
  ret = errno;

  if (ret)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("socket error %d \"%s\""), ret,
		  lw6sys_log_errno_str (ret));
    }
#endif

  return ret;
}
