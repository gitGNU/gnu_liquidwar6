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

(define lw6-load
  (lambda ()
    (begin
      (load "def.scm")
      (load "backend.scm")
      (load "bench.scm")
      (load "clear.scm")
      (load "command.scm")
      (load "config.scm")
      (load "console.scm")
      (load "control.scm")
      (load "cursor.scm")
      (load "db.scm")
      (load "demo.scm")
      (load "display.scm")
      (load "game.scm")
      (load "help.scm")
      (load "io.scm")
      (load "loader.scm")
      (load "local-game.scm")
      (load "log.scm")
      (load "loop.scm")
      (load "map.scm")
      (load "menu-connecting.scm")
      (load "menu-engine.scm")
      (load "menu.scm")
      (load "menu-exit.scm")
      (load "menu-ingame.scm")
      (load "menu-join.scm")
      (load "menu-map.scm")
      (load "menu-map-browse.scm")
      (load "menu-options.scm")
      (load "menu-options-time.scm")
      (load "menu-options-rules.scm")
      (load "menu-options-audio.scm")
      (load "menu-options-video.scm")
      (load "menu-options-system.scm")
      (load "menu-play.scm")
      (load "menu-player-detail.scm")
      (load "menu-players.scm")
      (load "menu-root.scm")
      (load "menu-score.scm")
      (load "menu-wait.scm")
      (load "mover.scm")
      (load "music.scm")
      (load "net.scm")
      (load "node.scm")
      (load "preview.scm")
      (load "quick-start.scm")
      (load "score.scm")
      (load "screenshot.scm")
      (load "server.scm")
      (load "sound.scm")
      (load "splash.scm")
      (load "speed.scm")
      (load "style.scm")
      (load "test-utils.scm")
      (load "wait.scm")
      (load "zoom.scm")
      )))
