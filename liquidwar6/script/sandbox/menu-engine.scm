;Liquid War 6 is a unique multiplayer wargame.
;Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>
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

(define %lw6-menu-stack
  '()
  )

(define lw6-push-menu 
  (lambda (menu)
    (begin 
      (set! %lw6-menu-stack (cons menu %lw6-menu-stack))
      ((lw6-menuitem-action "on-select"))
      )
    )
  )

(define lw6-pop-menu 
  (lambda ()
    (begin
      ((lw6-menu-action "on-pop"))
      ((lw6-menuitem-action "on-exit"))
      (set! %lw6-menu-stack (cdr %lw6-menu-stack))
      )
    )
  )

(define lw6-empty-menu-stack?
  (lambda ()
    (equal? '() %lw6-menu-stack)
    )
  )

(define lw6-empty-menu?
  (lambda ()
    (or (equal? '() (lw6-current-menu))
	(equal? '() (assoc-ref (lw6-current-menu) "items")))
    )
  )

(define lw6-clear-menu
  (lambda ()
    (set! %lw6-menu-stack '())
    )
  )

(define lw6-current-menu 
  (lambda ()
    (if (lw6-empty-menu-stack?)
	'()
	(car %lw6-menu-stack)
	)
    )
  )

(define lw6-current-menuitem
  (lambda ()
    (if (lw6-empty-menu?)
	'()
	(let (
	      (current-menu (lw6-current-menu))
	      )
	  (list-ref (assoc-ref current-menu "items")
		    (assoc-ref current-menu "selected-item"))
	  )
	)
    )
  )

(define lw6-prev-menuitem
  (lambda ()
    (begin
      ((lw6-menuitem-action "on-exit"))
      (let (
	    (current-menu (lw6-current-menu))
	    )
	(let
	    (
	     (selected-item (assoc-ref current-menu "selected-item"))
	     )
	  (if (> selected-item 0)
	      (begin
		(lw6-play-sound-beep-select)
		(assoc-set! (lw6-current-menu) "selected-item" (- selected-item 1))
		)
	      )
	  )
	)
      ((lw6-menuitem-action "on-select"))
      )
    )

  )

(define lw6-next-menuitem
  (lambda ()
    (begin
      ((lw6-menuitem-action "on-exit"))
      (let (
	    (current-menu (lw6-current-menu))
	    )
	(let
	    (
	     (selected-item (assoc-ref current-menu "selected-item"))
	     (items (assoc-ref current-menu "items"))
	     )
	  (if (< selected-item ( - (length items) 1))
	      (begin
		(lw6-play-sound-beep-select)
		(assoc-set! (lw6-current-menu) "selected-item" (+ selected-item 1))
		)
	      )
	  )
	)
      ((lw6-menuitem-action "on-select"))
      )
    )
  )

(define lw6-button
  (lambda (label on-enter on-select on-exit get-data) 
    (list
     (cons "type" 1 )
     (cons "enabled" 1 )
     (cons "value" 0 )
     (cons "selected-char" 0 )
     (cons "label" label )
     (cons "on-enter" on-enter )
     (cons "on-select" on-select )
     (cons "on-exit" on-exit)
     (cons "get-data" get-data)
     )
    )
  )

(define lw6-menu-action
  (lambda (action)
    (if (not (lw6-empty-menu-stack?))
	(
	 (lambda (f) 
	   (if f 
	       f 
	       (lambda() #f)
	       )
	   )
	 (assoc-ref (lw6-current-menu) action))
	(lambda () #f)
	)
    )
  )

(define lw6-menuitem-action
  (lambda (action)
    (if (not (lw6-empty-menu-stack?))
	(
	 (lambda (f) 
	   (if f 
	       f 
	       (lambda() #f)
	       )
	   )
	 (assoc-ref (lw6-current-menuitem) action))
	(lambda () #f)
	)
    )
  )

(define lw6-menu-pump-keys
  (lambda ()
    (let
	(
	 (key (c-lw6gfx-poll-key))
	 )
      (if
       key
       (begin
					;(display key)
	 (cond 
	  (
	   (c-lw6gfx-is-key-esc (assoc-ref key "sym"))
	   (lw6-pop-menu)
	   )
	  (
	   (c-lw6gfx-is-key-enter (assoc-ref key "sym"))
	   (begin
	     (lw6-play-sound-beep-valid)
	     ((lw6-menuitem-action "on-enter"))
	     )
	   )
	  (
	   (c-lw6gfx-is-key-quit (assoc-ref key "sym"))
	   (c-lw6gfx-send-quit)
	   )
	  (
	   (c-lw6gfx-is-key-up (assoc-ref key "sym"))
	   (lw6-prev-menuitem)
	   )
	  (
	   (c-lw6gfx-is-key-down (assoc-ref key "sym"))
	   (lw6-next-menuitem)
	   )
	  )
	 )
       )
      )
    )
  )

(define lw6-menu-pump-mouse
  (lambda ()
    (let
	(
	 (move (c-lw6gfx-poll-mouse-move))
	 (button-left (c-lw6gfx-poll-mouse-button 0))
	 (button-right (c-lw6gfx-poll-mouse-button 1))
	 )
      (begin
	(if (and move (not (lw6-empty-menu?))) 
	    (let
		(
		 (menuitem (c-lw6gfx-pick-menuitem (lw6-current-menu) (assoc-ref move "x") (assoc-ref move "y") %lw6-menu-style))
		 )
	      (let
		  ((changed #f))
		(begin
		  (if (and (not (equal? menuitem (assoc-ref (lw6-current-menu) "selected-item")))
			   (>= menuitem 0))
		      (set! changed #t)
		      )
		  (if changed
			  ((lw6-menuitem-action "on-exit"))
			  )
		  (if 
		   (>= menuitem 0)
		   (assoc-set! (lw6-current-menu) "selected-item" menuitem)
		       )
		  (if changed
		      (begin
			(lw6-play-sound-beep-select)
			((lw6-menuitem-action "on-select"))
			)
		      )
		  )
		)
	      ))
	)
      (begin
	(if button-left 
	    (begin
	      (lw6-play-sound-beep-valid)
	      ((lw6-menuitem-action "on-enter"))
	      )
	    )
	)
      (begin
	(if button-right (lw6-pop-menu))
	)
      )
    )
  )

(define lw6-menu
  (lambda () 
    (begin 
      (if (lw6-empty-menu-stack?)
	  (lw6-init-menu)
	  )
      (lw6-menu-pump-keys)
      (lw6-menu-pump-mouse)
      )
    )
  )
