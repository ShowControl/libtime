/*
 * File: test_time.c, author: John Sauter, date: November 3, 2019.
 * Test the time subroutines.
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
#include <unistd.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>

#include "time_subroutines.h"

static char *output_file = NULL;
static double running_time = 0.0;
static double delay_time = 0.0;
static int debug_level = 0;

/* routine to exit with an error message */
static void
errno_exit (const char *s)
{
  fprintf (stderr, "%s error %d, %s\n", s, errno, strerror (errno));
  exit (EXIT_FAILURE);
}

/* Print a log file of the clock.  */
void
do_logging (char *log_file_name)
{
  FILE *log_file;
  struct timespec ts_m, ts_r;
  const clockid_t id_m = CLOCK_MONOTONIC;
  const clockid_t id_r = CLOCK_REALTIME;
  double start_time, current_time;
  char text_buffer_1 [64];
  char text_buffer_2 [64];
  char text_buffer_3 [64];
  char text_buffer_4 [64];
  struct timespec delay_timespec;
  int nanoseconds;
  struct tm now_tm;
  __int128 current_time_int128;
  long long int current_time_long_long_int;
  struct tm local_time_tm;
  struct tm utc_time_tm;
  struct tm base_time_tm;
  int elapsed_time;
  int loop_counter;
  long long int delay_time_ns;
  int base_nanoseconds;
  long long int target_nanoseconds;
  struct tm target_time_tm;
  long long int add_nanoseconds;

  printf ("Size of TM struture is: %ld bytes or %ld ints.\n",
	  sizeof (base_time_tm), sizeof (base_time_tm) / sizeof (int));
  printf ("Offset of tm_sec is: %ld.\n", offsetof (struct tm, tm_sec));
  printf ("Offset of tm_min is: %ld.\n", offsetof (struct tm, tm_min));
  printf ("Offset of tm_hour is: %ld.\n", offsetof (struct tm, tm_hour));
  printf ("Offset of tm_mday is: %ld.\n", offsetof (struct tm, tm_mday));
  printf ("Offset of tm_mon is: %ld.\n", offsetof (struct tm, tm_mon));
  printf ("Offset of tm_year is: %ld.\n", offsetof (struct tm, tm_year));
  printf ("Offset of tm_wday is: %ld.\n", offsetof (struct tm, tm_wday));
  printf ("Offset of tm_yday is: %ld.\n", offsetof (struct tm, tm_yday));
  printf ("Offset of tm_isdst is: %ld.\n", offsetof (struct tm, tm_isdst));
  printf ("Size of tm_isdst is: %ld.\n",
	  sizeof(((struct tm *)0)->tm_isdst));
  printf ("Offset of tm_gmtoff is: %ld.\n", offsetof (struct tm, tm_gmtoff));
  printf ("Size of tm_gmtoff is: %ld.\n",
	  sizeof(((struct tm *)0)->tm_gmtoff));
  printf ("Offset of tm_zone is: %ld.\n", offsetof (struct tm, tm_zone));
  printf ("Size of tm_zone is: %ld.\n",
	  sizeof(((struct tm *)0)->tm_zone));
  
  log_file = fopen (log_file_name, "a+");
  if (log_file == NULL)
    {
      errno_exit ("Unable to open output file;");
    }

  
  fprintf (log_file, "UTC time string, UTC time with nanoseconds integer, "
	   "local time string, UTC time string, "
	   "elapsed_time, UTC time integer, "
	   "monotonic seconds, monotonic nanoseconds\n");

  /* Compute the start time, so we can stop when requested by -t.  */
  clock_gettime (id_m, &ts_m);
  start_time = (double) ts_m.tv_sec + (double) (ts_m.tv_nsec / 1e9);
  delay_timespec.tv_sec = (int) delay_time;
  delay_timespec.tv_nsec = ((int) (delay_time * 1E9)) - delay_timespec.tv_sec;
  delay_time_ns = delay_time * (double) 1e9;
  
  time_current_tm_nano (&base_time_tm, &base_nanoseconds);
  loop_counter = 0;
  
  while (1)
    {
      clock_gettime (id_r, &ts_r);
      clock_gettime (id_m, &ts_m);

      time_current_tm_nano (&now_tm, &nanoseconds);
      time_tm_nano_to_string (&now_tm, nanoseconds,
			      text_buffer_1, sizeof(text_buffer_1));
      time_tm_nano_to_integer (&now_tm, nanoseconds,
			       &current_time_int128);
      int128_to_string (&current_time_int128,
			text_buffer_2, sizeof(text_buffer_2));
      time_tm_to_integer (&now_tm, &current_time_long_long_int);
      time_UTC_to_local (&now_tm, &local_time_tm, INT_MIN);
      time_tm_nano_to_string (&local_time_tm, nanoseconds,
			      text_buffer_3, sizeof(text_buffer_3));
      time_local_to_UTC (&local_time_tm, &utc_time_tm, INT_MIN);
      time_tm_nano_to_string (&utc_time_tm, nanoseconds,
			      text_buffer_4, sizeof(text_buffer_4));

      elapsed_time = time_diff (&base_time_tm, &now_tm, INT_MIN);
      
      fprintf (log_file, "%s, %s, %s, %s, %d, %lld, %ld, %ld\n",
	       text_buffer_1, text_buffer_2, text_buffer_3, text_buffer_4,
	       elapsed_time, current_time_long_long_int,
	       (long int) ts_m.tv_sec, (long int) ts_m.tv_nsec);
      
      loop_counter = loop_counter + 1;
      time_copy_tm (&base_time_tm, &target_time_tm);
      target_nanoseconds = 0;
      add_nanoseconds = delay_time_ns * loop_counter;
      time_UTC_add_seconds_ns (&target_time_tm, &target_nanoseconds,
			       0, add_nanoseconds, INT_MIN);
      time_sleep_until (&target_time_tm, target_nanoseconds, INT_MIN); 
 
      /* clock_nanosleep (CLOCK_MONOTONIC, 0, &delay_timespec, NULL); */
      current_time = (double) ts_m.tv_sec + (double) (ts_m.tv_nsec / 1e9);
      if ((current_time - start_time) >= running_time)
	break;
    }
    
  fclose (log_file);
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
	       " Version 1.2 2019-04-04\n"
	       "Options:\n"
	       "-h | --help          Print this message\n"
	       "-o | --output-file   Where to put the log file\n"
	       "-r | --runtime       How many seconds to run\n"
	       "-d | --delay         How many seconds to delay between lines\n"
	       "-D | --debug-level   Amount of debugging output, default 0\n"
	       "", argv[0]);
    }
}

/* Options */
static const char short_options[] = "ho:r:d:D:";

static const struct option long_options[] = {
  {"help", no_argument, NULL, 'h'},
  {"output-file", required_argument, NULL, 'o'},
  {"runtime", required_argument, NULL, 't'},
  {"delay", required_argument, NULL, 'd'},
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

	case 'o':
	  output_file = optarg;
	  break;

	case 'r':
	  running_time = atof (optarg);
	  break;
	  
	case 'd':
	  delay_time = atof (optarg);
	  break;

	case 'D':
	  debug_level = atoi (optarg);
	  break;

	default:
	  usage (stderr, argc, argv);
	  exit (EXIT_FAILURE);
	}
    }

  if (output_file == NULL)
    {
      fprintf (stderr, "The output file must be specified.\n");
      exit (EXIT_FAILURE);
    }
  do_logging (output_file);
  exit (EXIT_SUCCESS);

  return 0;
}
