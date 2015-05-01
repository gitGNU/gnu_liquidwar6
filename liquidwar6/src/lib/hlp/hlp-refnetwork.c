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

#include "../net/net.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_network (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_SKIP_NETWORK,
			 _x_ ("If set, then game won't do anything network related. No listen, no connect, no nothing. You are playing locally."), 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_BROADCAST,
			 _x_
			 ("Allows the program to send broadcast messages on the network. It can be usefull to disable those if you don't use UDP node discovery and/or if there's a sysadmin arround who does not enjoy permanent broadcasts on his LAN."),
			 1);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PASSWORD,
			_x_
			("The password to use for network games. Do not use a valuable password, as this is stored as clear text on your hard drive. Still, the game will only send a hash/checksum of the password on the network so eavesdropper won't be able to read it. They can see the hash/checksum and use it if clever, but they can't guess the real password. A blank password means anyone can join your games when you act like a server."),
			"");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BIND_IP,
			_x_
			("The IP address to bind on when listening to network messages. You can use this to specifically use a given network interface, the default will listen on any available interface."),
			LW6NET_ADDRESS_ANY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_BIND_PORT,
			_x_
			("The IP port to bind on when listening to network messages. The default should work out of the box, and will ease up the discovery process. That is, if you use your own settings, automatic detection of your server by other servers might not work so well."),
			LW6NET_DEFAULT_PORT, 1, 65535);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CLI_BACKENDS,
			_x_
			("The client backends to use. Most of the time the default is fine, change it only if you specifically want to disactivate some protocol, or if you want to activate a custom-made client backend. It's a comma separated list."),
			"tcp,udp,http");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_SRV_BACKENDS,
			_x_
			("The server backends to use. Most of the time the default is fine, change it only if you specifically want to disactivate some protocol, or if you want to activate a custom-made server backend. It's a comma separated list."),
			"tcpd,udpd,httpd");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PUBLIC_URL,
			_x_
			("The public URL of your server. By default the game will pick up one for you. In fact, the clients discovering your server should guess the public URL, probably http://<your-ip>:<your-port>/ but you might need to use your own settings if you are using NAT or an Apache reverse-proxy to rewrite HTTP requests."),
			"");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_NODE_TITLE,
			_x_
			("The title of your node, that is the name which will be displayed when listing servers. This is different from player name, for there can be several players on a single computer. By default this will be set to hostname."),
			"");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_NODE_DESCRIPTION,
			_x_
			("The description of your node, that is a text that describes your server. This will typically appear when pointing a web client on the public server URL, it is for general information, so if there's something special about your server, say it here."),
			_x_ ("No description."));
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_KNOWN_NODES,
			_x_
			("List of known nodes, nodes which the program will try to contact first to get the list of other nodes. This is mostly usefull when program is launched for the first time, after this it should keep an up-to-date list of known servers in its internal database and automatically reconnect to them next time it starts. You might want to change this if you really want to connect to a given server which is not publically listed. The list is comma separated."),
			_x_ ("http://ufoot.org:8056/,http://ufoot.hd.free.fr:8056/"));

  ret = 1;

  return ret;
}
