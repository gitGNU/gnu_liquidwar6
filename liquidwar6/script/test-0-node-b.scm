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

(define lw6-test-node-b-join
  (lambda ()
    (let (
	  (ret #f)
	  (db-name (format #f "~a.test.node-b" (c-lw6p2p-db-default-name)))
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
	;; We wait a bit before starting the node-b node. This is to
	;; make sure server has had time to start before. In practice
	;; most players will connect on servers that already appear
	;; on the list of discovered servers, so this isn't a real
	;; issue but here, we need to help the batch to work.
	(c-lw6sys-delay (/ lw6-test-network-connect-delay 10))
	(let* (
	       (timestamp-0 (c-lw6sys-get-timestamp))
	       (seq-0 (c-lw6pil-suite-get-seq-0))
	       (id (c-lw6pil-suite-get-node-id 1))
	       (db (c-lw6p2p-db-new db-name))
	       (node (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
						   (cons "server-backends" "tcpd,udpd,httpd")
						   (cons "bind-ip" "0.0.0.0")
						   (cons "bind-port" 8058)
						   (cons "node-id" id)
						   (cons "public-url" "http://localhost:8058/")
						   (cons "password" "")
						   (cons "title" "")
						   (cons "description" (_ "Dummy test node B"))
						   (cons "bench" 10)
						   (cons "open-relay" #f)
						   (cons "known-nodes" "http://localhost:8057/")
						   (cons "network-reliability" 100)
						   (cons "trojan" #f)
						   )))
	       (dump #f)
	       (level #f)
	       (game-struct #f)
	       (game-state #f)
	       (pilot #f)
	       (time-limit (+ lw6-test-network-global-delay (c-lw6sys-get-timestamp)))
	       (connect-time (- time-limit lw6-test-network-connect-delay))
	       (connect-ret #f)
	       (server-entry #f)
	       )
	  (begin
	    (lw6-log-notice node)
	    ;; 1st step, we try to connect to server
	    (while (and (< (c-lw6sys-get-timestamp) connect-time) 
			(not (and connect-ret server-entry)))
		   (let (
			 (entries (c-lw6p2p-node-get-entries node))
			 )
		     (begin		     
		       (map (lambda(x) (if (and (equal? (assoc-ref x "url") "http://localhost:8057/")
						(assoc-ref x "id"))
					   (begin
					     ;; We wait a bit before trying to join, trying
					     ;; to join too fast could possibly ruin the process,
					     ;; a player would just try again, but here we really
					     ;; want it to work right away
					     (c-lw6sys-snooze)
					     (c-lw6p2p-node-poll node)
					     (set! server-entry x)
					     (set! connect-ret (c-lw6p2p-node-client-join 
								node
								(assoc-ref x "id")
								(assoc-ref x "url")))
					     )))
			    entries)	 
		       (c-lw6sys-idle)
		       (c-lw6p2p-node-poll node)
		       )))
	    (if server-entry
		(lw6-log-notice (format #f "OK, joining ~a" server-entry))
		(lw6-log-warning "unable to find server")
		)
	    ;; 2nd step, we wait for a dump
	    (while (and (< (c-lw6sys-get-timestamp) time-limit) server-entry (not dump))
		   (begin
		     (c-lw6sys-idle)
		     (c-lw6p2p-node-poll node)
		     (let* (
			    (msg (c-lw6p2p-node-get-next-draft-msg node))
			    (len (if msg (string-length msg) 0))
			    )
		       (if msg
			   (begin
			     (if (> len 100)
				 (begin
				   (lw6-log-notice (format #f "received ~a bytes message" len))
				   (set! ret #t) ;; todo, fix this and set it to true on real success
				   )
				 (begin
				   (lw6-log-notice (format #f "received ~a bytes message \"~a\"" len msg))
				   )
				 )
			     (set! dump (c-lw6pil-poll-dump msg
							    (c-lw6p2p-node-get-seq-max node)
							    (c-lw6sys-get-timestamp)))
			     (if dump
				 (begin
				   (lw6-log-notice (format #f "dump ~a" dump))
				   (set! level (assoc-ref dump "level"))
				   (set! game-struct (assoc-ref dump "game-struct"))
				   (set! game-state (assoc-ref dump "game-state"))
				   (set! pilot (assoc-ref dump "pilot"))
				   ))
			     )))
		     ))
	    ;; 3rd step, send our own messages, before node-c comes in
	    (if pilot
		(begin
		  (c-lw6p2p-node-poll node)
		  (map (lambda (command) (begin
					   (lw6-log-notice (format #f "sending command \"~a\" from test suite stage 1" command))
					   (c-lw6pil-send-command pilot command #t)
					   ))
		       (c-lw6pil-suite-get-commands-by-node-index 1 1))
		  (c-lw6pil-commit pilot)
		  ))
	    ;; 4th step, wait for node-c
	    (let (
		  (seq (c-lw6pil-get-last-commit-seq pilot))
		  )
	      (while (< (c-lw6sys-get-timestamp) time-limit)
		     (begin
		       (c-lw6sys-idle)
		       (c-lw6p2p-node-poll node)
		       (cond
			(
			 (c-lw6p2p-node-is-seed-needed node)
			 (let (
			       (seed-command (c-lw6pil-seed-command-generate pilot id))
			       )
			   (begin
			     (lw6-log-notice (format #f "seed-command -> ~a" seed-command))
			     (c-lw6p2p-node-put-local-msg node seed-command)
			     (c-lw6sys-idle)
			     (c-lw6p2p-node-poll node)
			     )
			   ))
			(
			 (c-lw6p2p-node-is-dump-needed node)
			 (let (
			       (dump-command (c-lw6pil-dump-command-generate pilot id))
			       )
			   (begin
			     (lw6-log-notice (format #f "(string-length dump-command) -> ~a" (string-length dump-command)))
			     (c-lw6p2p-node-put-local-msg node dump-command)
			     (c-lw6sys-idle)
			     (c-lw6p2p-node-poll node)
			     (set! ret #t) ;; todo, fix this and set it to true on real success
			     )
			   ))
			(
			 ;; Don't send NOP too often...
			 ;;(< (random 10000) 10)
			 #f
			 (let (
			       ;;(nop-command (lw6-command-nop (c-lw6pil-get-next-seq 
			       ;;			      pilot
			       ;;			      (c-lw6sys-get-timestamp)) 
			       ;;			     id))
			       (nop-command (lw6-command-nop seq id))
			       )
			   ;; OK, we put it with the same seq, else it won't show
			   ;; up in draft messages. Next versions should test
			   ;; reference messages anyway...
			   (c-lw6p2p-node-put-local-msg node nop-command)
			   ))
			)
		       )))
	    (c-lw6p2p-node-close node)
	    ))
	(c-lw6net-quit)
	(gc)     
	ret))))

(c-lw6-set-ret #f) ;; reset this to "failed" so that it has the right value is script is interrupted
(c-lw6-set-ret (and
		(lw6-test-run lw6-test-node-b-join)
		))
