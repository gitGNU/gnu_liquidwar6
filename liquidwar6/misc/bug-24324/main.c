/*
 * Sample program to illustrate Liquid War 6 bug #24324 on Savannah
 * https://savannah.gnu.org/bugs/?24324
 * Christian Mauduit <ufoot@ufoot.org>
 */

#include <stdlib.h>
#include <stdio.h>
#include <ltdl.h>

#define LIB_FILENAME "./lib.so"
#define CALLBACK_FUNC "test"

int
main (int argc, char *argv[])
{
  int ret = 0;
  lt_dlhandle dl_handler;
  int (*test_callback) (void);

  printf ("main begin\n");
  fflush (stdout);

  if (!lt_dlinit ())
    {
      dl_handler = lt_dlopen (LIB_FILENAME);
      if (dl_handler)
	{
	  test_callback = lt_dlsym (dl_handler, CALLBACK_FUNC);
	  if (test_callback)
	    {
	      ret = (*test_callback) ();
	    }
	  else
	    {
	      printf ("can't find %s: %s\n", CALLBACK_FUNC, dlerror ());
	    }
	  lt_dlclose (dl_handler);
	}
      else
	{
	  printf ("can't open %s: %s\n", LIB_FILENAME, dlerror ());
	}
    }
  else
    {
      printf ("lt_dlinit failed\n");
    }

  printf ("main end\n");
  fflush (stdout);


  return ret;
}
