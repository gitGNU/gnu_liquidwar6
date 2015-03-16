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

(define lw6-wait-menu-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Please wait...")
					    #f))
	  )
      (begin
	(assoc-set! menuitem "selected" #t)
	menuitem
	))))

(define lw6-wait-menu
  (lambda(help)
    (let (
	  (menu (lw6-menu-template (_ "Please wait...")
				   help
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-wait-menu-item))
	(assoc-set! menu "on-cancel" (lambda (m) (begin
						   (lw6-pop-menu m)
						   (lw6-game-idle))))
	menu
	))))
