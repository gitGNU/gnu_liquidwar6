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

(load "main.scm") ; load this to catch error in make dist

(define lw6-test-map
  (lambda ()
    (let (
	  (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	  )
      (begin
	(display the-map)
	(display "\n")))))

(define lw6-test-game-struct
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	  )
      (begin
	(display game-struct)
	(display "\n")))))

(define lw6-test-game-state
  (lambda ()
    (let* (
	   (the-map (c-lw6ldr-read-relative (c-lw6cfg-unified-get-map-path) "subflower" '() '() 640 480 25 33333))
	   (game-struct (c-lw6ker-build-game-struct the-map))
	   (game-state (c-lw6ker-build-game-state game-struct))
	  )
      (begin
	(display game-state)
	(display "\n")))))

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
		 #t))))))

(define lw6-test-db
  (lambda ()
    (begin
      (let* (
	     (db (c-lw6p2p-db-new (c-lw6p2p-db-default-name)))
	     )
	(begin
	  (display db)
	  (display "\n")
	  ))
      (gc)
      )))

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
					       (cons "description" (_ "This is a test node."))
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
      )))

(c-lw6sys-log 2 (_ "testing map"))
(lw6-test-map)
(c-lw6sys-log 2 (_ "testing game struct"))
(lw6-test-game-struct)
(c-lw6sys-log 2 (_ "testing game state"))
(lw6-test-game-state)
(c-lw6sys-log 2 (_ "testing pilot"))
(lw6-test-pilot)
(c-lw6sys-log 2 (_ "testing bot"))
(lw6-test-bot)
(c-lw6sys-log 2 (_ "testing smobs GC, step 1"))
(lw6-test-smobs-gc)
(c-lw6sys-log 2 (_ "testing smobs GC, step 2"))
(lw6-test-smobs-gc)
(c-lw6sys-log 2 (_ "testing async load"))
(lw6-test-async-load)
(c-lw6sys-log 2 (_ "testing db"))
(lw6-test-db)
(c-lw6sys-log 2 (_ "testing node"))
(lw6-test-node)
