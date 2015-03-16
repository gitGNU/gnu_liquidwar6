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

(define lw6-wait-is-any-key-pressed?
  (lambda ()
    (let* (
	  (dsp (lw6-get-game-global "dsp"))
	  )
      (or
       (c-lw6sys-signal-poll-quit)
       (c-lw6gui-mouse-pop-button-left dsp)
       (c-lw6gui-mouse-pop-button-right dsp)
       (c-lw6gui-mouse-pop-button-middle dsp)
       (c-lw6gui-mouse-pop-wheel-up dsp)
       (c-lw6gui-mouse-pop-wheel-down dsp)
       (c-lw6gui-keyboard-pop-arrow-up dsp)
       (c-lw6gui-keyboard-pop-arrow-down dsp)
       (c-lw6gui-keyboard-pop-arrow-left dsp)
       (c-lw6gui-keyboard-pop-arrow-right dsp)
       (c-lw6gui-keyboard-pop-key-enter dsp)
       (c-lw6gui-keyboard-pop-key-esc dsp)
       (c-lw6gui-keyboard-pop-key-ctrl dsp)
       (c-lw6gui-keyboard-pop-key-alt dsp)
       (c-lw6gui-keyboard-pop-key-pgup dsp)
       (c-lw6gui-keyboard-pop-key-pgdown dsp)
       (c-lw6gui-joystick1-pop-pad-up dsp)
       (c-lw6gui-joystick1-pop-pad-down dsp)
       (c-lw6gui-joystick1-pop-pad-left dsp)
       (c-lw6gui-joystick1-pop-pad-right dsp)
       (c-lw6gui-joystick1-pop-button-a dsp)
       (c-lw6gui-joystick1-pop-button-b dsp)
       (c-lw6gui-joystick1-pop-button-c dsp)
       (c-lw6gui-joystick1-pop-button-d dsp)
       (c-lw6gui-joystick1-pop-button-e dsp)
       (c-lw6gui-joystick1-pop-button-f dsp)
       (c-lw6gui-joystick2-pop-pad-up dsp)
       (c-lw6gui-joystick2-pop-pad-down dsp)
       (c-lw6gui-joystick2-pop-pad-left dsp)
       (c-lw6gui-joystick2-pop-pad-right dsp)
       (c-lw6gui-joystick2-pop-button-a dsp)
       (c-lw6gui-joystick2-pop-button-b dsp)
       (c-lw6gui-joystick2-pop-button-c dsp)
       (c-lw6gui-joystick2-pop-button-d dsp)
       (c-lw6gui-joystick2-pop-button-e dsp)
       (c-lw6gui-joystick2-pop-button-f dsp)))
    ))

(define lw6-wait-until-any-key
  (lambda ()
    (let* (
	  (dsp (lw6-get-game-global "dsp"))
	  )
      (begin
	(if dsp
	    (begin
	      (while (not (lw6-wait-is-any-key-pressed?))
		     (begin
		       (lw6-display-update)
		       (lw6-node-poll)
		       (lw6-console-poll)
		       (lw6-sound-poll)
		       (lw6-net)))
	      (c-lw6gui-input-reset dsp)))))
    ))
