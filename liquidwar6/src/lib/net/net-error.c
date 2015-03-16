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

#ifndef LW6_MS_WINDOWS
#include <errno.h>
#endif // LW6_MS_WINDOWS

#include "net.h"
#include "net-internal.h"

#define _ERRBUFFER_LEN 1000

#ifdef LW6_MS_WINDOWS
/*
 * Convenience fonction which returns the readable name of a
 * winsock error.
 */
char *
_lw6net_wsa_str (lw6sys_context_t * sys_context, int wsa_int)
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
#endif // WSA_INVALID_HANDLE
#ifdef WSA_NOT_ENOUGH_MEMORY
    case WSA_NOT_ENOUGH_MEMORY:
      ret = "WSA_NOT_ENOUGH_MEMORY";
      break;
#endif // WSA_NOT_ENOUGH_MEMORY
#ifdef WSA_INVALID_PARAMETER
    case WSA_INVALID_PARAMETER:
      ret = "WSA_INVALID_PARAMETER";
      break;
#endif // WSA_INVALID_PARAMETER
#ifdef WSA_OPERATION_ABORTED
    case WSA_OPERATION_ABORTED:
      ret = "WSA_OPERATION_ABORTED";
      break;
#endif // WSA_OPERATION_ABORTED
#ifdef WSA_IO_INCOMPLETE
    case WSA_IO_INCOMPLETE:
      ret = "WSA_IO_INCOMPLETE";
      break;
#endif // WSA_IO_INCOMPLETE
#ifdef WSA_IO_PENDING
    case WSA_IO_PENDING:
      ret = "WSA_IO_PENDING";
      break;
#endif // WSA_IO_PENDING
#ifdef WSAEINTR
    case WSAEINTR:
      ret = "WSAEINTR";
      break;
#endif // WSAEINTR
#ifdef WSAEBADF
    case WSAEBADF:
      ret = "WSAEBADF";
      break;
#endif // WSAEBADF
#ifdef WSAEACCES
    case WSAEACCES:
      ret = "WSAEACCES";
      break;
#endif // WSAEACCES
#ifdef WSAEFAULT
    case WSAEFAULT:
      ret = "WSAEFAULT";
      break;
#endif // WSAEFAULT
#ifdef WSAEINVAL
    case WSAEINVAL:
      ret = "WSAEINVAL";
      break;
#endif // WSAEINVAL
#ifdef WSAEMFILE
    case WSAEMFILE:
      ret = "WSAEMFILE";
      break;
#endif // WSAEMFILE
#ifdef WSAEWOULDBLOCK
    case WSAEWOULDBLOCK:
      ret = "WSAEWOULDBLOCK";
      break;
#endif // WSAEWOULDBLOCK
#ifdef WSAEINPROGRESS
    case WSAEINPROGRESS:
      ret = "WSAEINPROGRESS";
      break;
#endif // WSAEINPROGRESS
#ifdef WSAEALREADY
    case WSAEALREADY:
      ret = "WSAEALREADY";
      break;
#endif // WSAEALREADY
#ifdef WSAENOTSOCK
    case WSAENOTSOCK:
      ret = "WSAENOTSOCK";
      break;
#endif // WSAENOTSOCK
#ifdef WSAEDESTADDRREQ
    case WSAEDESTADDRREQ:
      ret = "WSAEDESTADDRREQ";
      break;
#endif // WSAEDESTADDRREQ
#ifdef WSAEMSGSIZE
    case WSAEMSGSIZE:
      ret = "WSAEMSGSIZE";
      break;
#endif // WSAEMSGSIZE
#ifdef WSAEPROTOTYPE
    case WSAEPROTOTYPE:
      ret = "WSAEPROTOTYPE";
      break;
#endif // WSAEPROTOTYPE
#ifdef WSAENOPROTOOPT
    case WSAENOPROTOOPT:
      ret = "WSAENOPROTOOPT";
      break;
#endif // WSAENOPROTOOPT
#ifdef WSAEPROTONOSUPPORT
    case WSAEPROTONOSUPPORT:
      ret = "WSAEPROTONOSUPPORT";
      break;
