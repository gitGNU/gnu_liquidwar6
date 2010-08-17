SELECT node_url FROM lw6_node WHERE (node_id IS NULL OR node_last_ping_timestamp<%d) AND (node_id IS NULL OR node_id!='%s') ORDER BY node_ping_delay_msec, node_last_ping_timestamp LIMIT %d;
