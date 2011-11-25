SELECT * FROM lw6_node WHERE node_available=1 ORDER BY node_ping_delay_msec DESC, node_title DESC, node_creation_timestamp DESC;
