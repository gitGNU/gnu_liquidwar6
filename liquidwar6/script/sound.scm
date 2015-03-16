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

(define lw6-play-fx-by-id
  (lambda (id)
    (let (
	  (snd (lw6-get-game-global "snd"))
	  )
      (if snd
	  (c-lw6snd-play-fx snd id)))))

(define lw6-play-fx-splash
  (lambda ()
    (lw6-play-fx-by-id 0)))

(define lw6-play-fx-beep-valid
  (lambda ()
    (lw6-play-fx-by-id 1)))

(define lw6-play-fx-beep-select
  (lambda ()
    (lw6-play-fx-by-id 2)))

(define lw6-play-fx-beep-no
  (lambda ()
    (lw6-play-fx-by-id 3)))

(define lw6-play-fx-start
  (lambda ()
    (lw6-play-fx-by-id 4)))

(define lw6-sound-poll
  (lambda ()
    (let (
	  (snd (lw6-get-game-global "snd"))
	  )
      (if snd
	  (c-lw6snd-poll snd)))))

