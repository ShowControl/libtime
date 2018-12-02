/*
 * File: example_05.c, author: John Sauter, date: December 2, 2018.
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

/* Example 5: using PTP (IEEE 1588) with the SMPTE ST-2059-2 profile.  */
void
example_5 (long long int time_stamp_value)
{
  struct tm time1_tm;
  struct tm time2_tm;
  struct tm time3_tm;
  char buffer1 [128];

  /* Construct the base date of 1972-01-01T00:00:00Z.  */
  time_current_tm (&time1_tm);
  time1_tm.tm_year = 1972 - 1900;
  time1_tm.tm_mon = 1 - 1;
  time1_tm.tm_mday = 1;
  time1_tm.tm_hour = 0;
  time1_tm.tm_min = 0;
  time1_tm.tm_sec = 0;

  /* Add the time stamp, which is an integer.  Note that the epoch
   * for SMPTE ST-2059-2 is 63,072,010 seconds before the base date.
   * PTP always uses fixed-length (SI) seconds.  */
  time_copy_tm (&time1_tm, &time2_tm);
  time_UTC_add_seconds (&time2_tm, time_stamp_value - 63072010, INT_MIN);

  /* Convert to local time and use strftime to make a very readable 
   * display.  */
  time_UTC_to_local (&time2_tm, &time3_tm, INT_MIN);
  strftime (&buffer1 [0], sizeof (buffer1),
	    "%A, %B %d, %Y, %I:%M:%S %p %Z", &time3_tm);

  /* Print the result.  */
  printf ("PTP %lld displays as %s.\n", time_stamp_value, buffer1);
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
	       "example_5\n"
	       " Version 1.0 2018-12-02\n"
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

/* main program: parse options, perform example and exit. */
int
main (int argc, char **argv)
{

  int date_val;
  
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

  example_5 (0);
  example_5 (15638408);
  example_5 (47174409);
  example_5 (63072010);
  exit (EXIT_SUCCESS);

  return 0;
}
