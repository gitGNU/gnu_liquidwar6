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

(define lw6-video-options-menu-fullscreen-item-label
  (lambda ()
    (let*
	(
	 (dsp (lw6-get-game-global "dsp"))
	 (res (c-lw6dsp-get-video-mode dsp))
	 )
      (if (assoc-ref res lw6def-fullscreen)
	  (_ "Fullscreen")
	  (_ "Windowed")))))

(define lw6-video-options-menu-fullscreen-item-toggle
  (lambda (menuitem)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (current-res (c-lw6dsp-get-video-mode dsp))
	   (width (assoc-ref current-res lw6def-width))
	   (height (assoc-ref current-res lw6def-height))
	   (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	   (high-res (assoc-ref fullscreen-modes "high"))
	   (max-width (assoc-ref high-res lw6def-width))
	   (max-height (assoc-ref high-res lw6def-height))
	   (windowed-mode-limit (lw6-config-get-number lw6def-windowed-mode-limit))
	   (limited-width (inexact->exact (round (* max-width windowed-mode-limit))))
	   (limited-height (inexact->exact (round (* max-height windowed-mode-limit))))
	   (limited-res (list (cons lw6def-width limited-width)
			      (cons lw6def-height limited-height)))
	   )
      (if (assoc-ref current-res lw6def-fullscreen)
	  (if (lw6-res-equal? current-res high-res)
	      (lw6-config-set-video! limited-width limited-height #f)
	      (lw6-config-set-video! width height #f))
	  (if (lw6-res-equal? current-res limited-res)
	      (lw6-config-set-video! max-width max-height #t)
	      (lw6-config-set-video! width height #t))))))

(define lw6-video-options-menu-fullscreen-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template-switch
		 lw6-video-options-menu-fullscreen-item-label
		 lw6-video-options-menu-fullscreen-item-toggle
		 lw6-video-options-menu-fullscreen-item-toggle
		 (_ "Use this to toggle between windowed and fullscreen mode")))
	  )
      (begin
  	(assoc-set! item "selected" #t)
	item))))

(define lw6-res-equal?
  (lambda (res1 res2)
    (and (= (assoc-ref res1 lw6def-width) (assoc-ref res2 lw6def-width))
	 (= (assoc-ref res1 lw6def-height) (assoc-ref res2 lw6def-height)))))

(define lw6-res-surface
  (lambda (res)
    (* (assoc-ref res lw6def-width) (assoc-ref res lw6def-height))))

(define lw6-video-set
  (lambda (name)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	   (preset-res (assoc-ref fullscreen-modes name))
	   (current-res (c-lw6dsp-get-video-mode dsp))
	   (width (assoc-ref preset-res lw6def-width))
	   (height (assoc-ref preset-res lw6def-height))
	   (fullscreen (assoc-ref current-res lw6def-fullscreen))
	   (windowed-mode-limit (lw6-config-get-number lw6def-windowed-mode-limit))
	   (high-res (assoc-ref fullscreen-modes "high"))
	   (max-width (assoc-ref high-res lw6def-width))
	   (max-height (assoc-ref high-res lw6def-height))
	   (limited-width (inexact->exact (round (* max-width windowed-mode-limit))))
	   (limited-height (inexact->exact (round (* max-height windowed-mode-limit))))
	   )
      (begin
	(if (not fullscreen)
	    (begin
	      (set! width (min limited-width width))
	      (set! height (min limited-height height))
	      ))
	(lw6-config-set-video! width height fullscreen)))))

(define lw6-video-set-low
  (lambda ()
    (lw6-video-set "low")))

(define lw6-video-set-standard
  (lambda ()
    (lw6-video-set "standard")))

(define lw6-video-set-high
  (lambda ()
    (lw6-video-set "high")))

(define lw6-video-options-menu-resolution-item-label
  (lambda ()
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (current-res (c-lw6dsp-get-video-mode dsp))
	   (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	   (low-res (assoc-ref fullscreen-modes "low"))
	   (standard-res (assoc-ref fullscreen-modes "standard"))
	   (high-res (assoc-ref fullscreen-modes "high"))
	   (label-prefix (cond
			  (
			   (lw6-res-equal? current-res low-res)
			   (_ "Low res")
			   )
			  (
			   (lw6-res-equal? current-res standard-res)
			   (_ "Standard res")
			  )
			  (
			   (lw6-res-equal? current-res high-res)
			   (_ "High res")
			   )
			  (
			   #t
			   (_ "User res")
			   )
			  )
			 ))
      (format #f "~a ~ax~a"
	      label-prefix
	      (assoc-ref current-res lw6def-width)
	      (assoc-ref current-res lw6def-height))
      )))

(define lw6-video-options-menu-resolution-item-plus
  (lambda (menuitem)
    (begin
      (let* (
	     (dsp (lw6-get-game-global "dsp"))
	     (current-res (c-lw6dsp-get-video-mode dsp))
	     (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	     (low-res (assoc-ref fullscreen-modes "low"))
	     (standard-res (assoc-ref fullscreen-modes "standard"))
	     (high-res (assoc-ref fullscreen-modes "high"))
	    )
	(cond
	 (
	  (< (lw6-res-surface current-res) (lw6-res-surface standard-res))
	  (lw6-video-set-standard)
	  )
	 (
	  (> (lw6-res-surface current-res) (lw6-res-surface standard-res))
	  (lw6-video-set-low)
	  )
	 (
	  #t
	  (lw6-video-set-high)
	  )
	 )))))

(define lw6-video-options-menu-resolution-item-minus
  (lambda (menuitem)
    (begin
      (let* (
	     (dsp (lw6-get-game-global "dsp"))
	     (current-res (c-lw6dsp-get-video-mode dsp))
	     (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	     (low-res (assoc-ref fullscreen-modes "low"))
	     (standard-res (assoc-ref fullscreen-modes "standard"))
	     (high-res (assoc-ref fullscreen-modes "high"))
	    )
	(cond
	 (
	  (< (lw6-res-surface current-res) (lw6-res-surface standard-res))
	  (lw6-video-set-high)
	  )
	 (
	  (> (lw6-res-surface current-res) (lw6-res-surface standard-res))
	  (lw6-video-set-standard)
	  )
	 (
	  #t
	  (lw6-video-set-low)
	  )
	 )))))

(define lw6-video-options-menu-resolution-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template-switch
		 lw6-video-options-menu-resolution-item-label
		 lw6-video-options-menu-resolution-item-plus
		 lw6-video-options-menu-resolution-item-minus
		 (_ "Change the window size, in windowed mode you could just resize it with the mouse")
		 ))
	  )
      item)))

