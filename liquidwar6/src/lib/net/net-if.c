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

#include "net.h"
#include "net-internal.h"

#define _PUBLIC_URL_MASK "http://%s:%d/"
#define _PUBLIC_URL_MASK_HTTP_PORT "http://%s/"

#ifdef LW6_MS_WINDOWS
#include <iphlpapi.h>
#endif

/**
 * lw6net_if_guess_local
 *
 * Guess the local IP address. This is not fool-proof,
 * and it probably cannot be as we can't handle all
 * user-specific configs involving multiple IP addresses,
 * virtual private networks, and so on. But this is just
 * to provide a default public IP address when starting
 * a network game, saavy users can always specify the right
 * interface/address if needed. Will return NULL if interface
 * can't be guessed.
 *
 * Return value: the IP as a string, dynamically allocated
 */
char *
lw6net_if_guess_local ()
{
  char *ret = NULL;

#ifdef LW6_MS_WINDOWS
  /*
   * use GetAdaptersInfo
   * http://msdn.microsoft.com/en-us/library/aa365917.aspx
   */

  // It is possible for an adapter to have multiple
  // IPv4 addresses, gateways, and secondary WINS servers
  // assigned to the adapter. 
  //
  // Note that this sample code only prints out the 
  // first entry for the IP address/mask, and gateway, and
  // the primary and secondary WINS server for each adapter. 

  TODO, sample code follows

  /*
  PIP_ADAPTER_INFO pAdapterInfo;
  PIP_ADAPTER_INFO pAdapter = NULL;
  DWORD dwRetVal = 0;
  UINT i;

  // variables used to print DHCP time info 
  struct tm newtime;
  char buffer[32];
  errno_t error;

  ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC (sizeof (IP_ADAPTER_INFO));
  if (pAdapterInfo == NULL)
    {
      printf ("Error allocating memory needed to call GetAdaptersinfo\n");
      return 1;
    }
  // Make an initial call to GetAdaptersInfo to get
  // the necessary size into the ulOutBufLen variable
  if (GetAdaptersInfo (pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
      FREE (pAdapterInfo);
      pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC (ulOutBufLen);
      if (pAdapterInfo == NULL)
	{
	  printf ("Error allocating memory needed to call GetAdaptersinfo\n");
	  return 1;
	}
    }

  if ((dwRetVal = GetAdaptersInfo (pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
      pAdapter = pAdapterInfo;
      while (pAdapter)
	{
	  printf ("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
	  printf ("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
	  printf ("\tAdapter Desc: \t%s\n", pAdapter->Description);
	  printf ("\tAdapter Addr: \t");
	  for (i = 0; i < pAdapter->AddressLength; i++)
	    {
	      if (i == (pAdapter->AddressLength - 1))
		printf ("%.2X\n", (int) pAdapter->Address[i]);
	      else
		printf ("%.2X-", (int) pAdapter->Address[i]);
	    }
	  printf ("\tIndex: \t%d\n", pAdapter->Index);
	  printf ("\tType: \t");
	  switch (pAdapter->Type)
	    {
	    case MIB_IF_TYPE_OTHER:
	      printf ("Other\n");
	      break;
	    case MIB_IF_TYPE_ETHERNET:
	      printf ("Ethernet\n");
	      break;
	    case MIB_IF_TYPE_TOKENRING:
	      printf ("Token Ring\n");
	      break;
	    case MIB_IF_TYPE_FDDI:
	      printf ("FDDI\n");
	      break;
	    case MIB_IF_TYPE_PPP:
	      printf ("PPP\n");
	      break;
	    case MIB_IF_TYPE_LOOPBACK:
	      printf ("Lookback\n");
	      break;
	    case MIB_IF_TYPE_SLIP:
	      printf ("Slip\n");
	      break;
	    default:
	      printf ("Unknown type %ld\n", pAdapter->Type);
	      break;
	    }

	  printf ("\tIP Address: \t%s\n",
		  pAdapter->IpAddressList.IpAddress.String);
	  printf ("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

	  printf ("\tGateway: \t%s\n",
		  pAdapter->GatewayList.IpAddress.String);
	  printf ("\t***\n");

	  if (pAdapter->DhcpEnabled)
	    {
	      printf ("\tDHCP Enabled: Yes\n");
	      printf ("\t  DHCP Server: \t%s\n",
		      pAdapter->DhcpServer.IpAddress.String);

	      printf ("\t  Lease Obtained: ");
	      // Display local time 
	      error =
		_localtime32_s (&newtime,
				(__time32_t *) & pAdapter->LeaseObtained);
	      if (error)
		printf ("Invalid Argument to _localtime32_s\n");
	      else
		{
		  // Convert to an ASCII representation 
		  error = asctime_s (buffer, 32, &newtime);
		  if (error)
		    printf ("Invalid Argument to asctime_s\n");
		  else
		    // asctime_s returns the string terminated by \n\0 
		    printf ("%s", buffer);
		}

	      printf ("\t  Lease Expires:  ");
	      error =
		_localtime32_s (&newtime,
				(__time32_t *) & pAdapter->LeaseExpires);
	      if (error)
		printf ("Invalid Argument to _localtime32_s\n");
	      else
		{
		  // Convert to an ASCII representation 
		  error = asctime_s (buffer, 32, &newtime);
		  if (error)
		    printf ("Invalid Argument to asctime_s\n");
		  else
		    // asctime_s returns the string terminated by \n\0 
		    printf ("%s", buffer);
		}
	    }
	  else
	    printf ("\tDHCP Enabled: No\n");

	  if (pAdapter->HaveWins)
	    {
	      printf ("\tHave Wins: Yes\n");
	      printf ("\t  Primary Wins Server:    %s\n",
		      pAdapter->PrimaryWinsServer.IpAddress.String);
	      printf ("\t  Secondary Wins Server:  %s\n",
		      pAdapter->SecondaryWinsServer.IpAddress.String);
	    }
	  else
	    printf ("\tHave Wins: No\n");
	  pAdapter = pAdapter->Next;
	  printf ("\n");
	}
    }
  else
    {
      printf ("GetAdaptersInfo failed with error: %d\n", dwRetVal);

    }
  if (pAdapterInfo)
    FREE (pAdapterInfo);

  return 0;
}
  */
#else
  /*
   * use getifaddrs
   * http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html#EXAMPLE
   */
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];

  if (!getifaddrs (&ifaddr))
    {
      /* Walk through linked list, maintaining head pointer so we
         can free list later */
      for (ifa = ifaddr; ifa != NULL && !ret; ifa = ifa->ifa_next)
	{
	  family = ifa->ifa_addr->sa_family;

	  if (family == AF_INET)
	    {
	      if (!(s = getnameinfo (ifa->ifa_addr,
				     sizeof (struct sockaddr_in),
				     host, NI_MAXHOST, NULL, 0,
				     NI_NUMERICHOST)))
		{
		  if (strcmp (host, LW6NET_ADDRESS_LOOPBACK))
		    {
		      ret = lw6sys_str_copy (host);
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("getnameinfo() failed \"%s\""),
			      gai_strerror (s));
		}
	    }
	}
      freeifaddrs (ifaddr);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("getifaddrs failed"));
    }

