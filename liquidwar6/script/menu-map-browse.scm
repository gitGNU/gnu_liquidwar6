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

(define lw6-map-browse-menuitem
  (lambda (entry)
    (let* (
	   (title (assoc-ref entry "title"))
	   (author (assoc-ref entry "author"))
	   (description (assoc-ref entry "description"))
	   (license (assoc-ref entry "license"))
	   (has-subdirs (assoc-ref entry "has-subdirs"))
	   (nb-submaps (assoc-ref entry "nb-submaps"))
	   (forbidden (assoc-ref entry "forbidden"))
	   (menuitem (lw6-menu-item-template title
					     (if has-subdirs
						 (format #f
							 (_ "Subfolder with ~a maps")
							 nb-submaps)
						 (if forbidden
						     (_ "You can't play this map yet")
						     (format #f "~a: ~a~%~a: ~a~%~a: ~a"
							     (_ "Author")
							     author
							     (_ "Description")
							     description
							     (_ "License")
							     license)))))
	   (relative-path (assoc-ref entry "relative-path"))
	   (network (lw6-get-game-global "network"))
	   )
      (if has-subdirs
	  (begin
	    (set! menuitem (assoc-set! menuitem "selected" (string-prefix? relative-path (lw6-config-get-string lw6def-chosen-map))))
	    (set! menuitem (assoc-set! menuitem "label" (format #f "~a/ (~a)" title nb-submaps)))
	    (set! menuitem (assoc-set! menuitem "on-select" (lambda (mi) (begin (lw6-game-idle)))))
	    (set! menuitem (assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-map-browse-menurelative (c-lw6sys-path-concat relative-path title))))))
	    (set! menuitem (assoc-set! menuitem "enabled" #t))
	    menuitem
	    )
	  (begin
	    (set! menuitem (assoc-set! menuitem "selected" (equal? relative-path (lw6-config-get-string lw6def-chosen-map))))
	    (set! menuitem (assoc-set! menuitem "on-select" (lambda (mi) (begin (lw6-game-preview) (lw6-loader-push-ldr-if-needed relative-path network) (lw6-config-set-string! lw6def-chosen-map relative-path)))))
	    (set! menuitem (assoc-set! menuitem "on-valid" (lambda (mi) (lw6-push-menu (lw6-play-menu-map)))))
	    (set! menuitem (assoc-set! menuitem "enabled" (not forbidden)))
	    menuitem
	    )
	  ))))

(define lw6-map-browse-menuitem-appender
  (lambda (menu)
    (lambda (entry)
      (lw6-append-menuitem! menu (lw6-map-browse-menuitem entry)))))

(define lw6-map-browse-menurelative
  (lambda (relative-path)
    (let* (
	   (map-parent-path (c-lw6sys-path-parent relative-path))
	   (file-only (c-lw6sys-path-file-only map-parent-path))
	   (map-path (c-lw6cfg-unified-get-map-path))
	   (entries (c-lw6ldr-get-entries map-path map-parent-path))
	   (title (if (or (not file-only)
			  (equal? file-only "")
			  (equal? file-only "."))
		      (_ "Choose map")
		      file-only))
	   (menu (lw6-menu-template title
				    (_ "Browse your hard drive to find a map")
				    #f))
	  )
      (begin
	(map (lw6-map-browse-menuitem-appender menu) entries)
	(set! menu (assoc-set! menu "on-push" (lambda (m) (lw6-game-preview))))
	(set! menu (assoc-set! menu "on-pop" (lambda (m) (lw6-game-idle))))
	menu
	))))

(define lw6-map-browse-menu
  (lambda ()
    (lw6-map-browse-menurelative (lw6-config-get-string lw6def-chosen-map))))

(define lw6-push-map-browse-menu
  (lambda ()
    (let* (
	   (chosen-map (lw6-config-get-string lw6def-chosen-map))
	   (chosen-map-splitted (c-lw6sys-path-split chosen-map))
	   (relative-path "")
	  )
      (begin
	(map (lambda (path-elem) (begin
				   (set! relative-path (c-lw6sys-path-concat relative-path path-elem))
				   (lw6-push-menu-nowarp (lw6-map-browse-menurelative relative-path))
				   ))
	     chosen-map-splitted)
	;(lw6-menu-warp-mouse)
	)
      )))
