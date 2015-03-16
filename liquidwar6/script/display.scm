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

(define lw6-display-param-init
  (lambda ()
    (let (
	  (display-param (make-hash-table 13))
	  )
      (begin
	(map (lambda (key) (hash-set! display-param key (lw6-config-is-true? key)))
	     (list lw6def-display-splash
		   lw6def-display-background
		   lw6def-display-preview
		   lw6def-display-map
		   lw6def-display-fighters
		   lw6def-display-cursors
		   lw6def-display-hud
		   lw6def-display-score
		   lw6def-display-menu
		   lw6def-display-meta
		   lw6def-display-progress
		   lw6def-display-log
		   lw6def-display-fps
		   lw6def-display-mps
		   lw6def-display-url
		   lw6def-display-mouse
		   lw6def-display-debug-zones
		   lw6def-display-debug-gradient
		   lw6def-capture
		   lw6def-gfx-debug
		   lw6def-fullscreen))
	(map (lambda (key) (hash-set! display-param key (lw6-config-get-number key)))
	     (list lw6def-target-fps
		   lw6def-gfx-cpu-usage
		   lw6def-debug-team-id
		   lw6def-debug-layer-id
		   lw6def-repeat-delay
		   lw6def-repeat-interval
		   lw6def-double-click-delay
		   lw6def-auto-release-delay
		   lw6def-log-timeout
		   lw6def-width
		   lw6def-height
		   lw6def-dirty-read))
	;(hash-set! display-param "main-cursor-id" #f)
	;(hash-set! display-param "main-cursor-x" 0)
	;(hash-set! display-param "main-cursor-y" 0)
	(hash-set! display-param "look" (lw6-get-game-global "look"))
	(hash-set! display-param "menu" #f)
	(hash-set! display-param "level" #f)
	(hash-set! display-param "game-struct" #f)
	(hash-set! display-param "game-state" #f)
	(hash-set! display-param "pilot" #f)
	display-param
	))))

(define lw6-display-update
  (lambda ()
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (game-state (lw6-get-game-global "game-state"))
	   (pilot (lw6-get-game-global "pilot"))
	   (node-id (lw6-get-game-global "node-id"))
	   (display-param (lw6-get-game-global "display-param"))
	   (menu (lw6-current-menu))
	   (menu-smob (assoc-ref menu "smob"))
	   (look (hash-ref display-param "look"))
	   )
      (begin
	(lw6-menu-action menu "on-display")
	(hash-set! display-param "menu" menu-smob)
	(if look
	    (c-lw6gui-look-set look lw6def-gfx-quality
			       (lw6-config-get-string lw6def-gfx-quality)))
	(if (and game-state pilot node-id)
	    (let* (
		   (seq (c-lw6pil-get-max-seq pilot))
		   (commands (lw6-command-all-local seq node-id))
		   (main-cursor (lw6-get-cursor "1"))
		   (main-cursor-id (if main-cursor (hash-ref main-cursor "id") "0"))
		   ;(main-cursor-x (if main-cursor (hash-ref main-cursor "x") 0))
		   ;(main-cursor-y (if main-cursor (hash-ref main-cursor "y") 0))
		   ;(main-cursor-mouse-controlled (if main-cursor (hash-ref main-cursor "mouse-controlled") 0))
		   )
	      (begin
		;;(c-lw6pil-sync-from-draft game-state pilot (lw6-config-get-number lw6def-dirty-read))
		(map (lambda (command) (c-lw6pil-local-command pilot command))
		     commands)
		(map (lambda (i) (let* (
					(cursor (lw6-get-cursor i))
					(cursor-id (if cursor (hash-ref cursor "id") #f))
					)
				   (if (and cursor cursor-id)
				       (c-lw6pil-local-cursors-set-mouse-controlled
					pilot cursor-id
					(hash-ref cursor "mouse-controlled")))))
		     lw6-cursor-keys)
		(c-lw6pil-local-cursors-set-main pilot main-cursor-id)
		;(hash-set! display-param "main-cursor-id" main-cursor-id)
		;(hash-set! display-param "main-cursor-x" (inexact->exact main-cursor-x))
		;(hash-set! display-param "main-cursor-y" (inexact->exact main-cursor-y))
		;(hash-set! display-param "main-cursor-mouse-controlled" main-cursor-mouse-controlled)
		)))
	(c-lw6dsp-update dsp display-param)
	))))

(define lw6-display-param-set!
  (lambda (key value)
    (let (
	  (display-param (lw6-get-game-global "display-param"))
	  )
      (hash-set! display-param key value)
      )))

(define lw6-display-param-get
  (lambda (key)
    (let (
	  (display-param (lw6-get-game-global "display-param"))
	  )
      (hash-ref display-param key)
      )))

(define lw6-display-param-set-boolean-from-config!
  (lambda (key)
    (lw6-display-param-set! key (lw6-config-is-true? key))))

(define lw6-display-splash-setup
  (lambda ()
    (begin
      (lw6-display-param-set-boolean-from-config! lw6def-display-splash)
      (lw6-display-param-set! lw6def-display-preview #f)
      (lw6-display-param-set! lw6def-display-map #f)
      (lw6-display-param-set! lw6def-display-fighters #f)
      (lw6-display-param-set! lw6def-display-cursors #f)
      (lw6-display-param-set! lw6def-display-hud #f)
      (lw6-display-param-set! lw6def-display-score #f)
      (lw6-display-param-set! lw6def-display-menu #f)
      (lw6-display-param-set! lw6def-display-meta #f)
      )))

(define lw6-display-idle-setup
  (lambda ()
    (begin
      (lw6-display-param-set! lw6def-display-splash #f)
      (lw6-display-param-set! lw6def-display-preview #f)
      (lw6-display-param-set! lw6def-display-map #f)
      (lw6-display-param-set! lw6def-display-fighters #f)
      (lw6-display-param-set! lw6def-display-cursors #f)
      (lw6-display-param-set! lw6def-display-hud #f)
      (lw6-display-param-set! lw6def-display-score #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-menu)
      (lw6-display-param-set-boolean-from-config! lw6def-display-meta)
      )))

(define lw6-display-preview-setup
  (lambda ()
    (begin
      (lw6-display-param-set! lw6def-display-splash #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-preview)
      (lw6-display-param-set-boolean-from-config! lw6def-display-map)
      (lw6-display-param-set-boolean-from-config! lw6def-display-fighters)
      (lw6-display-param-set! lw6def-display-cursors #f)
      (lw6-display-param-set! lw6def-display-hud #f)
      (lw6-display-param-set! lw6def-display-score #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-menu)
      (lw6-display-param-set-boolean-from-config! lw6def-display-meta)
      )))

(define lw6-display-running-setup
  (lambda ()
    (begin
      (lw6-display-param-set! lw6def-display-splash #f)
      (lw6-display-param-set! lw6def-display-preview #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-map)
      (lw6-display-param-set-boolean-from-config! lw6def-display-fighters)
      (lw6-display-param-set-boolean-from-config! lw6def-display-cursors)
      (lw6-display-param-set-boolean-from-config! lw6def-display-hud)
      (lw6-display-param-set! lw6def-display-score #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-menu)
      (lw6-display-param-set! lw6def-display-meta #f)
      )))

(define lw6-display-score-setup
  (lambda ()
    (begin
      (lw6-display-param-set! lw6def-display-splash #f)
      (lw6-display-param-set! lw6def-display-preview #f)
      (lw6-display-param-set! lw6def-display-map #f)
      (lw6-display-param-set! lw6def-display-fighters #f)
      (lw6-display-param-set! lw6def-display-cursors #f)
      (lw6-display-param-set! lw6def-display-hud #f)
      (lw6-display-param-set-boolean-from-config! lw6def-display-score)
      (lw6-display-param-set-boolean-from-config! lw6def-display-menu)
      (lw6-display-param-set-boolean-from-config! lw6def-display-meta)
      )))

(define lw6-display-no-score-setup
  (lambda ()
    (begin
      (lw6-display-param-set! lw6def-display-score #f)
      )))

