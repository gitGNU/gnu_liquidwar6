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

(define lw6-bench-need-update
  (lambda (force-bench)
    (or force-bench
	    (not (equal?
		  (lw6-config-get-number lw6def-bin-id)
		  (c-lw6sys-build-get-bin-id))))
    ))

(define lw6-bench
  (lambda (force-bench)
    (if (not (c-lw6sys-signal-poll-quit)) ;; check before, since this takes time
	(if (lw6-bench-need-update force-bench)
	    (let (
		  (dsp (lw6-get-game-global "dsp"))
		  )
	      (if dsp
		  (begin
		    (lw6-push-menu (lw6-wait-menu
				    (_ "Benchmarking your computer, thanks for your patience")))
		    (lw6-display-update)
		    (lw6-config-set-number! lw6def-bench-value (c-lw6pil-bench))
		    (lw6-pop-menu (lw6-current-menu))
		    (c-lw6gui-input-reset dsp))
		  (begin
		    (lw6-config-set-number! lw6def-bench-value (c-lw6pil-bench)))
		  ))))))
