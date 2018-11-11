/*
 * File: sleep_until_midnight.c, author: John Sauter, date: November 11, 2018.
 * Sleep until just before midnight, UTC.
 */

/*
 * Copyright © 2018 by John Sauter <John_Sauter@systemeyescomputerstore.com>

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

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>		/* getopt_long() */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "time_subroutines.h"

static int debug_level = 0;

/* Sleep until midnight  */
void
do_sleep ()
{
  struct tm time_tm;
  struct tm local_time_tm;
  char buffer1 [64];
  char buffer2 [64];
  
  time_current_tm (&time_tm);
  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
  printf ("now: %s local, %s UTC.\n", buffer1, buffer2);

  time_tm.tm_hour = 23;
  time_tm.tm_min = 58;
  time_tm.tm_sec = 0;
  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
  printf ("sleep until: %s local, %s UTC.\n", buffer1, buffer2);

  time_sleep_until (&time_tm, 0, INT_MIN);

  time_current_tm (&time_tm);
  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
  printf ("after sleep, now: %s local, %s UTC.\n", buffer1, buffer2);
  
  return;
}

/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] \n\n"
	       "sleep_until_midnight\n"
	       " Version 1.1 2018-11-11\n"
	       "Options:\n"
	       "-h | --help          Print this message\n"
	       "-D | --debug-level   Amount of debugging output, default 0\n"
	       "", argv[0]);
    }
}

/* Options */
static const char short_options[] = "hD:";

static const struct option long_options[] = {
  {"help", no_argument, NULL, 'h'},
  {"debug-level", required_argument, NULL, 'D'},
  {0, 0, 0, 0}
};

/* main program: parse options, perform sleep and exit. */
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
 
  do_sleep ();
  exit (EXIT_SUCCESS);

  return 0;
}
