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

(define lw6-root-menu-quick-start-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Quick start")
					    (_ "Start playing now!")))
	  )
      (begin
	(assoc-set! menuitem "selected" #t)
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-game-start-local-step1 lw6-game-start-quick-step2)))
	menuitem
	))))

(define lw6-root-menu-play-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Play")
					    (_ "Start a new game")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-play-menu))))
	menuitem
	))))

(define lw6-root-menu-map-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Choose map")
					    (_ "Browse map directory and choose a map to play on")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-map-menu))))
	menuitem
	))))

(define lw6-root-menu-players-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Players")
					    (_ "Choose player name, color (teams), controls (keyboard, mouse, joystick) and populate your map with bots")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-players-menu))))
	menuitem
	))))

(define lw6-root-menu-options-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Options")
					    (_ "Set up video, sound, game rules, and more")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-options-menu))))
	menuitem
	))))

(define lw6-root-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Liquid War 6")
				   (_ "Welcome to Liquid War 6, have fun!")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-root-menu-quick-start-item))
	(lw6-append-menuitem! menu (lw6-root-menu-play-item))
	(lw6-append-menuitem! menu (lw6-root-menu-map-item))
	(lw6-append-menuitem! menu (lw6-root-menu-players-item))
	(lw6-append-menuitem! menu (lw6-root-menu-options-item))
	(assoc-set! menu "on-cancel" (lambda (m) (lw6-push-menu (lw6-exit-menu))))
	menu
	))))
