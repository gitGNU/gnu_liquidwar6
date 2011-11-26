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

(define lw6-join-menu-items
  (lambda (menu entry)
    (let* (
	   (title (assoc-ref entry "title"))
	   (item (lw6-menu-item-template title))
	  )
      (begin
	(lw6-append-menuitem! menu item)
	))))

(define lw6-join-menu-none-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "No server found")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	item
	))))

(define lw6-join-menu
  (lambda()
    (let* (
	  (menu (lw6-menu-template (_ "Play")))
	  (node (lw6-get-game-global "node"))
	  (entries (if node (c-lw6p2p-node-get-entries node) #f))
	  )
      (begin
	(if entries
	    (map (lambda (entry) (lw6-join-menu-items menu entry)) entries)
	    (lw6-append-menuitem! menu (lw6-join-menu-none-item))
	    )
	menu
	))))