#endif // WSAEPROTONOSUPPORT
#ifdef WSAESOCKTNOSUPPORT
    case WSAESOCKTNOSUPPORT:
      ret = "WSAESOCKTNOSUPPORT";
      break;
#endif // WSAESOCKTNOSUPPORT
#ifdef WSAEOPNOTSUPP
    case WSAEOPNOTSUPP:
      ret = "WSAEOPNOTSUPP";
      break;
#endif // WSAEOPNOTSUPP
#ifdef WSAEPFNOSUPPORT
    case WSAEPFNOSUPPORT:
      ret = "WSAEPFNOSUPPORT";
      break;
#endif // WSAEPFNOSUPPORT
#ifdef WSAEAFNOSUPPORT
    case WSAEAFNOSUPPORT:
      ret = "WSAEAFNOSUPPORT";
      break;
#endif // WSAEAFNOSUPPORT
#ifdef WSAEADDRINUSE
    case WSAEADDRINUSE:
      ret = "WSAEADDRINUSE";
      break;
#endif // WSAEADDRINUSE
#ifdef WSAEADDRNOTAVAIL
    case WSAEADDRNOTAVAIL:
      ret = "WSAEADDRNOTAVAIL";
      break;
#endif // WSAEADDRNOTAVAIL
#ifdef WSAENETDOWN
    case WSAENETDOWN:
      ret = "WSAENETDOWN";
      break;
#endif // WSAENETDOWN
#ifdef WSAENETUNREACH
    case WSAENETUNREACH:
      ret = "WSAENETUNREACH";
      break;
#endif // WSAENETUNREACH
#ifdef WSAENETRESET
    case WSAENETRESET:
      ret = "WSAENETRESET";
      break;
#endif // WSAENETRESET
#ifdef WSAECONNABORTED
    case WSAECONNABORTED:
      ret = "WSAECONNABORTED";
      break;
#endif // WSAECONNABORTED
#ifdef WSAECONNRESET
    case WSAECONNRESET:
      ret = "WSAECONNRESET";
      break;
#endif // WSAECONNRESET
#ifdef WSAENOBUFS
    case WSAENOBUFS:
      ret = "WSAENOBUFS";
      break;
#endif // WSAENOBUFS
#ifdef WSAEISCONN
    case WSAEISCONN:
      ret = "WSAEISCONN";
      break;
#endif // WSAEISCONN
#ifdef WSAENOTCONN
    case WSAENOTCONN:
      ret = "WSAENOTCONN";
      break;
#endif // WSAENOTCONN
#ifdef WSAESHUTDOWN
    case WSAESHUTDOWN:
      ret = "WSAESHUTDOWN";
      break;
#endif // WSAESHUTDOWN
#ifdef WSAETOOMANYREFS
    case WSAETOOMANYREFS:
      ret = "WSAETOOMANYREFS";
      break;
#endif // WSAETOOMANYREFS
#ifdef WSAETIMEDOUT
    case WSAETIMEDOUT:
      ret = "WSAETIMEDOUT";
      break;
#endif // WSAETIMEDOUT
#ifdef WSAECONNREFUSED
    case WSAECONNREFUSED:
      ret = "WSAECONNREFUSED";
      break;
#endif // WSAECONNREFUSED
#ifdef WSAELOOP
    case WSAELOOP:
      ret = "WSAELOOP";
      break;
#endif // WSAELOOP
#ifdef WSAENAMETOOLONG
    case WSAENAMETOOLONG:
      ret = "WSAENAMETOOLONG";
      break;
#endif // WSAENAMETOOLONG
#ifdef WSAEHOSTDOWN
    case WSAEHOSTDOWN:
      ret = "WSAEHOSTDOWN";
      break;
#endif // WSAEHOSTDOWN
#ifdef WSAEHOSTUNREACH
    case WSAEHOSTUNREACH:
      ret = "WSAEHOSTUNREACH";
      break;
