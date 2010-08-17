DELETE FROM lw6_connection;
UPDATE lw6_node SET node_last_ping_timestamp=-3600, node_ping_delay_msec=3600000;