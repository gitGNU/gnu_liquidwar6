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
		 lw6def-c-lw6sys-build-get-version-base
		 lw6def-c-lw6sys-build-get-version-major
		 lw6def-c-lw6sys-build-get-version-minor
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
		 lw6def-c-lw6sys-build-get-enable-mod-gl1
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
      (lw6-log-notice (format #f "bazooka-size=~a"
			      (c-lw6sys-get-memory-bazooka-size)))
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

(define lw6-test-sys-debug
  (lambda ()
    (let (
	  (debug #f)
	  )
      (begin
	(set! debug (c-lw6sys-debug-get))
	(lw6-log-notice (format #f "debug=~a" debug))
	(c-lw6sys-debug-set debug)
	#t
	))))

(define lw6-test-sys-log-level
  (lambda ()
    (let (
	  (log-level #f)
	  )
      (begin
	(set! log-level (c-lw6sys-log-get-level))
	(lw6-log-notice (format #f "log-level=~a" log-level))
	(c-lw6sys-log-set-level log-level)
	#t
	))))

(define lw6-test-sys-log-backtrace-mode
  (lambda ()
    (let (
	  (log-backtrace-mode #f)
	  )
      (begin
	(set! log-backtrace-mode (c-lw6sys-log-get-backtrace-mode))
	(lw6-log-notice (format #f "log-backtrace-mode=~a" log-backtrace-mode))
	(c-lw6sys-log-set-backtrace-mode log-backtrace-mode)
	#t
	))))

(define lw6-test-sys-dump
  (lambda ()
    (begin
      (c-lw6sys-dump-clear)
      (c-lw6sys-dump "This is a message spanned on several lines\nLine 2;\nLine three!")
      #t
      )))

(define lw6-test-sys-env
  (lambda ()
    (begin
      (lw6-log-notice (format #f "username=~a" (c-lw6sys-get-username)))
      (lw6-log-notice (format #f "hostname=~a" (c-lw6sys-get-hostname)))
      (lw6-log-notice (format #f "$HOME=~a" (c-lw6sys-getenv "HOME")))
      (lw6-log-notice (format #f "$LW6_BENCH_VALUE=~a" (c-lw6sys-getenv-prefixed lw6def-bench-value)))
      #t
      )))

(define lw6-test-sys-hardware
  (lambda ()
    (begin
      (lw6-log-notice (format #f "username=~a" (c-lw6sys-megabytes-available)))
      (lw6-log-notice (format #f "hostname=~a" (c-lw6sys-openmp-get-num-procs)))
      #t
      )))

(define lw6-test-sys-time
  (lambda ()
    (begin
      (lw6-log-notice (format #f "timestamp=~a" (c-lw6sys-get-timestamp)))
      (lw6-log-notice (format #f "uptime=~a" (c-lw6sys-get-uptime)))
      (lw6-log-notice (format #f "cycle=~a" (c-lw6sys-get-cycle)))
      (c-lw6sys-delay 100)
      (c-lw6sys-sleep 0.01)
      (c-lw6sys-idle)
      (c-lw6sys-snooze)
      #t
      )))

(define lw6-test-sys-path
  (lambda ()
    (let (
	  (test-path "/this/is/a/path/with/a/filename.ext")
	  (test-file "other.file")
	  )
      (begin
	(lw6-log-notice (format #f "test-path=~a" test-path))
	(lw6-log-notice (format #f "test-file=~a" test-file))
	(lw6-log-notice (format #f "(concat ~a ~a) -> ~a" test-path test-file
				(c-lw6sys-path-concat test-path test-file)))
	(lw6-log-notice (format #f "(file-only ~a) -> ~a" test-path
				(c-lw6sys-path-file-only test-path)))
	(lw6-log-notice (format #f "(file-only ~a) -> ~a" test-file
				(c-lw6sys-path-file-only test-file)))
	(lw6-log-notice (format #f "(parent ~a) -> ~a" test-path
				(c-lw6sys-path-parent test-path)))
	(lw6-log-notice (format #f "(parent ~a) -> ~a" test-file
				(c-lw6sys-path-parent test-file)))
	(lw6-log-notice (format #f "(split ~a) -> ~a" test-path
				(c-lw6sys-path-split test-path)))
	(lw6-log-notice (format #f "(split ~a) -> ~a" test-file
				(c-lw6sys-path-split test-file)))
	#t
	))))

(define lw6-test-sys-signal
  (lambda ()
    (begin
      (c-lw6sys-signal-custom #f)
      (c-lw6sys-signal-default)
      (c-lw6sys-signal-custom #t)
      (c-lw6sys-signal-default)
      (lw6-log-notice (format #f "(poll-quit) -> ~a" (c-lw6sys-signal-poll-quit)))
      #t
      )))

(define lw6-test-sys-url
  (lambda ()
    (let* (
	   (url-src "HTTP://FOO.BAR:80/dir/file.ext")
	   (url-dst (c-lw6sys-url-canonize url-src))
	   (url-check "http://foo.bar/dir/file.ext/")
	   )
      (if (equal? url-dst url-check)
	  (begin
	    (lw6-log-notice (format #f "canonized URL \"~a\" -> \"~a\"" url-src url-dst))
	    #t
	    )
	  (begin
	    (lw6-log-warning (format #f "canonized URL failed \"~a\" != \"~a\"" url-dst url-check))
	    #f
	    )
	  ))))

(define lw6-test-hlp-about
  (lambda ()
    (begin
      (lw6-log-notice (format #f "(about ~a) -> ~a" lw6def-about (c-lw6hlp-about lw6def-about)))
      (lw6-log-notice (format #f "(default-value ~a) -> ~a" lw6def-zoom (c-lw6hlp-get-default-value lw6def-zoom)))
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

(define lw6-test-cfg
  (lambda ()
    (let (
	  (ret #f)
	  )
      (begin
	(c-lw6cfg-init)
	(c-lw6cfg-defaults)
	(c-lw6cfg-load (c-lw6sys-get-config-file))
	(let* (
	       (magic-number-exists (c-lw6cfg-option-exists lw6def-magic-number))
	       (magic-number-value (c-lw6cfg-get-option lw6def-magic-number))
	       )
	  (if (and magic-number-exists magic-number-value)
	      (begin
		(set! ret #t)
		(lw6-log-notice (format #f "magic-number=~a" magic-number-value))
		(c-lw6cfg-set-option magic-number-value magic-number-value)
		(lw6-log-notice (format #f "(c-lw6cfg-unified-get-user-dir) -> ~a" (c-lw6cfg-unified-get-user-dir)))
		(lw6-log-notice (format #f "(c-lw6cfg-unified-get-log-file) -> ~a" (c-lw6cfg-unified-get-log-file)))
		(lw6-log-notice (format #f "(c-lw6cfg-unified-get-music-path) -> ~a" (c-lw6cfg-unified-get-music-path)))
		)))
	(c-lw6cfg-save (c-lw6sys-get-config-file))
	(c-lw6cfg-quit)
	ret
	))))

(define lw6-test-gui-menu
  (lambda ()
    (let* (
	   (menu (c-lw6gui-menu-new "Test menu" "Test help" "Pop... UP" "Esc" #t))
	   (menu-item-1 (list (cons "label" "Test item 1")
			      (cons "tooltip" "Test tooltip 1")
			      (cons "value" 3)
			      (cons "enabled" #t)
			      (cons "selected" #t)
			      (cons "colored" #f)))
	   (menu-item-2 (list (cons "label" "Test item 2")
			      (cons "tooltip" "Test tooltip 2")
			      (cons "value" 5)
			      (cons "enabled" #t)
			      (cons "selected" #t)
			      (cons "colored" #f)))
	   (id-1 (c-lw6gui-menu-append menu menu-item-1))
	   (id-2 (c-lw6gui-menu-append menu menu-item-2))
	   (ret #f)
	   )
      (begin
	(set! menu-item-1 (assoc-set! menu-item-1 "id" id-1))
	(set! menu-item-2 (assoc-set! menu-item-2 "id" id-2))
	(lw6-log-notice menu)
	(if (c-lw6gui-menu-has-popup menu)
	    (begin
	      (set! ret #t)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-close-popup menu)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-set-breadcrumbs menu (list "lost" "in" "the" "woods"))
	      (set! menu-item-2 (assoc-set! menu-item-2 "label" "Modified item 2"))
	      (lw6-log-notice menu-item-2)
	      (c-lw6gui-menu-sync menu menu-item-2)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-select-esc menu #t)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-select menu 0 #f)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-scroll-down menu)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-scroll-up menu)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-select menu 1 #t)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-enable-esc menu #f)
	      (c-lw6gui-menu-enable-esc menu #t)
	      (lw6-log-notice menu)
	      (c-lw6gui-menu-enable-esc menu #f)
	      (lw6-log-notice menu)
	      ))
	ret
	))))

(define lw6-test-gui-input
  (lambda ()
    ;; todo (need some sort of DSP to do this...)
    #t
    ))

(define lw6-test-map
  (lambda ()
    (let (
	  (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	  )
      (begin
	(lw6-log-notice the-map)
	(if the-map #t #f)
	))))

(define lw6-test-game-struct
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   )
      (begin
	(lw6-log-notice game-struct)
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
	(lw6-log-notice game-state)
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
	   (round-0 (c-lw6pil-get-round-0 pilot))
	   (seq-0 (c-lw6pil-get-seq-0 pilot))
	   (round-from-seq (c-lw6pil-seq2round pilot seq-0))
	   (seq-from-round (c-lw6pil-round2seq pilot round-0))
	   )
      (begin
	(c-lw6sys-idle)
	(lw6-log-notice (format #f "pilot -> \"~a\"" pilot))
	(lw6-log-notice (format #f "round-0 -> \"~a\", seq-0 -> ~a" round-0 seq-0))
	(lw6-log-notice (format #f "round-from-seq -> \"~a\", seq-from-round -> ~a" round-from-seq seq-from-round))
	(c-lw6sys-snooze)
	(if (and pilot (= round-0 round-from-seq) (= seq-0 seq-from-round)) #t #f)
	))))

(define lw6-test-suite
  (lambda ()
    (let* (
	   (dump (c-lw6pil-suite-init (c-lw6sys-get-timestamp)))
	   (seq-0 (c-lw6pil-suite-get-seq-0))
	   (node-ids (map c-lw6pil-suite-get-node-id (list 0 1 2)))
	   (commands-by-node-index (map (lambda (node-index)
					  (map (lambda (stage)
						 (c-lw6pil-suite-get-commands-by-node-index node-index stage))
					       (list 0 1 2)))
					(list 0 1 2)))
	   (commands-by-stage (map c-lw6pil-suite-get-commands-by-stage
				   (list 0 1 2)))
	   (checkpoints (map c-lw6pil-suite-get-checkpoint
			     (list 0 1 2)))
	   )
      (begin
	(lw6-log-notice (format #f "dump = ~a" dump))
	(lw6-log-notice (format #f "seq-0 = ~a" seq-0))
	(lw6-log-notice (format #f "node-ids = ~a" node-ids))
	(lw6-log-notice (format #f "commands-by-node-index = ~a" commands-by-node-index))
	(lw6-log-notice (format #f "commands-by-stage = ~a" commands-by-stage))
	(lw6-log-notice (format #f "checkpoints = ~a" checkpoints))
	#t
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
	    (lw6-log-notice bot1)
	    (lw6-log-notice (c-lw6bot-next-move bot1))
	    (lw6-log-notice bot2)
	    (lw6-log-notice (c-lw6bot-next-move bot2))
	    (if (and bot1 bot2) #t #f)
	    ))))))

(define lw6-test-img
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   (game-state (c-lw6ker-build-game-state game-struct))
	   (jpeg (c-lw6img-screenshot game-state 85))
	   )
      (begin
	(lw6-log-notice (format #f "JPEG screenshot for \"~a\" is \"~a\"" game-state jpeg))
	(and game-state jpeg)
	))))

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
	(lw6-log-notice menu)
	(lw6-log-notice game-state)
	(lw6-log-notice game-look)
	(lw6-log-notice loader)
	(gc)
	(if (and menu game-state game-look loader) #t #f)
	))))

(define lw6-test-gen
  (lambda ()
    (let* (
	   (seed-new (c-lw6gen-seed-new))
	   (seed-normalize (c-lw6gen-seed-normalize "GA-BU-z0 ME_%!"))
	   (level-new (c-lw6gen-create-from-seed seed-new 800 450))
	   (level-normalize (c-lw6gen-create-from-seed seed-normalize 160 90))
	   )
      (begin
	(lw6-log-notice (format #f "new seed \"~a\" -> \"~a\"" seed-new level-new))
	(lw6-log-notice (format #f "normalize seed \"~a\" -> \"~a\"" seed-normalize level-normalize))
	(and seed-new seed-normalize level-new level-normalize)
	))))

(define lw6-test-async-load
  (lambda ()
    (let* (
	   (loader (c-lw6tsk-loader-new 1.0))
	   (loaded-ldr #f)
	   (loaded-gen #f)
	   )
      (begin
	(c-lw6tsk-loader-push-ldr loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(c-lw6tsk-loader-push-ldr loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(c-lw6tsk-loader-push-ldr loader (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333)
	(while (not loaded-ldr)
	       (begin
		 (set! loaded-ldr (c-lw6tsk-loader-pop loader))
		 (c-lw6sys-sleep 0.1)
		 (lw6-log-notice loader)
		 #t))
	(lw6-log-notice loaded-ldr)
	(c-lw6tsk-loader-push-gen loader "abcdabcdabcdabcd" 640 480 100 33333)
	(c-lw6tsk-loader-push-gen loader "1234123412341234" 640 480 100 33333)
	(while (not loaded-gen)
	       (begin
		 (set! loaded-gen (c-lw6tsk-loader-pop loader))
		 (c-lw6sys-sleep 0.1)
		 (lw6-log-notice loader)
		 #t))
	(lw6-log-notice loaded-gen)
	(if (and loader loaded-ldr loaded-gen) #t #f)
	))))

(define lw6-test-p2p-db
  (lambda ()
    (begin
      (let* (
	     (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
	     )
	(begin
	  (lw6-log-notice db)
	  (gc)
	  (if db #t #f)
	  )))))

(define lw6-test-p2p-node
  (lambda ()
    (begin
      (c-lw6net-init #f)
      (let* (
	     (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
	     (node (c-lw6p2p-node-new db (list (cons "client-backends" "tcp,udp")
					       (cons "server-backends" "tcpd,udpd,httpd")
					       (cons "bind-ip" "0.0.0.0")
					       (cons "bind-port" 8058)
					       (cons "node-id" (c-lw6sys-generate-id-64))
					       (cons "public-url" "http://localhost/")
					       (cons "password" "")
					       (cons "title" "")
					       (cons "description" (_ "Dummy test node"))
					       (cons "bench" 10)
					       (cons "open-relay" #f)
					       (cons "known-nodes" "http://ufoot.org:8058/")
					       (cons "network-reliability" 100)
					       (cons "trojan" #f)
					       )))
	     )
	(begin
	  (lw6-log-notice node)
	  (lw6-log-notice (c-lw6p2p-node-get-id node))
	  (c-lw6p2p-node-poll node)
	  (c-lw6sys-sleep 0.1)
	  (c-lw6p2p-node-close node)
	  ))
      (c-lw6net-quit)
      (gc)
      ;; if we get here, we have succeeded else node creation would raise an error
      #t)))

(define lw6-test-control-0
  (lambda ()
    (begin
      (lw6-log-notice (format #f "ret=~a" (c-lw6-get-ret)))
      #t)))

(c-lw6-set-ret #f) ;; reset this to "failed" so that it has the right value is script is interrupted
(c-lw6-set-ret (and
		(lw6-test-run lw6-test-sys-build)
		(lw6-test-run lw6-test-sys-options)
		(lw6-test-run lw6-test-sys-id)
		(lw6-test-run lw6-test-sys-bazooka)
		(lw6-test-run lw6-test-sys-debug)
		(lw6-test-run lw6-test-sys-log-level)
		(lw6-test-run lw6-test-sys-log-backtrace-mode)
		(lw6-test-run lw6-test-sys-dump)
		(lw6-test-run lw6-test-sys-env)
		(lw6-test-run lw6-test-sys-hardware)
		(lw6-test-run lw6-test-sys-time)
		(lw6-test-run lw6-test-sys-path)
		(lw6-test-run lw6-test-sys-signal)
		(lw6-test-run lw6-test-sys-url)
		(lw6-test-run lw6-test-hlp-about)
		(lw6-test-run lw6-test-hlp-lists)
		(lw6-test-run lw6-test-cfg)
		(lw6-test-run lw6-test-gui-menu)
		(lw6-test-run lw6-test-gui-input)
		(lw6-test-run lw6-test-map)
		(lw6-test-run lw6-test-game-struct)
		(lw6-test-run lw6-test-game-state)
		(lw6-test-run lw6-test-pilot)
		(lw6-test-run lw6-test-suite)
		(lw6-test-run lw6-test-img)
		(lw6-test-run lw6-test-bot)
		(lw6-test-run lw6-test-smobs-gc)
		(lw6-test-run lw6-test-smobs-gc)
		(lw6-test-run lw6-test-gen)
		(lw6-test-run lw6-test-async-load)
		(lw6-test-run lw6-test-p2p-db)
		(lw6-test-run lw6-test-p2p-node)
		(lw6-test-run lw6-test-control-0)
		))