#endif // WSAEHOSTUNREACH
#ifdef WSAENOTEMPTY
    case WSAENOTEMPTY:
      ret = "WSAENOTEMPTY";
      break;
#endif // WSAENOTEMPTY
#ifdef WSAEPROCLIM
    case WSAEPROCLIM:
      ret = "WSAEPROCLIM";
      break;
#endif // WSAEPROCLIM
#ifdef WSAEUSERS
    case WSAEUSERS:
      ret = "WSAEUSERS";
      break;
#endif // WSAEUSERS
#ifdef WSAEDQUOT
    case WSAEDQUOT:
      ret = "WSAEDQUOT";
      break;
#endif // WSAEDQUOT
#ifdef WSAESTALE
    case WSAESTALE:
      ret = "WSAESTALE";
      break;
#endif // WSAESTALE
#ifdef WSAEREMOTE
    case WSAEREMOTE:
      ret = "WSAEREMOTE";
      break;
#endif // WSAEREMOTE
#ifdef WSASYSNOTREADY
    case WSASYSNOTREADY:
      ret = "WSASYSNOTREADY";
      break;
#endif // WSASYSNOTREADY
#ifdef WSAVERNOTSUPPORTED
    case WSAVERNOTSUPPORTED:
      ret = "WSAVERNOTSUPPORTED";
      break;
#endif // WSAVERNOTSUPPORTED
#ifdef WSANOTINITIALISED
    case WSANOTINITIALISED:
      ret = "WSANOTINITIALISED";
      break;
#endif // WSANOTINITIALISED
#ifdef WSAEDISCON
    case WSAEDISCON:
      ret = "WSAEDISCON";
      break;
#endif // WSAEDISCON
#ifdef WSAENOMORE
    case WSAENOMORE:
      ret = "WSAENOMORE";
      break;
#endif // WSAENOMORE
#ifdef WSAECANCELLED
    case WSAECANCELLED:
      ret = "WSAECANCELLED";
      break;
#endif // WSAECANCELLED
#ifdef WSAEINVALIDPROCTABLE
    case WSAEINVALIDPROCTABLE:
      ret = "WSAEINVALIDPROCTABLE";
      break;
#endif // WSAEINVALIDPROCTABLE
#ifdef WSAEINVALIDPROVIDER
    case WSAEINVALIDPROVIDER:
      ret = "WSAEINVALIDPROVIDER";
      break;
#endif // WSAEINVALIDPROVIDER
#ifdef WSAEPROVIDERFAILEDINIT
    case WSAEPROVIDERFAILEDINIT:
      ret = "WSAEPROVIDERFAILEDINIT";
      break;
#endif // WSAEPROVIDERFAILEDINIT
#ifdef WSASYSCALLFAILURE
    case WSASYSCALLFAILURE:
      ret = "WSASYSCALLFAILURE";
      break;
#endif // WSASYSCALLFAILURE
#ifdef WSASERVICE_NOT_FOUND
    case WSASERVICE_NOT_FOUND:
      ret = "WSASERVICE_NOT_FOUND";
      break;
#endif // WSASERVICE_NOT_FOUND
#ifdef WSATYPE_NOT_FOUND
    case WSATYPE_NOT_FOUND:
      ret = "WSATYPE_NOT_FOUND";
      break;
#endif // WSATYPE_NOT_FOUND
#ifdef WSA_E_NO_MORE
    case WSA_E_NO_MORE:
      ret = "WSA_E_NO_MORE";
      break;
#endif // WSA_E_NO_MORE
#ifdef WSA_E_CANCELLED
    case WSA_E_CANCELLED:
      ret = "WSA_E_CANCELLED";
      break;
#endif // WSA_E_CANCELLED
#ifdef WSAEREFUSED
    case WSAEREFUSED:
      ret = "WSAEREFUSED";
      break;
#endif // WSAEREFUSED
#ifdef WSAHOST_NOT_FOUND
    case WSAHOST_NOT_FOUND:
      ret = "WSAHOST_NOT_FOUND";
      break;
