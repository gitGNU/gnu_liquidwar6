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

; returns game-instance
(define (lw6-prepare-map map-source)
  (let* (
	 (game-struct (c-lw6ker-build-game-struct (assoc-ref map-source "level")))
	 (game-state (c-lw6ker-build-game-state game-struct))
	 )
    (append map-source (list
			(cons "game-struct" game-struct)
			(cons "game-state" game-state)
			(cons "moves-per-tick"
			      (/ (* (c-lw6map-options-get-int
				     game-struct
				     lw6def-moves-per-round)
				    (c-lw6map-options-get-int
				     game-struct
				     lw6def-rounds-per-sec)
				    ) 1000))
			)
	    )
    )
  )

