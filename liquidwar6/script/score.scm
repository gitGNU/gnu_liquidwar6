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

(define lw6-score
  (lambda ()
    (let* (
	   (pilot (lw6-get-game-global "pilot"))
	   (cursor (lw6-get-cursor "1"))
	   (cursor-id (hash-ref cursor "id"))
	   (level (lw6-get-game-global "level"))
	   )
      (if (and pilot 
	       (c-lw6pil-is-over pilot))	    
	  (begin
	    (lw6-display-score-setup)
	    (if (and cursor-id (c-lw6pil-did-cursor-win pilot cursor-id))
		(if level ;; should also check we're local...
		    (begin
		      ;;(lw6-log-notice "X")
		      (c-lw6ldr-exp-validate level)
		      ))
		)
	    )
	  )
      )
    )
  )
