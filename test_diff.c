/*
 * File: test_diff.c, author: John Sauter, date: January 14, 2017.
 * Test the diff_time subroutine.
 */
/*
 * Copyright © 2017 by John Sauter <John_Sauter@systemeyescomputerstore.com>

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

#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>		/* getopt_long() */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "time_subroutines.h"

static int debug_level = 0;

/* Decode two times and print their difference.  */
void
do_test (char *A_time, char *B_time)
{
  struct tm A_tm, B_tm;
  int seconds;
  void *err_val;
  char A_string [256];
  char B_string [256];

  memset (&A_tm, 0, sizeof (A_tm));
  err_val = strptime (A_time, "%Y-%m-%dT%H:%M:%S", &A_tm);
  if (err_val == NULL)
    {
      printf ("error on A: %s.\n", A_time);
      return;
    }

  memset (&B_tm, 0, sizeof (B_tm));
  err_val = strptime (B_time, "%Y-%m-%dT%H:%M:%S", &B_tm);
  if (err_val == 0)
    {
      printf ("error on B: %s.\n", B_time);
      return;
    }
  seconds = time_diff (&A_tm, &B_tm, 0);
  time_tm_to_string (&A_tm, A_string, sizeof (A_string));
  time_tm_to_string (&B_tm, B_string, sizeof (B_string));
  printf ("%s to %s is %d seconds.\n",
	  A_string, B_string, seconds);
  return;
}


/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] A_time B_time\n\n"
	       "print the number of seconds between two times.\n"
	       "Version 2.0 2017-01-14\n"
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

  char *A_time;
  char *B_time;
  
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

  if (argc > 1)
    {
      A_time = argv [optind];
      B_time = argv [optind + 1];
    } else {
    usage (stderr, argc, argv);
    exit (EXIT_FAILURE);
  }
  
  do_test (A_time, B_time);
  exit (EXIT_SUCCESS);

  return 0;
}
