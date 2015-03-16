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

(define lw6-connecting-menu-item
  (lambda (id url title)
    (let (
	  (menuitem (lw6-menu-item-template (format #f (_ "Connect \"~a\"") title)
					    #f))
	  )
      (begin
	(assoc-set! menuitem "selected" #t)
	menuitem
	))))

(define lw6-connecting-menu
  (lambda(id url title)
    (let (
	  (menu (lw6-menu-template (_ "Connecting")
				   (format #f (_ "Please wait, connecting to remote server \"~a\" (~a)") title url)
				   #f))
	   (node (lw6-get-game-global "node"))
	  )
      (begin
	(if (and node (c-lw6p2p-node-client-join node id url))
	    (lw6-append-menuitem! menu (lw6-connecting-menu-item id url title)))
	(assoc-set! menu "on-cancel" (lambda (m)
				       (let ((dsp (lw6-get-game-global "dsp")))
					 (begin
					   (c-lw6gui-input-reset dsp)
					   (lw6-push-menu (lw6-options-menu-light))))))
	menu
	))))
