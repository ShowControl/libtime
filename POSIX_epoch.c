/*
 * File: POSIX_epoch.c, author: John Sauter, date: November 11, 2018.
 * Print the date of the POSIX epoch.
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

/* Print the date of the POSIX epoch.  */
void
do_print ()
{
  struct tm time1_tm;
  struct tm time2_tm;
  struct tm time3_tm;
  int seconds_since_epoch;
  char buffer1 [64];
  char buffer2 [64];
  char buffer3 [64];
  int seconds_count;
  
  /* To be sure we have a value of "seconds since the epoch"
   * that matches the current time, fetch the time twice,
   * and the value of "seconds since the epoch" between.
   * If the times match, the "seconds since the epoch"
   * corresponds to the fetched time.  If not, do the
   * two fetches over again.  Also, reject the value of
   * "seconds since the epoch" computed during a leap
   * second, since it isn't well-defined then.  */
  for (;;)
    {
      time_current_tm (&time1_tm);
      seconds_since_epoch = time (NULL);
      time_current_tm (&time2_tm);
      if ((time_diff (&time1_tm, &time2_tm, INT_MIN) == 0) &&
	  (time1_tm.tm_sec != 60))
	{
	  break;
	}
    }

  /* Subtract the number of SI seconds since the epoch from
   * the current time to get the time of the epoch.  */
  time_UTC_add_seconds (&time2_tm, -seconds_since_epoch, INT_MIN);

  /* Subtract the number of wall clock seconds since the epoch
   * from the current time to get the time of the epoch.  */
  time_copy_tm (&time1_tm, &time3_tm);
  time_UTC_add_seconds (&time3_tm, -seconds_since_epoch, 1972);
  
  /* Print the results.  */
  time_tm_to_string (&time1_tm, &buffer1 [0], sizeof (buffer1));
  time_tm_to_string (&time2_tm, &buffer2 [0], sizeof (buffer2));
  time_tm_to_string (&time3_tm, &buffer3 [0], sizeof (buffer3));
  printf ("Current time is %s.\n" 
	  "so-called seconds since the POSIX epoch is %d.\n" 
	  "%d SI seconds before %s is %s.\n"
	  "%d wall-clock seconds before %s is %s.\n",
	  buffer1,
	  seconds_since_epoch,
	  seconds_since_epoch, buffer1, buffer2,
	  seconds_since_epoch, buffer1, buffer3);

  /* The PTP epoch is 441763210 seconds before 1972-01-01T00:00:00Z.
   */
  time1_tm.tm_year = 1972 - 1900;
  time1_tm.tm_mon = 1 - 1;
  time1_tm.tm_mday = 1;
  time1_tm.tm_hour = 0;
  time1_tm.tm_min = 0;
  time1_tm.tm_sec = 0;
  seconds_count = 441763210;
  
  time_tm_to_string (&time1_tm, &buffer1 [0], sizeof (buffer1));
  time_copy_tm (&time1_tm, &time2_tm);
  time_UTC_add_seconds (&time2_tm, -seconds_count, INT_MIN);
  time_tm_to_string (&time2_tm, &buffer2 [0], sizeof (buffer2));
  time_copy_tm (&time1_tm, &time3_tm);
  time_UTC_add_seconds (&time3_tm, -seconds_count, 1972);
  time_tm_to_string (&time3_tm, &buffer3 [0], sizeof (buffer3));
  printf ("%d SI seconds before %s is %s\n"
	  " or %s wall-clock seconds.\n",
	  seconds_count, buffer1, buffer2, buffer3);
    
  return;
}

/* Print a helpful message.  */
static void
usage (FILE *fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] \n\n"
	       "Print the date of the POSIX epoch\n"
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

/* main program: parse options, perform print and exit. */
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
 
  do_print ();
  exit (EXIT_SUCCESS);

  return 0;
}