#endif // WSAHOST_NOT_FOUND
#ifdef WSATRY_AGAIN
    case WSATRY_AGAIN:
      ret = "WSATRY_AGAIN";
      break;
#endif // WSATRY_AGAIN
#ifdef WSANO_RECOVERY
    case WSANO_RECOVERY:
      ret = "WSANO_RECOVERY";
      break;
#endif // WSANO_RECOVERY
#ifdef WSANO_DATA
    case WSANO_DATA:
      ret = "WSANO_DATA";
      break;
#endif // WSANO_DATA
#ifdef WSA_QOS_RECEIVERS
    case WSA_QOS_RECEIVERS:
      ret = "WSA_QOS_RECEIVERS";
      break;
#endif // WSA_QOS_RECEIVERS
#ifdef WSA_QOS_SENDERS
    case WSA_QOS_SENDERS:
      ret = "WSA_QOS_SENDERS";
      break;
#endif // WSA_QOS_SENDERS
#ifdef WSA_QOS_NO_SENDERS
    case WSA_QOS_NO_SENDERS:
      ret = "WSA_QOS_NO_SENDERS";
      break;
#endif // WSA_QOS_NO_SENDERS
#ifdef WSA_QOS_NO_RECEIVERS
    case WSA_QOS_NO_RECEIVERS:
      ret = "WSA_QOS_NO_RECEIVERS";
      break;
#endif // WSA_QOS_NO_RECEIVERS
#ifdef WSA_QOS_REQUEST_CONFIRMED
    case WSA_QOS_REQUEST_CONFIRMED:
      ret = "WSA_QOS_REQUEST_CONFIRMED";
      break;
#endif // WSA_QOS_REQUEST_CONFIRMED
#ifdef WSA_QOS_ADMISSION_FAILURE
    case WSA_QOS_ADMISSION_FAILURE:
      ret = "WSA_QOS_ADMISSION_FAILURE";
      break;
#endif // WSA_QOS_ADMISSION_FAILURE
#ifdef WSA_QOS_POLICY_FAILURE
    case WSA_QOS_POLICY_FAILURE:
      ret = "WSA_QOS_POLICY_FAILURE";
      break;
#endif // WSA_QOS_POLICY_FAILURE
#ifdef WSA_QOS_BAD_STYLE
    case WSA_QOS_BAD_STYLE:
      ret = "WSA_QOS_BAD_STYLE";
      break;
#endif // WSA_QOS_BAD_STYLE
#ifdef WSA_QOS_BAD_OBJECT
    case WSA_QOS_BAD_OBJECT:
      ret = "WSA_QOS_BAD_OBJECT";
      break;
#endif // WSA_QOS_BAD_OBJECT
#ifdef WSA_QOS_TRAFFIC_CTRL_ERROR
    case WSA_QOS_TRAFFIC_CTRL_ERROR:
      ret = "WSA_QOS_TRAFFIC_CTRL_ERROR";
      break;
#endif // WSA_QOS_TRAFFIC_CTRL_ERROR
#ifdef WSA_QOS_GENERIC_ERROR
    case WSA_QOS_GENERIC_ERROR:
      ret = "WSA_QOS_GENERIC_ERROR";
      break;
#endif // WSA_QOS_GENERIC_ERROR
#ifdef WSA_QOS_ESERVICETYPE
    case WSA_QOS_ESERVICETYPE:
      ret = "WSA_QOS_ESERVICETYPE";
      break;
#endif // WSA_QOS_ESERVICETYPE
#ifdef WSA_QOS_EFLOWSPEC
    case WSA_QOS_EFLOWSPEC:
      ret = "WSA_QOS_EFLOWSPEC";
      break;
#endif // WSA_QOS_EFLOWSPEC
#ifdef WSA_QOS_EPROVSPECBUF
    case WSA_QOS_EPROVSPECBUF:
      ret = "WSA_QOS_EPROVSPECBUF";
      break;
#endif // WSA_QOS_EPROVSPECBUF
#ifdef WSA_QOS_EFILTERSTYLE
    case WSA_QOS_EFILTERSTYLE:
      ret = "WSA_QOS_EFILTERSTYLE";
      break;
