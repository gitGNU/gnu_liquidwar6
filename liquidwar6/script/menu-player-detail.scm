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

(define lw6-player-detail-menu-name-item
  (lambda (player-prefix)
    (let* (
	   (player-status-key (string-concatenate (list player-prefix "-status")))
	   (player-name-key (string-concatenate (list player-prefix "-name")))
	   (player-name (lw6-config-get-string player-name-key))
	   (item (lw6-menu-item-list-boolean-template
		  player-status-key
		  player-name
		  (_ "Not playing")
		  (_ "Activate or disable a human player")))
	   )
      (begin
	item))))

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
	   (value (assoc-ref menuitem "value"))
	   (player-color-key (assoc-ref menuitem "player-color-key"))
	   (player-color (c-lw6map-team-color-index-to-key value))
	   (player-color-tooltip (c-lw6hlp-about player-color))
	  )
      (begin
	(lw6-config-set-string! player-color-key player-color)
	(set! menuitem (assoc-set! menuitem "tooltip" player-color-tooltip))
	;;(lw6-game-put-local-teams)
      ))))

(define lw6-player-detail-menu-color-item
  (lambda (player-prefix)
    (let* (
	   (player-color-key (string-concatenate (list player-prefix "-color")))
	   (player-color (lw6-config-get-string player-color-key))
	   (player-color-tooltip (c-lw6hlp-about player-color))
	   (item (list (cons "player-color-key" player-color-key)
		       (cons "tooltip" player-color-tooltip)))
	   )
      (begin
	(set! item (lw6-menu-item-list-template-update
		    item
		    lw6-menu-item-list-label-func
		    lw6-player-detail-menu-color-item-update-func
		    lw6-player-detail-menu-color-item-index-func
		    (c-lw6map-team-color-list)
		    ))
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
      (set! menuitem (assoc-set! menuitem "tooltip" (lw6-control-index-to-tooltip value)))
      )))

(define lw6-player-detail-menu-control-item
  (lambda (player-prefix)
    (let* (
	   (player-control-key (string-concatenate (list player-prefix "-control")))
	   (player-control (lw6-config-get-string player-control-key))
	   (item (list (cons "player-control-key" player-control-key)
		       (cons "tooltip" (_ "Choose the control device"))))
	  )
      (begin
	(set! item (lw6-menu-item-list-template-update
	 item
	 lw6-menu-item-list-label-func
	 lw6-player-detail-menu-control-item-update-func
	 lw6-player-detail-menu-control-item-index-func
	 lw6-control-list
	 ))
	(set! item (assoc-set! item "selected" #f))
	(set! item (assoc-set! item "value" (lw6-control-key-to-index player-control)))
	item
	))))

(define lw6-player-detail-menu
  (lambda(player-prefix)
    (let* (
	   (player-name-key (string-concatenate (list player-prefix "-name")))
	   (player-name (lw6-config-get-string player-name-key))
	   (menu (lw6-menu-template player-name
				    (format #f (_ "Change options for player ~a") player-name)
				    #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-player-detail-menu-name-item player-prefix))
	(lw6-append-menuitem! menu (lw6-player-detail-menu-color-item player-prefix))
	(lw6-append-menuitem! menu (lw6-player-detail-menu-control-item player-prefix))
	(set! menu (assoc-set! menu "on-push" (lambda (m) (begin (lw6-game-preview) (lw6-loader-push-ldr-if-needed (lw6-config-get-string lw6def-chosen-map) (lw6-get-game-global "network"))))))
	(set! menu (assoc-set! menu "on-pop" (lambda (m) (begin (lw6-game-idle)))))
	menu
	))))
