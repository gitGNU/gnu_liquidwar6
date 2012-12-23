;; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author	 : ufoot@ufoot.org

(define lw6-test-network-global-delay 180000)
(define lw6-test-network-connect-delay 60000)
(define lw6-test-network-shift-delay 7000)
(define lw6-test-network-update-delay 3000)

(define lw6-test-run
  (lambda (proc)
    (let (
	  (ret #f)
	  (proc-repr  (format #f "~a" proc))
	  )
      (begin
	(lw6-log-notice (format #f "test ~a BEGIN" proc-repr))
	(set! ret (proc))
	;; Note, for some reason Guile seems to collapse if ret
	;; is undefined, so be carefull, really set ret
	;; to something in test functions
	(if ret
	    (lw6-log-notice (format #f "test ~a END returned ~a" proc-repr ret))
	    (lw6-log-warning (format #f "test ~a END failed" proc-repr)))
	ret))))

(define lw6-test-log-message
  (lambda (type msg)
    (let (
	  (len (string-length msg))
	  )
      (if (>  len 100) 
	  (lw6-log-notice (format #f "received ~a bytes ~a message" len type))
	  (lw6-log-notice (format #f "received ~a message \"~a\"" type msg))
	  )
      )))

(define lw6-test-update-info
  (lambda (node level game-state)
    (let (
	  (param (list (cons "round" (c-lw6ker-get-rounds game-state))
		       (cons "level" (c-lw6map-get-title level))
		       (cons "nb-colors" (c-lw6ker-get-nb-colors game-state))
		       (cons "max-nb-colors" (c-lw6map-get-max-nb-colors level))
		       (cons "nb-cursors" (c-lw6ker-get-nb-cursors game-state))
		       (cons "max-nb-cursors" (c-lw6map-get-max-nb-cursors level))
		       (cons "nb-nodes" (c-lw6ker-get-nb-nodes game-state))
		       (cons "max-nb-nodes" (c-lw6map-get-max-nb-nodes level))
		       (cons "screenshot" (c-lw6img-screenshot game-state 85))))
	  )
      (begin
	(lw6-log-notice (format #f "updating info of node \"~a\" with param \"~a\"" node param))
	(c-lw6p2p-node-update-info node param)
	))))

;; Utility function that builds an assoc similar to the
;; one returned by c-lw6pil-suite-get-checkpoint
(define lw6-test-checkpoint
  (lambda (game-state pilot)
    (list (cons "game-state-checksum" (c-lw6ker-game-state-checksum game-state))
	  (cons "seq" (c-lw6pil-get-reference-current-seq pilot))
	  (cons "round" (c-lw6ker-get-rounds game-state)))
    ))

	   