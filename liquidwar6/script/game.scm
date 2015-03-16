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

(define %lw6-game-globals
  (make-hash-table 31)
)

(define lw6-purge-game-globals
  (lambda ()
    (set! %lw6-game-globals (make-hash-table 31))
  ))

(define lw6-set-game-global!
  (lambda (key value)
    (hash-set! %lw6-game-globals key value)
    )
  )

(define lw6-get-game-global
  (lambda (key)
    (hash-ref %lw6-game-globals key)
    )
  )

(define lw6-game-param-update
  (lambda ()
    (begin
      (lw6-set-game-global! "default-param" (lw6-default-param))
      (lw6-set-game-global! "forced-param" (lw6-forced-param))
      )))

(define lw6-game-reset
  (lambda ()
    (begin
      (lw6-set-game-global! "game-state" #f)
      (lw6-display-param-set! "game-state" #f)
      (lw6-set-game-global! "pilot" #f)
      (lw6-display-param-set! "pilot" #f)
      (lw6-set-game-global! "cursors" (make-hash-table 5))
      (let ((cursors (lw6-get-game-global "cursors")))
	(map (lambda (cursor-key)
	       (hash-set! cursors (string-concatenate (list "cursor-" cursor-key))
			  (lw6-make-cursor cursor-key)))
	     lw6-cursor-keys))
      (let (
	    (dsp (lw6-get-game-global "dsp"))
	    (snd (lw6-get-game-global "snd"))
	    )
	(if dsp (c-lw6gui-input-reset dsp))
	)
      (lw6-set-game-global! "solo" (if (lw6-get-game-global "solo") #t #f))
      (lw6-set-game-global! "network" (if (lw6-get-game-global "network") #t #f))
      (lw6-game-param-update)
      )))

(define lw6-init-game-globals
  (lambda ()
    (begin
      (lw6-set-game-global! "node-id" (c-lw6sys-generate-id-64))
      (lw6-set-game-global! lw6def-map-path (c-lw6cfg-unified-get-map-path))
      (lw6-game-param-update)
      (lw6-set-game-global! "loader" (c-lw6tsk-loader-new (lw6-config-get-number lw6def-loader-sleep)))
      (lw6-set-game-global! "loaded" (list))
      (lw6-set-game-global! "look" (lw6-default-look))
      (lw6-set-game-global! "display-param" (lw6-display-param-init))
      (lw6-set-game-global! "zoom-next-in" (c-lw6sys-get-timestamp))
      (lw6-set-game-global! "zoom-next-out" (c-lw6sys-get-timestamp))
      (lw6-game-splash)
      )
    )
  )

(define lw6-game-splash
  (lambda ()
    (lw6-game-param-update)
    (lw6-set-game-global! "command-func" lw6-command-func-idle)
    (lw6-display-splash-setup)
    (lw6-set-game-global! "loaded-func" lw6-loaded-func-idle)
    (lw6-game-reset)
    )
  )

(define lw6-game-idle
  (lambda ()
    (lw6-game-param-update)
    (lw6-set-game-global! "command-func" lw6-command-func-idle)
    (lw6-display-idle-setup)
    (lw6-set-game-global! "loaded-func" lw6-loaded-func-idle)
    (lw6-game-reset)
    )
  )

(define lw6-game-preview
  (lambda ()
    (lw6-game-param-update)
    (lw6-set-game-global! "command-func" lw6-command-func-idle)
    (lw6-display-preview-setup)
    (lw6-set-game-global! "loaded-func" lw6-loaded-func-preview)
    (lw6-game-reset)
    )
  )

(define lw6-game-running
  (lambda ()
    (lw6-game-param-update)
    ; command-func is set elsewhere, depends on game settings
    (lw6-display-running-setup)
    (lw6-set-game-global! "loaded-func" lw6-loaded-func-idle)
    ; we garbage collect before the "realtime" part
    (gc)
    (lw6-play-fx-start)
    (lw6-music-game (lw6-get-game-global "level"))
    )
  )

(define lw6-game-put-local-teams
  (lambda ()
    (let* (
	   (loaded (lw6-get-game-global "loaded"))
	   (level (assoc-ref loaded "level"))
	   (game-struct (assoc-ref loaded "game-struct"))
	   (game-state (assoc-ref loaded "game-state"))
	   (game-state-preview (if (and game-struct game-state) (c-lw6ker-dup-game-state game-struct game-state) #f))
	   (node-id (lw6-get-game-global "node-id"))
	  )
      (if (and level game-struct game-state-preview)
	  (begin
	    (lw6-set-game-global! "loaded" (assoc-set! loaded "game-state-preview" game-state-preview))
	    (c-lw6ker-register-node game-state-preview node-id)
	    (lw6-cursor-set-configured-player! game-state-preview "1" node-id)
	    (lw6-cursor-set-configured-player! game-state-preview "2" node-id)
	    (lw6-cursor-set-configured-player! game-state-preview "3" node-id)
	    (lw6-cursor-set-configured-player! game-state-preview "4" node-id)
	    (lw6-display-param-set! "level" level)
	    (lw6-display-param-set! "game-struct" game-struct)
	    (lw6-display-param-set! "game-state" game-state-preview)
	    )))))

(define lw6-game-start-local-step1
  (lambda (step2-func)
    (let* (
	   (loaded (lw6-get-game-global "loaded"))
	   (level (assoc-ref loaded "level"))
	   (game-struct (assoc-ref loaded "game-struct"))
	   (game-state-loaded (assoc-ref loaded "game-state"))
	   (bench-value-loaded (assoc-ref loaded "bench-value"))
	   (game-state-display (if (and game-struct game-state-loaded
					(equal? (lw6-loader-local-bench-value) bench-value-loaded))
				   (c-lw6ker-dup-game-state game-struct game-state-loaded) #f))
	   (node-id (lw6-get-game-global "node-id"))
	  )
      (if game-state-display
	  (begin
	    (lw6-set-game-global! "level" level)
	    (lw6-set-game-global! "game-struct" game-struct)
	    (lw6-set-game-global! "game-state" game-state-display)
	    (lw6-display-param-set! "level" level)
	    (lw6-display-param-set! "game-struct" game-struct)
	    (lw6-display-param-set! "game-state" game-state-display)
	    (lw6-set-game-global! "loaded-func" lw6-loaded-func-idle)
	    (step2-func)
	    )
	  (begin
	    (lw6-push-menu (lw6-wait-menu
			    (_ "Preparing map, this can take some time")))
	    (lw6-loader-push-ldr-if-needed (lw6-config-get-string lw6def-chosen-map) #f)
	    (lw6-set-game-global! "loaded-func" (lw6-loaded-func-local-game step2-func))
	    )
	  ))))

(define lw6-game-start-network-step1
  (lambda (step2-func)
    (let* (
	   (loaded (lw6-get-game-global "loaded"))
	   (level (assoc-ref loaded "level"))
	   (game-struct (assoc-ref loaded "game-struct"))
	   (game-state-loaded (assoc-ref loaded "game-state"))
	   (bench-value-loaded (assoc-ref loaded "bench-value"))
	   (game-state-display (if (and game-struct game-state-loaded
					(equal? (lw6-loader-network-bench-value) bench-value-loaded))
				   (c-lw6ker-dup-game-state game-struct game-state-loaded) #f))
	   (node-id (lw6-get-game-global "node-id"))
	   )
      (if game-state-display
	  (begin
	    (lw6-set-game-global! "level" level)
	    (lw6-set-game-global! "game-struct" game-struct)
	    (lw6-set-game-global! "game-state" game-state-display)
	    (lw6-display-param-set! "level" level)
	    (lw6-display-param-set! "game-struct" game-struct)
	    (lw6-display-param-set! "game-state" game-state-display)
	    (lw6-set-game-global! "loaded-func" lw6-loaded-func-idle)
	    (step2-func)
	    )
	  (begin
	    (lw6-push-menu (lw6-wait-menu
			    (_ "Preparing map, this can take some time")))
	    (lw6-loader-push-ldr-if-needed (lw6-config-get-string lw6def-chosen-map) #t)
	    (lw6-set-game-global! "loaded-func" (lw6-loaded-func-network-game step2-func))
	    )
	  ))))

(define lw6-game-start-solo-step2
  (lambda ()
    (let* (
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (node-id (lw6-get-game-global "node-id"))
	   (rounds (c-lw6ker-get-rounds game-state))
	   (seq-0 (c-lw6pil-seq-random-0))
	   (bot-keys (list-head lw6-cursor-bot-keys (string->number (c-lw6map-param-get level lw6def-nb-bots))))
	   )
      (begin
	(c-lw6pil-execute-command game-state (lw6-command-register (+ seq-0 rounds) node-id) seq-0)
	(map (lambda (player-key)
	       (let (
		     (command (lw6-cursor-prepare-map-defined-player-command
			       level game-state seq-0 player-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
	     (list "1"))
	(map (lambda (bot-key)
	       (let (
		     (command (lw6-cursor-prepare-bot-command
			       level game-state seq-0 bot-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
	     bot-keys)
	(let (
	      (pilot (c-lw6pil-build-pilot game-state
					   seq-0
					   (c-lw6sys-get-timestamp)))
	      )
	  (begin
	    (lw6-set-game-global! "pilot" pilot)
	    (lw6-display-param-set! "pilot" pilot)
	    (map (lambda (player-key)
		   (begin
		     (lw6-cursor-init-pos game-state player-key)
		     (lw6-cursor-init-map-defined-universal-mover player-key)
		     ))
		 (list "1"))
	    (map (lambda (player-key)
		   (begin
		     (lw6-cursor-init-pos game-state player-key)
		     (lw6-cursor-init-map-defined-bot-mover level game-state pilot player-key)
		     ))
		 bot-keys)
	    ;;(lw6-cursor-set-universal-if-needed)
	    (lw6-push-menu (lw6-ingame-menu))
	    (lw6-game-running)
	    (lw6-set-game-global! "command-func" lw6-command-func-local)
	    ))
	(lw6-set-game-global! "solo" #t)
	(lw6-set-game-global! "network" #f)
	))))

(define lw6-game-start-1on1-step2
  (lambda ()
    (let* (
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (node-id (lw6-get-game-global "node-id"))
	   (rounds (c-lw6ker-get-rounds game-state))
	   (seq-0 (c-lw6pil-seq-random-0))
	   )
      (begin
	(c-lw6pil-execute-command game-state (lw6-command-register (+ seq-0 rounds) node-id) seq-0)
	(map (lambda (player-key)
	       (let (
		     (command (lw6-cursor-prepare-map-defined-player-command
			       level game-state seq-0 player-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
	     (list "1" "2"))
	(let (
	      (pilot (c-lw6pil-build-pilot game-state
					   seq-0
					   (c-lw6sys-get-timestamp)))
	      )
	  (begin
	    (lw6-set-game-global! "pilot" pilot)
	    (lw6-display-param-set! "pilot" pilot)
	    (map (lambda (player-key)
		   (begin
		     (lw6-cursor-init-pos game-state player-key)
		     (lw6-cursor-init-map-defined-semi-universal-mover player-key)
		     ))
		 (list "1" "2"))
	    ;;(lw6-cursor-set-universal-if-needed)
	    (lw6-push-menu (lw6-ingame-menu))
	    (lw6-game-running)
	    (lw6-set-game-global! "command-func" lw6-command-func-local)
	    ))
	(lw6-set-game-global! "solo" #f)
	(lw6-set-game-global! "network" #f)
	))))

(define lw6-game-start-local-step2
  (lambda ()
    (let* (
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (node-id (lw6-get-game-global "node-id"))
	   (rounds (c-lw6ker-get-rounds game-state))
	   (bot-keys (list-head lw6-cursor-bot-keys (lw6-config-get-number lw6def-nb-bots)))
	   (seq-0 (c-lw6pil-seq-random-0))
	   )
      (begin
	(c-lw6pil-execute-command game-state (lw6-command-register (+ seq-0 rounds) node-id) seq-0)
	(map (lambda (player-key)
	       (let (
		     (command (lw6-cursor-prepare-configured-player-command
			       game-state seq-0 player-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
	     lw6-cursor-player-keys)
	(map (lambda (bot-key)
	       (let (
		     (command (lw6-cursor-prepare-bot-command
			       level game-state seq-0 bot-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
	     bot-keys)
	(let (
	      (pilot (c-lw6pil-build-pilot game-state
					   seq-0
					   (c-lw6sys-get-timestamp)))
	      )
	  (begin
	    (lw6-set-game-global! "pilot" pilot)
	    (lw6-display-param-set! "pilot" pilot)
	    (map (lambda (player-key)
		   (begin
		     (lw6-cursor-init-pos game-state player-key)
		     (lw6-cursor-init-configured-mover player-key)
		     ))
		 lw6-cursor-player-keys)
	    (map (lambda (player-key)
		   (begin
		     (lw6-cursor-init-pos game-state player-key)
		     (lw6-cursor-init-map-defined-bot-mover level game-state pilot player-key)
		     ))
		 bot-keys)
	    (lw6-cursor-set-universal-if-needed)
	    (lw6-push-menu (lw6-ingame-menu))
	    (lw6-game-running)
	    (lw6-set-game-global! "command-func" lw6-command-func-local)))
	(lw6-set-game-global! "solo" #f)
	(lw6-set-game-global! "network" #f)
	))))

(define lw6-game-start-demo-step2
  (lambda ()
    (begin
      (lw6-game-start-solo-step2)
      (lw6-cursor-init-bot-mover "1" "random")
      (lw6-set-game-global! "solo" #f)
      (lw6-set-game-global! "network" #f)
      )))

(define lw6-game-start-quick-step2
  (lambda ()
    (begin
      (lw6-game-start-solo-step2)
      (lw6-set-game-global! "solo" #f)
      (lw6-set-game-global! "network" #f)
      )))

(define lw6-game-start-server-step2
  (lambda ()
    (let* (
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (node-id (lw6-get-game-global "node-id"))
	   (rounds (c-lw6ker-get-rounds game-state))
	   (seq-0 (c-lw6pil-seq-random-0))
	   (node (lw6-get-game-global "node"))
	   )
      (if node
	  (begin
	    (c-lw6p2p-node-server-start node seq-0)
	    (c-lw6pil-execute-command game-state (lw6-command-register (+ seq-0 rounds) node-id) seq-0)
	    (map (lambda (player-key)
	       (let (
		     (command (lw6-cursor-prepare-configured-player-command
			       game-state seq-0 player-key node-id))
		     )
		 (if command (c-lw6pil-execute-command game-state command seq-0))))
		 lw6-cursor-network-keys)
	    (let (
		  (pilot (c-lw6pil-build-pilot game-state
					       seq-0
					       (c-lw6sys-get-timestamp)))
		  )
	      (begin
		(lw6-set-game-global! "pilot" pilot)
		(lw6-display-param-set! "pilot" pilot)
		(map (lambda (player-key)
		       (begin
			 (lw6-cursor-init-pos game-state player-key)
			 (lw6-cursor-init-configured-mover player-key)
			 ))
		     lw6-cursor-player-keys)
		(lw6-cursor-set-universal-if-needed)
		(lw6-push-menu (lw6-ingame-menu))
		(lw6-game-running)
		(lw6-set-game-global! "command-func" lw6-command-func-network)))
	    (lw6-set-game-global! "solo" #f)
	    (lw6-set-game-global! "network" #t)
	    )
	  )
      )))

(define lw6-game-start-join-step2
  (lambda ()
    (let* (
	   (level (lw6-get-game-global "level"))
	   (game-state (lw6-get-game-global "game-state"))
	   (node-id (lw6-get-game-global "node-id"))
	   (rounds (c-lw6ker-get-rounds game-state))
	   (seq-0 (c-lw6pil-seq-random-0))
	   (node (lw6-get-game-global "node"))
	   )
      (begin
	(lw6-log-notice "join TODO")
	(lw6-set-game-global! "solo" #f)
	(lw6-set-game-global! "network" #t)
	)
      )))

