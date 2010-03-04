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

(define lw6-player-detail-menu-name-item-list
  (lambda ()
    (append (list (cons "" #t) 
		  (cons "" #f))
	    (c-lw6bot-get-backends))))

(define lw6-player-detail-menu-name-item-label-func
  (lambda (menuitem)
    (let* (
	   (values-list (assoc-ref menuitem "values-list"))
	   (player-name (assoc-ref menuitem "player-name"))
	   (player-bot-key (assoc-ref menuitem "player-bot-key"))
	   (player-bot (lw6-config-get-string player-bot-key))
	   (bot-label (assoc-ref values-list player-bot))
	   (player-status-key (assoc-ref menuitem "player-status-key"))
	   (player-status (lw6-config-is-true? player-status-key))
	   )
      (if player-status
	  (if (equal? player-bot "")
	      player-name
	      (if bot-label
		  bot-label
		  (_ "Unknown bot")))
	  (_ "Not playing")))))

(define lw6-player-detail-menu-name-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	   (player-bot-key (assoc-ref menuitem "player-bot-key"))
	   (player-status-key (assoc-ref menuitem "player-status-key"))
	   (p (list-ref (lw6-player-detail-menu-name-item-list) value))
	   (p1 (car p))
	   (p2 (cdr p))
	   )
      (begin
	(lw6-config-set-string! player-bot-key p1)
	(lw6-config-set-boolean! player-status-key 
				 (if (equal? p1 "")
				     p2
				     #t))
	(lw6-game-put-local-teams)
	))))

(define lw6-player-detail-menu-name-item-index-func
  (lambda (menuitem)
    (let* (
	   (values-list (assoc-ref menuitem "values-list"))
	   (player-bot-key (assoc-ref menuitem "player-bot-key"))
	   (player-bot (lw6-config-get-string player-bot-key))
	   (player-status-key (assoc-ref menuitem "player-status-key"))
	   (player-status (lw6-config-is-true? player-status-key))
	   (tmp 0)
	   (index 0)
	  )
      (begin
	(map (lambda (v) (if (equal? player-bot (car v)) 
			     (set! index tmp)
			     (set! tmp (1+ tmp))
			     ))
	     values-list)
	(if (and (= index 1) player-status)
	    (set! index 0))
	index
	))))

(define lw6-player-detail-menu-name-item
  (lambda (player-prefix)
    (let* (
	   (item (list))
	   (player-name-key (string-concatenate (list player-prefix "-name")))
	   (player-name (lw6-config-get-string player-name-key))
	   (player-status-key (string-concatenate (list player-prefix "-status")))
	   (player-status (lw6-config-get-string player-status-key))
	   (player-bot-key (string-concatenate (list player-prefix "-bot")))
	   (player-bot (lw6-config-get-string player-bot-key))
	   )
      (begin
	(set! item (assoc-set! item "player-bot-key" player-bot-key))
	(set! item (assoc-set! item "player-name" player-name))
	(set! item (assoc-set! item "player-status-key" player-status-key))
	(set! item (lw6-menu-item-list-template-update
	 item
	 lw6-player-detail-menu-name-item-label-func
	 lw6-player-detail-menu-name-item-update-func
	 lw6-player-detail-menu-name-item-index-func
	 (lw6-player-detail-menu-name-item-list)))
	(set! item (assoc-set! item "selected" #t))
	item
	))))

(define lw6-player-detail-menu-color-item-index-func
  (lambda (menuitem)
    (let* (
	   (player-color-key (assoc-ref menuitem "player-color-key"))
	   (player-color (lw6-config-get-string player-color-key))
	   )
      (c-lw6map-team-color-key-to-index player-color)
      )))

(define lw6-player-detail-menu-color-item-update-func
  (lambda (menuitem)
    (let* (
	   (player-color-key (assoc-ref menuitem "player-color-key"))
	   (value (assoc-ref menuitem "value"))
	  )
      (begin
	(lw6-config-set-string! player-color-key (c-lw6map-team-color-index-to-key value))
	(lw6-game-put-local-teams)
      ))))

(define lw6-player-detail-menu-color-item
  (lambda (player-prefix)
    (let* (
	   (player-color-key (string-concatenate (list player-prefix "-color")))
	   (player-color (lw6-config-get-string player-color-key))
	   (item (list (cons "player-color-key" player-color-key)))
	  )
      (begin
	(set! item (lw6-menu-item-list-template-update
	 item
	 lw6-menu-item-list-label-func 
	 lw6-player-detail-menu-color-item-update-func
	 lw6-player-detail-menu-color-item-index-func
	 lw6-color-list))
	(set! item (assoc-set! item "selected" #f))
	(set! item (assoc-set! item "value" (c-lw6map-team-color-key-to-index player-color)))
	(set! item (assoc-set! item "colored" #t))
	item
	))))

(define lw6-player-detail-menu-control-item-index-func
  (lambda (menuitem)
    (let* (
	   (player-control-key (assoc-ref menuitem "player-control-key"))
	   (player-control (lw6-config-get-string player-control-key))
	   )
      (lw6-control-key-to-index player-control)
      )))

(define lw6-player-detail-menu-control-item-update-func
  (lambda (menuitem)
    (let* (
	   (player-control-key (assoc-ref menuitem "player-control-key"))
	   (value (assoc-ref menuitem "value"))
	  )
      (lw6-config-set-string! player-control-key (lw6-control-index-to-key value))
      )))

(define lw6-player-detail-menu-control-item
  (lambda (player-prefix)
    (let* (
	   (player-control-key (string-concatenate (list player-prefix "-control")))
	   (player-control (lw6-config-get-string player-control-key))
	   (item (list (cons "player-control-key" player-control-key)))
	  )
      (begin
	(set! item (lw6-menu-item-list-template-update
	 item
	 lw6-menu-item-list-label-func 
	 lw6-player-detail-menu-control-item-update-func
	 lw6-player-detail-menu-control-item-index-func
	 lw6-control-list))
	(set! item (assoc-set! item "selected" #f))
	(set! item (assoc-set! item "value" (lw6-control-key-to-index player-control)))
	item
	))))

(define lw6-player-detail-menu
  (lambda(player-prefix)
    (let* (
	   (player-name-key (string-concatenate (list player-prefix "-name")))
	   (player-name (lw6-config-get-string player-name-key))
	   (menu (lw6-menu-template player-name))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-player-detail-menu-name-item player-prefix))	
	(lw6-append-menuitem! menu (lw6-player-detail-menu-color-item player-prefix))	
	(lw6-append-menuitem! menu (lw6-player-detail-menu-control-item player-prefix))	
	(set! menu (assoc-set! menu "on-push" (lambda (m) (begin (lw6-game-preview) (lw6-loader-push-if-needed (lw6-config-get-string lw6def-chosen-map))))))
	(set! menu (assoc-set! menu "on-pop" (lambda (m) (begin (lw6-game-idle)))))
	menu
	))))
