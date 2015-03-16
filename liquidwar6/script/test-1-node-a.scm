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

(load "test-0-node-a.scm") ; perform basic tests first

(define lw6-test-node-a-more
  (lambda ()
    (let (
	  (ret #f)
	  (db-name (format #f "~a.test.node-a" (c-lw6p2p-db-default-name)))
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
	       (timestamp-0 (c-lw6sys-get-timestamp))
	       (timestamp timestamp-0)
	       (seq-0 (c-lw6pil-suite-get-seq-0))
	       (next-seq seq-0)
	       (id (c-lw6pil-suite-get-node-id 0))
	       (db (c-lw6p2p-db-new db-name))
	       (node (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
						 (cons "server-backends" "tcpd,udpd,httpd")
						 (cons "bind-ip" "0.0.0.0")
						 (cons "bind-port" 8058)
						 (cons "node-id" id)
						 (cons "public-url" "http://localhost:8058/")
						 (cons "password" "")
						 (cons "title" "")
						 (cons "description" (_ "Dummy test node A"))
						 (cons "bench" 10)
						 (cons "open-relay" #f)
						 (cons "known-nodes" "http://localhost:8059/")
						 (cons "network-reliability" 100)
						 (cons "trojan" #f)
						 )))
	       (dump (c-lw6pil-suite-init timestamp-0))
	       (level (assoc-ref dump "level"))
	       (game-struct (assoc-ref dump "game-struct"))
	       (game-state (assoc-ref dump "game-state"))
	       (pilot (assoc-ref dump "pilot"))
	       (time-limit (+ lw6-test-network-global-delay timestamp-0))
	       (stage 0)
	       (next-update-info 0)
	       (exit-timestamp 0)
	       (seed-sent #f)
	       (dump-sent #f)
	       (checkpoint-0-ok #f)
	       (checkpoint-2-ok #f)
	       (checkpoint-4-ok #f)
	       )
	  (begin
	    (lw6-log-notice node)
	    (c-lw6p2p-node-poll node)
	    (c-lw6pil-commit pilot)
	    (c-lw6p2p-node-server-start node seq-0)
	    (while (and (< timestamp time-limit) (< stage 100))
		   (begin
		     (set! timestamp (c-lw6sys-get-timestamp))
		     (c-lw6sys-idle)
		     (c-lw6p2p-node-poll node)
		     ;; update node info, this is important for our peers
		     ;; might be wanting to poll this
		     (if (> timestamp next-update-info)
			 (begin
			   (set! next-update-info (+ timestamp lw6-test-network-update-delay))
			   (c-lw6pil-sync-from-reference game-state pilot)
			   (lw6-test-update-info node level game-state)
			   (c-lw6p2p-node-put-local-msg node (lw6-test-nop node stage))
			   ))
		     ;; Normally, we should get the next seq with a command like:
		     ;;(set! next-seq (c-lw6pil-get-next-seq pilot timestamp))
		     ;; but... here we don't want the seq to depend on current timestamp,
		     ;; rather, we want them to be fixed so that there are no surprises
		     ;; due to a script being fast or slow, whatever. So what we do
		     ;; is that we just increment last-commit-seq by 1.
		     (set! next-seq (max (c-lw6p2p-node-get-local-seq-last node) (1+ (c-lw6pil-get-last-commit-seq pilot))))
		     (cond
		      (
		       (c-lw6p2p-node-is-seed-needed node)
		       (let (
			     (seed-command (c-lw6pil-seed-command-generate pilot id next-seq))
			     )
			 (begin
			   (lw6-log-notice (format #f "seed-command -> ~a" seed-command))
			   (c-lw6p2p-node-put-local-msg node seed-command)
			   (c-lw6sys-idle)
			   (c-lw6p2p-node-poll node)
			   (set! seed-sent #t)
			   (if (and dump-sent (= stage 10))
			       (set! stage 20))
			   )
			 ))
		      (
		       (c-lw6p2p-node-is-dump-needed node)
		       (let (
			     (dump-command (c-lw6pil-dump-command-generate pilot id next-seq))
			     )
			 (begin
			   (lw6-log-notice (format #f "(string-length dump-command) -> ~a" (string-length dump-command)))
			   (c-lw6p2p-node-put-local-msg node dump-command)
			   (c-lw6sys-idle)
			   (c-lw6p2p-node-poll node)
			   (set! dump-sent #t)
			   (if (and seed-sent (= stage 10))
			       (set! stage 20))
			   )
			 ))
		      )
		     ;; pump all draft messages
		     (let* (
			    (msg (c-lw6p2p-node-get-next-draft-msg node))
			    )
		       (while msg
			      (begin
				(lw6-test-log-message "draft" msg)
				(c-lw6pil-send-command pilot msg #f)
				(set! msg (c-lw6p2p-node-get-next-draft-msg node))
				)
			      ))
		     ;; pump all reference messages
		     (let* (
			    (msg (c-lw6p2p-node-get-next-reference-msg node))
			    )
		       (while msg
			      (begin
				(lw6-test-log-message "reference" msg)
				(c-lw6pil-send-command pilot msg #t)
				(set! msg (c-lw6p2p-node-get-next-reference-msg node))
				)
			      ))
		     ;; commit now, even if there are no messages, won't harm
		     (c-lw6pil-commit pilot)
		     (cond
		      (
		       (= stage 0)
		       (begin
			 ;; Now proceed, putting the messages in the queue for good
			 (lw6-log-notice "stage 10 & 2, putting messages in queue")
			 (map (lambda (command) (begin
						  (lw6-log-notice (format #f "sending command \"~a\" from test suite stage 10 & 2" command))
						  (c-lw6p2p-node-put-local-msg node command)
						  ))
			      (append (c-lw6pil-suite-get-commands-by-node-index 0 0)
				      (c-lw6pil-suite-get-commands-by-node-index 0 1))
			      )
			 (set! stage 10)
			 )
		       )
		      (
		       (= stage 20)
		       (if (>= (c-lw6pil-get-reference-current-seq pilot)
			       (assoc-ref (c-lw6pil-suite-get-checkpoint 0) "seq"))
			   (begin
			     ;; Now verifying that at this stage the game-state
			     ;; is correct, will validate the whole test suite at
			     ;; this point, it could fail later, but in that case
			     ;; other nodes would receive garbage and *they* would
			     ;; fail.
			     (set! checkpoint-0-ok (lw6-test-verify-checksum game-state pilot 0))
			     ;; Now proceed, putting the messages in the queue for good
			     (lw6-log-notice "stage 30 & 4, putting messages in queue")
			     (map (lambda (command) (begin
						      (lw6-log-notice (format #f "sending command \"~a\" from test suite stage 30 & 4" command))
						      (c-lw6p2p-node-put-local-msg node command)
						      ))
				  (append (c-lw6pil-suite-get-commands-by-node-index 0 2)
					  (c-lw6pil-suite-get-commands-by-node-index 0 3))
				  )
			     (set! stage 30)
			     ))
		       )
		      (
		       ;; Wait for peer to be at least the right round
		       (= stage 30)
		       (if (lw6-test-check-nodes pilot node
						 (list 0 1 2)
						 (list "http://localhost:8058/"  "http://localhost:8059/"  "http://localhost:8060/")
						 2)
			   (set! stage 50))
		       )
		      (
		       (= stage 50)
		       (begin
			 ;; Now verifying that at this stage the game-state
			 ;; is correct, will validate the whole test suite at
			 ;; this point, it could fail later, but in that case
			 ;; other nodes would receive garbage and *they* would
			 ;; fail.
			 (set! checkpoint-2-ok (lw6-test-verify-checksum game-state pilot 2))
			 ;; Now proceed, putting the messages in the queue for good
			 (lw6-log-notice "stage 60 & 6, putting messages in queue")
			 (map (lambda (command) (begin
						  (lw6-log-notice (format #f "sending command \"~a\" from test suite stage 60 & 6" command))
						  (c-lw6p2p-node-put-local-msg node command)
						  ))
			      (append (c-lw6pil-suite-get-commands-by-node-index 0 4)
				      (c-lw6pil-suite-get-commands-by-node-index 0 5))
			      )
			 (set! stage 60)
			 )
		       )
		      (
		       (= stage 60)
		       (if (>= (c-lw6pil-get-reference-current-seq pilot)
			       (assoc-ref (c-lw6pil-suite-get-checkpoint 4) "seq"))
			   (begin
			     (set! checkpoint-4-ok (lw6-test-verify-checksum game-state pilot 4))
			     ;; Will now end the test, we're done, it might be right
			     ;; or wrong, but there's no point in waiting
			     (lw6-log-notice "done with test, ready to quit")
			     (set! stage 70)
			     )
			   )
		       )
		      (
		       (= stage 70)
		       (if (lw6-test-check-nodes pilot node
						 (list 0 1 2)
						 (list "http://localhost:8058/"  "http://localhost:8059/"  "http://localhost:8060/")
						 4)
			   (begin
			     (set! stage 80)
			     (set! exit-timestamp (+ timestamp lw6-test-network-exit-delay))
			     ))
		       )
		      (
		       (= stage 80)
		       (begin
			 (lw6-log-info "done with test, waiting for some time to give peers a chance to finish their process")
			 (if (> timestamp exit-timestamp)
			     (set! stage 100))
			 )
		       )
		      )))
	    ;; Condition of success is: all checkpoints are OK
	    (lw6-log-notice (format #f "test summary checkpoint-0-ok=~a checkpoint-2-ok=~a checkpoint-4-ok=~a"
				    checkpoint-0-ok checkpoint-2-ok checkpoint-4-ok))
	    (set! ret (and checkpoint-0-ok checkpoint-2-ok checkpoint-4-ok))
	    (c-lw6p2p-node-close node)
	    ))
	(c-lw6net-quit)
	(gc)
	ret
	))))

(c-lw6-set-ret (and
		(c-lw6-get-ret)
		(lw6-test-run lw6-test-node-a-more)
		))
