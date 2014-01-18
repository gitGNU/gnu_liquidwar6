;; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>
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

(define lw6-net
  (lambda ()
    (let (
	  (node (lw6-get-game-global "node"))
	  (node-id (lw6-get-game-global "node-id"))
	  (pilot (lw6-get-game-global "pilot"))
	  )
      (if node
	  (begin
	    (lw6-node-poll)
	    (if pilot
		(let* (
		       (timestamp (c-lw6sys-get-timestamp))
		       (next-seq (c-lw6pil-get-next-seq pilot timestamp))
		       )
		  (cond
		   (
		    (c-lw6p2p-node-is-seed-needed node)
		    (let (
			  (seed-command (c-lw6pil-seed-command-generate pilot node-id next-seq))
			  )
		      (begin
			(lw6-log-notice (format #f "seed-command -> ~a" seed-command))
			(c-lw6p2p-node-put-local-msg node seed-command)
			(c-lw6sys-idle)
			))
		    (
		     (c-lw6p2p-node-is-dump-needed node)
		     (let (
			   (dump-command (c-lw6pil-dump-command-generate pilot node-id next-seq))
			   )
		       (begin
			 (lw6-log-notice (format #f "(string-length dump-command) -> ~a" (string-length dump-command)))
			 (c-lw6p2p-node-put-local-msg node dump-command)
			 (c-lw6sys-idle)
			 ))
		     )
		    )
		   )
		  ))))
      )))
