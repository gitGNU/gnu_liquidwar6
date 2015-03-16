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
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author	 : ufoot@ufoot.org

(define lw6-test-network-global-delay 300000)
(define lw6-test-network-connect-delay 90000)
(define lw6-test-network-shift-delay 10000)
(define lw6-test-network-update-delay 3000)
(define lw6-test-network-exit-delay 7000)

(define lw6-test-run
  (lambda (proc)
    (let (
	  (ret #f)
	  (proc-repr  (format #f "~a" proc))
	  )
      (begin
	(lw6-log-notice (format #f "test ~a BEGIN" proc-repr))
	(set! ret (proc))
	;; Note, for some reason Guile seems to collapse if ret
	;; is undefined, so be carefull, really set ret
	;; to something in test functions
	(if ret
	    (lw6-log-notice (format #f "test ~a END returned ~a" proc-repr ret))
	    (lw6-log-warning (format #f "test ~a END failed" proc-repr)))
	ret))))

(define lw6-test-log-message
  (lambda (type msg)
    (let (
	  (len (string-length msg))
	  )
      (if (>  len 100)
	  (lw6-log-notice (format #f "received ~a bytes ~a message" len type))
	  (lw6-log-notice (format #f "received ~a message \"~a\"" type msg))
	  )
      )))

(define lw6-test-update-info
  (lambda (node level game-state)
    (let (
	  (param (list (cons "round" (c-lw6ker-get-rounds game-state))
		       (cons "level" (c-lw6map-get-title level))
		       (cons "nb-colors" (c-lw6ker-get-nb-colors game-state))
		       (cons "max-nb-colors" (c-lw6map-get-max-nb-colors level))
		       (cons "nb-cursors" (c-lw6ker-get-nb-cursors game-state))
		       (cons "max-nb-cursors" (c-lw6map-get-max-nb-cursors level))
		       (cons "nb-nodes" (c-lw6ker-get-nb-nodes game-state))
		       (cons "max-nb-nodes" (c-lw6map-get-max-nb-nodes level))
		       (cons "screenshot" (c-lw6img-screenshot game-state 85))))
	  )
      (begin
	(lw6-log-notice (format #f "updating info of node \"~a\" with param \"~a\"" node param))
	(c-lw6p2p-node-update-info node param)
	(lw6-log-notice (format #f "new entries are \"~a\"" (c-lw6p2p-node-get-entries node #f)))
	))))

;; Utility function that builds an assoc similar to the
;; one returned by c-lw6pil-suite-get-checkpoint
(define lw6-test-checkpoint
  (lambda (game-state pilot)
    (list (cons "game-state-checksum" (c-lw6ker-game-state-checksum game-state))
	  (cons "seq" (c-lw6pil-get-reference-current-seq pilot))
	  (cons "round" (c-lw6ker-get-rounds game-state)))
    ))

;; Generate a NOP command and log our current state as well, for
;; debugging purposes. The NOP command itself is required for if
;; we don't send regular stuff on the network, messages can be lost
;; because never sent, and it's hard to plan this in advance in the
;; static suite messages arrays.
(define lw6-test-nop
  (lambda (node stage)
    (let (
	  (nop-command (lw6-command-nop (c-lw6p2p-node-get-local-seq-last node)
					(c-lw6p2p-node-get-id node)))
	  )
      (begin
	(lw6-log-notice (format #f "nop-command -> ~a" nop-command))
	(lw6-log-notice (format #f "current state stage=~a \"~a\"" stage node))
	nop-command
	))))

;; Check a given checksum, this is used for debugging, to
;; make double-triple sure the game-state we get is the right one.
;; This is a really strong test as the slightest difference between
;; messages will cause a checksum difference. Returns #t if they
;; are the same (the checksums) which means the test succeeded.
(define lw6-test-verify-checksum
  (lambda (game-state pilot stage)
    (begin
      (c-lw6pil-sync-from-reference game-state pilot)
      (let (
	    (ref-checkpoint (c-lw6pil-suite-get-checkpoint stage))
	    (this-checkpoint (lw6-test-checkpoint game-state pilot))
	    )
	(if (equal? ref-checkpoint this-checkpoint)
	    (begin
	      (lw6-log-notice (format #f "checkpoint OK ~a" this-checkpoint))
	      #t
	      )
	    (begin
	      (lw6-log-warning (format #f "bad checkpoint ~a vs ~a" this-checkpoint ref-checkpoint))
	      #f
	      )
	    )))))


;; Waits until other nodes are connected, and if they are connected, have
;; at least reached a given round. This is usefull to make sure a given
;; point has been reached, however this one does not check that the checksum
;; is right, it's just a timing function.
(define lw6-test-check-nodes
  (lambda (pilot node suite-node-ids node-urls checkpoint)
    (let (
	  (entries (c-lw6p2p-node-get-entries node #f))
	  (timestamp #f)
	  (ret #f)
	  )
      (if (or (not pilot)
	      (and (>= (c-lw6pil-get-reference-current-seq pilot)
		       (assoc-ref (c-lw6pil-suite-get-checkpoint checkpoint) "seq"))
		   . (map (lambda (x) (c-lw6p2p-node-is-peer-registered node (c-lw6pil-suite-get-node-id x))) suite-node-ids)
	       ))
	  (begin
	    (set! timestamp (c-lw6sys-get-timestamp))
	    (set! ret #t)
	    (map (lambda(x) (if (and (member (assoc-ref x "url") node-urls)
				     ;;(not (equal? (assoc-ref x "id") (c-lw6p2p-node-get-id node))))
				     #t)
				(if (and
				     (assoc-ref x "round")
				     (>= (assoc-ref x "round")
					 (assoc-ref (c-lw6pil-suite-get-checkpoint checkpoint) "round"))
				     )
				    (begin
				      (lw6-log-notice (format #f "checked entry id=~a url=\"~a\" round=~a"
							      (assoc-ref x "id")
							      (assoc-ref x "url")
							      (assoc-ref x "round")
							      ))
				      (c-lw6sys-idle)
				      (c-lw6p2p-node-poll node)
				      )
				    (begin
				      (lw6-log-notice (format #f "waiting for entry id=~a url=\"~a\" round=~a to be at least at round ~a"
							      (assoc-ref x "id")
							      (assoc-ref x "url")
							      (assoc-ref x "round")
							      (assoc-ref (c-lw6pil-suite-get-checkpoint checkpoint) "round")
							      ))
				      (set! ret #f)
				      (c-lw6sys-snooze)
				      )
				    )
				))
		 entries)
	    (c-lw6sys-idle)
	    (c-lw6p2p-node-poll node)
	    ret)
	  (begin
	    (lw6-log-notice (format #f "waiting for all nodes to be connected"))
	    (c-lw6sys-snooze)
	    #f
	  )
	  )
      )))
