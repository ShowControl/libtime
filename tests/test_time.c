/*
 * File: test_time.c, author: John Sauter, date: June 8, 2025.
 * Test the time subroutines.
 */

/*
 * Copyright © 2025 by John Sauter <John_Sauter@systemeyescomputerstore.com>

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
#include <unistd.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>

#include "time_subroutines.h"

static int debug_level = 0;

/* Perform some tests. */
void
do_tests ()
{
  struct timespec ts_m, ts_r;
  const clockid_t id_m = CLOCK_MONOTONIC;
  const clockid_t id_r = CLOCK_REALTIME;
  char text_buffer_1 [64];
  char text_buffer_2 [64];
  char text_buffer_3 [64];
  char text_buffer_4 [64];
  int nanoseconds;
  struct tm now_tm;
  long long int current_time_long_long_int;
  struct tm local_time_tm;
  struct tm utc_time_tm;
  struct tm base_time_tm;
  int elapsed_time;
  int base_nanoseconds;

  __int128 current_time_int128;
    
  printf ("Size of TM struture is: %zu bytes or %zu ints.\n",
	  sizeof (base_time_tm), sizeof (base_time_tm) / sizeof (int));
  printf ("Offset of tm_sec is: %zu.\n", offsetof (struct tm, tm_sec));
  printf ("Offset of tm_min is: %zu.\n", offsetof (struct tm, tm_min));
  printf ("Offset of tm_hour is: %zu.\n", offsetof (struct tm, tm_hour));
  printf ("Offset of tm_mday is: %zu.\n", offsetof (struct tm, tm_mday));
  printf ("Offset of tm_mon is: %zu.\n", offsetof (struct tm, tm_mon));
  printf ("Offset of tm_year is: %zu.\n", offsetof (struct tm, tm_year));
  printf ("Offset of tm_wday is: %zu.\n", offsetof (struct tm, tm_wday));
  printf ("Offset of tm_yday is: %zu.\n", offsetof (struct tm, tm_yday));
  printf ("Offset of tm_isdst is: %zu.\n", offsetof (struct tm, tm_isdst));
  printf ("Size of tm_isdst is: %zu.\n",
	  sizeof(((struct tm *)0)->tm_isdst));
  printf ("Offset of tm_gmtoff is: %zu.\n", offsetof (struct tm, tm_gmtoff));
  printf ("Size of tm_gmtoff is: %zu.\n",
	  sizeof(((struct tm *)0)->tm_gmtoff));
  printf ("Offset of tm_zone is: %zu.\n", offsetof (struct tm, tm_zone));
  printf ("Size of tm_zone is: %zu.\n",
	  sizeof(((struct tm *)0)->tm_zone));
  
  time_current_tm_nano (&base_time_tm, &base_nanoseconds);
  
  clock_gettime (id_r, &ts_r);
  clock_gettime (id_m, &ts_m);

  time_current_tm_nano (&now_tm, &nanoseconds);
  time_tm_nano_to_string (&now_tm, nanoseconds,
			  text_buffer_1, sizeof(text_buffer_1));
  printf ("UTC time string: %s.\n", text_buffer_1);
  
  time_tm_nano_to_integer (&now_tm, nanoseconds,
			   &current_time_int128);
  int128_to_string (&current_time_int128,
		    text_buffer_2, sizeof(text_buffer_2));
  printf ("UTC time integer with nanoseconds: %s.\n", text_buffer_2);
  
  time_tm_to_integer (&now_tm, &current_time_long_long_int);
  printf ("UTC time integer: %lld.\n", current_time_long_long_int);
  
  time_UTC_to_local (&now_tm, &local_time_tm, INT_MIN);
  time_tm_nano_to_string (&local_time_tm, nanoseconds,
			  text_buffer_3, sizeof(text_buffer_3));
  printf ("Local time with nanoseconds: %s.\n", text_buffer_3);
  
  time_local_to_UTC (&local_time_tm, &utc_time_tm, INT_MIN);
  time_tm_nano_to_string (&utc_time_tm, nanoseconds,
			  text_buffer_4, sizeof(text_buffer_4));
  printf ("back to UTC time: %s.\n", text_buffer_4);

  elapsed_time = time_diff (&base_time_tm, &now_tm, INT_MIN);
  printf ("Elapsed time: %d.\n", elapsed_time);
      
  return;
}


/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options]\n\n"
	       "log the progress of time.\n"
	       " Version 1.3 2019-11-16\n"
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

/* main program: parse options, create file and exit. */
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

  do_tests ();
  exit (EXIT_SUCCESS);

  return 0;
}
