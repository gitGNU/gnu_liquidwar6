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

(define lw6-main 
  (lambda () 
    (begin
      (load "def.scm")
      (load "backend.scm")
      (load "bench.scm")
      (load "clear.scm")
      (load "command.scm")
      (load "config.scm")
      (load "console.scm")
      (load "control.scm")
      (load "cursor.scm")
      (load "db.scm")
      (load "demo.scm")
      (load "display.scm")
      (load "game.scm")
      (load "help.scm")
      (load "io.scm")
      (load "loader.scm")
      (load "local-game.scm")
      (load "log.scm")
      (load "loop.scm")
      (load "map.scm")
      (load "menu-engine.scm")
      (load "menu.scm")
      (load "menu-exit.scm")
      (load "menu-ingame.scm")
      (load "menu-map.scm")
      (load "menu-options.scm")
      (load "menu-options-time.scm")
      (load "menu-options-rules.scm")
      (load "menu-options-audio.scm")
      (load "menu-options-video.scm")
      (load "menu-options-system.scm")
      (load "menu-play.scm")
      (load "menu-player-detail.scm")
      (load "menu-players.scm")
      (load "menu-root.scm")
      (load "menu-wait.scm")
      (load "mover.scm")
      (load "music.scm")
      (load "net.scm")
      (load "node.scm")
      (load "preview.scm")
      (load "quick-start.scm")
      (load "score.scm")
      (load "server.scm")
      (load "sound.scm")
      (load "splash.scm")
      (load "speed.scm")
      (load "style.scm")
      (load "wait.scm")
      (load "zoom.scm")

      ; Show the binary pedigree
      (lw6-log-notice (format #f (_ "running \"~a\"") 
			      (c-lw6sys-get-script-file)))
      (lw6-log-notice (format #f (_ "log file \"~a\"") 
			      (c-lw6sys-get-log-file)))
      (if (or (c-lw6sys-build-is-mac-os-x) (c-lw6sys-build-is-ms-windows))
	  (lw6-log-info (format #f 
				(_ "environment \"~a\" is \"~a\"") 
				"GUILE_LOAD_PATH" 
				(c-lw6sys-getenv "GUILE_LOAD_PATH"))))
      (if (c-lw6sys-build-is-mac-os-x)
	  (lw6-log-info (format #f 
				(_ "environment \"~a\" is \"~a\"") 
				"DYLD_FALLBACK_LIBRARY_PATH" 
				(c-lw6sys-getenv "DYLD_FALLBACK_LIBRARY_PATH"))))
      (c-lw6cfg-init)
      (lw6-load-config)
      (if (not (equal? 
		(lw6-config-get-number lw6def-bin-id)
		(c-lw6sys-build-get-bin-id)))
	  (if (lw6-config-is-true? lw6def-reset-config-on-upgrade)
	      (begin
		(lw6-log-info (format #f (_ "upgrading to version ~a")
				      (c-lw6sys-build-get-version)))
		(lw6-config-defaults)
		(c-lw6cfg-save (c-lw6sys-get-config-file))
		(lw6-load-config)
		(lw6-db-reset)
		)))
      (c-lw6sys-set-memory-bazooka-size (lw6-config-get-number lw6def-memory-bazooka-size))
      (c-lw6sys-set-memory-bazooka-eraser (lw6-config-is-true? lw6def-memory-bazooka-eraser))
      (c-lw6sys-dump-clear)
      (c-lw6ldr-print-examples)
      (c-lw6net-init (lw6-config-is-true? lw6def-net-log))
      (c-lw6sys-signal-custom)
      (lw6-init-game-globals)
      (if (not (lw6-config-is-true? lw6def-server))
	  (let (
		(gfx-backend (lw6-check-gfx-backend (lw6-config-get-string lw6def-gfx-backend)))
		(snd-backend (lw6-check-snd-backend (lw6-config-get-string lw6def-snd-backend)))
		)
	    (if gfx-backend
		(let (
		      (dsp (c-lw6dsp-new
			    gfx-backend
			    (lw6-get-game-global "display-param")))
		      )
		  (if dsp
		      (begin
			(lw6-set-game-global! "dsp" dsp)
			(lw6-config-set-string! lw6def-gfx-backend gfx-backend)		    
			(lw6-config-update-video)
			(if snd-backend
			    (let (
				  (snd (c-lw6snd-new 
					snd-backend
					(lw6-config-get-number lw6def-fx-volume) 
					(lw6-config-get-number lw6def-water-volume) 
					(lw6-config-get-number lw6def-music-volume)))
				  )
			      (if snd
				  (lw6-set-game-global! "snd" snd)
				  (lw6-config-set-string! lw6def-snd-backend snd-backend)
				  )))
			;; (lw6-init-game-globals)
			(lw6-console-init)
			(if (not (lw6-bench-need-update #f))
			    (lw6-node-start)) ;; start it now if bench OK
			(lw6-init-menu)
			(lw6-splash)
			(lw6-game-idle)
			(lw6-music-ambiance)
			(lw6-bench #f)
			(lw6-node-start) ;; node must be start *after* bench
			(lw6-game-loop)
			(c-lw6dsp-release dsp) ;; do it before node-stop
			(c-lw6cns-quit)
			(lw6-node-stop)
			(c-lw6-release) ;; global safety release
			(lw6-clear)
			)
		      (begin
			(lw6-log-error (_ "no graphical backend found, game can only run in server mode"))
			(lw6-server)
			)
		      ))))
	  (begin
	    (lw6-log-info (_ "running in server mode"))
	    (lw6-server)
	    ))
      (c-lw6net-quit)
      (lw6-config-set-number! lw6def-bin-id (c-lw6sys-build-get-bin-id))
      (lw6-save-config)
      (c-lw6cfg-quit)
      )))


