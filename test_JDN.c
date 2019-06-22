/*
 * File: test_JDN.c, author: John Sauter, date: June 18, 2019.
 * Test the JDN conversion.
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
static void
display_JDN (int year_no, int month_no, int mday_no)
{
  int the_JDN;
  
  the_JDN = time_Julian_day_number (year_no, month_no, mday_no);
  printf ("Year %i Month %i Day %i is JDN %i.5.\n",
	  year_no, month_no, mday_no, the_JDN);
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
	       "Display a Julian Day Number.\n"
	       " Version 1.0 2019-06-16\n"
	       "Options:\n"
	       "-h | --help          Print this message\n"
	       "-y | --year          Year\n"
	       "-m | --month         Month\n"
	       "-d | --day           Day of the month\n"
	       "-D | --debug-level   Amount of debugging output, default 0\n"
	       "", argv[0]);
    }
}

/* Options */
static const char short_options[] = "ho:r:d:D:";

static const struct option long_options[] = {
  {"help", no_argument, NULL, 'h'},
  {"year", required_argument, NULL, 'y'},
  {"month", required_argument, NULL, 'm'},
  {"day", required_argument, NULL, 'd'},
  {"debug-level", required_argument, NULL, 'D'},
  {0, 0, 0, 0}
};

/* main program: parse options, print result and exit. */
int
main (int argc, char **argv)
{

  int year_no = 1972;
  int month_no = 6;
  int mday_no = 30;
  
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

	case 'y':
	  year_no = atoi (optarg);
	  break;
	  
	case 'm':
	  month_no = atoi (optarg);
	  break;
	  
	case 'd':
	  mday_no = atoi (optarg);
	  break;
	  
	case 'D':
	  debug_level = atoi (optarg);
	  break;

	default:
	  usage (stderr, argc, argv);
	  exit (EXIT_FAILURE);
	}
    }

  display_JDN (year_no, month_no, mday_no);
  exit (EXIT_SUCCESS);

  return 0;
}
