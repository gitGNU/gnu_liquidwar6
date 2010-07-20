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

(define lw6-server
  (lambda ()
    (begin
      (lw6-config-set-number! lw6def-bin-id (c-lw6sys-build-get-bin-id))
      (lw6-save-config)
      (lw6-node-start)
      (if (lw6-config-is-true? lw6def-display-console)
	  (c-lw6cns-init))
      ;; we can't use the dsp object to track quit message
      (while (not  (lw6-get-game-global "quit"))
	     (begin
	       (c-lw6sys-idle)
	       (lw6-node-poll)
	       (lw6-console)
		 ))
      (lw6-node-stop))
    ))
