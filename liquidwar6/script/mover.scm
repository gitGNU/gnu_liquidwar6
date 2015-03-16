;; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
; ;
;;(at your option) any later version.
; ;This program is distributed in the hope that it will be useful,
; ;but WITHOUT ANY WARRANTY; without even the implied warranty of
; ;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; ;GNU General Public License for more details.
;;
; ;You should have received a copy of the GNU General Public License
; ;along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
; ;Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
; ;Contact author        : ufoot@ufoot.org

(define %lw6-sensitivity-correction 2.)

(define lw6-mover-slow-down
  (lambda (cursor sensitivity key)
    (let (
	  (value (hash-ref cursor key))
	  )
      (hash-set! cursor key (cond
			     ((> value 0)
			      (max 0 (- value (* 2.0 (/ sensitivity %lw6-sensitivity-correction)))))
			     ((< value 0)
			      (min 0 (+ value (* 2.0 (/ sensitivity %lw6-sensitivity-correction)))))
			     (#t 0)
			     )))))

(define lw6-mover-minus
  (lambda (cursor sensitivity max-cursor-speed key)
    (let (
	  (value (hash-ref cursor key))
	  )
      (hash-set! cursor key (cond
			     ((> value 0)
			      0)
			     (#t
			      (max (- 0 max-cursor-speed) (- value (/ sensitivity %lw6-sensitivity-correction))))
			     )))))

(define lw6-mover-plus
  (lambda (cursor sensitivity max-cursor-speed key)
    (let (
	  (value (hash-ref cursor key))
	  )
      (hash-set! cursor key (cond
			     ((< value 0)
			      0)
			     (#t
			      (min max-cursor-speed (+ value (/ sensitivity %lw6-sensitivity-correction))))
			     )))))

(define lw6-mover-generic
  (lambda (get-state-func pop-fire-func pop-fire2-func sensitivity-func max-cursor-speed-func)
    (lambda (cursor)
      (let* (
	     (state (get-state-func))
	     (fire (or (pop-fire-func) (hash-ref cursor "fire")))
	     (fire2 (or (pop-fire2-func) (hash-ref cursor "fire2")))
	     (up (assoc-ref state "up"))
	     (down (assoc-ref state "down"))
	     (left (assoc-ref state "left"))
	     (right (assoc-ref state "right"))
	     (sensitivity (sensitivity-func))
	     (max-cursor-speed (max-cursor-speed-func))
	     (moved #f)
	    )
	(begin
	  (hash-set! cursor "fire" fire)
	  (hash-set! cursor "fire2" fire2)
	  (if up
	      (if down
		  #t
		  (lw6-mover-minus cursor sensitivity max-cursor-speed "vy"))
	      (if down
		  (lw6-mover-plus cursor sensitivity max-cursor-speed "vy")
		  (lw6-mover-slow-down cursor sensitivity "vy")))
	  (if left
	      (if right
		  #t
		  (lw6-mover-minus cursor sensitivity max-cursor-speed "vx"))
	      (if right
		  (lw6-mover-plus cursor sensitivity max-cursor-speed "vx")
		  (lw6-mover-slow-down cursor sensitivity "vx")))
	  (if (or (not (= (hash-ref cursor "vx") 0))
		  (not (= (hash-ref cursor "vy") 0)))
	      (begin
		(hash-set! cursor "mouse-controlled" #f)
		(hash-set! cursor "x" (+ (hash-ref cursor "x") (hash-ref cursor "vx")))
		(hash-set! cursor "y" (+ (hash-ref cursor "y") (hash-ref cursor "vy")))
		#t)
	      #f
	      )
	)))))

(define lw6-mover-mouse-func
  (lambda (cursor)
    (let*
	(
	 (dsp (lw6-get-game-global "dsp"))
	 (mouse-state (c-lw6gui-mouse-poll-move dsp))
	 ;; Default binding is to map left simple on fire, and right
	 ;; on alternate fire, but on a tablet PC and/or tactile screen
	 ;; we use simple/double-clicks instead
	 (use-double-click (lw6-config-is-true? lw6def-use-double-click))
	 (fire (or (if use-double-click
		       (c-lw6gui-mouse-pop-double-click dsp) #f)
		   (c-lw6gui-mouse-pop-button-right dsp)
		   (hash-ref cursor "fire")))
	 (fire2 (or (if use-double-click
			(c-lw6gui-mouse-pop-triple-click dsp) #f)
		    (c-lw6gui-mouse-pop-button-middle dsp)
		    (hash-ref cursor "fire2")))
	 (map-pos-x (assoc-ref mouse-state "map-pos-x"))
	 (map-pos-y (assoc-ref mouse-state "map-pos-y"))
	 (menu-esc (assoc-ref mouse-state "menu-esc"))
	 )
      (begin
	;; We flush the buttons, test is done
	(c-lw6gui-mouse-pop-button-left dsp)
	(c-lw6gui-mouse-pop-button-right dsp)
	(c-lw6gui-mouse-pop-button-middle dsp)
	(c-lw6gui-mouse-pop-simple-click dsp)
	(c-lw6gui-mouse-pop-double-click dsp)
	(c-lw6gui-mouse-pop-triple-click dsp)
	(hash-set! cursor "fire" fire)
	(hash-set! cursor "fire2" fire2)
	(if (or mouse-state (hash-ref cursor "mouse-controlled"))
	    (begin
	      (if (not mouse-state)
		  (begin
		    (set! mouse-state (c-lw6gui-mouse-get-state dsp))
		    (set! map-pos-x (assoc-ref mouse-state "map-pos-x"))
		    (set! map-pos-y (assoc-ref mouse-state "map-pos-y"))
		    ))
	      (if (or (not (= (hash-ref cursor "x") map-pos-x))
		      (not (= (hash-ref cursor "y") map-pos-y))
	              (not (= (hash-ref cursor "map-pos-x") map-pos-x))
		      (not (= (hash-ref cursor "map-pos-y") map-pos-y)))
		  (begin
		    (hash-set! cursor "x" map-pos-x)
		    (hash-set! cursor "y" map-pos-y)
		    (hash-set! cursor "map-pos-x" map-pos-x)
		    (hash-set! cursor "map-pos-y" map-pos-y)
		    (hash-set! cursor "mouse-controlled" #t)
		    #t)
		  #f)
	      #t
	      )
	    #f
	    )))))

(define lw6-mover-keyboard-func
  (lw6-mover-generic (lambda () (c-lw6gui-keyboard-get-move-pad (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-keyboard-pop-key-ctrl (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-keyboard-pop-key-alt (lw6-get-game-global "dsp")))
		     (lambda () (lw6-config-get-number lw6def-cursor-sensitivity))
		     (lambda () (lw6-config-get-number lw6def-max-cursor-speed))
		     ))

(define lw6-mover-joystick1-func
  (lw6-mover-generic (lambda () (c-lw6gui-joystick1-get-move-pad (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-joystick1-pop-button-c (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-joystick1-pop-button-d (lw6-get-game-global "dsp")))
		     (lambda () (lw6-config-get-number lw6def-cursor-sensitivity))
		     (lambda () (lw6-config-get-number lw6def-max-cursor-speed))
		     ))

(define lw6-mover-joystick2-func
  (lw6-mover-generic (lambda () (c-lw6gui-joystick2-get-move-pad (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-joystick2-pop-button-c (lw6-get-game-global "dsp")))
		     (lambda () (c-lw6gui-joystick2-pop-button-d (lw6-get-game-global "dsp")))
		     (lambda () (lw6-config-get-number lw6def-cursor-sensitivity))
		     (lambda () (lw6-config-get-number lw6def-max-cursor-speed))
		     ))

(define lw6-mover-bot-func
  (lambda (cursor)
    (let* (
	   (bot (hash-ref cursor "bot"))
	   (pos (if bot (c-lw6bot-next-move bot) #f))
	   )
      (if (and bot pos)
	  (begin
	    (hash-set! cursor "x" (assoc-ref pos "x"))
	    (hash-set! cursor "y" (assoc-ref pos "y"))
	    (hash-set! cursor "fire" (or (assoc-ref pos "fire") (hash-ref cursor "fire")))
	    (hash-set! cursor "fire2" (or (assoc-ref pos "fire2") (hash-ref cursor "fire2")))
	    ;;(tmp (list (assoc-ref pos "x") (assoc-ref pos "y")))
	    )))))

(define lw6-mover-universal-func
  (lambda (cursor)
    (begin
      ((lw6-mover-generic (lambda ()
			    (let* (
				   (dsp (lw6-get-game-global "dsp"))
				   (keyboard-state (c-lw6gui-keyboard-get-move-pad dsp))
				   (joystick1-state (c-lw6gui-joystick1-get-move-pad dsp))
				   (joystick2-state (c-lw6gui-joystick2-get-move-pad dsp))
				   )
			      (map (lambda (dir)
				     (cons dir (or (assoc-ref keyboard-state dir)
						   (assoc-ref joystick1-state dir)
						   (assoc-ref joystick2-state dir))))
				   (list "up" "down" "left" "right"))
				))
			  (lambda () (or (c-lw6gui-keyboard-pop-key-ctrl (lw6-get-game-global "dsp"))
					 (c-lw6gui-joystick1-pop-button-c (lw6-get-game-global "dsp"))
					 (c-lw6gui-joystick2-pop-button-c (lw6-get-game-global "dsp"))))
			  (lambda () (or (c-lw6gui-keyboard-pop-key-alt (lw6-get-game-global "dsp"))
					 (c-lw6gui-joystick1-pop-button-d (lw6-get-game-global "dsp"))
					 (c-lw6gui-joystick2-pop-button-d (lw6-get-game-global "dsp"))))
			  (lambda () (lw6-config-get-number lw6def-cursor-sensitivity))
			  (lambda () (lw6-config-get-number lw6def-max-cursor-speed))
			  ) cursor)
      (lw6-mover-mouse-func cursor)
      )))

(define lw6-mover-semi-universal-1-func
  (lambda (cursor)
    (begin
      (lw6-mover-joystick1-func cursor)
      (lw6-mover-mouse-func cursor)
      )))

(define lw6-mover-semi-universal-2-func
  (lambda (cursor)
    ((lw6-mover-generic (lambda ()
			  (let* (
				 (dsp (lw6-get-game-global "dsp"))
				 (keyboard-state (c-lw6gui-keyboard-get-move-pad dsp))
				 (joystick2-state (c-lw6gui-joystick2-get-move-pad dsp))
				 )
			    (map (lambda (dir)
				   (cons dir (or (assoc-ref keyboard-state dir)
						 (assoc-ref joystick2-state dir))))
				 (list "up" "down" "left" "right"))
				))
			(lambda () (or (c-lw6gui-keyboard-pop-key-ctrl (lw6-get-game-global "dsp"))
				       (c-lw6gui-joystick2-pop-button-c (lw6-get-game-global "dsp"))))
			(lambda () (or (c-lw6gui-keyboard-pop-key-alt (lw6-get-game-global "dsp"))
				       (c-lw6gui-joystick2-pop-button-d (lw6-get-game-global "dsp"))))
			(lambda () (lw6-config-get-number lw6def-cursor-sensitivity))
			(lambda () (lw6-config-get-number lw6def-max-cursor-speed))
			) cursor)
    ))

(define lw6-mover
  (lambda ()
    (map (lambda (cursor-key) (let* (
				     (game-state (lw6-get-game-global "game-state"))
				     (cursor (lw6-get-cursor cursor-key))
				     (mover (hash-ref cursor "mover"))
				     )
				(if (and game-state mover cursor (hash-ref cursor "x") (hash-ref cursor "y"))
				    (begin
				      (mover cursor)
				      (let* (
					     (coords (c-lw6pil-fix-coords-x10 game-state (hash-ref cursor "x") (hash-ref cursor "y") 0.0))
					     ;; (coords (list (cons "x" (hash-ref cursor "x")) (cons "y" (hash-ref cursor "y")) 0.0))
					     (x (assoc-ref coords "x"))
					     (y (assoc-ref coords "y"))
					     )
					(begin
					  (hash-set! cursor "x" x)
					  (hash-set! cursor "y" y)
					  ))
				      )
				    )))
	 lw6-cursor-keys)))
