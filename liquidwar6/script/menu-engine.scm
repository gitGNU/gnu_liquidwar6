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

(define %lw6-menu-stack
  '()
  )

(define lw6-purge-menu-stack
  (lambda ()
    (set! %lw6-menu-stack '())
    ))

(define lw6-empty-menu-stack?
  (lambda ()
    (equal? '() %lw6-menu-stack)
    ))

(define lw6-menu-update-selected-item
  (lambda (m)
    (let (
	  (counter 0)
	  (last-selected 0)
	  )
      (begin
	(map (lambda (mi) 
	       (begin
		 (if (assoc-ref mi "selected")
		     (set! last-selected counter))
		 (set! counter (+ counter 1))
		 ))
	     (assoc-ref m "items"))
	(set! m (assoc-set! m "selected-item" last-selected))
	(set! counter 0)
	(map (lambda (mi) 
	       (begin
		 (assoc-set! mi "selected" (equal? counter last-selected))
		 (set! counter (+ counter 1))
		 ))
	     (assoc-ref m "items"))
	m
       ))))
    
;(define lw6-menu-warp-mouse
;  (lambda ()
;    (let ((menu (lw6-current-menu)))
;      (if menu
;	  (c-lw6gfx-warp-mouse-on-menuitem (assoc-ref menu "smob") (assoc-ref menu "selected-item") (lw6-get-game-global "look"))))))

;(define lw6-menu-center
;  (lambda ()
;    (let ((menu (lw6-current-menu)))
;      (if menu
;	  (c-lw6gfx-center-menu (assoc-ref menu "smob") (assoc-ref menu "selected-item") (lw6-get-game-global "look"))))))

(define lw6-push-menu-nowarp
  (lambda (menu)
    (begin 
      (lw6-menu-action (lw6-current-menu) "on-push-child")
      (set! %lw6-menu-stack (cons menu %lw6-menu-stack))
      (lw6-menu-action menu "on-push") 
      (lw6-menu-update-selected-item menu)
      (lw6-menu-sync menu)
      (lw6-menuitem-action (lw6-current-menuitem) "on-select")
      ;(lw6-menu-center)
      )))

(define lw6-push-menu 
  (lambda (menu)
    (begin 
      (lw6-push-menu-nowarp menu)
      ;(lw6-menu-warp-mouse)
      )))

(define lw6-pop-menu 
  (lambda (menu)
    (begin
      (lw6-menu-action menu "on-pop")
      (set! %lw6-menu-stack (cdr %lw6-menu-stack))
      (lw6-menu-action (lw6-current-menu) "on-pop-child")
      (let (
	    (menu (lw6-current-menu))
	    )
	(if menu
	    (begin
	      ;(lw6-menu-center)
	      ;(lw6-menu-warp-mouse)
	      ))))))

(define lw6-current-menu 
  (lambda ()
    (if (lw6-empty-menu-stack?)
	#f
	(car %lw6-menu-stack)
	)))

(define lw6-menu-has-items?
  (lambda (menu)
    (let (
	  (items (if menu (assoc-ref menu "items") #f))
	  )
      (if (and items (> (length items) 0)) #t #f))))

(define lw6-menu-pump-all?
  (lambda ()
    (let (
	  (menu (lw6-current-menu))
	  )
      (if menu
	  (lw6-menu-has-items? menu)
	  #f))))

(define lw6-current-menuitem
  (lambda ()
    (let* (
	   (current-menu (lw6-current-menu))
	   (items (if current-menu (assoc-ref current-menu "items")))
	   )
      (if (lw6-menu-has-items? current-menu)
	  (list-ref items (assoc-ref current-menu "selected-item"))
	  #f))))

(define lw6-set-menuitem!
  (lambda (i allow-scroll)
    (let (
	  (menuitem (lw6-current-menuitem))
	  )
      (if menuitem
	  (let* (
		 (menu (lw6-current-menu))
		 (items (assoc-ref menu "items"))
		 (selected-item (assoc-ref menu "selected-item"))
		 (nb-items (length items))
		 (new-selected-item (max 0 (min i (- nb-items 1))))
		 )
	    (if (= selected-item new-selected-item)
		#f
		(begin
		  (lw6-play-sound-beep-select)
		  (lw6-menuitem-action menuitem "on-unselect")
		  (assoc-set! menu "selected-item" new-selected-item)
		  (assoc-set! menu "allow-scroll" allow-scroll)
		  (lw6-menu-sync menu)
		  (lw6-menuitem-action (lw6-current-menuitem) "on-select")
		  ;(if warp-mouse (lw6-menu-warp-mouse))
		  #t
		  )))))))

(define lw6-prev-menuitem
  (lambda (allow-scroll)
    (let (
	  (menu (lw6-current-menu))
	  (menuitem (lw6-current-menuitem))
	  )
      (if menuitem
	  (lw6-set-menuitem! (- (assoc-ref menu "selected-item") 1) allow-scroll)
	  ))))

(define lw6-next-menuitem
  (lambda (allow-scroll)
    (let (
	  (menu (lw6-current-menu))
	  (menuitem (lw6-current-menuitem))
	  )
      (if menuitem
	  (lw6-set-menuitem! (+ (assoc-ref menu "selected-item") 1) allow-scroll)
	  ))))

(define lw6-menu-action
  (lambda (menu action)
    (if (and menu action)
	((assoc-ref menu action) menu)
	#f )))

(define lw6-menuitem-action
  (lambda (menuitem action)
    (if (and menuitem action)
	((assoc-ref menuitem action) menuitem)
	#f )))

(define lw6-menu-template
  (lambda (title)
    (list
     (cons "smob" (c-lw6gui-menu-new title 
				     (_ "Esc") 
				     (lw6-config-is-true? lw6def-use-esc-button)))
     (cons "selected-item" 0)
     (cons "allow-scroll" #f)
     (cons "items" (list))
     (cons "on-pop" (lambda (m) #t))
     (cons "on-push" (lambda (m) #t))
     (cons "on-push-child" (lambda (m) #t))
     (cons "on-pop-child" (lambda (m) #t))
     (cons "on-cancel" lw6-pop-menu)
     (cons "on-display" (lambda (m) #t)))))

(define lw6-menu-item-template
  (lambda (label)
    (let (
	  (menuitem (list (cons "label" label)))
	  )
      (begin
	(set! menuitem (lw6-menu-item-template-update menuitem))
	menuitem
	))))

(define lw6-menu-item-template-update
  (lambda (menuitem)
    (let (
	  (mi menuitem)
	  )
      (begin
	(set! mi (assoc-set! mi "value" 0))
	(set! mi (assoc-set! mi "enabled" #t))
	(set! mi (assoc-set! mi "selected" #f))
	(set! mi (assoc-set! mi "colored" #f))
	(set! mi (assoc-set! mi "on-valid" (lambda (mi) #t)))
	(set! mi (assoc-set! mi "on-plus" (lambda (mi) #t)))
	(set! mi (assoc-set! mi "on-minus" (lambda (mi) #t)))
	(set! mi (assoc-set! mi "on-select" (lambda (mi) #t)))
	(set! mi (assoc-set! mi "on-unselect" (lambda (mi) #t)))
	mi
	))))

(define lw6-menu-item-list-label-func
  (lambda (menuitem)
    (let* (
	   (values-list (assoc-ref menuitem "values-list"))
	   (value (assoc-ref menuitem "value"))
	   )
      (list-ref values-list value)
      )))

(define lw6-menu-item-list-number-update-func
  (lambda (param)
    (lambda (menuitem)
      (let* (
	     (values-list (assoc-ref menuitem "values-list"))
	     (value (assoc-ref menuitem "value"))
	     )
	(lw6-config-set-number! param value)
	))))

(define lw6-menu-item-list-boolean-update-func
  (lambda (param)
    (lambda (menuitem)
      (let* (
	     (values-list (assoc-ref menuitem "values-list"))
	     (value (assoc-ref menuitem "value"))
	     )
	(lw6-config-set-boolean! param (not (= 0 value)))
	))))

(define lw6-menu-item-list-number-index-func
  (lambda (param)
    (lambda (menuitem)
      (let* (
	     (values-list (assoc-ref menuitem "values-list"))
	     (value (lw6-config-get-number param))
	     (max-index (- (length values-list) 1))
	     )
	(begin
	  (if (< value 0)
	      (set! value 0))
	  (if (> value max-index)
	      (set! value max-index))
	  value
	)))))

(define lw6-menu-item-list-boolean-index-func
  (lambda (param)
    (lambda (menuitem)
      (let* (
	     (values-list (assoc-ref menuitem "values-list"))
	     (value (assoc-ref menuitem "value"))
	     )
	(if (lw6-config-is-true? param) 1 0)
	))))

(define lw6-menu-item-list-template-update
  (lambda (item label-func update-func index-func values-list)
    (begin
      (set! item (lw6-menu-item-template-update item))
      (set! item (assoc-set! item "values-list" values-list))
      (set! item (assoc-set! item "value" (index-func item)))
      (set! item (assoc-set! item "label" (label-func item)))
      (set! item (assoc-set! item "on-plus" 
		  (lambda (mi)		      
		    (let* (
			   (menu-smob (assoc-ref (lw6-current-menu) "smob"))
			   (value (assoc-ref mi "value"))
			   )
		      (begin
			(set! value (+ value 1))
			(if (>= value (length values-list))
			    (set! value 0))
			(assoc-set! mi "value" value)
			(update-func mi)
			(assoc-set! mi "label" (label-func mi))
			(c-lw6gui-menu-sync menu-smob mi)
			)))))
      (set! item (assoc-set! item "on-minus" 
		  (lambda (mi)		      
		    (let* (
			   (menu-smob (assoc-ref (lw6-current-menu) "smob"))
			   (value (assoc-ref mi "value"))
			   )
		      (begin
			(set! value (- value 1))
			(if (< value 0)
			    (set! value (- (length values-list) 1)))
			(assoc-set! mi "value" value)
			(update-func mi)
			(assoc-set! mi "label" (label-func mi))
			(c-lw6gui-menu-sync menu-smob mi)
			)))))
      (set! item (assoc-set! item "on-valid" (assoc-ref item "on-plus")))
      item
      )))

(define lw6-menu-item-list-template
  (lambda (label-func update-func index-func values-list)
    (lw6-menu-item-list-template-update (list) label-func update-func index-func values-list)))
    
(define lw6-menu-item-list-number-template
  (lambda (config-key labels)
    (lw6-menu-item-list-template 
     lw6-menu-item-list-label-func
     (lw6-menu-item-list-number-update-func config-key)
     (lw6-menu-item-list-number-index-func config-key)
     labels
     )))

(define lw6-menu-item-list-boolean-template
  (lambda (config-key label-true label-false)
    (lw6-menu-item-list-template 
     lw6-menu-item-list-label-func
     (lw6-menu-item-list-boolean-update-func config-key)
     (lw6-menu-item-list-boolean-index-func config-key)
     (list label-false label-true)
     )))

(define lw6-menu-item-template-switch
  (lambda (label-func on-plus on-minus)
    (let (
	  (item (lw6-menu-item-template (label-func)))
	  )
      (begin
	(assoc-set! item "on-valid" 
		    (lambda (mi)		      
		      (begin
			(on-plus mi)
			(assoc-set! mi "label" (label-func))
			(c-lw6gui-menu-sync (assoc-ref (lw6-current-menu) "smob") mi))))
	(assoc-set! item "on-plus" 
		    (lambda (mi)		      
		      (begin
			(on-plus mi)
			(assoc-set! mi "label" (label-func))
			(c-lw6gui-menu-sync (assoc-ref (lw6-current-menu) "smob") mi))))
	(assoc-set! item "on-minus" 
		    (lambda (mi)		      
		      (begin
			(on-minus mi)
			(assoc-set! mi "label" (label-func))
			(c-lw6gui-menu-sync (assoc-ref (lw6-current-menu) "smob") mi))))
	item))))

(define lw6-append-menuitem!
  (lambda (menu menuitem)
    (let* (
	   (menu-smob (assoc-ref menu "smob"))
	   (id (c-lw6gui-menu-append menu-smob menuitem))
	  )
      (set! menu (assoc-set! menu "items" (append (assoc-ref menu "items") (list (assoc-set! menuitem "id" id))))))))

(define lw6-menu-sync
  (lambda (menu)
    (c-lw6gui-menu-select (assoc-ref menu "smob") 
			  (assoc-ref menu "selected-item")
			  (assoc-ref menu "allow-scroll"))
    ))

(define lw6-menu-pump-buttons
  (lambda ()
    (let
	(
	 (dsp (lw6-get-game-global "dsp"))
	 (menu (lw6-current-menu))
	 (menuitem (lw6-current-menuitem))
	 )
      (if
       menu
       (begin
       (if (lw6-menu-pump-all?)
	   (cond 
	    ((or
	      (c-lw6gui-keyboard-pop-key-up dsp)
	      (c-lw6gui-joystick1-pop-button-up dsp)
	      (c-lw6gui-joystick2-pop-button-up dsp))
	     (if
	      (lw6-prev-menuitem #f)
	      (lw6-play-sound-beep-select)
	      )
	     )
	    ((c-lw6gui-mouse-pop-wheel-up dsp)
	     (if
	      (lw6-prev-menuitem #t)
	      (lw6-play-sound-beep-select)
	      )
	     )
	    ((or
	      (c-lw6gui-keyboard-pop-key-down dsp)
	      (c-lw6gui-joystick1-pop-button-down dsp)
	      (c-lw6gui-joystick2-pop-button-down dsp))
	     (if
	      (lw6-next-menuitem #f)
	      (lw6-play-sound-beep-select)
	      )
	     )
	    ((c-lw6gui-mouse-pop-wheel-down dsp)
	     (if
	      (lw6-next-menuitem #t)
	      (lw6-play-sound-beep-select)
	      )
	     )
	    ((or
	      (c-lw6gui-keyboard-pop-key-left dsp)
	      (c-lw6gui-joystick1-pop-button-left dsp)
	      (c-lw6gui-joystick2-pop-button-left dsp))
	     (if
	      (lw6-menuitem-action menuitem "on-minus")
	      (lw6-play-sound-beep-valid)
	      ) 
	     )
	    ((or
	      (c-lw6gui-keyboard-pop-key-right dsp)
	      (c-lw6gui-joystick1-pop-button-right dsp)
	      (c-lw6gui-joystick2-pop-button-right dsp))
	     (if
	      (lw6-menuitem-action menuitem "on-plus")
	      (lw6-play-sound-beep-valid)
	      ) 
	     )
	    ((or
	      (c-lw6gui-keyboard-pop-key-enter dsp)
	      (c-lw6gui-joystick1-pop-button-a dsp)
	      (c-lw6gui-joystick2-pop-button-a dsp))
	     (if
	      (lw6-menuitem-action menuitem "on-valid")
	      (lw6-play-sound-beep-valid)
	      )
	     )
	    ))
       (cond
	((or
	  (c-lw6gui-keyboard-pop-key-esc dsp)
	  (c-lw6gui-mouse-pop-button-right dsp)
	  (c-lw6gui-joystick1-pop-button-b dsp)
	  (c-lw6gui-joystick2-pop-button-b dsp))
	 (if
	  (lw6-menu-action menu "on-cancel")
	  (lw6-play-sound-beep-valid)
	  )
	 )
	((c-lw6gui-mouse-pop-button-left dsp)
	 (let*
	     (
	      (mouse-state (c-lw6gui-mouse-get-state dsp))
	      (menu-position (assoc-ref mouse-state "menu-position"))
	      (menu-scroll (assoc-ref mouse-state "menu-scroll"))
	      (menu-esc (assoc-ref mouse-state "menu-esc"))
	      )
	   (cond (
		  (>= menu-position 0)
		  (begin
		    (lw6-set-menuitem! menu-position #f)
		    (set! menuitem (lw6-current-menuitem))
		    (if
		     (lw6-menuitem-action menuitem "on-valid")
		     (lw6-play-sound-beep-valid)
		     )
		    )
		  )
		 (
		  (> menu-scroll 0)
		  (lw6-next-menuitem #f)
		  )
		 (
		  (< menu-scroll 0)
		  (lw6-prev-menuitem #f)
		  )
		 (
		  menu-esc
		  (if
		   (lw6-menu-action menu "on-cancel")
		   (lw6-play-sound-beep-valid)
		   )
		  )
		 )
	   )
	 )))))))

(define lw6-menu-pump-mouse
  (lambda ()
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (menu (lw6-current-menu))
	   (menu-smob (if menu (assoc-ref menu "smob")))
	   )
      (if (and (lw6-menu-pump-all?)
	       (not (lw6-config-is-true? lw6def-click-to-focus)))
	  (let*
	      (
	       (mouse-state (c-lw6gui-mouse-poll-move dsp))
	       (menu-position (assoc-ref mouse-state "menu-position"))
	       (menu-scroll (assoc-ref mouse-state "menu-scroll"))
	       (menu-esc (assoc-ref mouse-state "menu-esc"))
	       (menuitem (lw6-current-menuitem))
	       )
	    (if mouse-state
		(begin
		  (if (and menu menuitem)
		      (cond (
			     (>= menu-position 0)
			     (lw6-set-menuitem! menu-position #f)
			     )
			    (
			     (> menu-scroll 0)
			     (lw6-next-menuitem #f)
			     )
			    (
			     (< menu-scroll 0)
			     (lw6-prev-menuitem #f)
			     )
			    ))
		  (if menu-smob
		      (c-lw6gui-menu-select-esc menu-smob menu-esc))
		  ))
	    )
	  (let*
	      (
	       (mouse-state (c-lw6gui-mouse-get-state dsp))
	       (menu-esc (assoc-ref mouse-state "menu-esc"))
	       )
	    (if menu-smob
		(c-lw6gui-menu-select-esc menu-smob menu-esc)))	       
	  ))))

(define lw6-menu
  (lambda () 
    (begin 
      (if (lw6-empty-menu-stack?)
	  (c-lw6gui-input-send-quit (lw6-get-game-global "dsp"))
	  )
      (lw6-menu-pump-buttons)
      (lw6-menu-pump-mouse)
      (if (not (lw6-menu-pump-all?))
	  (lw6-zoom-pump))
      )))
