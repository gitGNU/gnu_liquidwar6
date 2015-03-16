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

(define lw6-exit-menu-quit-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Quit")
					    (_ "Yes, quit Liquid War 6")))
	  )
      (begin
	(assoc-set! menuitem "on-valid"
		    (lambda (mi)
		      (c-lw6sys-signal-send-quit)))
	menuitem
	))))

(define lw6-exit-menu-resume-item
  (lambda ()
    (let (
	  (menuitem (lw6-menu-item-template (_ "Resume")
					    (_ "No, return back to game")))
	  )
      (begin
	(assoc-set! menuitem "on-valid" (lambda (mi) (lw6-pop-menu (lw6-current-menu))))
	menuitem
	))))

(define lw6-exit-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "Quit Liquid War 6?")
				   (_ "Procrastinating on games is mother of all vices")
				   #f))
	  )
      (begin
	(c-lw6gui-menu-enable-esc (assoc-ref menu "smob") #f)
	(lw6-append-menuitem! menu (lw6-exit-menu-quit-item))
	(lw6-append-menuitem! menu (lw6-exit-menu-resume-item))
	menu
	))))
