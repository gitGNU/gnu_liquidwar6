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

(define lw6-players-menu-player-item-update
  (lambda (menuitem)
    (let* (
	   (player-prefix (assoc-ref menuitem "player-prefix"))
	   (player-status-key (string-concatenate (list player-prefix "-status")))
	   (player-status (lw6-config-is-true? player-status-key))
	   (player-name-key (string-concatenate (list player-prefix "-name")))
	   (player-name (lw6-config-get-string player-name-key))
	   (player-bot-key (string-concatenate (list player-prefix "-bot")))
	   (player-bot (lw6-config-get-string player-bot-key))
	   (player-color-key (string-concatenate (list player-prefix "-color")))
	   (player-color (lw6-config-get-string player-color-key))
	   (player-control-key (string-concatenate (list player-prefix "-control")))
	   (player-control (lw6-config-get-string player-control-key))
	   (label (if player-status
		      (if (equal? player-bot "")
			  player-name
			  (assoc-ref (c-lw6bot-get-backends) player-bot))
		      (_ "Free slot")))
	   )
      (begin
	(set! menuitem (assoc-set! menuitem "label" label))
	(set! menuitem (assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-player-detail-menu player-prefix)))))
	(set! menuitem (assoc-set! menuitem "value" (c-lw6map-team-color-key-to-index player-color)))
	(set! menuitem (assoc-set! menuitem "colored" player-status))
	menuitem
	))))    
    
(define lw6-players-menu-player-item
  (lambda (player-prefix)
    (let* (
	   (item (lw6-menu-item-template ""))
	  )
      (begin
	(set! item (assoc-set! item "player-prefix" player-prefix))
	(set! item (assoc-set! item "selected" #f))
	(set! item (lw6-players-menu-player-item-update item))
	item
	))))

(define lw6-players-menu-on-pop-child
  (lambda (menu)
    (let* (
	  (menuitem (lw6-current-menuitem))
	  (menu-smob (assoc-ref menu "smob"))
	  )
      (if (assoc-ref menuitem "player-prefix")
	  (begin
	    (set! menuitem (lw6-players-menu-player-item-update menuitem))
	    (c-lw6gui-menu-sync menu-smob menuitem)
	    )
	  ))))

(define lw6-players-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Players")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-players-menu-player-item "player1"))	
	(lw6-append-menuitem! menu (lw6-players-menu-player-item "player2"))
	(lw6-append-menuitem! menu (lw6-players-menu-player-item "player3"))
	(lw6-append-menuitem! menu (lw6-players-menu-player-item "player4"))
	(set! menu (assoc-set! menu "on-pop-child" lw6-players-menu-on-pop-child))
	menu
	))))
