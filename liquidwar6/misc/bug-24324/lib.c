/*
 * Sample program to illustrate Liquid War 6 bug #24324 on Savannah
 * https://savannah.gnu.org/bugs/?24324
 * Christian Mauduit <ufoot@ufoot.org>
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#define WIDTH 320
#define HEIGHT 240

int
test (void)
{
  int ret = 0;
  const SDL_VideoInfo *info = NULL;

  printf ("test begin\n");
  fflush (stdout);

  if (!SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO))
    {
      printf ("SDL_Init\n");
      fflush (stdout);
      info = SDL_GetVideoInfo ();
      if (info)
	{
	  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
	  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 5);
	  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
	  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
	  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	  if (SDL_SetVideoMode
	      (WIDTH, HEIGHT, info->vfmt->BitsPerPixel,
	       SDL_OPENGL | SDL_RESIZABLE))
	    {
	      sleep (1);
	      printf ("SDL_SetVideoMode\n");
	      fflush (stdout);
	    }
	  else
	    {
	      printf ("SDL_SetVideoMode failed: %s\n", SDL_GetError ());
	      fflush (stdout);
	    }
	}
      else
	{
	  printf ("SDL_GetVideoInfo failed: %s\n", SDL_GetError ());
	  fflush (stdout);
	}

      SDL_Quit ();
    }
  else
    {
      printf ("SDL_Init failed: %s\n", SDL_GetError ());
      fflush (stdout);
    }

  printf ("test end\n");
  fflush (stdout);

  return ret;
}
