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

(define lw6-time-options-menu-time-item-list
  (list
   30
   60
   120
   180
   300
   600
   900
   1200
   1800
   2700
   3600
   5400
   7200
   10800
   14400
   21600
   43200
   86400))

(define lw6-time-options-menu-time-item-label-func
  (lambda (menuitem)
    (let* (
	   (total-time (lw6-config-get-number lw6def-total-time))
	   (hours (quotient total-time 3600))
	   (minutes1 (modulo (quotient total-time 600) 6))
	   (minutes2 (modulo (quotient total-time 60) 10))
	   (seconds1 (modulo (quotient total-time 10) 6))
	   (seconds2 (modulo total-time 10))
	  )
      (format #f
	      (_ "Time ~a:~a~a:~a~a")
	      hours minutes1 minutes2 seconds1 seconds2)
      )))

(define lw6-time-options-menu-time-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	  )
      (begin
	(lw6-config-set-number! lw6def-total-time (list-ref lw6-time-options-menu-time-item-list value))
	))))

(define lw6-time-options-menu-time-item-index-func
  (lambda (menuitem)
    (let* (
	   (total-time (lw6-config-get-number lw6def-total-time))
	   (index 0)
	  )
      (begin
	(map (lambda (v) (if (> total-time v) (set! index (+ index 1)))) lw6-time-options-menu-time-item-list)
	(if (>= index (length lw6-time-options-menu-time-item-list))
	    (set! index (- (length lw6-time-options-menu-time-item-list) 1)))
	index
	))))

(define lw6-time-options-menu-time-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-time-options-menu-time-item-label-func
     lw6-time-options-menu-time-item-update-func
     lw6-time-options-menu-time-item-index-func
     lw6-time-options-menu-time-item-list
     (_ "Defines how long the game will last, this is a maximum limit"))
    ))

(define lw6-time-options-menu-speed-item-list
  (list
   0.1
   0.2
   0.33
   0.5
   0.66
   1.0
   1.5
   2.0
   3.0
   5.0
   10.0))

(define lw6-time-options-menu-speed-item-label-func
  (lambda (menuitem)
    (let* (
	   (speed (lw6-config-get-number lw6def-speed))
	   )
      (cond (
	     (= speed 1.0)
	     (format #f (_ "Default speed"))
	     )
	    (
	     (> speed 1.0)
	     (format #f (_ "Speed x~a") (inexact->exact (round speed)))
	     )
	    (
	     (< speed 1.0)
	     (format #f (_ "Speed ~a%") (inexact->exact (round (* 100 speed))))
	     )
	    (
	     #t
	     (_ "Unknown speed")
	     )))))

(define lw6-time-options-menu-speed-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	   (speed (list-ref lw6-time-options-menu-speed-item-list value))
	  )
      (begin
	(lw6-config-set-number! lw6def-speed speed)
	))))

(define lw6-time-options-menu-speed-item-index-func
  (lambda (menuitem)
    (let* (
	   (speed (lw6-config-get-number lw6def-speed))
	   (index 0)
	  )
      (begin
	(map (lambda (v) (if (> speed v) (set! index (+ index 1)))) lw6-time-options-menu-speed-item-list)
	(if (>= index (length lw6-time-options-menu-speed-item-list))
	    (set! index (- (length lw6-time-options-menu-speed-item-list) 1)))
	index
	))))

(define lw6-time-options-menu-speed-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-time-options-menu-speed-item-label-func
     lw6-time-options-menu-speed-item-update-func
     lw6-time-options-menu-speed-item-index-func
     lw6-time-options-menu-speed-item-list
     (_ "Defines the global speed of the game, if set to a high value, there will be more moves per second"))
    ))

(define lw6-time-options-menu-bench-item-label-func
  (lambda (menuitem)
    (let* (
	   (bench-value (lw6-config-get-number lw6def-bench-value))
	   )
    (format #f (_ "Bench ~a") bench-value))))

(define lw6-time-options-menu-bench-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template "" (_ "Bench is counted in an arbitrary unit, it gives a global idea of how fast your computer is, as far as this game is concerned")))
	  )
      (begin
	(assoc-set! menuitem "label" (lw6-time-options-menu-bench-item-label-func menuitem))
	(assoc-set! menuitem "on-valid" (lambda (mi) (begin
						       (lw6-bench #t)
						       (assoc-set! mi "label" (lw6-time-options-menu-bench-item-label-func menuitem))
						       (let (
							     (menu-smob (assoc-ref (lw6-current-menu) "smob"))
							     )
							 (c-lw6gui-menu-sync menu-smob mi))
						       )))
	menuitem
	))))

(define lw6-time-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Time & speed")
				   (_ "Shorten and/or accelerate your games")
				   #f))
	  (old-total-time (lw6-config-get-number lw6def-total-time))
	  (old-speed (lw6-config-get-number lw6def-speed))
	  (old-bench-value (lw6-config-get-number lw6def-bench-value))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-time-options-menu-time-item))
	(lw6-append-menuitem! menu (lw6-time-options-menu-speed-item))
	(lw6-append-menuitem! menu (lw6-time-options-menu-bench-item))
	(set! menu(assoc-set! menu "on-pop" (lambda (m) (lw6-loader-purge))))
	menu
	))))
