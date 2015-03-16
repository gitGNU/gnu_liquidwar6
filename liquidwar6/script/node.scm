;; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author        : ufoot@ufoot.org

(define lw6-node-start
  (lambda ()
    (if (lw6-get-game-global "node")
	#t ;; nothing to do, keep current node
	(if (lw6-config-is-true? lw6def-skip-network)
	    #f ;; no node, no nothing
	    (let* (
		   (cli-backends (lw6-config-get-string lw6def-cli-backends))
		   (srv-backends (lw6-config-get-string lw6def-srv-backends))
		   (bind-ip (lw6-config-get-string lw6def-bind-ip))
		   (bind-port (lw6-config-get-number lw6def-bind-port))
		   (node-id (c-lw6sys-generate-id-64))
		   (public-url (lw6-config-get-string lw6def-public-url))
		   (title (lw6-config-get-string lw6def-node-title))
		   (description (lw6-config-get-string lw6def-node-description))
		   (password (lw6-config-get-string lw6def-password))
		   (bench (lw6-config-get-number lw6def-bench-value))
		   (open-relay (lw6-config-is-true? lw6def-open-relay))
		   (known-nodes (lw6-config-get-string lw6def-known-nodes))
		   (network-reliability (lw6-config-get-number lw6def-network-reliability))
		   (trojan (lw6-config-is-true? lw6def-trojan))
		   (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
		   )
	      (if db
		  (begin
		    (lw6-set-game-global! "db" db)
		    (let (
			  (node (c-lw6p2p-node-new db (list (cons "client-backends" cli-backends)
							    (cons "server-backends" srv-backends)
							    (cons "bind-ip" bind-ip)
							    (cons "bind-port" bind-port)
							    (cons "node-id" node-id)
							    (cons "public-url" public-url)
							    (cons "title" title)
							    (cons "description" description)
							    (cons "password" password)
							    (cons "bench" bench)
							    (cons "open-relay" open-relay)
							    (cons "known-nodes" known-nodes)
							    (cons "network-reliability" network-reliability)
							    (cons "trojan" trojan)
							    )))
			  )
		      (if node
			  (begin
			    (lw6-set-game-global! "node" node)
			    (lw6-set-game-global! "node-id" (c-lw6p2p-node-get-id node)))
			  )
		      ))))))))

(define lw6-node-poll
  (lambda ()
    (let (
	  (node (lw6-get-game-global "node"))
	  )
      (if node
	  (begin
	    (c-lw6p2p-node-poll node)
	    ;;(lw6-log-notice node)
	    ))
      )))

(define lw6-node-stop
  (lambda ()
    (let (
	  (node (lw6-get-game-global "node"))
	  )
      (if node
	  (begin
	    (c-lw6p2p-node-close node)
	    ;;(lw6-log-notice node)
	    ))
      )))
