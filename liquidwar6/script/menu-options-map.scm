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

(define lw6-map-options-menu-per-100-2-label
  (lambda (v)
    (inexact->exact (round (/ (* v v) 100)))))

(define lw6-map-options-menu-armies-size-item-list
  (list
   (cons 10 30)
   (cons 15 40)
   (cons 20 60)
   (cons 30 60)
   (cons 40 70)
   (cons 50 80)))

(define lw6-map-options-menu-armies-size-item-label-func
  (lambda (menuitem)
      (format #f 
	      (_ "Armies ~a% - ~a%")
	      (lw6-map-options-menu-per-100-2-label
	       (lw6-config-get-number lw6def-single-army-size))
	      (lw6-map-options-menu-per-100-2-label
	       (lw6-config-get-number lw6def-total-armies-size)))
      ))

(define lw6-map-options-menu-armies-size-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	   (pair (list-ref lw6-map-options-menu-armies-size-item-list value))
	  )
      (lw6-config-set-number! lw6def-single-army-size (car pair))
      (lw6-config-set-number! lw6def-total-armies-size (cdr pair))
      (lw6-game-param-update)
      (lw6-loader-push (lw6-config-get-string lw6def-chosen-map))
      )))

(define lw6-map-options-menu-armies-size-item-index-func
  (lambda (menuitem)
    (let* (
	   (total-armies-size (lw6-config-get-number lw6def-total-armies-size))
	   (index 0)
	  )
      (begin
	(map (lambda (v) (if (> total-armies-size (cdr v)) (set! index (+ index 1)))) lw6-map-options-menu-armies-size-item-list)
	(if (>= index (length lw6-map-options-menu-armies-size-item-list))
	    (set! index (- (length lw6-map-options-menu-armies-size-item-list) 1)))
	index
	))))

(define lw6-map-options-menu-armies-size-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-map-options-menu-armies-size-item-label-func
     lw6-map-options-menu-armies-size-item-update-func
     lw6-map-options-menu-armies-size-item-index-func
     lw6-map-options-menu-armies-size-item-list)
    ))

(define lw6-map-options-menu-keep-ratio-item
  (lambda ()
    (let (
	  (label-func (lambda ()
			(if (lw6-config-is-true? lw6def-keep-ratio)
			    (_ "Keep map ratio")
			    (_ "Fit map to screen"))))
	  (toggle-func (lambda (menuitem)
			 (begin
			   (lw6-config-change-boolean! lw6def-keep-ratio)
			   (lw6-game-param-update)
			   (lw6-loader-push (lw6-config-get-string lw6def-chosen-map)))))
	  )
      (lw6-menu-item-template-switch label-func toggle-func toggle-func))))

(define lw6-map-options-menu-use-texture-item
  (lambda ()
    (let (
	  (label-func (lambda ()
			(if (lw6-config-is-true? lw6def-use-texture)
			    (_ "Use texture")
			    (_ "Ignore texture"))))
	  (toggle-func (lambda (menuitem)
			 (begin
			   (lw6-config-change-boolean! lw6def-use-texture)
			   (lw6-game-param-update)
			   (lw6-loader-push (lw6-config-get-string lw6def-chosen-map)))))
	  )
      (lw6-menu-item-template-switch label-func toggle-func toggle-func))))

(define lw6-map-options-menu-use-xml-item
  (lambda ()
    (let (
	  (label-func (lambda ()
			(cond 
			 (
			  (and (lw6-config-is-true? lw6def-use-rules-xml)
			       (lw6-config-is-true? lw6def-use-hints-xml)
			       (lw6-config-is-true? lw6def-use-style-xml))
			  (_ "Use XML files")
			  )
			 (
			  (and (not (lw6-config-is-true? lw6def-use-rules-xml))
			       (not (lw6-config-is-true? lw6def-use-hints-xml))
			       (not (lw6-config-is-true? lw6def-use-style-xml)))
			  (_ "Ignore XML files")
			  )
			 (
			  #t				  
			  (format #f (_ "Use ~a")
				  (string-join
				   (let (
					 (used (list))
					 )
				     (begin
				       (if (lw6-config-is-true? lw6def-use-rules-xml)
					   (set! used (append used (list "rules.xml"))))
				       (if (lw6-config-is-true? lw6def-use-hints-xml)
					   (set! used (append used (list "hints.xml"))))
				       (if (lw6-config-is-true? lw6def-use-style-xml)
					   (set! used (append used (list "style.xml"))))
				       used))
				   (_ " and ")
				   ))))))
	  (toggle-func (lambda (menuitem)
			 (begin
			   (lw6-config-change-boolean! lw6def-use-rules-xml)
			   (lw6-config-set-boolean! lw6def-use-hints-xml
						    (lw6-config-is-true? lw6def-use-rules-xml))
			   (lw6-config-set-boolean! lw6def-use-style-xml
						    (lw6-config-is-true? lw6def-use-rules-xml))
			   (lw6-game-param-update)
			   (lw6-loader-push (lw6-config-get-string lw6def-chosen-map)))))
	  )
      (lw6-menu-item-template-switch label-func toggle-func toggle-func))))

(define lw6-map-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Map options")))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-map-options-menu-armies-size-item))
	(lw6-append-menuitem! menu (lw6-map-options-menu-keep-ratio-item))
	(lw6-append-menuitem! menu (lw6-map-options-menu-use-texture-item))
	(lw6-append-menuitem! menu (lw6-map-options-menu-use-xml-item))
	(set! menu (assoc-set! menu "on-pop" (lambda (m) (lw6-game-idle))))
	(set! menu (assoc-set! menu "on-push" 
			       (lambda (m) 
				 (begin 
				   (lw6-loader-push-if-needed
				    (lw6-config-get-string lw6def-chosen-map))
				   (lw6-game-preview)))))
	menu
	))))
