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

(define lw6-screenshot
  (lambda ()
    (let* (
	   (node (lw6-get-game-global "node"))
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (pilot (lw6-get-game-global "pilot"))
	   (network (lw6-get-game-global "network"))
	   (params (list))
	   )
      (if (and node level game-state pilot)
	  (begin
	    (if network
		(begin
		  (c-lw6pil-sync-from-draft game-state pilot (lw6-config-is-true? lw6def-dirty-read))
		  (set! params (append params (list (cons "round" (c-lw6ker-get-rounds game-state))
						    (cons "level" (c-lw6map-get-title level))
						    (cons "nb-colors" (c-lw6ker-get-nb-colors game-state))
						    (cons "max-nb-colors" (c-lw6map-get-max-nb-colors level))
						    (cons "nb-cursors" (c-lw6ker-get-nb-cursors game-state))
						    (cons "max-nb-cursors" (c-lw6map-get-max-nb-cursors level))
						    (cons "nb-nodes" (c-lw6ker-get-nb-nodes game-state))
						    (cons "max-nb-nodes" (c-lw6map-get-max-nb-nodes level))
						    (cons "screenshot" (c-lw6img-screenshot game-state 85))
						    )))
		  )
		(set! params (append params (list (cons "round" 0)
						  (cons "level" "")
						  (cons "nb-colors" 0)
						  (cons "max-nb-colors" 0)
						  (cons "nb-cursors" 0)
						  (cons "max-nb-cursors" 0)
						  (cons "nb-nodes" 0)
						  (cons "max-nb-nodes" 0)
						  (cons "screenshot" #f)
						  ))))
	    ;;(lw6-log-notice params)
	    (c-lw6p2p-node-update-info node params)
	    )))))

