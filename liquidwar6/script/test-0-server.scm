;; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author	 : ufoot@ufoot.org

(load "load.scm") ; load this to catch error in make dist
(lw6-load) ; will load all includes but main, trap syntax errors at least

(define lw6-test-server-join
  (lambda ()
    (let (
	  (ret #f)
	  (db-name (format #f "~a.test.server" (c-lw6p2p-db-default-name)))
	  )
      (begin
	(c-lw6net-init #t)
	;; It's important to reset test DBs before using them,
	;; else there could be temporaries IDs mismatches.
	;; In real use cases, only our local ID changes,
	;; and we really do want something with same URL but
	;; different ID than before to somewhat "not connect very well",
	;; at least for some time.
	(c-lw6p2p-db-reset db-name)
	(let* (
	       (db (c-lw6p2p-db-new db-name))
	       (node-1 (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
						   (cons "server-backends" "tcpd,udpd,httpd")
						   (cons "bind-ip" "0.0.0.0")
						   (cons "bind-port" 8057)
						   (cons "public-url" "http://localhost:8057/")
						   (cons "password" "")
						   (cons "title" "")
						   (cons "description" (_ "Dummy test node"))
						   (cons "bench" 10)
						   (cons "open-relay" #f)
						   (cons "known-nodes" "http://localhost:8058/")
						   (cons "network-reliability" 100)
						   (cons "trojan" #f)
						   )))
	       (id-1 (c-lw6p2p-node-get-id node-1))
	       (the-map-1 (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	       (game-struct-1 (c-lw6ker-build-game-struct the-map-1))
	       (game-state-1 (c-lw6ker-build-game-state game-struct-1))
	       (pilot-1 (c-lw6pil-build-pilot game-state-1
					      1000000000000
					      0))
	       (time-limit (+ lw6-test-network-global-delay (c-lw6sys-get-timestamp)))
	       )
	  (begin
	    (lw6-log-notice node-1)
	    (c-lw6p2p-node-poll node-1)
	    (c-lw6pil-send-command pilot-1 (format #f "1000000000010 ~a REGISTER" id-1) #t)
	    (c-lw6pil-send-command pilot-1 (format #f "1000000000010 ~a ADD 5678" id-1) #t)
	    (c-lw6p2p-node-server-start node-1 1000000000000)
	    (while (< (c-lw6sys-get-timestamp) time-limit)
		   (begin
		     (c-lw6sys-idle)
		     (c-lw6p2p-node-poll node-1)
		     (cond
		      (
		       (c-lw6p2p-node-is-dump-needed node-1)
		       (let (
			     (dump-command (c-lw6pil-dump-command-generate pilot-1 id-1))
			     )
			 (begin
			   (lw6-log-notice (format #f "(string-length dump-command) -> ~a" (string-length dump-command)))
			   (c-lw6p2p-node-put-local-msg node-1 dump-command)
			   (set! ret #t) ;; todo, fix this and set it to true on real success
			   )
			 ))
		      (
		       ;; Don't send NOP too often...
		       (< (random 10000) 10)
		       (let (
			     (nop-command (lw6-command-nop (c-lw6pil-get-next-seq 
							    pilot-1
							    (c-lw6sys-get-timestamp)) 
							   id-1))
			       )
			 (c-lw6p2p-node-put-local-msg node-1 nop-command)
			 ))
		      )
		     ))
	    (c-lw6p2p-node-close node-1)
	    ))
	(c-lw6net-quit)
	(gc)
	ret))))

(c-lw6-set-ret #f) ;; reset this to "failed" so that it has the right value is script is interrupted
(c-lw6-set-ret (and
		(lw6-test-run lw6-test-server-join)
		))
