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

(define lw6-audio-options-menu-volume-item-label
  (lambda (config-key label-0 label-format)
    (lambda ()
      (let
	  (
	   (value (lw6-config-get-number config-key))
	   )
	(if (> value 0.0)
	    (format #f label-format (inexact->exact (* 100 value)))
	    label-0)))))

(define lw6-audio-options-menu-volume-item-minus
  (lambda (config-key setter)
    (lambda (menuitem)
      (let
	  (
	   (value (lw6-config-get-number config-key))
	   (snd (lw6-get-game-global "snd"))
	   )
	(begin
	  (set! value (- value 0.1))
	  (if (< value 0.0)
	      (set! value 1.0))
	  (lw6-config-set-number! config-key value)
	  (if snd (setter snd value))
	  )))))

(define lw6-audio-options-menu-volume-item-plus
  (lambda (config-key setter)
    (lambda (menuitem)
      (let
	  (
	   (value (lw6-config-get-number config-key))
	   (snd (lw6-get-game-global "snd"))
	   )
	(begin
	  (set! value (+ value 0.1))
	  (if (> value 1.0)
	      (set! value 0.0))
	  (lw6-config-set-number! config-key value)
	  (if snd (setter snd value))
	  )))))

(define lw6-audio-options-menu-fx-volume-item-label
  (lw6-audio-options-menu-volume-item-label
   lw6def-fx-volume
   (_ "No sound FX")
   (_ "Sound FX ~a%")))

(define lw6-audio-options-menu-fx-volume-item-minus
  (lw6-audio-options-menu-volume-item-minus
   lw6def-fx-volume
   c-lw6snd-set-fx-volume))

(define lw6-audio-options-menu-fx-volume-item-plus
  (lw6-audio-options-menu-volume-item-plus
   lw6def-fx-volume
   c-lw6snd-set-fx-volume))

(define lw6-audio-options-menu-fx-volume-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template-switch
		 lw6-audio-options-menu-fx-volume-item-label
		 lw6-audio-options-menu-fx-volume-item-plus
		 lw6-audio-options-menu-fx-volume-item-minus
		 (_ "Change volume for sound FX, this includes all sounds but music and background water sound")
		 ))
	  )
      (begin
  	(set! item (assoc-set! item "selected" #t))
	item))))

(define lw6-audio-options-menu-water-volume-item-label
  (lw6-audio-options-menu-volume-item-label
   lw6def-water-volume
   (_ "No water sounds")
   (_ "Water sounds ~a%")))

(define lw6-audio-options-menu-water-volume-item-minus
  (lw6-audio-options-menu-volume-item-minus
   lw6def-water-volume
   c-lw6snd-set-water-volume))

(define lw6-audio-options-menu-water-volume-item-plus
  (lw6-audio-options-menu-volume-item-plus
   lw6def-water-volume
   c-lw6snd-set-water-volume))

(define lw6-audio-options-menu-water-volume-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template-switch
		 lw6-audio-options-menu-water-volume-item-label
		 lw6-audio-options-menu-water-volume-item-plus
		 lw6-audio-options-menu-water-volume-item-minus
		 (_ "Change volume for water sound, the bubbling sound you can here in the background")

		 ))
	  )
      (begin
  	(set! item (assoc-set! item "selected" #t))
	item))))

(define lw6-audio-options-menu-music-volume-item-label
  (lw6-audio-options-menu-volume-item-label
   lw6def-music-volume
   (_ "No music")
   (_ "Music ~a%")))

(define lw6-audio-options-menu-music-volume-item-minus
  (lw6-audio-options-menu-volume-item-minus
   lw6def-music-volume
   c-lw6snd-set-music-volume))

(define lw6-audio-options-menu-music-volume-item-plus
  (lw6-audio-options-menu-volume-item-plus
   lw6def-music-volume
   c-lw6snd-set-music-volume))

(define lw6-audio-options-menu-music-volume-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-template-switch
		 lw6-audio-options-menu-music-volume-item-label
		 lw6-audio-options-menu-music-volume-item-plus
		 lw6-audio-options-menu-music-volume-item-minus
		 (_ "Change music volume")
		 ))
	  )
      (begin
	item))))

(define lw6-audio-options-menu-backend-item-list
  (lambda ()
    (c-lw6snd-get-backends)))

(define lw6-audio-options-menu-backend-item-label-func
  (lambda (menuitem)
    (let* (
	   (values-list (assoc-ref menuitem "values-list"))
	   (value (assoc-ref menuitem "value"))
	   )
      (format #f (_ "Backend ~a")
	      (cdr (list-ref values-list value)))
      )))

(define lw6-audio-options-menu-backend-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	   (pair (list-ref (lw6-audio-options-menu-backend-item-list) value))
	   (snd-backend (car pair))
	   (snd (lw6-get-game-global "snd"))
	  )
      (if snd-backend
	  (begin
	    (lw6-config-set-string! lw6def-snd-backend (car pair))
	    (if snd
	    (c-lw6snd-release snd))
	    (set! snd
		  (c-lw6snd-new snd-backend
				(lw6-config-get-number lw6def-fx-volume)
				(lw6-config-get-number lw6def-water-volume)
				(lw6-config-get-number lw6def-music-volume)))
	    (lw6-set-game-global! "snd" snd)
	)))))

(define lw6-audio-options-menu-backend-item-index-func
  (lambda (menuitem)
    (let* (
	   (snd-backend (lw6-config-get-string lw6def-snd-backend))
	   (index 0)
	   (t 0)
	   (backend-list (lw6-audio-options-menu-backend-item-list))
	  )
      (begin
	(map (lambda (v) (if (equal? snd-backend (car v))
			     (set! index t)
			     (set! t (+ 1 t))
			     ))
	     backend-list)
	(if (>= index (length backend-list))
	    (set! index (- (length backend-list) 1)))
	index
	))))

(define lw6-audio-options-menu-backend-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-audio-options-menu-backend-item-label-func
     lw6-audio-options-menu-backend-item-update-func
     lw6-audio-options-menu-backend-item-index-func
     (lw6-audio-options-menu-backend-item-list)
     (_ "The sound backend is the library used to play sounds, if in doubt, don't touch")
     )
    ))

(define lw6-audio-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Sound options")
				   (_ "Change volumes and audio settings here")
				   #f))
	  (snd (lw6-get-game-global "snd"))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-audio-options-menu-fx-volume-item))
	(lw6-append-menuitem! menu (lw6-audio-options-menu-water-volume-item))
	(lw6-append-menuitem! menu (lw6-audio-options-menu-music-volume-item))
	(if snd
	    (lw6-append-menuitem! menu (lw6-audio-options-menu-backend-item)))
	menu
	))))
