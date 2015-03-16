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

(define lw6-config-set-string!
  (lambda (key value)
    (c-lw6cfg-set-option key value)
    )
  )

(define lw6-config-get-string
  (lambda (key)
    (c-lw6cfg-get-option key)
    )
  )

(define lw6-config-set-number!
  (lambda (key value)
    (begin
      (c-lw6cfg-set-option key (number->string value))
      )
    )
  )

(define lw6-config-get-number
  (lambda (key)
    (string->number (c-lw6cfg-get-option key))
    )
  )

(define lw6-config-set-boolean!
  (lambda (key value)
    (c-lw6cfg-set-option key (if value "true" "false"))
    )
  )

(define lw6-config-is-true?
  (lambda (key)
    (equal? (c-lw6cfg-get-option key) "true")
    )
  )

(define lw6-config-change-boolean!
  (lambda (key)
    (c-lw6cfg-set-option key (if (lw6-config-is-true? key) "false" "true"))
    )
  )

(define lw6-config-defaults
  (lambda ()
    (let (
	  (default-gfx-backend (lw6-get-default-gfx-backend))
	  (default-snd-backend (lw6-get-default-snd-backend))
	  )
      (begin
	(c-lw6cfg-defaults)
	(if default-gfx-backend
	    (lw6-config-set-string! lw6def-gfx-backend default-gfx-backend))
	(if default-snd-backend
	    (lw6-config-set-string! lw6def-snd-backend default-snd-backend))
	)
      )
    )
  )

(define lw6-config-fix
  (lambda ()
    (begin
      ;;      (lw6-config-set-string! lw6def-public-url
      ;;			      (c-lw6sys-url-canonize
      ;;			       (lw6-config-get-string
      ;;				lw6def-public-url)))
      #f
      )
    )
  )

(define lw6-load-config
  (lambda ()
    (begin
      (lw6-config-defaults)
      (c-lw6cfg-load (c-lw6sys-get-config-file))
      (c-lw6cfg-save (c-lw6sys-get-config-file))
      (c-lw6cfg-load (c-lw6sys-get-config-file))
      (lw6-config-fix)
      )
    )
  )

(define lw6-config-set-video!
  (lambda (width height fullscreen)
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (requested-res (list (cons lw6def-width width)
				(cons lw6def-height height)
				(cons lw6def-fullscreen fullscreen)))
	   (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	   (low-res (assoc-ref fullscreen-modes "low"))
	   (standard-res (assoc-ref fullscreen-modes "standard"))
	   (high-res (assoc-ref fullscreen-modes "high"))
	   )
      (begin
	(lw6-display-param-set! lw6def-width width)
	(lw6-display-param-set! lw6def-height height)
	(lw6-display-param-set! lw6def-fullscreen fullscreen)
	(lw6-display-update)
	(let (
	      (frames (c-lw6dsp-get-nb-frames dsp))
	      )
	  (while (< (c-lw6dsp-get-nb-frames dsp) (+ 1 frames))
		 (c-lw6sys-idle)))
	(let (
	      (obtained-res (c-lw6dsp-get-video-mode dsp))
	      )
	  (begin
	    (lw6-config-set-number! lw6def-width (assoc-ref obtained-res lw6def-width))
	    (lw6-config-set-number! lw6def-height (assoc-ref obtained-res lw6def-height))
	    (lw6-config-set-boolean! lw6def-fullscreen (assoc-ref obtained-res lw6def-fullscreen))
	    (lw6-display-param-set! lw6def-width (assoc-ref obtained-res lw6def-width))
	    (lw6-display-param-set! lw6def-height (assoc-ref obtained-res lw6def-height))
	    (lw6-display-param-set! lw6def-fullscreen (assoc-ref obtained-res lw6def-fullscreen))
	    ))))))

(define lw6-config-update-video
  (lambda ()
    (let* (
	   (dsp (lw6-get-game-global "dsp"))
	   (res (c-lw6dsp-get-video-mode dsp))
	   (fullscreen-modes (c-lw6dsp-get-fullscreen-modes dsp))
	   (low-res (assoc-ref fullscreen-modes "low"))
	   (high-res (assoc-ref fullscreen-modes "high"))
	   )
      (begin
	(lw6-config-set-number! lw6def-width (assoc-ref res lw6def-width))
	(lw6-config-set-number! lw6def-height (assoc-ref res lw6def-height))
	(lw6-config-set-boolean! lw6def-fullscreen (assoc-ref res lw6def-fullscreen))
	))))

(define lw6-save-config
  (lambda ()
    (c-lw6cfg-save (c-lw6sys-get-config-file))
    ))

(define lw6-config-subset
  (lambda (key-list)
    (let* ((target (list)))
	  (begin
	    (map (lambda (key) (set! target (assoc-set! target key (lw6-config-get-string key)))) key-list)
	    target))))

(define lw6-default-map
  (lambda ()
    (lw6-config-subset (list lw6def-use-texture
			     lw6def-use-rules-xml
			     lw6def-use-hints-xml
			     lw6def-use-style-xml
			     lw6def-use-teams-xml))))

(define lw6-default-map-rules
  (lambda ()
    (lw6-config-subset (c-lw6hlp-list-map-rules))))

(define lw6-default-map-hints
  (lambda ()
    (lw6-config-subset (c-lw6hlp-list-map-hints))))

(define lw6-default-map-style
  (lambda ()
    (lw6-config-subset (c-lw6hlp-list-map-style))))

(define lw6-default-map-teams
  (lambda ()
    (lw6-config-subset (c-lw6hlp-list-map-teams))))

(define lw6-default-param
  (lambda ()
    (append (lw6-default-map) (lw6-default-map-rules) (lw6-default-map-hints) (lw6-default-map-style) (lw6-default-map-teams))))

(define lw6-forced-param
  (lambda ()
    (lw6-config-subset (string-split (lw6-config-get-string lw6def-force) #\,))))

(define lw6-default-look
  (lambda ()
    (let (
	  (look (c-lw6gui-default-look))
	  )
	 (begin
	   (map (lambda (key)
		  (c-lw6gui-look-set look key (lw6-config-get-string key)))
		(c-lw6hlp-list-map-style))
	   look))))