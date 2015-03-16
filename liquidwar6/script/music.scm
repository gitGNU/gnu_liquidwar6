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

(define lw6-music-file-or-random
  (lambda (music-file music-filter music-exclude music-dir music-path)
    (let (
	  (snd (lw6-get-game-global "snd"))
	  )
      (if snd
	  (if (and music-dir music-path music-file (c-lw6snd-is-music-file snd music-dir music-path music-file))
	      (c-lw6snd-play-music-file snd music-dir music-path music-file)
	      (c-lw6snd-play-music-random snd music-path music-filter music-exclude))))))

(define lw6-music-ambiance
  (lambda ()
    (let (
	  (music-file (lw6-config-get-string lw6def-ambiance-file))
	  (music-filter (lw6-config-get-string lw6def-ambiance-filter))
	  (music-exclude (lw6-config-get-string lw6def-ambiance-exclude))
	  ;; Here we get map-dir but there's normally nothing in map dir
	  ;; at this level for it's at the root. Very well, we indeed to not
	  ;; want any map specific music here.
	  (music-dir (c-lw6sys-get-map-dir))
	  (music-path (c-lw6cfg-unified-get-music-path))
	  )
      (lw6-music-file-or-random music-file music-filter music-exclude music-dir music-path))))

(define lw6-music-game
  (lambda (level)
    (let* (
	   (look (lw6-get-game-global "look"))
	   (music-file (c-lw6gui-look-get look lw6def-music-file))
	   (music-filter (c-lw6gui-look-get look lw6def-music-filter))
	   (music-exclude (c-lw6gui-look-get look lw6def-music-exclude))
	   (music-dir (c-lw6map-get-music-dir level))
	   (music-path (c-lw6cfg-unified-get-music-path))
	   )
      (lw6-music-file-or-random music-file music-filter music-exclude music-dir music-path))))