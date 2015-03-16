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

(define lw6-score-get-winner
  (lambda ()
    (let* (
	   (pilot (lw6-get-game-global "pilot"))
	   (winner-color-index (if pilot (c-lw6pil-get-winner pilot) #f))
	   (winner-color-label (if winner-color-index (c-lw6map-team-color-index-to-label winner-color-index) ""))
	  )
      winner-color-label
      )))

(define lw6-score-get-looser
  (lambda ()
    (let* (
	   (pilot (lw6-get-game-global "pilot"))
	   (looser-color-index (if pilot (c-lw6pil-get-looser pilot) #f))
	   (looser-color-label (if looser-color-index (c-lw6map-team-color-index-to-label looser-color-index) ""))
	  )
      looser-color-label
      )))

(define lw6-score
  (lambda ()
    (let* (
	   (pilot (lw6-get-game-global "pilot"))
	   (cursor (lw6-get-cursor "1"))
	   (cursor-id (hash-ref cursor "id"))
	   (level (lw6-get-game-global "level"))
	   (dsp (lw6-get-game-global "dsp"))
	   (won #f)
	   )
      (if (and pilot
	       (c-lw6pil-is-over pilot)
	       (not (lw6-display-param-get lw6def-display-score)))
	  (begin
	    (c-lw6gui-input-reset dsp)
	    (lw6-display-score-setup)
	    (lw6-pop-menu (lw6-current-menu))
	    ;;(lw6-pop-menu-raw)
	    (if (lw6-get-game-global "solo")
		(begin
		  (set! won (and cursor-id
				 (c-lw6pil-did-cursor-win pilot cursor-id)
				 level
				 (c-lw6ldr-exp-validate level)))
		  (if won
		      (let* (
			     (map-path (lw6-get-game-global lw6def-map-path))
			     (relative-path (lw6-config-get-string lw6def-chosen-map))
			     (entry (c-lw6ldr-chain-entry map-path relative-path))
			     )
		    (if entry
			(lw6-config-set-string! lw6def-chosen-map (assoc-ref entry "relative-path")))))
		  (lw6-push-menu (lw6-score-solo-menu won)))
		(begin
		  (lw6-push-menu (lw6-score-menu))
		  )))))))

