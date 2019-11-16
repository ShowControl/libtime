/*
 * File: example_02.c, author: John Sauter, date: November 16, 2019.
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

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>		/* getopt_long() */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "../time_subroutines.h"

static int debug_level = 0;

/* Example 2: Periodic Backups.  */
void
example_2 ()
{
  struct tm time_tm;
  struct tm local_time_tm;
  struct tm backup_tm;
  struct tm extra_time_tm;
  struct tm int_time_tm;
  char buffer1 [64];
  char buffer2 [64];
  int found_time, day_incr, diff;
  
  time_current_tm (&time_tm);
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
  printf ("now: %s\n", buffer1);
  local_time_tm.tm_hour = 3;
  local_time_tm.tm_min = 0;
  local_time_tm.tm_sec = 0;

  found_time = 0;
  day_incr = 1;
  while (!found_time)
    {
      time_copy_tm (&local_time_tm, &extra_time_tm);
      time_local_add_days (&extra_time_tm, day_incr, 1, INT_MIN);
      if ((extra_time_tm.tm_wday == 0) ||
	  (extra_time_tm.tm_wday == 1))
	day_incr = day_incr + 1;
      else
	found_time = 1;
    }

  time_local_to_UTC (&extra_time_tm, &time_tm, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  printf ("Next scheduled backup is at %s.\n", buffer1);

  /* To illustrate, assume the last backup was 29 days ago.  
   */
  time_current_tm (&backup_tm);
  time_UTC_add_days (&backup_tm, -29, -1, INT_MIN);
  time_tm_to_string (&backup_tm, &buffer2 [0], sizeof (buffer2));
  printf ("Assume the last full backup was %s.\n", buffer2);
  
  time_local_add_months (&extra_time_tm, -1, 1, INT_MIN);
  time_local_to_UTC (&extra_time_tm, &int_time_tm, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  printf ("One month before the next scheduled backup is %s.\n",
	  buffer1);
  diff = time_diff (&int_time_tm, &backup_tm, INT_MIN);
  if (diff < 0)
    printf ("Next backup is full.\n");
  else
    printf ("Next backup is incremental.\n");
}

/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] \n\n"
	       "example_2\n"
	       " Version 1.3 2019-11-16\n"
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

/* main program: parse options, perform test and exit. */
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
 
  example_2 ();
  exit (EXIT_SUCCESS);

  return 0;
}
