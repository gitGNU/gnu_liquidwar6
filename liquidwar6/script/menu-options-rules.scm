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

(define lw6-rules-options-menu-respawn-team-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-list-number-template 
				    lw6def-respawn-team
				    (list
				     (_ "One life (last wins)")
				     (_ "Respawn (deatchmatch)"))))
	  )
      (begin
	item
	))))

(define lw6-rules-options-menu-color-conflict-mode-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-list-number-template 
				    lw6def-color-conflict-mode
				    (list
				     (_ "Cursors can have any color")
				     (_ "Can collaborate locally")
				     (_ "One cursor per team"))))
	  )
      (begin
	item
	))))

(define lw6-rules-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Rules")))
	  (old-respawn-team (lw6-config-get-number lw6def-respawn-team))
	  (old-color-conflict-mode (lw6-config-get-number lw6def-color-conflict-mode))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-rules-options-menu-respawn-team-item))
	(lw6-append-menuitem! menu (lw6-rules-options-menu-color-conflict-mode-item))
	(set! menu (assoc-set! menu "on-pop" 
			       (lambda (m) 
				 (if 
				  (or (not (equal? old-respawn-team (lw6-config-get-number lw6def-respawn-team)))
				      (not (equal? old-color-conflict-mode (lw6-config-get-number lw6def-color-conflict-mode))))
				  (begin (lw6-game-param-update) 
					 (lw6-loader-push (lw6-config-get-string lw6def-chosen-map)))))))
	menu
	))))
