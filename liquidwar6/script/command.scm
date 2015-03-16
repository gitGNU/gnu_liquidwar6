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

(define lw6-command-nop
  (lambda (seq node-id)
    (format #f "~a ~a NOP" seq node-id)))

(define lw6-command-register
  (lambda (seq node-id)
    (format #f "~a ~a REGISTER" seq node-id)))

(define lw6-command-unregister
  (lambda (seq node-id)
    (format #f "~a ~a UNREGISTER" seq node-id)))

(define lw6-command-add
  (lambda (seq node-id cursor-id team-color)
    (format #f "~a ~a ADD ~a ~a" seq node-id cursor-id team-color)))

(define lw6-command-remove
  (lambda (seq node-id cursor-id)
    (format #f "~a ~a REMOVE ~a" seq node-id cursor-id)))

(define lw6-command-set
  (lambda (seq node-id cursor-id x y fire fire2)
    (format #f "~a ~a SET ~a ~a ~a ~a ~a" seq node-id cursor-id
	    (inexact->exact (floor (or x 0)))
	    (inexact->exact (floor (or y 0)))
	    (if fire 1 0)
	    (if fire2 1 0)
	    )))

(define lw6-command-all-local
  (lambda (seq node-id)
    (let (
	  (commands (list))
	  )
      (begin
	(map (lambda (cursor-key)
	       (let (
		     (cursor (lw6-get-cursor cursor-key))
		     )
		     ;;;)
		 #t))
	     lw6-cursor-keys)
	(map (lambda (cursor-key)
	       (let (
		      (cursor (lw6-get-cursor cursor-key))
		     )
		 (begin
		   (if (hash-ref cursor "status")
		       (let (
			     (cursor-id (hash-ref cursor "id"))
			     (x (hash-ref cursor "x"))
			     (y (hash-ref cursor "y"))
			     (fire (hash-ref cursor "fire"))
			     (fire2 (hash-ref cursor "fire2"))
			     )
			 (set! commands (append commands (list (lw6-command-set seq node-id cursor-id x y fire fire2))
						)
			       )
			 )
		       )
		   ;;(hash-set! cursor "fire" #f)
		   ;;(lw6-log-notice commands)
		   )))
	     lw6-cursor-keys)
	commands
    ))))

(define lw6-command-all-network
  (lambda (seq node-id)
    (let (
	  (commands (list))
	  )
      (begin
	commands
	))))

(define lw6-command-func-idle
  (lambda (for-real)
    #f))

(define lw6-command-func-local
  (lambda (for-real)
    (let (
	  (pilot (lw6-get-game-global "pilot"))
	  (node-id (lw6-get-game-global "node-id"))
	  (timestamp (c-lw6sys-get-timestamp))
	  )
      (if (and pilot node-id timestamp)
	  (begin
	    (lw6-speed-calibrate-local-pilot pilot)
	    (if (c-lw6pil-is-over pilot)
		#f ;; game over
		(let* (
		       (last-commit-seq (c-lw6pil-get-last-commit-seq pilot))
		       (next-seq (c-lw6pil-get-next-seq pilot timestamp))
		       (delta-seq (- next-seq last-commit-seq))
		       )
		  (if (or (>= delta-seq 2)
			  (and (>= delta-seq 1) for-real))
		      (let (
			(commands (if for-real
				      (lw6-command-all-local next-seq node-id)
				      (list (lw6-command-nop (- next-seq 1) node-id))))
			)
			(begin
			  ;;(tmp commands)
			  (map (lambda (command) (c-lw6pil-send-command pilot command #t)) commands)
			  (if for-real
			      (map (lambda (cursor-key)
				     (let (
					   (cursor (lw6-get-cursor cursor-key))
					   )
				       (begin
					 (hash-set! cursor "fire" #f)
					 (hash-set! cursor "fire2" #f)
					 )))
				   lw6-cursor-keys))
			  (c-lw6pil-commit pilot)
			  ))))))))
    ))

(define lw6-command-func-network
  (lambda (for-real)
    (let (
	  (pilot (lw6-get-game-global "pilot"))
	  (node-id (lw6-get-game-global "node-id"))
	  (node (lw6-get-game-global "node"))
	  (timestamp (c-lw6sys-get-timestamp))
	  )
      (if (and pilot node-id timestamp)
	  (begin
	    (lw6-speed-calibrate-local-pilot pilot)
	    (if (c-lw6pil-is-over pilot)
		#f ;; game over
		(let* (
		       (last-commit-seq (c-lw6pil-get-last-commit-seq pilot))
		       (next-seq (c-lw6pil-get-next-seq pilot timestamp))
		       (delta-seq (- next-seq last-commit-seq))
		       )
		  (if (or (>= delta-seq 2)
			  (and (>= delta-seq 1) for-real))
		      (let (
			(commands (if for-real
				      (lw6-command-all-local next-seq node-id)
				      (list (lw6-command-nop (- next-seq 1) node-id))))
			)
			(begin
			  ;;(tmp commands)
			  (map (lambda (command) (c-lw6pil-send-command pilot command #f)) commands)
			  (map (lambda (command) (c-lw6p2p-node-put-local-msg node command)) commands)
			  (if for-real
			      (map (lambda (cursor-key)
				     (let (
					   (cursor (lw6-get-cursor cursor-key))
					   )
				       (begin
					 (hash-set! cursor "fire" #f)
					 (hash-set! cursor "fire2" #f)
					 )))
				   lw6-cursor-network-keys))
			  (let* (
				 (msg (c-lw6p2p-node-get-next-reference-msg node))
				 )
			    (while msg
				   (begin
				     (c-lw6pil-send-command pilot msg #t)
				     (set! msg (c-lw6p2p-node-get-next-reference-msg node))
				     )
				   ))
			  (c-lw6pil-commit pilot)
			  ))))))))
    ))

(define lw6-command
  (lambda (for-real)
    (let (
	  (command-func (lw6-get-game-global "command-func"))
	  )
      (begin
	(if command-func
	    (command-func for-real)
	    )
	)
      )
    )
  )
