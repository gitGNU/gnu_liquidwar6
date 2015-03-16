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

(define lw6-play-menu-solo-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Solo game")
					(_ "Play alone against the computer, try and win levels to get access to new colors and weapons")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	(assoc-set! item "on-valid" (lambda (mi) (lw6-game-start-local-step1 lw6-game-start-solo-step2)))
	item
	))))

(define lw6-play-menu-1on1-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "1 on 1")
					(_ "Play with a friend locally on this computer")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-game-start-local-step1 lw6-game-start-1on1-step2)))
	item
	))))

(define lw6-play-menu-local-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Local multiplayer")
					(_ "Start a local multiplayer session, with up to 4 players, plus some bots")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-game-start-local-step1 lw6-game-start-local-step2)))
	item
	))))

(define lw6-play-menu-join-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Join network game")
					(_ "Join an existing network game, DOES NOT WORK YET")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-join-menu))))
	item
	))))

(define lw6-play-menu-server-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Start server")
					(_ "Start a server, DOES NOT WORK YET")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-game-start-network-step1 lw6-game-start-server-step2)))
	item
	))))

(define lw6-play-menu-demo-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Demo")
					(_ "Watch the AI play against itself")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-game-start-local-step1 lw6-game-start-demo-step2)))
	item
	))))

(define lw6-play-menu-back-to-main
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "Back to main menu")
					(_ "Go back to main menu")))
	  )
      (begin
	(assoc-set! item "on-valid" (lambda (mi) (lw6-init-menu)))
	item
	))))

(define lw6-play-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Play")
				   (_ "Choose your game type")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-play-menu-solo-item))
	(lw6-append-menuitem! menu (lw6-play-menu-1on1-item))
	(lw6-append-menuitem! menu (lw6-play-menu-local-item))
	(if (not (lw6-config-is-true? lw6def-skip-network))
	    (begin
	      (lw6-append-menuitem! menu (lw6-play-menu-join-item))
	      (lw6-append-menuitem! menu (lw6-play-menu-server-item))
	      ))
	(lw6-append-menuitem! menu (lw6-play-menu-demo-item))
	menu
	))))

(define lw6-play-menu-map
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Play")
				   (_ "Choose your game type and play with chosen map")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-play-menu-solo-item))
	(lw6-append-menuitem! menu (lw6-play-menu-1on1-item))
	(lw6-append-menuitem! menu (lw6-play-menu-local-item))
	(if (not (lw6-config-is-true? lw6def-skip-network))
	    (lw6-append-menuitem! menu (lw6-play-menu-server-item)))
	(lw6-append-menuitem! menu (lw6-play-menu-back-to-main))
	menu
	))))
