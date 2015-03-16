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

(define lw6-map-menu-map-browse-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Browse maps")
					    (_ "Browse map directory and choose a map to play on")))
	  )
      (begin
	(assoc-set! menuitem "selected" #t)
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-map-browse-menu)))
	menuitem
	))))

(define lw6-map-menu-map-random-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Random map")
					    (_ "Play on a randomly generated map")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi)
					  (begin (lw6-game-preview)
						 (lw6-loader-push-gen (c-lw6gen-seed-new)
								      (lw6-get-game-global "network")))))
	menuitem
	))))

(define lw6-map-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Map")
				   (_ "Choose the map you want to play on")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-map-menu-map-browse-item))
	(lw6-append-menuitem! menu (lw6-map-menu-map-random-item))
	menu
	))))

