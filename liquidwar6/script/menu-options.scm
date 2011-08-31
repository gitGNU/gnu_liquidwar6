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

(define lw6-options-menu-time-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Time & speed")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-time-options-menu))))
	item
	))))

(define lw6-options-menu-rules-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Rules")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-rules-options-menu))))
	item
	))))

(define lw6-options-menu-video-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Video")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-video-options-menu))))
	item
	))))

(define lw6-options-menu-sound-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Sound")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-audio-options-menu))))
	item
	))))

(define lw6-options-menu-system-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "System")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-system-options-menu))))
	item
	))))

(define lw6-options-menu-stop-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Stop current game")))
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

(define lw6-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Options")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-options-menu-time-item))
	(lw6-append-menuitem! menu (lw6-options-menu-rules-item))
	(lw6-append-menuitem! menu (lw6-options-menu-video-item))
	(lw6-append-menuitem! menu (lw6-options-menu-sound-item))
	(lw6-append-menuitem! menu (lw6-options-menu-system-item))
	menu
	))))

(define lw6-options-menu-light
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Options")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-options-menu-stop-item))
	(lw6-append-menuitem! menu (lw6-options-menu-video-item))
	(lw6-append-menuitem! menu (lw6-options-menu-sound-item))
	(lw6-append-menuitem! menu (lw6-options-menu-system-item))
	menu
	))))
