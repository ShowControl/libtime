/*
 * File: check_time.c, author: John Sauter, date: November 3, 2019.
 * Used by "make check" to be sure the subroutines work.
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

#include "time_subroutines.h"

static int debug_level = 0;

/* Call all of the entry points.  */
int
do_test ()
{
  struct tm program_start_tm;
  struct tm time_tm;
  struct tm local_time_tm;
  struct tm backup_tm;
  struct tm start_time_tm;
  struct tm prev_time_tm;
  struct tm extra_time_tm;
  struct tm intermediate_time_tm;
  struct tm int_time_tm;
  char buffer1 [64];
  char buffer2 [64];
  char buffer3 [64];
  char buffer4 [64];
  int i, j, k, l;
  int trigger, nanoseconds, program_start_nanoseconds;
  long long int long_nanoseconds;
  long long int seconds1, seconds2, seconds3;
  long long int the_time;
  __int128 big_number;

  printf ("Program start.\n");
  time_current_tm_nano (&program_start_tm, &program_start_nanoseconds);

  printf ("Big number -10.\n");
  big_number = -10;
  int128_to_string (&big_number, &buffer1 [0], sizeof (buffer1));
  if ((buffer1 [0] != '-') || (buffer1 [1] != '1') ||
      (buffer1 [2] != '0'))
    {
      printf ("wrong result from int128_to_string: %s.\n",
	      buffer1);
      return 1;
    }

  printf ("Big number 0.\n");
  big_number = 0;
  int128_to_string (&big_number, &buffer1 [0], sizeof (buffer1));
  if ((buffer1 [0] != '0') || (buffer1 [1] != 0))
    {
      printf ("wrong result from int128_to_string: \n"
	      " should be 0, was %s.\n",
	      buffer1);
      return 1;
    }

  printf ("Big number -2**127.\n");
  big_number = 1;
  for (i=0;i<127;i++)
    {
      int128_to_string (&big_number, &buffer1 [0], sizeof (buffer1));
      big_number = big_number * 2;
      int128_to_string (&big_number, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	{
	  printf ("%d: Big number doubled, from %s to %s.\n",
		  i, buffer1, buffer2);
	}
    }
      
  int128_to_string (&big_number, &buffer1 [0], sizeof (buffer1));
  if (strcmp (&buffer1 [0], "-170141183460469231731687303715884105728") != 0)
    {
      printf ("wrong result from int128_to_string: \n"
	      " should be -2**127, was %s.\n",
	      buffer1);
      return 1;
    }
  
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_nano_to_string (&time_tm, nanoseconds, &buffer1 [0],
			  sizeof (buffer1));
  time_tm_to_string (&local_time_tm, &buffer2 [0], sizeof(buffer2));
  time_UTC_to_local (&program_start_tm, &local_time_tm, INT_MIN);
  time_tm_nano_to_string (&program_start_tm, program_start_nanoseconds,
			  &buffer3 [0], sizeof (buffer3));
  time_tm_to_string (&local_time_tm, &buffer4 [0], sizeof(buffer4));
  seconds1 = time_diff (&program_start_tm, &time_tm, INT_MIN);
  if (seconds1 < 0)
    {
      printf ("Time is flowing backwards.\n");
      printf ("Start time is %s.  Current time is %s.\n",
	      buffer4, buffer2);
      printf ("Current: %s is \n     %s and %d nanoseconds\n",
	      buffer1, buffer2, nanoseconds);
    }
  
  long_nanoseconds = nanoseconds;
  for (i=0;i<100;i++)
    {
      time_UTC_add_seconds_ns (&time_tm, &long_nanoseconds,
			       0, 1e7, INT_MIN);
      time_sleep_until (&time_tm, long_nanoseconds, INT_MIN);
    }
  for (i=0;i<1000;i++)
    {
      time_current_tm_nano (&time_tm, &nanoseconds);
      long_nanoseconds = nanoseconds;
      time_UTC_add_seconds_ns (&time_tm, &long_nanoseconds,
			       0, -1e6, INT_MIN);
      time_sleep_until (&time_tm, long_nanoseconds, INT_MIN);
    }
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_nano_to_string (&local_time_tm, nanoseconds,
			  &buffer1 [0], sizeof(buffer1));
  time_tm_nano_to_integer (&time_tm, nanoseconds, &big_number);
  time_tm_to_integer (&time_tm, &the_time);
  int128_to_string (&big_number, &buffer2 [0], sizeof (buffer2));
      
  time_copy_tm (&time_tm, &start_time_tm);
  start_time_tm.tm_year = 1800 - 1900;
  start_time_tm.tm_mon = 0;
  start_time_tm.tm_mday = 1;
  start_time_tm.tm_hour = 0;
  start_time_tm.tm_min = 0;
  start_time_tm.tm_sec = 1;

  for (j=2006;j<=2006;j++)
    {
      start_time_tm.tm_year = j - 1900;
      time_current_tm (&time_tm);
      time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
      printf ("year %d.\n", j);

      for (k=0;k<=0;k++)
	{
	  start_time_tm.tm_mon = k;
	  time_current_tm (&time_tm);
	  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
	  time_tm_to_string (&local_time_tm, &buffer1 [0],
			     sizeof(buffer1));
	  printf ("month %d.\n", k);
	  trigger = 1;
	      
	  for (l=1;l<=1;l++)
	    {
	      start_time_tm.tm_mday = l;
	      
	      for (i=-60;i<=(86400+60);i++)
		{
		  time_copy_tm (&start_time_tm, &time_tm);
		  long_nanoseconds = 0;
		  time_UTC_add_seconds_ns (&time_tm, &long_nanoseconds,
					   0, i*1e9, INT_MIN);
		  if (time_tm.tm_sec == 60)
		    trigger = 1;
		  time_copy_tm (&time_tm, &extra_time_tm);
		  time_UTC_add_seconds (&extra_time_tm, 2, INT_MIN);
		  time_copy_tm (&extra_time_tm, &int_time_tm);
		  time_UTC_add_seconds (&extra_time_tm, -4, INT_MIN);
		  time_copy_tm (&extra_time_tm, &backup_tm);
		  time_UTC_add_seconds (&extra_time_tm, 2, INT_MIN);
		  seconds3 = time_diff (&time_tm, &extra_time_tm, INT_MIN);
		  if (seconds3 != 0)
		    {
		      time_tm_to_string (&time_tm, &buffer1 [0],
					 sizeof (buffer1));
		      time_tm_to_string (&int_time_tm, &buffer2 [0],
					 sizeof (buffer2));
		      time_tm_to_string (&backup_tm, &buffer3 [0],
					 sizeof (buffer3));
		      time_tm_to_string (&extra_time_tm, &buffer4 [0],
					 sizeof (buffer4));
		      printf ("%llds: %s +2 %s -4 %s +2 %s.\n",
			      seconds3, buffer1, buffer2, buffer3,
			      buffer4);
		      return 1;
		    }

		  if (time_tm.tm_sec < 60)
		    {
		      time_copy_tm (&time_tm, &extra_time_tm);
		      time_UTC_add_days (&extra_time_tm, 1, 1, INT_MIN);
		      time_UTC_add_days (&extra_time_tm, -1, 1, INT_MIN);
		      seconds3 = time_diff (&time_tm, &extra_time_tm, INT_MIN);
		      if (seconds3 != 0)
			{
			  time_tm_to_string (&time_tm, &buffer2 [0],
					     sizeof (buffer2));
			  time_tm_to_string (&extra_time_tm, &buffer3 [0],
					     sizeof (buffer3));
			  printf ("%llds: day: %s -> %s.\n",
				  seconds3, buffer2, buffer3);
			  return 1;
			}
		      
		      if (time_tm.tm_mday <= 28)
			{
			  time_copy_tm (&time_tm, &extra_time_tm);
			  time_UTC_add_months (&extra_time_tm, 1, 1, INT_MIN);
			  time_UTC_add_months (&extra_time_tm, -1, 1, INT_MIN);
			  seconds3 =
			    time_diff (&time_tm, &extra_time_tm, INT_MIN);
			  if (seconds3 != 0)
			    {
			      time_tm_to_string (&time_tm, &buffer2 [0],
						 sizeof (buffer2));
			      time_tm_to_string (&extra_time_tm,
						 &buffer3 [0],
						 sizeof (buffer3));
			      printf ("%llds: month: %s -> %s.\n",
				      seconds3, buffer2, buffer3);
			      return 1;
			    } 
			  
			  time_copy_tm (&time_tm, &extra_time_tm);
			  time_UTC_add_years (&extra_time_tm, 1, 1, INT_MIN);
			  time_UTC_add_years (&extra_time_tm, -1, 1, INT_MIN);
			  seconds3 =
			    time_diff (&time_tm, &extra_time_tm, INT_MIN);
			  if (seconds3 != 0)
			    {
			      time_tm_to_string (&time_tm, &buffer2 [0],
						 sizeof (buffer2));
			      time_tm_to_string (&extra_time_tm,
						 &buffer3 [0],
						 sizeof (buffer3));
			      printf ("%llds: years: %s -> %s.\n",
				      seconds3, buffer2, buffer3);
			      return 1;
			    }
			}
		    }
		  
		  time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
		  time_copy_tm (&extra_time_tm, &intermediate_time_tm);
		  long_nanoseconds = 0;
		  time_local_add_seconds_ns (&extra_time_tm, &long_nanoseconds,
					     0, 2*1e9, INT_MIN);
		  time_local_add_seconds_ns (&extra_time_tm, &long_nanoseconds,
					     0, -4*1e9, INT_MIN);
		  time_local_add_seconds (&extra_time_tm, 2, INT_MIN);
		  seconds2 = time_diff (&extra_time_tm,
					&intermediate_time_tm, INT_MIN);
		  if (seconds2 != 0)
		    {
		      time_tm_to_string (&time_tm, &buffer1 [0],
					 sizeof (buffer1));
		      time_tm_to_string (&extra_time_tm, &buffer2 [0],
					 sizeof (buffer2));
		      time_tm_to_string (&intermediate_time_tm,
					 &buffer3 [0], sizeof (buffer3));
		      printf ("%llds: %s: %s -> %s.\n",
			      seconds2, buffer1, buffer2, buffer3);
		      return 1;
		    }
		  
		  time_local_to_UTC (&extra_time_tm, &intermediate_time_tm,
				     INT_MIN);
		  seconds1 = time_diff (&time_tm, &intermediate_time_tm,
					    INT_MIN);
		  if ((seconds1 != 0))
		    {
		      time_tm_to_string (&time_tm, &buffer2 [0],
					 sizeof (buffer2));
		      time_tm_to_string (&extra_time_tm, &buffer3 [0],
					 sizeof (buffer3));
		      time_tm_to_string (&intermediate_time_tm,
					 &buffer1 [0], sizeof (buffer1));
		      printf ("%llds: %s -> %s -> %s.\n",
			      seconds1, buffer2, buffer3, buffer1);
		      return 1;
		    }
		  else
		    {
		      if ((debug_level > 0) || (trigger != 0))
			{
			  time_tm_to_string (&time_tm, &buffer3 [0],
					     sizeof (buffer3));
			  time_tm_to_string (&extra_time_tm, &buffer1 [0],
					     sizeof (buffer1));
			  printf ("OK: %s <-> %s.\n",
				  buffer3, buffer1);
			  trigger = 0;
			}
		    }
		}
	    }
	}
    }    

  memset (&start_time_tm, 0, sizeof(start_time_tm));
  start_time_tm.tm_year = 1883 - 1900;
  start_time_tm.tm_mon = 1 - 1;
  start_time_tm.tm_mday = 31;
  start_time_tm.tm_hour = 0;
  start_time_tm.tm_min = 0;
  start_time_tm.tm_sec = 0;
  time_UTC_normalize (&start_time_tm, 0, INT_MIN);
  time_UTC_add_months (&start_time_tm, 1, -1, INT_MIN);
  time_UTC_add_days (&start_time_tm, 16, 1, INT_MIN);
  time_copy_tm (&start_time_tm, &prev_time_tm);

  time_tm_to_string (&start_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start at: %s.\n", buffer1);

  for (i=0; i<2000; i++)
    {
      time_copy_tm (&start_time_tm, &time_tm);
      
      time_UTC_add_years (&time_tm, i, 1, INT_MIN);
      time_UTC_add_years (&time_tm, -i, 1, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if (seconds1 != 0)
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add years %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}

      time_UTC_add_months (&time_tm, i, 1, INT_MIN);
      time_UTC_add_months (&time_tm, -i, 1, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if (seconds1 != 0)
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add months %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}

      time_UTC_add_days (&time_tm, i, 1, INT_MIN);
      time_UTC_add_days (&time_tm, -i, 1, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if (seconds1 != 0)
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add days %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}
	  
      time_UTC_add_hours (&time_tm, i, 1, INT_MIN);
      time_UTC_add_hours (&time_tm, -i, 1, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if (seconds1 != 0)
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add hours %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}

      time_UTC_add_minutes (&time_tm, i, 1, INT_MIN);
      time_UTC_add_minutes (&time_tm, -i, 1, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if (seconds1 != 0)
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add minutes %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}
	  
      time_UTC_add_seconds (&time_tm, i, INT_MIN);
      time_UTC_add_seconds (&time_tm, -i, INT_MIN);
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      if ((seconds1 != 0) || (debug_level > 0))
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	  printf ("add seconds %d: %s, %s\n", i, buffer1, buffer2);
	  return 1;
	}
	  
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	printf ("start at %s\n", buffer2);
      
      time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
      time_tm_to_string (&extra_time_tm, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	printf ("convert to local time %s\n", buffer2);
      
      time_local_add_years (&extra_time_tm, i, -1, INT_MIN);
      time_tm_to_string (&extra_time_tm, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	printf ("add %d years %s\n", i, buffer2);
      
      time_local_to_UTC (&extra_time_tm, &time_tm, INT_MIN);
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	printf ("back to UTC %s\n", buffer2);
      
      time_copy_tm (&time_tm, &intermediate_time_tm);
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      if (debug_level > 0)
	printf ("start at %s\n", buffer2);
      time_UTC_add_months (&intermediate_time_tm, 1, 1, INT_MIN);
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			 sizeof (buffer4));
      if (debug_level > 0)
	printf ("plus one month: %s\n", buffer4);
      time_UTC_add_days (&intermediate_time_tm, -5, 1, INT_MIN);
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			 sizeof (buffer4));
      if (debug_level > 0)
	printf ("minus five days: %s\n", buffer4);
      time_UTC_add_hours (&intermediate_time_tm, 6, -1, INT_MIN);
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			     sizeof (buffer4));
      if (debug_level > 0)
	printf ("plus six hours: %s\n", buffer4);
      time_UTC_add_minutes (&intermediate_time_tm, -20, 1, INT_MIN);
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			     sizeof (buffer4));
      if (debug_level > 0)
	printf ("minus twenty minutes: %s\n", buffer4);
      time_UTC_add_seconds (&intermediate_time_tm, -i, INT_MIN);
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			 sizeof (buffer4));
      if (debug_level > 0)
	printf ("minus %d seconds: %s\n", i, buffer4);
      time_copy_tm (&intermediate_time_tm, &extra_time_tm);
      time_UTC_add_seconds (&extra_time_tm, i, INT_MIN);
      time_UTC_add_minutes (&extra_time_tm, 20, 1, INT_MIN);
      time_UTC_add_hours (&extra_time_tm, -6, -1, INT_MIN);
      time_UTC_add_days (&extra_time_tm, 5, 1, INT_MIN);
      time_UTC_add_months (&extra_time_tm, -1, 1, INT_MIN);
      time_tm_to_string (&prev_time_tm, &buffer1 [0], sizeof (buffer1));
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      time_tm_to_string (&extra_time_tm, &buffer3 [0], sizeof (buffer3));
      time_tm_to_string (&intermediate_time_tm, &buffer4 [0],
			 sizeof (buffer4));
      seconds1 = time_diff (&time_tm, &intermediate_time_tm, INT_MIN);
      seconds2 = time_diff (&intermediate_time_tm, &extra_time_tm, INT_MIN);
      seconds3 = time_diff (&time_tm, &extra_time_tm, INT_MIN);
      if (((seconds1 + i) != 2266800) && ((seconds1 + i) != 2180400) &&
	  ((seconds1 + i) != 2007600) && ((seconds1 + i) != 2094000) &&
	  ((seconds1 + i) != 2266801))
	{
	  printf ("%d: %s->%s is %lld seconds\n",
		  i, buffer2, buffer4, i+seconds1);
	  return 1;
	}
      if ((((seconds2 - i) != -2266800) && ((seconds2 - i) != -2180400) &&
	   ((seconds2 - i) != -2007600) && ((seconds2 - i) != -2094000) &&
	   ((seconds2 - i) != -2266801)) ||
	  (seconds3 != 0))
	{
	  printf ("%d: %s->%s->%s is %lld, %lld seconds\n",
		  i, buffer2, buffer4, buffer3, seconds2 - i, seconds3);
	  return 1;
	}
      
      time_copy_tm (&time_tm, &prev_time_tm);
    }

  time_tm.tm_year = -4000-1900;
  time_tm.tm_mon = 1 - 1;
  time_tm.tm_mday = 1;
  time_tm.tm_hour = 0;
  time_tm.tm_min = 0;
  time_tm.tm_sec = 0;

  time_copy_tm (&time_tm, &extra_time_tm);
  extra_time_tm.tm_year = 4000 - 1900;
  seconds1 = time_diff (&time_tm, &extra_time_tm, INT_MIN);
  seconds2 = time_diff (&time_tm, &extra_time_tm, 1972);
  if (debug_level > 0)
    printf ("%lld SI or %lld wall-clock seconds make 8000 years.\n",
	    seconds1, seconds2);

  time_copy_tm (&time_tm, &extra_time_tm);
  for (i=0;i<400;i++)
    {
      time_UTC_add_days (&time_tm, 1, 1, INT_MIN);
      time_UTC_add_years (&time_tm, 1, -1, INT_MIN);
    }
  time_tm.tm_year = 1899 - 1900;
  for (i=0;i<400;i++)
    time_UTC_add_days (&time_tm, 1, 1, INT_MIN);
  time_tm.tm_year = 1999 - 1900;
  for (i=0;i<400;i++)
    time_UTC_add_days (&time_tm, 1, 1, INT_MIN);
  seconds1 = time_diff (&extra_time_tm, &time_tm, INT_MIN);
  if (debug_level > 0)
    printf ("%lld.\n", seconds1);

  time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
  extra_time_tm.tm_year = 2000 - 1900;
  extra_time_tm.tm_mon = 2 - 1;
  extra_time_tm.tm_mday = 28;
  time_local_add_days (&extra_time_tm, 1, 1, INT_MIN);
  time_local_add_years (&extra_time_tm, 1, 1, INT_MIN);
  time_local_add_years (&extra_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("%s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 12 - 1;
  extra_time_tm.tm_mday = 31;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 0;
  extra_time_tm.tm_sec = 0;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_local_add_seconds (&extra_time_tm, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one second: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_years (&extra_time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one year, rounding down: %s.\n", buffer1);
  time_local_add_years (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one year, rounding up: %s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 2 - 1;
  extra_time_tm.tm_mday = 29;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 0;
  extra_time_tm.tm_sec = 0;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_local_add_seconds (&extra_time_tm, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one second: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_years (&extra_time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one year, rounding down: %s.\n", buffer1);
  time_local_add_years (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one year, rounding up: %s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 2 - 1;
  extra_time_tm.tm_mday = 29;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 0;
  extra_time_tm.tm_sec = 0;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_local_add_seconds (&extra_time_tm, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one second: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_months (&extra_time_tm, 48, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add 48 months, rounding down: %s.\n", buffer1);
  time_local_add_months (&int_time_tm, 48, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add 48 months, rounding up: %s.\n", buffer1);
  time_local_add_months (&extra_time_tm, -48, 1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract 48 months, rounding up: %s.\n", buffer1);

  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 12 - 1;
  extra_time_tm.tm_mday = 31;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 0;
  extra_time_tm.tm_sec = 0;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_local_add_seconds (&extra_time_tm, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one second: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_months (&extra_time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one month, rounding down: %s.\n", buffer1);
  time_local_add_months (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one month, rounding up: %s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 12 - 1;
  extra_time_tm.tm_mday = 31;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 59;
  extra_time_tm.tm_sec = 60;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_days (&extra_time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one day, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &extra_time_tm);
  time_local_add_days (&extra_time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one day, rounding down: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_days (&extra_time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one day, rounding down: %s.\n", buffer1);
  time_local_add_days (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one day, rounding up: %s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 12 - 1;
  extra_time_tm.tm_mday = 31;
  extra_time_tm.tm_hour = 19;
  extra_time_tm.tm_min = 59;
  extra_time_tm.tm_sec = 60;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_hours (&extra_time_tm, 5, 1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add five hours, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &extra_time_tm);
  time_local_add_hours (&extra_time_tm, 5, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add five hours, rounding down: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_hours (&extra_time_tm, -5, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract five hours, rounding down: %s.\n", buffer1);
  time_local_add_hours (&int_time_tm, -5, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract five hours, rounding up: %s.\n", buffer1);
  
  extra_time_tm.tm_year = 2016 - 1900;
  extra_time_tm.tm_mon = 12 - 1;
  extra_time_tm.tm_mday = 31;
  extra_time_tm.tm_hour = 23;
  extra_time_tm.tm_min = 59;
  extra_time_tm.tm_sec = 59;
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_minutes (&extra_time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one minute, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &extra_time_tm);
  time_local_add_minutes (&extra_time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one minute, rounding down: %s.\n", buffer1);
  time_copy_tm (&extra_time_tm, &int_time_tm);
  time_local_add_minutes (&extra_time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one minute, rounding down: %s.\n", buffer1);
  time_local_add_minutes (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one minute, rounding up: %s.\n", buffer1);

  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 2016 - 1900;
  time_tm.tm_mon = 12 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 59;
  time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_years (&time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one year, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_years (&time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one year, rounding down: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_years (&time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one year, rounding down: %s.\n", buffer1);
  time_UTC_add_years (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one year, rounding up: %s.\n", buffer1);

  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 2016 - 1900;
  time_tm.tm_mon = 2 - 1;
  time_tm.tm_mday = 28;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 59;
  time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_years (&time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one year, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_years (&time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one year, rounding down: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_years (&time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one year, rounding down: %s.\n", buffer1);
  time_UTC_add_years (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one year, rounding up: %s.\n", buffer1);
  
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 2016 - 1900;
  time_tm.tm_mon = 3 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 59;
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_months (&time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one month, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_months (&time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one month, rounding down: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_months (&time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one month, rounding down: %s.\n", buffer1);
  time_UTC_add_months (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one month, rounding up: %s.\n", buffer1);
  
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 2016 - 1900;
  time_tm.tm_mon = 12 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 59;
  time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_hours (&time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one hour, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_hours (&time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one hour, rounding down: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_hours (&time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one hour, rounding down: %s.\n", buffer1);
  time_UTC_add_hours (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one hour, rounding up: %s.\n", buffer1);
  
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 2016 - 1900;
  time_tm.tm_mon = 12 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 59;
  time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_minutes (&time_tm, 1, 1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("add one minute, rounding up: %s.\n", buffer1);
  time_copy_tm (&int_time_tm, &time_tm);
  time_UTC_add_minutes (&time_tm, 1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or add one minute, rounding down: %s.\n", buffer1);
  time_copy_tm (&time_tm, &int_time_tm);
  time_UTC_add_minutes (&time_tm, -1, -1, INT_MIN);
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("subtract one minute, rounding down: %s.\n", buffer1);
  time_UTC_add_minutes (&int_time_tm, -1, 1, INT_MIN);
  time_tm_to_string (&int_time_tm, &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("or subtract one minute, rounding up: %s.\n", buffer1);

  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 1880 - 1900;
  time_tm.tm_mon = 12 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 58;
  time_tm.tm_sec = 59;
  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer2 [0], sizeof (buffer2));
  if (debug_level > 0)
    printf ("start with: %s -> %s.\n", buffer1, buffer2);
  time_copy_tm (&local_time_tm, &int_time_tm);
  for (i=0;i<10;i++)
    {
      time_local_add_minutes (&local_time_tm, 1, 1, INT_MIN);
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      if (debug_level > 0)
	printf ("add one minute, rounding up: %s.\n", buffer1);
    }
  for (i=0;i<10;i++)
    {
      time_local_add_minutes (&local_time_tm, -1, -1, INT_MIN);
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      if (debug_level > 0)
	printf ("subtract one minute, rounding down: %s.\n", buffer1);
    }
  
  /* additional tests here */
  
  time_current_tm_nano (&time_tm, &nanoseconds);
  time_tm.tm_year = 1800 - 1900;
  time_tm.tm_mon = 12 - 1;
  time_tm.tm_mday = 31;
  time_tm.tm_hour = 23;
  time_tm.tm_min = 59;
  time_tm.tm_sec = 58;
  time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  time_tm_nano_to_string (&time_tm, nanoseconds,
			  &buffer1 [0], sizeof (buffer1));
  if (debug_level > 0)
    printf ("start with: %s.\n", buffer1);
  time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
  time_UTC_to_local (&time_tm, &int_time_tm, 1972);
  time_local_to_UTC (&extra_time_tm, &intermediate_time_tm, INT_MIN);
  time_tm_nano_to_string (&extra_time_tm, nanoseconds,
			  &buffer1 [0], sizeof (buffer1));
  time_tm_nano_to_string (&int_time_tm, nanoseconds,
			  &buffer2 [0], sizeof (buffer2));
  time_tm_nano_to_string (&intermediate_time_tm, nanoseconds,
			  &buffer3 [0], sizeof (buffer3));
  seconds1 = time_diff (&time_tm, &intermediate_time_tm, INT_MIN);
  if ((seconds1 != 0) || (debug_level > 0))
    {
      printf (" Convert to local time with SI seconds: %s.\n",
	      buffer1);
      printf ("   (With variable seconds: %s.)\n",
	      buffer2);
      printf (" convert local time back to UTC: %s;\n"
	      "  off by %lld seconds.\n",
	      buffer3, seconds1);
    }
  
  time_tm.tm_year = 1900 - 1900;
  time_copy_tm (&time_tm, &extra_time_tm);
  time_UTC_add_minutes (&time_tm, -1, 1, INT_MIN);
  for (i=0;i<60;i++)
    time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  seconds1 = time_diff (&time_tm, &extra_time_tm, INT_MIN);
  if (seconds1 != 0)
    {
      time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      printf ( "%lld: %s != %s\n", seconds1, buffer1, buffer2);
      return 1;
    }

  time_tm.tm_year = 2000 - 1900;
  time_UTC_add_minutes (&time_tm, -1, 1, INT_MIN);
  for (i=0;i<60;i++)
    time_UTC_add_seconds (&time_tm, 1, INT_MIN);
  if (seconds1 != 0)
    {
      time_tm_to_string (&extra_time_tm, &buffer1 [0], sizeof (buffer1));
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      printf ( "%lld: %s != %s\n", seconds1, buffer1, buffer2);
      return 1;
    }

  time_current_tm_nano (&time_tm, &nanoseconds);
  seconds1 = time_diff (&program_start_tm, &time_tm, INT_MIN);
  if (debug_level > 0)
    {
      time_tm_nano_to_string (&program_start_tm, program_start_nanoseconds,
			      &buffer1 [0], sizeof (buffer1));
      time_tm_nano_to_string (&time_tm, nanoseconds,
			      &buffer2 [0], sizeof (buffer2));
      printf ("program ran from %s to %s.\n", buffer1, buffer2);
    }
  nanoseconds = nanoseconds - program_start_nanoseconds;
  printf ("Successful completion.\n");
  return 0;
}

/* Print a helpful message.  */
static void
usage (FILE * fp, int argc, char **argv)
{
  if (argc >= 1)
    {
      fprintf (fp,
	       "Usage: %s [options] \n\n"
	       "test_local\n"
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

/* main program: parse options, perform test and exit. */
int
main (int argc, char **argv)
{
  int result;
  
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
 
  result = do_test ();
  exit (result);

  return 0;
}