#endif

  return ret;
}

/**
 * lw6net_if_guess_public_url
 *
 * @bind_ip: the IP address used to bind on
 * @bind_port: the IP port used to bind on
 *
 * Guess the server public url, based 
 * on @lw6net_if_guess_local which tries to find a
 * valid local IP address which is not loopback. This is only
 * in case @bind_ip is 0.0.0.0 (listen on all addresses) else
 * it will just use @bind_ip as you would expect.
 * Function isn't foolproof, that's why one can override
 * its default with a user settings.
 *
 * Return value: the IP as a string, dynamically allocated
 */
char *
lw6net_if_guess_public_url (char *bind_ip, int bind_port)
{
  char *ret = NULL;
  char *ip = NULL;

  if (!strcmp (bind_ip, LW6NET_ADDRESS_ANY))
    {
      ip = lw6net_if_guess_local ();
      if (!ip)
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _
		      ("can't find local network interface, using loopback"));
	  ip = lw6sys_str_copy (LW6NET_ADDRESS_LOOPBACK);
	}
    }
  else
    {
      ip = lw6sys_str_copy (bind_ip);
    }
  if (ip)
    {
      if (bind_port == LW6NET_HTTP_PORT)
	{
	  ret = lw6sys_new_sprintf (_PUBLIC_URL_MASK_HTTP_PORT, ip);
	}
      else
	{
	  ret = lw6sys_new_sprintf (_PUBLIC_URL_MASK, ip, bind_port);
	}
      LW6SYS_FREE (ip);
    }

  return ret;
}
