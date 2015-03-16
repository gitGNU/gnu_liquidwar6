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

(define lw6-system-options-menu-display-fps-item-label
  (lambda ()
    (if (lw6-config-is-true? lw6def-display-fps)
	(_ "Display FPS")
	(_ "FPS hidden"))))

(define lw6-system-options-menu-display-fps-item-toggle
  (lambda (menuitem)
    (begin
      (lw6-config-set-boolean! lw6def-display-fps (not (lw6-config-is-true? lw6def-display-fps)))
      (lw6-display-param-set! lw6def-display-fps (lw6-config-is-true? lw6def-display-fps))
      (lw6-display-update)
      )))

(define lw6-system-options-menu-display-fps-item
  (lambda ()
    (lw6-menu-item-template-switch
     lw6-system-options-menu-display-fps-item-label
     lw6-system-options-menu-display-fps-item-toggle
     lw6-system-options-menu-display-fps-item-toggle
     (_ "Use this to check how fast your display is"))))

(define lw6-system-options-menu-display-mps-item-label
  (lambda ()
    (if (lw6-config-is-true? lw6def-display-mps)
	(_ "Display MPS")
	(_ "MPS hidden"))))

(define lw6-system-options-menu-display-mps-item-toggle
  (lambda (menuitem)
    (begin
      (lw6-config-set-boolean! lw6def-display-mps (not (lw6-config-is-true? lw6def-display-mps)))
      (lw6-display-param-set! lw6def-display-mps (lw6-config-is-true? lw6def-display-mps))
      (lw6-display-update)
      )))

(define lw6-system-options-menu-display-mps-item
  (lambda ()
    (lw6-menu-item-template-switch
     lw6-system-options-menu-display-mps-item-label
     lw6-system-options-menu-display-mps-item-toggle
     lw6-system-options-menu-display-mps-item-toggle
     (_ "MPS stands for Moves Per Second, displaying this might reveal some slowing down in gameplay"))))

(define lw6-system-options-menu-display-meta-item-label
  (lambda ()
    (if (lw6-config-is-true? lw6def-display-meta)
	(_ "Verbose display")
	(_ "Clean display"))))

(define lw6-system-options-menu-display-meta-item-toggle
  (lambda (menuitem)
    (begin
      (lw6-config-set-boolean! lw6def-display-meta (not (lw6-config-is-true? lw6def-display-meta)))
      (lw6-display-param-set! lw6def-display-meta (lw6-config-is-true? lw6def-display-meta))
      (lw6-display-update)
      )))

(define lw6-system-options-menu-display-meta-item
  (lambda ()
    (lw6-menu-item-template-switch
     lw6-system-options-menu-display-meta-item-label
     lw6-system-options-menu-display-meta-item-toggle
     lw6-system-options-menu-display-meta-item-toggle
     (_ "Do you want breadcrumbs, help and tooltips to be displayed?"))))

(define lw6-system-options-menu-display-console-item-label
  (lambda ()
    (if (lw6-config-is-true? lw6def-display-console)
	(_ "Console enabled")
	(_ "Console disabled"))))

(define lw6-system-options-menu-display-console-item-toggle
  (lambda (menuitem)
    (begin
      (if (lw6-config-is-true? lw6def-display-console)
	  (begin (lw6-config-set-boolean! lw6def-display-console #f)
		 (c-lw6cns-quit))
	  (begin (lw6-config-set-boolean! lw6def-display-console #t)
		 (c-lw6cns-init))
	  ))))

(define lw6-system-options-menu-display-console-item
  (lambda ()
    (lw6-menu-item-template-switch
     lw6-system-options-menu-display-console-item-label
     lw6-system-options-menu-display-console-item-toggle
     lw6-system-options-menu-display-console-item-toggle
     (_ "Console is a Guile interpreter which will allow you to type any command interactively, use at your own risk"))))

(define lw6-system-options-menu
  (lambda()
    (let (
	  (menu (lw6-menu-template (_ "System options")
				   (_ "All those weird options and tweaks you can live without")
				   #f))
	  )
      (begin
	(lw6-append-menuitem! menu (lw6-system-options-menu-display-fps-item))
	(lw6-append-menuitem! menu (lw6-system-options-menu-display-mps-item))
	(lw6-append-menuitem! menu (lw6-system-options-menu-display-meta-item))
	(if (c-lw6cns-console-support)
	    (lw6-append-menuitem! menu (lw6-system-options-menu-display-console-item)))
	menu
	))))
