/*
 * File: print_DTAI.c, author: John Sauter, date: January 15, 2017.
 * Print the value of DTAI for all days.
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

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>		/* getopt_long() */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "time_subroutines.h"

static int debug_level = 0;

/* Print the value of DTAI for all Julian Day Numbers.  */
void
do_print (int variable_length_seconds_before_1972)
{

  int JDN, new_JDN, value, old_value;
  struct tm time_tm;
  char buffer1 [64];

  time_current_tm (&time_tm);
  time_tm.tm_year = -4000 - 1900;
  time_tm.tm_mon = 0;
  time_tm.tm_mday = 1;
  time_tm.tm_hour = 0;
  time_tm.tm_min = 0;
  time_tm.tm_sec = 0;

  JDN = time_Julian_day_number (time_tm.tm_year + 1900,
				time_tm.tm_mon + 1,
				time_tm.tm_mday);
  old_value = 0;
  while (JDN <= 3000000)
    {
      value = time_DTAI (JDN, variable_length_seconds_before_1972);
      if (value != old_value)
	{
	  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
	  printf ("JDN %d (%s) has DTAI %d.\n", JDN, buffer1, value);
	  old_value = value;
	}
      
      time_UTC_add_days (&time_tm, 1, 1,
			 variable_length_seconds_before_1972);
      new_JDN = time_Julian_day_number (time_tm.tm_year + 1900,
					time_tm.tm_mon + 1,
					time_tm.tm_mday);
      if (new_JDN != (JDN + 1))
	{
	  printf ("Problem in time_Julian_day.\n");
	  return;
	}
      
      JDN = new_JDN;
    }
}

/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{

  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] \n\n"
	       "print DTAI for all Julian Day Numbers.\n"
	       "Version 1.0 2017-01-15\n"
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

  int variable_length_seconds_before_1972;
  
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

  printf ("argc is %d.\n", argc);

  variable_length_seconds_before_1972 = 0;
  if (argc > 1)
    variable_length_seconds_before_1972 = atoi (argv [optind]);
  if (variable_length_seconds_before_1972 == 1)
    printf ("variable-length seconds before 1972.\n");

  do_print (variable_length_seconds_before_1972);
  exit (EXIT_SUCCESS);

  return 0;
}