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

(define lw6-exit-menu 
  (lambda () 
    (list
     (cons "id" "exit" )
     (cons "label" (_ "Exit?") )
     (cons "type" 1 )
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items" 
	   (list
	    (lw6-button (_ "Exit") (lambda () (c-lw6gfx-send-quit)) #f #f #f)
	    (lw6-button (_ "Resume LW6") (lambda () (lw6-push-menu (lw6-main-menu))) #f #f #f)
	    )
	   ) 
     )
    )
  )

(define lw6-main-menu 
  (lambda () 
    (list
     (cons "id" "main" )
     (cons "label" (_ "Liquid War 6") )
     (cons "type" 1 )
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items" 
	   (list
	    (lw6-button (_ "Quick start") 
			lw6-quick-start #f #f #f)
	    (lw6-button (_ "Local game") #f #f #f #f)
	    (lw6-button (_ "Network game") #f #f #f #f)
	    (lw6-button (_ "Level") 
			(lambda () (begin (lw6-push-menu (lw6-level-menu "")) (lw6-preview-chosen))) lw6-preview-chosen lw6-game-idle #f)
	    (lw6-button (_ "Teams") #f #f #f #f)
	    (lw6-button (_ "Options") 
			(lambda () (lw6-push-menu (lw6-options-menu))) #f #f #f)
	    (lw6-button (_ "About") 
			(lambda () (lw6-push-menu (lw6-about-menu))) #f #f #f)
	    )
	   ) 
     )
    )
  )

(define lw6-quick-start-menu 
  (lambda ()
    (list
     (cons "id" lw6def-quick-start)
     (cons "label" (_ "Quick start"))
     (cons "type" 1)
     (cons "on-pop" lw6-game-idle)
     (cons "selected-item" 0)
     (cons "items"
	   (list)
	   )
     )
    )
  )

(define lw6-about-menu 
  (lambda () 
    (list
     (cons "id" lw6def-about )
     (cons "label" (_ "About") )
     (cons "type" 1 )
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items" 
	   (list
	    (lw6-button (_ "Authors") #f #f #f #f)
	    (lw6-button (_ "Copyright") #f #f #f #f)
	    (lw6-button (_ "License") #f #f #f #f)
	    (lw6-button (_ "Version") #f #f #f #f)
	    )
	   ) 
     )
    )
  )

(define lw6-view-level-menu 
  (lambda () 
    (list
     (cons "id" "view-level" )
     (cons "label" (_ "View level") )
     (cons "type" 1 )
     (cons "on-pop" lw6-preview-stop-demo)
     (cons "selected-item" 0)
     (cons "items" 
	   (list)
	   ) 
     )
    )
  )

(define lw6-confirm-level-menu 
  (lambda (path) 
    (list
     (cons "id" "confirm-level" )
     (cons "label" (_ "Confirm level choice") )
     (cons "type" 1 )
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items" 
	   (list
	    (lw6-button (_ "Choose it") 
			(lambda () 
			  (begin
			    (lw6-config-set-string! lw6def-chosen-map path)
			    (lw6-load-chosen-map)
			    (lw6-pop-menu)
			    (while (equal? 
				    (assoc-ref (lw6-current-menu) "id") 
				    "level")
				   (lw6-pop-menu)
				   )
			    )
			  ) #f #f #f)
	    (lw6-button (_ "View") 
			(lambda () 
			  (begin
			    (lw6-preview-start-demo)
			    (lw6-push-menu (lw6-view-level-menu))
			    )
			  ) #f #f #f)
	    (lw6-button (_ "Cancel") 
			(lambda () 
			  (lw6-pop-menu)
			  ) #f #f #f)
	    )
	   ) 
     )
    )
  )

(define lw6-level-menu-buttons
  (lambda (dir)
    (map (lambda (x) 
	   (let (
		 (title (assoc-ref x "title"))
		 (absolute-path (assoc-ref x "absolute-path"))
		 (relative-path (assoc-ref x "relative-path"))
		 (has-subdirs (assoc-ref x "has-subdirs"))
		 )
	     (lw6-button 
	      (if has-subdirs
		  (string-append "/" title)
		  title		  
		  )		
	      (lambda () (if has-subdirs
			     (lw6-push-menu 
			      (lw6-level-menu relative-path))
			     (lw6-push-menu
			      (lw6-confirm-level-menu relative-path))
			     ))
	      (lambda () (if (not has-subdirs)
			     (let (
				   (map-source (lw6-get-game-global "map-source"))
				   )
			       (if (not (and map-source
					     (equal? relative-path 
						   (assoc-ref map-source "path"))))
				   (lw6-preview relative-path)
				   )
			       )
			     )
		      )
	      lw6-update-map-source-from-chosen
	      (lambda () relative-path)
	      )
	     )	   
	   ) 
	 (c-lw6ldr-get-entries (c-lw6cfg-unified-get-map-path) dir))
    )
  )

(define lw6-find-best-level-item
  (lambda (items path)
    (let 
	(
	 (i 0)
	 (max-length 0)
	 (best-level-item 0)
	 )
       (begin
	 (while (< i (length items))
		(let*
		    (
		     (item (list-ref items i))
		     (item-path ((assoc-ref item "get-data")))
		     (item-path-length (string-length item-path))
		     )
		  (begin
		    (if (<= item-path-length (string-length path))
			(begin
			  (if (and
			       (equal? 
				(substring path 0 item-path-length)
				item-path)
			       (> item-path-length max-length))
			      (begin
				(set! max-length item-path-length)
				(set! best-level-item i)
				))))
		    (set! i (+ i 1))
		    )		  
		  )
		)
	 best-level-item
	 )
       )
    )
  )

(define lw6-level-menu 
  (lambda (dir)
    (let* (
	  (items (lw6-level-menu-buttons dir))
	  (selected-item (lw6-find-best-level-item items (lw6-config-get-string lw6def-chosen-map)))
	  )
      (list
	 (cons "id" "level" )
	 (cons "label" (_ "Level") )
	 (cons "type" 1 )
	 (cons "on-pop" #f)
	 (cons "selected-item" selected-item)
	 (cons "items" items)
	)
      )
    )
  )

(define lw6-options-menu 
  (lambda ()
    (list
     (cons "id" "options")
     (cons "label" (_ "Options"))
     (cons "type" 1)
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items"
	   (list
	    (lw6-button (_ "Graphics") 
			(lambda () (lw6-push-menu (lw6-graphics-options-menu)))
			#f #f #f)
	    (lw6-button (_ "Sound") 
			(lambda () (lw6-push-menu (lw6-sound-options-menu)))
			#f #f #f)
	    )
	   )
     )
    ))

(define lw6-graphics-options-menu 
  (lambda ()
    (list
     (cons "id" "graphics-options")
     (cons "label" (_ "Graphics options"))
     (cons "type" 1)
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items"
	   (list
	    (lw6-button (_ "Fullscreen") 
			#f
			#f #f #f)
	    (lw6-button (_ "Graphics backend") 
			(lambda () (lw6-push-menu (lw6-graphics-backend-menu)))
			#f #f #f)
	    )
	   )
     )
    ))

(define lw6-sound-options-menu 
  (lambda ()
    (list
     (cons "id" "sound-options")
     (cons "label" (_ "Sound options"))
     (cons "type" 1)
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items"
	   (list
	    (lw6-button (_ "Sound fx volume") 
			#f
			#f #f #f)
	    (lw6-button (_ "Music volume") 
			#f
			#f #f #f)
	    (lw6-button (_ "Sound backend") 
			(lambda () (lw6-push-menu (lw6-sound-backend-menu)))
			#f #f #f)
	    )
	   )
     )
    ))

(define lw6-gfx-backend-item->menu-item
  (lambda (backend-item)
    (let (
	  (id (car backend-item))
	  (label (cdr backend-item))
	  )
      (lw6-button label
		  (lambda () (let (
				   (current-time (c-lw6gfx-get-ticks))
				   )
			       (begin
				 (c-lw6gfx-quit)
			       (c-lw6gfx-destroy-backend)
			       (c-lw6gfx-create-backend id)
			       (c-lw6gfx-init (lw6-config-get-number lw6def-width) (lw6-config-get-number lw6def-height) (lw6-config-is-true? lw6def-fullscreen) current-time)
			       )))
		  #f #f #f)
      )
    ))

(define lw6-snd-backend-item->menu-item
  (lambda (backend-item)
    (let (
	  (id (car backend-item))
	  (label (cdr backend-item))
	  )
      (lw6-button label
		  (lambda () (begin
			       (c-lw6snd-quit)
			       (c-lw6snd-destroy-backend)
			       (c-lw6snd-create-backend id)
			       (c-lw6snd-init 0.5 0.5)))
		  #f #f #f)
      )
    ))

(define lw6-graphics-backend-menu 
  (lambda ()
    (list
     (cons "id" "graphics-backend")
     (cons "label" (_ "Graphics backend"))
     (cons "type" 1)
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items"
	   (map lw6-gfx-backend-item->menu-item (c-lw6gfx-get-backends))
	   )
     )
    ))

(define lw6-sound-backend-menu 
  (lambda ()
    (list
     (cons "id" "sound-backend")
     (cons "label" (_ "Sound backend"))
     (cons "type" 1)
     (cons "on-pop" #f)
     (cons "selected-item" 0)
     (cons "items"
	   (map lw6-snd-backend-item->menu-item (c-lw6snd-get-backends))
	   )
     )
    ))

(define lw6-init-menu
  (lambda ()
    (begin
      (lw6-push-menu (lw6-exit-menu))
      (lw6-push-menu (lw6-main-menu))
      )
    )
  )
