;Liquid War 6 is a unique multiplayer wargame.
;Copyright (C)  2005, 2006, 2007, 2008, 2009  Christian Mauduit <ufoot@ufoot.org>
;
;This program is free software; you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;
;(at your option) any later version.
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

(define lw6-wait-any-key
  (lambda ()
    (let* (
	  (dsp (lw6-get-game-global "dsp"))
	  )
      (begin
	(while (not (or
		     (c-lw6sys-signal-poll-quit)
		     (c-lw6gui-mouse-pop-button-left dsp)
		     (c-lw6gui-mouse-pop-button-right dsp)
		     (c-lw6gui-mouse-pop-wheel-up dsp)
		     (c-lw6gui-mouse-pop-wheel-down dsp)
		     (c-lw6gui-keyboard-pop-key-up dsp)
		     (c-lw6gui-keyboard-pop-key-down dsp)
		     (c-lw6gui-keyboard-pop-key-left dsp)
		     (c-lw6gui-keyboard-pop-key-right dsp)
		     (c-lw6gui-keyboard-pop-key-enter dsp)
		     (c-lw6gui-keyboard-pop-key-esc dsp)
		     (c-lw6gui-keyboard-pop-key-pgup dsp)
		     (c-lw6gui-keyboard-pop-key-pgdown dsp)
		     (c-lw6gui-joystick1-pop-button-up dsp)
		     (c-lw6gui-joystick1-pop-button-down dsp)
		     (c-lw6gui-joystick1-pop-button-left dsp)
		     (c-lw6gui-joystick1-pop-button-right dsp)
		     (c-lw6gui-joystick1-pop-button-a dsp)
		     (c-lw6gui-joystick1-pop-button-b dsp)
		     (c-lw6gui-joystick1-pop-button-c dsp)
		     (c-lw6gui-joystick1-pop-button-d dsp)
		     (c-lw6gui-joystick2-pop-button-up dsp)
		     (c-lw6gui-joystick2-pop-button-down dsp)
		     (c-lw6gui-joystick2-pop-button-left dsp)
		     (c-lw6gui-joystick2-pop-button-right dsp)
		     (c-lw6gui-joystick2-pop-button-a dsp)
		     (c-lw6gui-joystick2-pop-button-b dsp)
		     (c-lw6gui-joystick2-pop-button-c dsp)
		     (c-lw6gui-joystick2-pop-button-d dsp)))
	       (begin 
		 (lw6-display-update)
		 (lw6-node-poll)
		 (lw6-console)
		 (lw6-net)
		 ))
	(c-lw6gui-input-reset dsp)))))
