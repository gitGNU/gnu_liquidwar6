INSERT OR REPLACE INTO lw6_node (
       node_creation_timestamp,
       node_id,
       node_url,
       node_title,
       node_description,
       node_bench,
       ip,
       port,
       is_local
) VALUES (
       %d,
       '%s',
       '%s',
       '%s',
       '%s',
       %d,
       '%s',
       %d,
       %d
);

