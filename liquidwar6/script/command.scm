;Liquid War 6 is a unique multiplayer wargame.
;Copyright (C)  2005, 2006, 2007, 2008, 2009  Christian Mauduit <ufoot@ufoot.org>
;
;This program is free software; you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;
;Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;Contact author        : ufoot@ufoot.org

(define lw6-command-nop
  (lambda (round server-id)
    (format #f "~a ~a NOP" round server-id)))

(define lw6-command-register
  (lambda (round server-id)
    (format #f "~a ~a REGISTER" round server-id)))

(define lw6-command-unregister
  (lambda (round server-id)
    (format #f "~a ~a UNREGISTER" round server-id)))

(define lw6-command-add
  (lambda (round server-id cursor-id team-color)
    (format #f "~a ~a ADD ~a ~a" round server-id cursor-id team-color)))

(define lw6-command-remove
  (lambda (round server-id cursor-id)
    (format #f "~a ~a REMOVE ~a" round server-id cursor-id)))

(define lw6-command-set
  (lambda (round server-id cursor-id x y)
    (format #f "~a ~a SET ~a ~a ~a" round server-id cursor-id
	    (inexact->exact (floor x))
	    (inexact->exact (floor y))
	    )))

(define lw6-command-all-local
  (lambda (round server-id)
    (let (
	  (commands (list))
	  )
      (begin
	(map (lambda (cursor-key)
	       (let (
		      (cursor (lw6-get-cursor cursor-key))		      
		     )
		 (if (hash-ref cursor "status")
		     (let (
			   (cursor-id (hash-ref cursor "id"))
			   (x (hash-ref cursor "x"))
			   (y (hash-ref cursor "y"))
			   )
		       (set! commands (append commands (list (lw6-command-set round server-id cursor-id x y))))))))
	     (list "1" "2" "3" "4"))
	commands
    ))))

(define lw6-command-func-idle
  (lambda (for-real)
    #f))

(define lw6-command-func-local
  (lambda (for-real) 
    (let (
	  (pilot (lw6-get-game-global "pilot"))
	  (server-id (lw6-get-game-global "server-id"))
	  (timestamp (c-lw6sys-get-timestamp))
	  )
      (if (and pilot server-id timestamp)
	  (begin
	    (lw6-speed-calibrate-local-pilot pilot)
	    (if (c-lw6pil-is-over pilot)
		#f ;; game over
		(let* (
		       (last-commit-round (c-lw6pil-get-last-commit-round pilot))
		       (next-round (c-lw6pil-get-next-round pilot timestamp))
		       (delta-round (- next-round last-commit-round))
		       )
		  (if (or (>= delta-round 2) 
			  (and (>= delta-round 1) for-real))
		      (let (
			(commands (if for-real
				      (lw6-command-all-local next-round server-id)
				      (list (lw6-command-nop (- next-round 1) server-id))))
			)
			(begin
			  ;;(tmp commands)
			  (map (lambda (command) (c-lw6pil-send-command pilot command #t)) commands)
			  (c-lw6pil-commit pilot)
			  ))))))))
    ))

(define lw6-command-func-network
  (lambda (for-real) 
    #f
    ;todo
    ))

(define lw6-command
  (lambda (for-real)
    (let (
	  (command-func (lw6-get-game-global "command-func"))
	  )
      (if command-func
	  (command-func for-real)
	  )
      )
    )
  )