(define lw6-video-options-menu-backend-item-list
  (lambda ()
    (c-lw6gfx-get-backends)))

(define lw6-video-options-menu-backend-item-label-func
  (lambda (menuitem)
    (let* (
	   (values-list (assoc-ref menuitem "values-list"))
	   (value (assoc-ref menuitem "value"))
	   )
      (format #f (_ "Backend ~a")
	      (cdr (list-ref values-list value)))
      )))

(define lw6-video-options-menu-backend-item-update-func
  (lambda (menuitem)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (value (assoc-ref menuitem "value"))
	   (pair (list-ref (lw6-video-options-menu-backend-item-list) value))
	   (gfx-backend (car pair))
	  )
      (if gfx-backend
	  (begin
	    (lw6-config-set-string! lw6def-gfx-backend gfx-backend)
	    (c-lw6dsp-release dsp)
	    (set! dsp
		  (c-lw6dsp-new gfx-backend
			    (lw6-get-game-global "display-param")))
	    (lw6-set-game-global! "dsp" dsp)
	)))))

(define lw6-video-options-menu-backend-item-index-func
  (lambda (menuitem)
    (let* (
	   (gfx-backend (lw6-config-get-string lw6def-gfx-backend))
	   (index 0)
	   (temp 0)
	   (backend-list (lw6-video-options-menu-backend-item-list))
	  )
      (begin
	(map (lambda (v) (if (equal? gfx-backend (car v))
			     (set! index temp)
			     (set! temp (+ 1 temp))
			     ))
	     backend-list)
	(if (>= index (length backend-list))
	    (set! index (- (length backend-list) 1)))
	index
	))))

(define lw6-video-options-menu-backend-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-video-options-menu-backend-item-label-func
     lw6-video-options-menu-backend-item-update-func
     lw6-video-options-menu-backend-item-index-func
     (lw6-video-options-menu-backend-item-list)
     (_ "The graphics backend is the library used for rendering images, if in doubt, don't touch")
     )
    ))

(define lw6-video-options-menu-on-display
  (lambda (menu fullscreen-item resolution-item)
    (lambda (menu)
      (let (
	    (menu-smob (assoc-ref menu "smob"))
	    )
      (begin
	(lw6-config-update-video)
	(assoc-set! fullscreen-item "label" (lw6-video-options-menu-fullscreen-item-label))
	(c-lw6gui-menu-sync menu-smob fullscreen-item)
	(assoc-set! resolution-item "label" (lw6-video-options-menu-resolution-item-label))
	(c-lw6gui-menu-sync menu-smob resolution-item)
	(lw6-menu-sync menu)
	)))))

(define lw6-video-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Video options")
				   (_ "Change screen resolution, video mode, ...")
				   #f))
	  )
      (begin
	(lw6-config-update-video)
	(lw6-append-menuitem! menu (lw6-video-options-menu-fullscreen-item))
	(lw6-append-menuitem! menu (lw6-video-options-menu-resolution-item))
	(assoc-set! menu "on-display" (lw6-video-options-menu-on-display menu (list-ref (assoc-ref menu "items") 0) (list-ref (assoc-ref menu "items") 1)))
	(lw6-append-menuitem! menu (lw6-video-options-menu-backend-item))
	menu
	))))

