-- Liquid War 6 is a unique multiplayer wargame.
-- Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
--
-- Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
-- Contact author        : ufoot@ufoot.org

CREATE TABLE IF NOT EXISTS lw6_node(
        -- Constant data 
	node_creation_timestamp INTEGER,
	node_version VARCHAR(64),
	node_codename VARCHAR(64),
	node_stamp INTEGER,
	node_id VARCHAR(16),
	node_url VARCHAR(1024) PRIMARY KEY,
	node_title VARCHAR(256),
	node_description VARCHAR(2048),
	node_has_password INTEGER,
        node_bench INTEGER,
        node_open_relay INTEGER,
	-- Variable data
	node_community_id VARCHAR(16),
	node_round INTEGER,
	node_level VARCHAR(1024),
	node_required_bench INTEGER,
	node_nb_colors INTEGER,
	node_max_nb_colors INTEGER,
	node_nb_cursors INTEGER,
	node_max_nb_cursors INTEGER,
	node_nb_nodes INTEGER,
	node_max_nb_nodes INTEGER,
	-- Additionnal data
	node_ip VARCHAR(64),
	node_port INTEGER,
	node_last_ping_timestamp INTEGER NOT NULL,
	node_ping_delay_msec INTEGER DEFAULT 3600000 NOT NULL,
	node_available INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS lw6_connection(
	connection_ptr VARCHAR(16) NOT NULL PRIMARY KEY,
	backend_ptr VARCHAR(16) NOT NULL,
	local_node_id VARCHAR(16) NOT NULL, 
	remote_node_id INTEGER(16) DEFAULT NULL,
	is_server INTEGER DEFAULT NULL,
	is_oob INTEGER DEFAULT NULL
);
