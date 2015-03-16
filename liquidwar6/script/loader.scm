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

(define lw6-loader-purge
  (lambda ()
    (begin
      (lw6-game-param-update)
      (map (lambda (key)
	     (lw6-set-game-global! key #f))
	   (list "loaded" "level" "game-state" "pilot")))))

(define lw6-loader-local-bench-value
  (lambda ()
    (max 1 (round (min
		   (+ (lw6-config-get-number lw6def-bench-value)
		      (lw6-config-get-number lw6def-local-bench-delta))
		   (lw6-config-get-number lw6def-max-local-bench-value))))))

(define lw6-loader-network-bench-value
  (lambda ()
    (max 1 (round (min
		   (+ (lw6-config-get-number lw6def-bench-value)
		      (lw6-config-get-number lw6def-network-bench-delta))
		   (lw6-config-get-number lw6def-max-network-bench-value))))))

(define lw6-loader-bench-value
  (lambda (network)
    (if network 
	(lw6-loader-network-bench-value)
	(lw6-loader-local-bench-value))))

(define lw6-loader-push-ldr
  (lambda (relative-path network)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (loader (lw6-get-game-global "loader"))
	   (map-path (lw6-get-game-global lw6def-map-path))
	   (default-param (lw6-get-game-global "default-param"))
	   (forced-param (lw6-get-game-global "forced-param"))
	   (video-mode (c-lw6dsp-get-video-mode dsp))
	   (width (assoc-ref video-mode "width"))
	   (height (assoc-ref video-mode "height"))
	   (bench-value (lw6-loader-bench-value network))
	   (magic-number (lw6-config-get-number lw6def-magic-number))
	   )
      (begin
	(lw6-set-game-global! "loaded" (assoc-set! (list) "bench-value" bench-value))
	(c-lw6tsk-loader-push-ldr loader
				  map-path
				  relative-path
				  default-param
				  forced-param
				  width
				  height
				  bench-value
				  magic-number)))))

(define lw6-loader-push-ldr-if-needed
  (lambda (relative-path network)
    (if (and (equal? relative-path (lw6-config-get-string lw6def-chosen-map))
	     (assoc-ref (lw6-get-game-global "loaded") "level")
	     (assoc-ref (lw6-get-game-global "loaded") "bench-value")
	     (equal? (assoc-ref (lw6-get-game-global "loaded") "bench-value")
		     (lw6-loader-bench-value network))
	     )
	#t
	(lw6-loader-push-ldr relative-path network))))

(define lw6-loader-push-gen
  (lambda (seed network)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (loader (lw6-get-game-global "loader"))
	   (video-mode (c-lw6dsp-get-video-mode dsp))
	   (width (assoc-ref video-mode "width"))
	   (height (assoc-ref video-mode "height"))
	   (bench-value (lw6-loader-bench-value network))
	   (magic-number (lw6-config-get-number lw6def-magic-number))
	   )
      (begin
	(lw6-set-game-global! "loaded" (assoc-set! (list) "bench-value" bench-value))
	(c-lw6tsk-loader-push-gen loader
				  seed
				  width
				  height
				  bench-value
				  magic-number)))))

(define lw6-loaded-func-idle
  (lambda (loaded)
    #t))

(define lw6-loaded-func-preview
  (lambda (loaded)
    (begin
      (lw6-set-game-global! "look" (c-lw6map-get-look (assoc-ref loaded "level")))
      (lw6-set-game-global! "loaded" loaded)
      (map (lambda (key) (lw6-display-param-set! key (assoc-ref loaded key)))
	   (list "level" "game-struct" "game-state"))
      (lw6-display-param-set! "look" (lw6-get-game-global "look"))
      (lw6-game-put-local-teams))))

(define lw6-loaded-func-local-game
  (lambda (step2-func)
    (lambda (loaded)
      (begin
	(lw6-set-game-global! "look" (c-lw6map-get-look (assoc-ref loaded "level")))
	(lw6-set-game-global! "loaded" loaded)
	(map (lambda (key) (lw6-display-param-set! key (assoc-ref loaded key)))
	     (list "level" "game-struct" "game-state"))
	(lw6-display-param-set! "look" (lw6-get-game-global "look"))
	(lw6-pop-menu (lw6-current-menu))
	(lw6-game-start-local-step1 step2-func)))))

(define lw6-loaded-func-network-game
  (lambda (step2-func)
    (lambda (loaded)
      (begin
	(lw6-set-game-global! "look" (c-lw6map-get-look (assoc-ref loaded "level")))
	(lw6-set-game-global! "loaded" loaded)
	(map (lambda (key) (lw6-display-param-set! key (assoc-ref loaded key)))
	     (list "level" "game-struct" "game-state"))
	(lw6-display-param-set! "look" (lw6-get-game-global "look"))
	(lw6-pop-menu (lw6-current-menu))
	(lw6-game-start-network-step1 step2-func)))))

(define lw6-loader
  (lambda ()
    (let (
	  (loader (lw6-get-game-global "loader"))
	  )
      (if loader
	  (let* (
		 (loaded (c-lw6tsk-loader-pop loader))
		 (loaded-func (lw6-get-game-global "loaded-func"))
		 )
	    (if (and loaded loaded-func)
		(loaded-func loaded)))))))
