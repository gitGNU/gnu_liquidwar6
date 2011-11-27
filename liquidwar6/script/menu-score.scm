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

(define lw6-score-menu-replay-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Replay")
					(_ "Replay this level, since you didn't win")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	(assoc-set! item "on-valid" (lambda (mi) 
				      (lw6-pop-menu (lw6-current-menu))
				      (lw6-pop-menu (lw6-current-menu))
				      (lw6-game-idle)
				      (let (
					    (menu (lw6-current-menu))
					    (menuitem (lw6-current-menuitem))
					    )
					(if (and menu menuitem)
					    (lw6-menuitem-action menuitem "on-valid")))))
	item
	))))

(define lw6-score-menu-continue-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Continue")
					(_ "Advance to next level")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	(assoc-set! item "on-valid" (lambda (mi) 
				      (lw6-pop-menu (lw6-current-menu))
				      (lw6-pop-menu (lw6-current-menu))
				      (lw6-game-idle)
				      (lw6-loader-purge)
				      (lw6-game-start-local-step1 lw6-game-start-solo-step2)))
	item
	))))

(define lw6-score-menu-back-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Back to previous menu")
					(_ "Go back to previous menu")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (begin
						   (lw6-pop-menu (lw6-current-menu))
						   (lw6-pop-menu (lw6-current-menu))
						   (lw6-game-idle)
						   (lw6-music-ambiance)
						   )))
	item
	))))

(define lw6-score-menu-next
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Next")
				   (_ "And now?")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-score-menu-replay-item))
	(lw6-append-menuitem! menu (lw6-score-menu-back-item))
	menu
	))))

(define lw6-score-solo-menu-next
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Next")
				   (_ "And now?")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-score-menu-continue-item))
	(lw6-append-menuitem! menu (lw6-score-menu-back-item))
	menu
	))))

(define lw6-score-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Score")
				   (_ "And the winner is...")))
	  )
      (begin
	;; no items here!
	(assoc-set! menu "on-cancel" (lambda (m) 
				       (let ((dsp (lw6-get-game-global "dsp")))
					 (begin
					   (c-lw6gui-input-reset dsp)					  
					   (lw6-push-menu (lw6-score-menu-next))))))
	menu
	))))

(define lw6-score-solo-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Score")
				   (_ "And the winner is...")))
	  )
      (begin
	;; no items here!
	(assoc-set! menu "on-cancel" (lambda (m) 
				       (let ((dsp (lw6-get-game-global "dsp")))
					 (begin
					   (c-lw6gui-input-reset dsp)					  
					   (lw6-push-menu (lw6-score-solo-menu-next))))))
	menu
	))))

