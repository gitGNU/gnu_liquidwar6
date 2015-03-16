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

(define lw6-splash
  (lambda ()
    (let (
	  (snd (lw6-get-game-global "snd"))
	  (music-path (lw6-config-get-string lw6def-music-path))
	  (music-filter (lw6-config-get-string lw6def-music-filter))
	  )
      (if (and
	   (lw6-config-is-true? lw6def-display-splash)
	   (not (lw6-config-is-true? lw6def-quick-start)))
	  (begin
	    (lw6-play-fx-splash)
	    (lw6-wait-until-any-key)
	    (lw6-play-fx-beep-valid))))))