#endif // WSA_QOS_EFILTERSTYLE
#ifdef WSA_QOS_EFILTERTYPE
    case WSA_QOS_EFILTERTYPE:
      ret = "WSA_QOS_EFILTERTYPE";
      break;
#endif // WSA_QOS_EFILTERTYPE
#ifdef WSA_QOS_EFILTERCOUNT
    case WSA_QOS_EFILTERCOUNT:
      ret = "WSA_QOS_EFILTERCOUNT";
      break;
#endif // WSA_QOS_EFILTERCOUNT
#ifdef WSA_QOS_EOBJLENGTH
    case WSA_QOS_EOBJLENGTH:
      ret = "WSA_QOS_EOBJLENGTH";
      break;
#endif // WSA_QOS_EOBJLENGTH
#ifdef WSA_QOS_EFLOWCOUNT
    case WSA_QOS_EFLOWCOUNT:
      ret = "WSA_QOS_EFLOWCOUNT";
      break;
#endif // WSA_QOS_EFLOWCOUNT
#ifdef WSA_QOS_EUNKOWNPSOBJ
    case WSA_QOS_EUNKOWNPSOBJ:
      ret = "WSA_QOS_EUNKOWNPSOBJ";
      break;
#endif // WSA_QOS_EUNKOWNPSOBJ
#ifdef WSA_QOS_EPOLICYOBJ
    case WSA_QOS_EPOLICYOBJ:
      ret = "WSA_QOS_EPOLICYOBJ";
      break;
#endif // WSA_QOS_EPOLICYOBJ
#ifdef WSA_QOS_EFLOWDESC
    case WSA_QOS_EFLOWDESC:
      ret = "WSA_QOS_EFLOWDESC";
      break;
#endif // WSA_QOS_EFLOWDESC
#ifdef WSA_QOS_EPSFLOWSPEC
    case WSA_QOS_EPSFLOWSPEC:
      ret = "WSA_QOS_EPSFLOWSPEC";
      break;
#endif // WSA_QOS_EPSFLOWSPEC
#ifdef WSA_QOS_EPSFILTERSPEC
    case WSA_QOS_EPSFILTERSPEC:
      ret = "WSA_QOS_EPSFILTERSPEC";
      break;
#endif // WSA_QOS_EPSFILTERSPEC
#ifdef WSA_QOS_ESDMODEOBJ
    case WSA_QOS_ESDMODEOBJ:
      ret = "WSA_QOS_ESDMODEOBJ";
      break;
#endif // WSA_QOS_ESDMODEOBJ
#ifdef WSA_QOS_ESHAPERATEOBJ
    case WSA_QOS_ESHAPERATEOBJ:
      ret = "WSA_QOS_ESHAPERATEOBJ";
      break;
#endif // WSA_QOS_ESHAPERATEOBJ
#ifdef WSA_QOS_RESERVED_PETYPE
    case WSA_QOS_RESERVED_PETYPE:
      ret = "WSA_QOS_RESERVED_PETYPE";
      break;
#endif // WSA_QOS_RESERVED_PETYPE
    }

  return ret;
}
#endif

/**
 * lw6net_last_error:
 *
 * @sys_context: global system context
 *
 * Reports the last network error. This is basically a debug function,
 * designed mostly for Microsoft Winsock API, but can be safely called
 * on any platform.
 *
 * Return value: the last error code, has no universal meaning, depends
 *   on the platform you're working on.
 */
int
lw6net_last_error (lw6sys_context_t * sys_context)
{
  int ret = 0;

#ifdef LW6_MS_WINDOWS
  ret = WSAGetLastError ();
  if (ret || errno)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("winsock error %d \"%s\""), ret, _lw6net_wsa_str (sys_context, ret));
    }
#else // LW6_MS_WINDOWS
  ret = errno;

  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("socket error %d \"%s\""), ret, lw6sys_log_errno_str (sys_context, ret));
    }
#endif // LW6_MS_WINDOWS

  return ret;
}
