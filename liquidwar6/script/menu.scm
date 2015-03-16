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

(define lw6-init-menu
  (lambda ()
    (cond
     ((lw6-config-is-true? lw6def-quick-start)
      (begin
	(lw6-purge-menu-stack)
	(lw6-push-menu (lw6-root-menu))
	(lw6-game-start-local-step1 lw6-game-start-quick-step2)
	))
     ((lw6-config-is-true? lw6def-demo)
      (lw6-game-start-local-step1 lw6-game-start-demo-step2))
     (#t
      (lw6-push-menu (lw6-root-menu)))
     )))
