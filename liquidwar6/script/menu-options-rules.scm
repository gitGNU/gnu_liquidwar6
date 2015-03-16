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

(define lw6-rules-options-menu-respawn-team-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-list-number-template
				    lw6def-respawn-team
				    (list
				     (_ "One life (last wins)")
				     (_ "Respawn (deatchmatch)"))
				    (_ "Choose wether you want the game to end when everyone is dead but the winner or if you prefer to keep playing until time limit, with teams respawning")))
	  )
      (begin
	item
	))))

(define lw6-rules-options-menu-use-team-profiles-item
  (lambda ()
    (let (
	  (item (lw6-menu-item-list-number-template
				    lw6def-use-team-profiles
				    (list
				     (_ "All teams are equal")
				     (_ "Use team profiles"))
				    (_ "Team profiles can really change the game, depending on your color you could move faster or slower, have a stronger or weaker attack, and so on")))
	  )
      (begin
	item
	))))

(define lw6-rules-options-menu-per-100-2-label
  (lambda (v)
    (inexact->exact (round (/ (* v v) 100)))))

(define lw6-rules-options-menu-armies-size-item-list
  (list
   (cons 10 30)
   (cons 15 40)
   (cons 20 60)
   (cons 30 60)
   (cons 40 70)
   (cons 50 80)))

(define lw6-rules-options-menu-armies-size-item-label-func
  (lambda (menuitem)
      (format #f
	      (_ "Armies ~a% - ~a%")
	      (lw6-rules-options-menu-per-100-2-label
	       (lw6-config-get-number lw6def-single-army-size))
	      (lw6-rules-options-menu-per-100-2-label
	       (lw6-config-get-number lw6def-total-armies-size)))
      ))

(define lw6-rules-options-menu-armies-size-item-update-func
  (lambda (menuitem)
    (let* (
	   (value (assoc-ref menuitem "value"))
	   (pair (list-ref lw6-rules-options-menu-armies-size-item-list value))
	  )
      (lw6-config-set-number! lw6def-single-army-size (car pair))
      (lw6-config-set-number! lw6def-total-armies-size (cdr pair))
      )))

(define lw6-rules-options-menu-armies-size-item-index-func
  (lambda (menuitem)
    (let* (
	   (total-armies-size (lw6-config-get-number lw6def-total-armies-size))
	   (index 0)
	  )
      (begin
	(map (lambda (v) (if (> total-armies-size (cdr v)) (set! index (+ index 1)))) lw6-rules-options-menu-armies-size-item-list)
	(if (>= index (length lw6-rules-options-menu-armies-size-item-list))
	    (set! index (- (length lw6-rules-options-menu-armies-size-item-list) 1)))
	index
	))))

(define lw6-rules-options-menu-armies-size-item
  (lambda ()
    (lw6-menu-item-list-template
     lw6-rules-options-menu-armies-size-item-label-func
     lw6-rules-options-menu-armies-size-item-update-func
     lw6-rules-options-menu-armies-size-item-index-func
     lw6-rules-options-menu-armies-size-item-list
     (_ "Defines how crowded the battlefield is, if you raise this, there will be a lot of fighters in a limited space"))
    ))

(define lw6-rules-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Rules")
				   (_ "Changing rules will modify the gameplay, you can also be curious, read the documentation and try config file and command line options")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-rules-options-menu-respawn-team-item))
	(lw6-append-menuitem! menu (lw6-rules-options-menu-use-team-profiles-item))
	(lw6-append-menuitem! menu (lw6-rules-options-menu-armies-size-item))
	(set! menu(assoc-set! menu "on-pop" (lambda (m) (lw6-loader-purge))))
	menu
	))))
