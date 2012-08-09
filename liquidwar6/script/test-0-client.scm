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

(load "load.scm") ; load this to catch error in make dist
(lw6-load) ; will load all includes but main, trap syntax errors at least

(define lw6-test-client-join
  (lambda ()
    (let (
	  (ret #f)
	  )
      (begin
      (c-lw6net-init #t)
      (let* (
	     (db (c-lw6p2p-db-new (format #f "~a.test.client" (c-lw6p2p-db-default-name))))
	     (node-2 (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
						 (cons "server-backends" "tcpd,udpd,httpd")
						 (cons "bind-ip" "0.0.0.0")
						 (cons "bind-port" 8058)
						 (cons "public-url" "http://localhost:8058/")
						 (cons "password" "")
						 (cons "title" "")
						 (cons "description" (_ "Dummy test node"))
						 (cons "bench" 10)
						 (cons "open-relay" #f)
						 (cons "known-nodes" "http://localhost:8057/")
						 (cons "network-reliability" 100)
						 (cons "trojan" #f)
						 )))
	     (id-2 (c-lw6p2p-node-get-id node-2))
	     (time-limit (+ 60000 (c-lw6sys-get-timestamp)))
	     (connect-time (- time-limit 15000))
	     (connect-ret #f)
	     (server-entry #f)
	     )
	(begin
	  (lw6-log-notice node-2)
	  (while (and (< (c-lw6sys-get-timestamp) connect-time) 
		      (not connect-ret))
		 (let (
		       (entries (c-lw6p2p-node-get-entries node-2))
		       )
		   (begin		     
		     (map (lambda(x) (if (and (equal? (assoc-ref x "url") "http://localhost:8057/")
					      (assoc-ref x "id"))
					 (begin
					   (set! server-entry x)
					   (set! connect-ret (c-lw6p2p-node-client-join 
							      node-2
							      (assoc-ref x "id")
							      (assoc-ref x "url")))
					   )))
			  entries)	 
		     (c-lw6sys-snooze)
		     (c-lw6p2p-node-poll node-2)
		     )))
	  (if server-entry
	      (lw6-log-notice (format #f "OK, joining ~a" server-entry))
	      (lw6-log-warning "unable to find server")
	      )
	  (while (and (< (c-lw6sys-get-timestamp) time-limit) server-entry)
		 (begin
		   (c-lw6sys-snooze)
		   (c-lw6p2p-node-poll node-2)
		   (set! ret #t) ;; todo, fix this and set it to true on real success
		   ))
	  (c-lw6p2p-node-close node-2)
	  ))
      (c-lw6net-quit)
      (gc)     
      ;; if we get here, we have succeeded else node creation would raise an error
      ret))))

(c-lw6-set-ret #f) ;; reset this to "failed" so that it has the right value is script is interrupted
(c-lw6-set-ret (and
		(lw6-test-client-join)
		))
