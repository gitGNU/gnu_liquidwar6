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

(read-enable 'positions)
;;(debug-enable 'debug)
(debug-enable 'backtrace)
;;(debug-set! frames 12)
(debug-set! depth 50)

(define (lazy-catch-handler key . args)
  ;; (let ((stack (make-stack #t lazy-catch-handler)))
  (let ((stack (make-stack #t 5)))
    ;; do whatever you want with stack, such as
    ;; saving it off or calling display-backtrace
    (let (
	  (backtrace-port (open-output-string))
	  (stack (make-stack #t))
	  )
      (begin
	(display-backtrace stack backtrace-port
			   (- (stack-length stack) 5)
			   (- (stack-length stack) 8))
	(c-lw6sys-dump
	 (format #f
		 "~a\n~a"
		 (apply format (cons #f (cons (cadr args) (caddr args))))
		 (get-output-string backtrace-port)))
	(c-lw6sys-log 0
		      (format #f
			      "~a \"~a\""
			      (_ "scheme error")
			      (apply format (cons #f (cons (cadr args) (caddr args))))
			      ))
	(close-output-port backtrace-port)
	(c-lw6-release)
	)
      )
    )
  ;; (apply throw key args)
  )

(catch #t
       (lambda ()
	 (lazy-catch #t
		     (lambda ()
		       (begin
			 (c-lw6sys-log 3 (_ "running scheme code with Guile"))
			 (load "main.scm")
			 (lw6-main)
			 (c-lw6sys-log 3 (_ "leaving Guile"))
			 )
		       )
		     lazy-catch-handler
		     )
	 )
       (lambda (key . args)
	 (begin
	   (c-lw6sys-log 3 (_ "catched Guile error"))
	   )
	 )
       )


