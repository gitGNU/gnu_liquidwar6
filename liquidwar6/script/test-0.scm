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
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author        : ufoot@ufoot.org

(load "load.scm") ; load this to catch error in make dist
(lw6-load) ; will load all includes but main, trap syntax errors at least

(define lw6-test-sys-build
  (lambda ()
    (begin
      (map (lambda (procname)
	     (let (
		   (retval (eval-string (string-concatenate (list "(" procname ")"))))
		   )
	       (lw6-log-notice (format #f "~a -> \"~a\"" procname retval))))
	   (list lw6def-c-lw6sys-build-get-package-tarname
		 lw6def-c-lw6sys-build-get-package-name
		 lw6def-c-lw6sys-build-get-package-string
		 lw6def-c-lw6sys-build-get-package-id
		 lw6def-c-lw6sys-build-get-version
		 lw6def-c-lw6sys-build-get-codename
		 lw6def-c-lw6sys-build-get-stamp
		 lw6def-c-lw6sys-build-get-md5sum
		 lw6def-c-lw6sys-build-get-copyright
		 lw6def-c-lw6sys-build-get-license
		 lw6def-c-lw6sys-build-get-home-url
		 lw6def-c-lw6sys-build-get-bugs-url
		 lw6def-c-lw6sys-build-get-configure-args
		 lw6def-c-lw6sys-build-get-gcc-version
		 lw6def-c-lw6sys-build-get-cflags
		 lw6def-c-lw6sys-build-get-ldflags
		 lw6def-c-lw6sys-build-get-hostname
		 lw6def-c-lw6sys-build-get-date
		 lw6def-c-lw6sys-build-get-time
		 lw6def-c-lw6sys-build-get-host-cpu
		 lw6def-c-lw6sys-build-get-endianness
		 lw6def-c-lw6sys-build-get-pointer-size
		 lw6def-c-lw6sys-build-is-x86
		 lw6def-c-lw6sys-build-get-host-os
		 lw6def-c-lw6sys-build-is-gnu
		 lw6def-c-lw6sys-build-is-unix
		 lw6def-c-lw6sys-build-is-ms-windows
		 lw6def-c-lw6sys-build-is-mac-os-x
		 lw6def-c-lw6sys-build-is-gp2x
		 lw6def-c-lw6sys-build-get-top-srcdir
		 lw6def-c-lw6sys-build-get-prefix
		 lw6def-c-lw6sys-build-get-datadir
		 lw6def-c-lw6sys-build-get-libdir
		 lw6def-c-lw6sys-build-get-includedir
		 lw6def-c-lw6sys-build-get-localedir
		 lw6def-c-lw6sys-build-get-docdir
		 lw6def-c-lw6sys-build-get-enable-console
		 lw6def-c-lw6sys-build-get-enable-gtk
		 lw6def-c-lw6sys-build-get-enable-mod-gl
		 lw6def-c-lw6sys-build-get-enable-mod-csound
		 lw6def-c-lw6sys-build-get-enable-mod-ogg
		 lw6def-c-lw6sys-build-get-enable-mod-http
		 lw6def-c-lw6sys-build-get-enable-openmp
		 lw6def-c-lw6sys-build-get-enable-optimize
		 lw6def-c-lw6sys-build-get-enable-allinone
		 lw6def-c-lw6sys-build-get-enable-fullstatic
		 lw6def-c-lw6sys-build-get-enable-paranoid
		 lw6def-c-lw6sys-build-get-enable-gprof
		 lw6def-c-lw6sys-build-get-enable-instrument
		 lw6def-c-lw6sys-build-get-enable-profiler
		 lw6def-c-lw6sys-build-get-enable-gcov
		 lw6def-c-lw6sys-build-get-enable-valgrind
		 lw6def-c-lw6sys-build-get-bin-id	       
		 ))
      #t
      )))

(define lw6-test-sys-options
  (lambda ()
    (begin
      (map (lambda (procname)
	     (let (
		   (retval (eval-string (string-concatenate (list "(" procname ")"))))
		   )
	       (lw6-log-notice (format #f "~a -> \"~a\"" procname retval))))
	   (list lw6def-c-lw6sys-get-cwd
		 lw6def-c-lw6sys-get-default-user-dir
		 lw6def-c-lw6sys-get-default-config-file
		 lw6def-c-lw6sys-get-default-log-file
		 lw6def-c-lw6sys-get-default-prefix
		 lw6def-c-lw6sys-get-default-mod-dir
		 lw6def-c-lw6sys-get-default-data-dir
		 lw6def-c-lw6sys-get-default-music-dir
		 lw6def-c-lw6sys-get-default-music-path
		 lw6def-c-lw6sys-get-default-map-dir
		 lw6def-c-lw6sys-get-default-map-path
		 lw6def-c-lw6sys-get-default-script-file
		 lw6def-c-lw6sys-get-run-dir
		 lw6def-c-lw6sys-get-user-dir
		 lw6def-c-lw6sys-get-config-file
		 lw6def-c-lw6sys-get-log-file
		 lw6def-c-lw6sys-get-prefix
		 lw6def-c-lw6sys-get-mod-dir
		 lw6def-c-lw6sys-get-data-dir
		 lw6def-c-lw6sys-get-music-dir
		 lw6def-c-lw6sys-get-music-path
		 lw6def-c-lw6sys-get-map-dir
		 lw6def-c-lw6sys-get-map-path
		 lw6def-c-lw6sys-get-script-file
		 ))
      #t
      )))

(define lw6-test-sys-id
  (lambda ()
    (begin
      (map (lambda (procname)
	     (let (
		   (retval (eval-string (string-concatenate (list "(" procname ")"))))
		   )
	       (lw6-log-notice (format #f "~a -> \"~a\"" procname retval))))
	   (list lw6def-c-lw6sys-generate-id-16
		 lw6def-c-lw6sys-generate-id-32
		 lw6def-c-lw6sys-generate-id-64
		 ))
      #t
      )))

(define lw6-test-sys-bazooka
  (lambda ()
    (begin
      ;; double bazooka size
      (c-lw6sys-set-memory-bazooka-size 
       (* 2 (c-lw6sys-get-memory-bazooka-size)))
      (c-lw6sys-set-memory-bazooka-eraser 
       (not (c-lw6sys-get-memory-bazooka-eraser)))
      (lw6-log-notice (format #f "bazooka-eraser=~a"
			      (c-lw6sys-get-memory-bazooka-eraser)))
      ;; run it again to get back to previous mode
      (c-lw6sys-set-memory-bazooka-eraser 
       (not (c-lw6sys-get-memory-bazooka-eraser)))
      (lw6-log-notice (format #f "bazooka-size=~a"
			      (c-lw6sys-get-memory-bazooka-size)))
      #t
      )))

(define lw6-test-hlp-lists
  (lambda ()
    (begin
      (map (lambda (procname)
	     (let (
		   (retval (eval-string (string-concatenate (list "(" procname ")"))))
		   )
	       (lw6-log-notice (format #f "~a -> ~a items" procname (length retval)))))
	   (list lw6def-c-lw6hlp-list-quick
		 lw6def-c-lw6hlp-list-doc
		 lw6def-c-lw6hlp-list-show
		 lw6def-c-lw6hlp-list-path
		 lw6def-c-lw6hlp-list-players
		 lw6def-c-lw6hlp-list-input
		 lw6def-c-lw6hlp-list-graphics
		 lw6def-c-lw6hlp-list-sound
		 lw6def-c-lw6hlp-list-network
		 lw6def-c-lw6hlp-list-map
		 lw6def-c-lw6hlp-list-map-rules
		 lw6def-c-lw6hlp-list-map-hints
		 lw6def-c-lw6hlp-list-map-style
		 lw6def-c-lw6hlp-list-map-teams
		 lw6def-c-lw6hlp-list-funcs
		 lw6def-c-lw6hlp-list-hooks
		 lw6def-c-lw6hlp-list-advanced
		 lw6def-c-lw6hlp-list-aliases
		 lw6def-c-lw6hlp-list-team-colors
		 lw6def-c-lw6hlp-list-weapons
		 lw6def-c-lw6hlp-list
		 ))
      #t
      )))

(define lw6-test-map
  (lambda ()
    (let (
	  (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	  )
      (begin
	(display the-map)
	(display "\n")
	(if the-map #t #f)
	))))

(define lw6-test-game-struct
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   )
      (begin
	(display game-struct)
	(display "\n")
	(if game-struct #t #f)
	))))

(define lw6-test-game-state
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   (game-state (c-lw6ker-build-game-state game-struct))
	   )
      (begin
	(display game-state)
	(display "\n")
	(if game-state #t #f)
	))))

(define lw6-test-pilot
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   (game-state (c-lw6ker-build-game-state game-struct))
	   (pilot (c-lw6pil-build-pilot game-state 					   
					(c-lw6pil-seq-random-0)
					0))
	   )
      (begin
	(c-lw6sys-sleep 1)
	(display pilot)
	(display "\n")
	(c-lw6sys-sleep 1)
	(if pilot #t #f)
	))))

(define lw6-test-bot
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   (game-state (c-lw6ker-build-game-state game-struct))
	   (pilot (c-lw6pil-build-pilot game-state 					   
					(c-lw6pil-seq-random-0)
					0))
	   )
      (begin
	(c-lw6pil-execute-command game-state "123123123123 1234123412341234 REGISTER" 123123123123)
	(c-lw6pil-execute-command game-state "123123123123 1234123412341234 ADD 1234 RED" 123123123123)
	(c-lw6pil-execute-command game-state "123123123123 1234123412341234 ADD 2345 GREEN" 123123123123)
	(let (
	      (bot1 (c-lw6bot-new "idiot" game-state pilot 2 "1234" 1.0 100))
	      (bot2 (c-lw6bot-new "random" game-state pilot 2 "2345" 1.0 100))
	      )
	  (begin
	    (c-lw6ker-do-round game-state)
	    (display bot1)
	    (display "\n")
	    (display (c-lw6bot-next-move bot1))
	    (display "\n")
	    (display bot2)
	    (display "\n")
	    (display (c-lw6bot-next-move bot2))
	    (display "\n")
	    (if (and bot1 bot2) #t #f)
	    ))))))

(define lw6-test-smobs-gc
  (lambda ()
    (let (
	  (menu (c-lw6gui-menu-new "Test" "This is the (usefull) help" "Pop... UP" "Esc" #t))
	  (game-state (c-lw6ker-build-game-state 
		       (c-lw6ker-build-game-struct 
			(c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))))
	  (game-look (c-lw6gui-default-look))
	  (loader (c-lw6tsk-loader-new 1.5))
	  )
      (begin
	(gc)
	(display menu)
	(display "\n")
	(display game-state)
	(display "\n")
	(display game-look)
	(display "\n")
	(display loader)
	(display "\n")
	(gc)
	(if (and menu game-state game-look loader) #t #f)
	))))

(define lw6-test-async-load
  (lambda ()
    (let* (
	   (loader (c-lw6tsk-loader-new 1.0))
	   (loaded #f)
	   )
      (begin
	(c-lw6tsk-loader-push loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(c-lw6tsk-loader-push loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(c-lw6tsk-loader-push loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(while (not loaded)
	       (begin
		 (set! loaded (c-lw6tsk-loader-pop loader))
		 (c-lw6sys-sleep 0.1)
		 (display loader)
		 (display "\n")
		 #t))
	(if (and loader loaded) #t #f)
	))))

(define lw6-test-db
  (lambda ()
    (begin
      (let* (
	     (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
	     )
	(begin
	  (display db)
	  (display "\n")
	  (gc)
	  (if db #t #f)
	  )))))

(define lw6-test-node
  (lambda ()
    (begin
      (c-lw6net-init #f)
      (let* (
	     (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
	     (node (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
					       (cons "server-backends" "tcpd,udpd,httpd")
					       (cons "bind-ip" "0.0.0.0")
					       (cons "bind-port" 8056)
					       (cons "public-url" "http://localhost/")
					       (cons "password" "")
					       (cons "title" "")
					       (cons "description" (_ "Dummy test node"))
					       (cons "bench" 10)
					       (cons "open-relay" #f)
					       (cons "known-nodes" "http://ufoot.org:8056/")
					       (cons "network-reliability" 100)
					       (cons "trojan" #f)
					       )))
	     )
	(begin
	  (display node)
	  (display "\n")
	  (display (c-lw6p2p-node-get-id node))
	  (display "\n")
	  (c-lw6p2p-node-poll node)
	  (c-lw6sys-sleep 0.1)
	  (c-lw6p2p-node-close node)
	  ))
      (c-lw6net-quit)
      (gc)
      ;; if we get here, we have succeeded else node creation would raise an error
      #t)))

(c-lw6-set-ret #f) ;; reset this to "failed" so that it has the right value is script is interrupted
(c-lw6-set-ret (and
		(lw6-test-sys-build)
		(lw6-test-sys-options)
		(lw6-test-sys-id)
		(lw6-test-sys-bazooka)
		(lw6-test-hlp-lists)
		(lw6-test-map)
		(lw6-test-game-struct)
		(lw6-test-game-state)
		(lw6-test-pilot)
		(lw6-test-bot)
		(lw6-test-smobs-gc)
		(lw6-test-smobs-gc)
		(lw6-test-async-load)
		(lw6-test-db)
		(lw6-test-node)))
