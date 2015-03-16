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

(define lw6-control-list
  (list
   (_ "Mouse")
   (_ "Keyboard")
   (_ "Joystick 1")
   (_ "Joystick 2")
   (_ "Custom")
   ))

(define lw6-control-key-to-label-format
  (lambda (key)
    (cond
     ((equal? key "mouse")
      (_ "~a (mouse)"))
     ((equal? key "keyboard")
      (_ "~a (keyb)"))
     ((equal? key "joystick1")
      (_ "~a (joy1)"))
     ((equal? key "joystick2")
      (_ "~a (joy2)"))
     ((equal? key "custom")
      (_ "~a (custom)"))
     (#t
      (_ "~a (mouse)"))
     )))

(define lw6-control-key-to-index
  (lambda (key)
    (cond
     ((equal? key "mouse")
      0)
     ((equal? key "keyboard")
      1)
     ((equal? key "joystick1")
      2)
     ((equal? key "joystick2")
      3)
     ((equal? key "custom")
      4)
     (#t 0)
     )))

(define lw6-control-index-to-key
  (lambda (index)
    (list-ref
     (list
      "mouse"
      "keyboard"
      "joystick1"
      "joystick2"
      "custom")
     index)))

(define lw6-control-index-to-tooltip
  (lambda (index)
    (list-ref
     (list
      (format #f (_ "Move cursor: pointer~%Menu: left button~%Primary fire: right button~%Alternate fire: middle button~%Zoom: wheel"))
      (format #f (_ "Move cursor: arrows~%Menu: ENTER and ESC~%Primary fire: CTRL key~%Alternate fire: ALT key~%Zoom: PGUP and PGDOWN"))
      (format #f (_ "Move cursor: pad/stick~%Menu: A and B buttons~%Primary fire: C button~%Alternate fire: D button~%Zoom: E and F buttons"))
      (format #f (_ "Move cursor: pad/stick~%Menu: A and B buttons~%Primary fire: C button~%Alternate fire: D button~%Zoom: E and F buttons"))
      (format #f (_ "Not working yet"))
      )
     index)))



