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

(define lw6-cursor-keys
  (list "1" "2" "3" "4" "bot1" "bot2" "bot3" "bot4" "bot5" "bot6" "bot7" "bot8" "bot9"))

(define lw6-cursor-player-keys
  (list "1" "2" "3" "4"))

(define lw6-cursor-bot-keys
  (list "bot1" "bot2" "bot3" "bot4" "bot5" "bot6" "bot7" "bot8" "bot9"))

(define lw6-cursor-network-keys
  (list "1"))

(define lw6-make-cursor
  (lambda (cursor-key)
    (let (
	  (cursor (make-hash-table 13))
	  )
      (begin
	(hash-set! cursor "status" #f)
	(hash-set! cursor "key" cursor-key)
	(hash-set! cursor "id" (c-lw6sys-generate-id-16))
	(hash-set! cursor "x" -1)
	(hash-set! cursor "y" -1)
	(hash-set! cursor "fire" #f)
	(hash-set! cursor "fire2" #f)
	(hash-set! cursor "mouse-controlled" #f)
	(hash-set! cursor "vx" 0.0)
	(hash-set! cursor "vy" 0.0)
	(hash-set! cursor "map-pos-x" 0)
	(hash-set! cursor "map-pos-y" 0)
	(hash-set! cursor "letter" #f)
	(hash-set! cursor "team-color" #f)
	(hash-set! cursor "mover" (lambda (c) #f))
	(hash-set! cursor "bot" #f)
	cursor
      ))))

(define lw6-get-cursor
  (lambda (cursor-key)
    (let* (
	   (cursors (lw6-get-game-global "cursors"))
	   (cursor (hash-ref cursors (string-concatenate (list "cursor-" cursor-key))))
	   )
      cursor)))

(define lw6-disable-cursor
  (lambda (cursor-key)
    (let (
	  (cursors (lw6-get-game-global "cursors"))
	  )
      (begin
	(hash-set! cursors
		   (string-concatenate (list "cursor-" cursor-key))
		   (lw6-make-cursor cursor-key))))))

(define lw6-enable-cursor
  (lambda (cursor-key)
    (let* (
	   (cursors (lw6-get-game-global "cursors"))
	   (cursor (hash-ref cursors (string-concatenate (list "cursor-" cursor-key))))
	   )
      (begin
	(hash-set! cursor "status" #t)
	cursor))))

(define lw6-cursor-set-configured-player!
  (lambda (game-state cursor-key node-id)
    (let* (
	   (player-status-key (string-concatenate (list "player" cursor-key "-status")))
	   (player-color-key (string-concatenate (list "player" cursor-key "-color")))
	   (player-control-key (string-concatenate (list "player" cursor-key "-control")))
	   (player-status (lw6-config-is-true? player-status-key))
	   (player-color (lw6-config-get-string player-color-key))
	   (player-control (lw6-config-get-string player-control-key))
	   )
      (if player-status
	  (let* (
		 (cursor (lw6-enable-cursor cursor-key))
		 (cursor-id (hash-ref cursor "id"))
		 (node-id (lw6-get-game-global "node-id"))
		 )
	    (begin
	      (c-lw6ker-add-cursor game-state node-id cursor-id player-color)
	      ))
	  (lw6-disable-cursor cursor-key))
      )))

(define lw6-cursor-prepare-configured-player-command
  (lambda (game-state seq-0 cursor-key node-id)
    (let* (
	   (player-status-key (string-concatenate (list "player" cursor-key "-status")))
	   (player-color-key (string-concatenate (list "player" cursor-key "-color")))
	   (player-control-key (string-concatenate (list "player" cursor-key "-control")))
	   (player-status (lw6-config-is-true? player-status-key))
	   (player-color (lw6-config-get-string player-color-key))
	   (player-control (lw6-config-get-string player-control-key))
	   )
      (if player-status
	  (let* (
		 (cursor (lw6-enable-cursor cursor-key))
		 (cursor-id (lw6-get-cursor-id-not-in-game-state game-state))
		 )
	    (begin
	      (hash-set! cursor "id" cursor-id)
	      (lw6-command-add seq-0 node-id cursor-id player-color)
	      ))
	  #f)
      )))

(define lw6-cursor-prepare-map-defined-player-command
  (lambda (level game-state seq-0 cursor-key node-id)
    (let* (
	   (player-color (c-lw6map-param-get level
					     (if (equal? cursor-key "1")
						 lw6def-player1-color
						 lw6def-player2-color)))
	   (cursor (lw6-enable-cursor cursor-key))
	   (cursor-id (lw6-get-cursor-id-not-in-game-state game-state))
	   )
      (begin
	(hash-set! cursor "id" cursor-id)
	(lw6-command-add seq-0 node-id cursor-id player-color)
	))))

(define lw6-cursor-prepare-bot-command
  (lambda (level game-state seq-0 cursor-key node-id)
    (let* (
	   (bot-color-key (string-concatenate (list cursor-key "-color")))
	   (bot-color (c-lw6map-param-get level bot-color-key))
	   )
      (let* (
	     (cursor (lw6-enable-cursor cursor-key))
	     (cursor-id (lw6-get-cursor-id-not-in-game-state game-state))
	     )
	(begin
	  (hash-set! cursor "id" cursor-id)
	  (lw6-command-add seq-0 node-id cursor-id bot-color))))))

(define lw6-cursor-prepare-demo-player-command
  (lambda (game-state seq-0 cursor-key node-id color)
    (let* (
	   (cursor (lw6-enable-cursor cursor-key))
	   (cursor-id (lw6-get-cursor-id-not-in-game-state game-state))
	   )
      (begin
	(hash-set! cursor "id" cursor-id)
	(lw6-command-add seq-0 node-id cursor-id color)
	))))

(define lw6-cursor-prepare-quick-start-player-command
  lw6-cursor-prepare-demo-player-command)

(define lw6-get-cursor-id-not-in-game-state
  (lambda (game-state)
    (let (
	  (cursor-id (c-lw6sys-generate-id-16))
	  )
      (if (c-lw6ker-get-cursor game-state cursor-id)
	  (lw6-get-cursor-id-not-in-game-state game-state)
	  cursor-id))))

(define lw6-cursor-init-pos
  (lambda (game-state cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   (cursor-id (hash-ref cursor "id"))
	   (cursor-c (c-lw6ker-get-cursor game-state cursor-id))
	   )
      (if cursor
	  (begin
	    (hash-set! cursor "x" (assoc-ref cursor-c "x"))
	    (hash-set! cursor "y" (assoc-ref cursor-c "y"))
	    (hash-set! cursor "letter" (assoc-ref cursor-c "letter"))
	    (hash-set! cursor "team-color" (assoc-ref cursor-c "team-color"))
	    )))))

(define lw6-cursor-init-configured-mover
  (lambda (cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   (player-status-key (string-concatenate (list "player" cursor-key "-status")))
	   (player-control-key (string-concatenate (list "player" cursor-key "-control")))
	   (player-status (lw6-config-is-true? player-status-key))
	   (player-control (lw6-config-get-string player-control-key))
	   )
      (if player-status
	  (hash-set! cursor "mover" (cond
				     ((equal? player-control "mouse")
				      lw6-mover-mouse-func)
				     ((equal? player-control "keyboard")
				      lw6-mover-keyboard-func)
				     ((equal? player-control "joystick1")
				      lw6-mover-joystick1-func)
				     ((equal? player-control "joystick2")
				      lw6-mover-joystick2-func)
				     (#t
				      (lambda (c) #f))
				     ))))))

(define lw6-cursor-init-map-defined-universal-mover
  (lambda (cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   )
      (hash-set! cursor "mover" lw6-mover-universal-func))))

(define lw6-cursor-init-map-defined-semi-universal-mover
  (lambda (cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   )
      (if (equal? cursor-key "1")
	  (hash-set! cursor "mover" lw6-mover-semi-universal-1-func)
	  (hash-set! cursor "mover" lw6-mover-semi-universal-2-func)
	  ))))

(define lw6-cursor-init-map-defined-bot-mover
  (lambda (level game-state pilot cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   (cursor-id (hash-ref cursor "id"))
	   (bot-ai (c-lw6map-param-get level (string-concatenate (list cursor-key "-ai"))))
	   (bot-speed (string->number (c-lw6map-param-get level lw6def-bot-speed)))
	   (bot-iq (string->number (c-lw6map-param-get level lw6def-bot-iq)))
	   (bot (c-lw6bot-new
		 bot-ai
		 game-state
		 pilot
		 (lw6-config-get-number lw6def-dirty-read)
		 cursor-id
		 bot-speed
		 bot-iq))
	   )
      (begin
	(hash-set! cursor "bot" bot)
	(hash-set! cursor "mover" lw6-mover-bot-func)
	))))

(define lw6-cursor-init-bot-mover
  (lambda (cursor-key bot-engine)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   )
      (let (
	    (bot (c-lw6bot-new
		  bot-engine
		  (lw6-get-game-global "game-state")
		  (lw6-get-game-global "pilot")
		  (lw6-config-get-number lw6def-dirty-read)
		  (hash-ref cursor "id")
		  (lw6-config-get-number lw6def-bot-speed)
		  (lw6-config-get-number lw6def-bot-iq)))
	    )
	(begin
	  (hash-set! cursor "bot" bot)
	  (hash-set! cursor "mover" lw6-mover-bot-func)
	  )))))

(define lw6-cursor-init-universal-mover
  (lambda (cursor-key)
    (let* (
	   (cursor (lw6-get-cursor cursor-key))
	   )
      (hash-set! cursor "mover" lw6-mover-universal-func)
      )))

(define lw6-cursor-does-config-match-universal?
  (lambda (cursor-key)
    (let* (
	   (player-status-key (string-concatenate (list "player" cursor-key "-status")))
	   (player-control-key (string-concatenate (list "player" cursor-key "-control")))
	   (player-status (lw6-config-is-true? player-status-key))
	   (player-control (lw6-config-get-string player-control-key))
	   )
      (begin
	(and player-status
	     (or (equal? player-control "keyboard")
		 (equal? player-control "mouse")
		 (equal? player-control "joystick1")
		 (equal? player-control "joystick2")))
	)
      )))

(define lw6-cursor-set-universal-if-needed
  (lambda ()
    (if (and (lw6-cursor-does-config-match-universal? "1")
	     (not (lw6-cursor-does-config-match-universal? "2"))
	     (not (lw6-cursor-does-config-match-universal? "3"))
	     (not (lw6-cursor-does-config-match-universal? "4")))
	(let* (
	       (cursor (lw6-get-cursor "1"))
	       )
	  (hash-set! cursor "mover" lw6-mover-universal-func)))
    ))

