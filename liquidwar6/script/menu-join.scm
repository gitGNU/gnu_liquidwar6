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

(define lw6-join-menu-item
  (lambda (menu entry)
    (let* (
	   (title (assoc-ref entry "title"))
	   (version (assoc-ref entry "version"))
	   (id (assoc-ref entry "id"))
	   (url (assoc-ref entry "url"))
	   (description (assoc-ref entry "description"))
	   (password (if (assoc-ref entry "password") (_ "yes") (_ "no")))
	   (level (assoc-ref entry "level"))
	   (item (lw6-menu-item-template title
					 (format #f
						 "~a: ~a~%~a: ~a~%~a: ~a~%~a: ~a~%~a: ~a~%~a: ~a"
						 (_ "Version") version
						 (_ "Id") id
						 (_ "URL") url
						 (_ "Description") description
						 (_ "Password") password
						 (_ "Map") level)))
	   )
      (begin
	(set! item (assoc-set! item "node-id" id))
	(set! item (assoc-set! item "on-valid" (lambda (mi) (lw6-push-menu (lw6-connecting-menu id url title)))))
	(lw6-append-menuitem! menu item)
	item
	))))

(define lw6-join-menu-none-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template (_ "No server found")
					(_ "No remote node could be found on the network")))
	  )
      (begin
	(assoc-set! item "selected" #t)
	item
	))))

(define lw6-join-menu-update
  (lambda (m)
    (let* (
	  (menu-smob (assoc-ref m "smob"))
	  (selected-item (assoc-ref m "selected-item"))
	  (current-menuitem (lw6-current-menuitem))
	  (current-node-id (if current-menuitem (assoc-ref current-menuitem "node-id") #f))
	  (tmp-item 0)
	  (new-id 0)
	  (node (lw6-get-game-global "node"))
	  (entries (if node (c-lw6p2p-node-get-entries node #t) #f))
	)
      (if menu-smob
	  (begin
	    (c-lw6gui-menu-remove-all menu-smob)
	    (set! m (assoc-set! m "smob" menu-smob))
	    (set! m (assoc-set! m "items" (list)))
	    (set! m (assoc-set! m "selected-item" 0))
	    (if entries
		(map (lambda (entry)
		       (let* (
			      (menuitem (lw6-join-menu-item m entry))
			      (node-id (if menuitem (assoc-ref menuitem "node-id") #f))
			      )
			 (begin
			   ;;(lw6-log-notice (format #f "~a ~a ~a" node-id current-node-id tmp-item))
			   ;;(lw6-log-notice menuitem)
			   (if (and node-id current-node-id (equal? node-id current-node-id))
			       (set! selected-item tmp-item))
			   (set! tmp-item (+ tmp-item 1))
			   menuitem
			   )))
		     entries)
		(lw6-append-menuitem! m (lw6-join-menu-none-item))
		)
	    (if current-node-id
		(begin
		  ;;(lw6-log-notice selected-item)
		  (set! m (assoc-set! m "selected-item" selected-item))
		  ))
	    ;;(lw6-log-notice m)
	    m
	    )
	  #f
	  ))))

(define lw6-join-menu
  (lambda()
    (let* (
	  (menu (lw6-menu-template (_ "Join server")
				   (_ "Choose a server and connect to it, all the nodes which you can technically connect to are listed, however you might not be able to connect to some of them, just because they are either idle, full, running another version of the game, etc.")
				   (_ "Network does NOT work yet")))
	  (node (lw6-get-game-global "node"))
	  (entries (if node (c-lw6p2p-node-get-entries node #t) #f))
	  )
      (begin
	(lw6-set-game-global! "solo" #f)
	(lw6-set-game-global! "network" #t)
	(assoc-set! menu "update" lw6-join-menu-update)
	(set! menu (lw6-join-menu-update menu))
	menu
	))))
