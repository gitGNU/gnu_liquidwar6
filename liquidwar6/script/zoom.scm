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

(define lw6-zoom-pump
  (lambda ()
    (let* (
	  (dsp (lw6-get-game-global "dsp"))
	  (look (lw6-get-game-global "look"))
	  (old-zoom (string->number (c-lw6gui-look-get look "dynamic-zoom")))
	  (zoom-next-in (lw6-get-game-global "zoom-next-in"))
	  (zoom-next-out (lw6-get-game-global "zoom-next-out"))
	  (timestamp (c-lw6sys-get-timestamp))
	  )
      (begin
	(cond
	 (
	  (and (or
		(c-lw6gui-keyboard-pop-key-pgup dsp)
		(c-lw6gui-mouse-pop-wheel-up dsp)
		(c-lw6gui-joystick1-pop-button-e dsp)
		(c-lw6gui-joystick2-pop-button-e dsp))
	       (> timestamp zoom-next-in)) ; must be tested *after* input
	  (c-lw6gui-look-zoom-in look (lw6-config-get-number lw6def-zoom-step))
	  )
	 (
	  (and (or
		(c-lw6gui-keyboard-pop-key-pgdown dsp)
		(c-lw6gui-mouse-pop-wheel-down dsp)
		(c-lw6gui-joystick1-pop-button-f dsp)
		(c-lw6gui-joystick2-pop-button-f dsp))
	       (> timestamp zoom-next-out)) ; must be teste *after* input
	  (c-lw6gui-look-zoom-out look (lw6-config-get-number lw6def-zoom-step))
	  )
	 )
	(let (
	      (new-zoom (string->number (c-lw6gui-look-get look "dynamic-zoom")))
	      )
	  ;; weird, dunno why, but sometimes old-zoom (or nez?) isn't defined
	  (if (and new-zoom old-zoom)
	      (if (or (= old-zoom 1.0)
		      (> (* (- old-zoom 1.0) (- new-zoom 1.0)) 0.0))
		  #t ; both greater than 1 or lower than 1
		  (begin
		    (c-lw6gui-look-set look "dynamic-zoom" "1.0")
		    (if (> old-zoom 1.0)
			(lw6-set-game-global! "zoom-next-out"
					      (+ timestamp
						 (lw6-config-get-number lw6def-zoom-stick-delay)))
			(lw6-set-game-global! "zoom-next-in"
					      (+ timestamp
						 (lw6-config-get-number lw6def-zoom-stick-delay)))
			))
		  )))))))
