/*
 * File: test_for_disabled_adjtimex.c, author: John Sauter, 
 * date: December 14, 2019
 * Make sure the Linux adjtimex function works.
 */

/*
 * Copyright © 2019 by John Sauter <John_Sauter@systemeyescomputerstore.com>

 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *  The author's contact information is as follows:
 *    John Sauter
 *    System Eyes Computer Store
 *    20A Northwest Blvd.  Ste 345
 *    Nashua, NH  03063-4066
 *    telephone: (603) 424-1188
 *    e-mail: John_Sauter@systemeyescomputerstore.com
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>		/* getopt_long() */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "time_subroutines.h"

static int debug_level = 0;

static int
test_for_disabled_adjtimex ()
{
  int adjtimex_is_disabled;

  adjtimex_is_disabled = time_test_for_disabled_adjtimex();

  if (adjtimex_is_disabled != 0)
    {
      /* Some high security environments disable the adjtimex function,
       * even when, as here, it is just fetching information.  
       * The adjtimex function is the only way to determine that
       * there is a leap second in progress, so complain if it
       * doesn't work.  */
      printf ("The current time will not be correct during a leap second\n"
	      "because the Linux adjtimex function is not working.\n");
      exit (EXIT_FAILURE);
    }
  
  return (0);
}

/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options]\n\n"
	       "Test for disabled adjtimex.\n"
	       " Version 2.0 2019-12-14\n"
	       "Options:\n"
	       "-h | --help          Print this message\n"
	       "-D | --debug-level   Amount of debugging output, default 0\n"
	       "", argv[0]);
    }
}

/* Options */
static const char short_options[] = "ho:r:d:D:";

static const struct option long_options[] = {
  {"help", no_argument, NULL, 'h'},
  {"debug-level", required_argument, NULL, 'D'},
  {0, 0, 0, 0}
};

/* main program: parse options, call function and exit. */
int
main (int argc, char **argv)
{

  for (;;)
    {
      int index;
      int c;

      c = getopt_long (argc, argv, short_options, long_options, &index);

      if (-1 == c)
	break;

      switch (c)
	{
	case 0:		/* getopt_long() flag */
	  break;

	case 'h':
	  usage (stdout, argc, argv);
	  exit (EXIT_SUCCESS);

	case 'D':
	  debug_level = atoi (optarg);
	  break;

	default:
	  usage (stderr, argc, argv);
	  exit (EXIT_FAILURE);
	}
    }

  test_for_disabled_adjtimex();
  exit (EXIT_SUCCESS);

  return 0;
}
