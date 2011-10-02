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

(define lw6-score-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Score")))
	  )
      (begin
	;; no items here!
	(tmp "score")
	(assoc-set! menu "on-pop" (lambda (m) 
				    (let ((dsp (lw6-get-game-global "dsp")))
				      (begin
					(c-lw6gui-input-reset dsp)					  
					(lw6-display-no-score-setup)
					))))
	menu
	))))

(define lw6-score-solo-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Score")))
	  )
      (begin
	;; no items here!
	(tmp "score exp")
	(assoc-set! menu "on-pop" (lambda (m) 
				    (let ((dsp (lw6-get-game-global "dsp")))
				      (begin
					(c-lw6gui-input-reset dsp)					  
					(lw6-display-no-score-setup)
					(lw6-game-start-local-step1 lw6-game-start-solo-step2)))))
	menu
	))))
