/*
 * File: test_add.c, author: John Sauter, date: November 11, 2018.
 * Test the various time adding entry points.
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

/* Generate lots of output.  */
void
do_test ()
{
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
  long long int nanoseconds;
  long long int add_nanoseconds;
  long long int add_seconds;
  int i, j, k, l;
  int test_mode, trigger;
  long long int seconds1, seconds2, seconds3;
  long long int scount, max_scount;
  int found_time, day_incr, diff;
  
  time_current_tm (&time_tm);
  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
  printf ("now: %s\n", buffer1);

  test_mode = 5;
  max_scount =
    (long long int) 86400 * (long long int) 366 * (long long int) 400;

  if (test_mode == 11)
    {
      /* determine the time of the POSIX Epoch.  */
      seconds1 = time (NULL);
      time_UTC_add_seconds (&time_tm, -seconds1, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("POSIX epoch is %s.\n", buffer1);
    }
  
  if (test_mode == 10)
    {
      local_time_tm.tm_year = 2016 - 1900;
      local_time_tm.tm_mon = 1 - 1;
      local_time_tm.tm_mday = 1;
      local_time_tm.tm_hour = 19;
      local_time_tm.tm_min = 0;
      local_time_tm.tm_sec = 0;
      day_incr = 0;
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("Base time is %s.\n", buffer1);
      
      while (1)
	{
	  time_copy_tm (&local_time_tm, &extra_time_tm);
	  time_local_add_days (&extra_time_tm, day_incr, 1, INT_MIN);
	  extra_time_tm.tm_hour = 19;
	  extra_time_tm.tm_min = 0;
	  extra_time_tm.tm_sec = 0;
	  if (extra_time_tm.tm_year != (2016 - 1900))
	    break;
	  time_local_to_UTC (&extra_time_tm, &time_tm, INT_MIN);
	  time_UTC_add_seconds (&time_tm, -2, INT_MIN);
	  time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
	  time_tm_to_string (&extra_time_tm, &buffer2 [0],
			     sizeof (buffer2));
	  printf ("Launch at %s, 2 sec before local %s.\n",
		  buffer1, buffer2);
	  day_incr = day_incr + 1;
	}
    }
  
  if (test_mode == 9)
    {
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
      printf ("First scheduled backup is at %s.\n", buffer1);

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
  
  if (test_mode == 8)
    {
      local_time_tm.tm_year = 2017 - 1900;
      local_time_tm.tm_mon = 6 - 1;
      local_time_tm.tm_mday = 30;
      local_time_tm.tm_hour = 9;
      local_time_tm.tm_min = 0;
      local_time_tm.tm_sec = 0;

      time_local_to_UTC (&local_time_tm, &time_tm, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("9 AM local is %s.\n", buffer1);
    }

  if (test_mode == 7)
    {
      time_copy_tm (&time_tm, &start_time_tm);
      start_time_tm.tm_year = 1700 - 1900;
      start_time_tm.tm_mon = 0;
      start_time_tm.tm_mday = 1;
      start_time_tm.tm_hour = 0;
      start_time_tm.tm_min = 0;
      start_time_tm.tm_sec = 0;

      for (scount=0;scount<=max_scount;scount++)
	{
	  time_copy_tm (&start_time_tm, &time_tm);
	  time_UTC_add_seconds (&time_tm, scount, INT_MIN);
	  time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
	  time_local_to_UTC (&extra_time_tm, &int_time_tm, INT_MIN);
	  seconds1 = time_diff (&time_tm, &int_time_tm, INT_MIN);
	  if ((seconds1 != 0) || ((scount % 100000) == 0))
	    {
	      time_tm_to_string (&time_tm, &buffer1 [0],
				 sizeof (buffer1));
	      time_tm_to_string (&int_time_tm, &buffer2 [0],
				 sizeof (buffer2));
	      time_tm_to_string (&extra_time_tm, &buffer4 [0],
				 sizeof (buffer4));
	      printf ("%llds: %s -> %s -> %s.\n",
		  seconds1, buffer1, buffer4, buffer2);
	    }
	}
      
    }
  
  if (test_mode == 6)
    {
      time_copy_tm (&local_time_tm, &start_time_tm);
      start_time_tm.tm_year = 1845 - 1900;
      start_time_tm.tm_mon = 11;
      start_time_tm.tm_mday = 31;
      start_time_tm.tm_hour = 9;
      start_time_tm.tm_min = 18;
      start_time_tm.tm_sec = 50;
      start_time_tm.tm_gmtoff = (9 * 3600) + (18 * 60) + 59;
      time_copy_tm (&start_time_tm, &extra_time_tm);
      time_local_add_seconds (&extra_time_tm, 2, INT_MIN);
      time_copy_tm (&extra_time_tm, &int_time_tm);
      time_local_add_seconds (&extra_time_tm, -4, INT_MIN);
      time_copy_tm (&extra_time_tm, &intermediate_time_tm);
      time_local_add_seconds (&extra_time_tm, 2, INT_MIN);
      seconds2 = time_diff (&extra_time_tm, &start_time_tm, INT_MIN);
      if (1 || (seconds2 != 0))
	{
	  time_tm_to_string (&start_time_tm, &buffer1 [0],
			     sizeof (buffer1));
	  time_tm_to_string (&int_time_tm, &buffer2 [0],
			     sizeof (buffer2));
	  time_tm_to_string (&intermediate_time_tm, &buffer3 [0],
			     sizeof (buffer3));
	  time_tm_to_string (&extra_time_tm, &buffer4 [0],
			     sizeof (buffer4));
	  printf ("%llds: %s +2 %s -4 %s +2 %s.\n",
		  seconds2, buffer1, buffer2, buffer3, buffer4);
	}
    }     

  if (test_mode == 5)
    {
      time_copy_tm (&time_tm, &start_time_tm);
      start_time_tm.tm_year = 1800 - 1900;
      start_time_tm.tm_mon = 0;
      start_time_tm.tm_mday = 1;
      start_time_tm.tm_hour = 0;
      start_time_tm.tm_min = 0;
      start_time_tm.tm_sec = 1;

      for (j=2006;j<=2050;j++)
	{
	  start_time_tm.tm_year = j - 1900;
	  time_current_tm (&time_tm);
	  time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
	  time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof(buffer1));
	  printf ("year %d at %s\n", j, buffer1);

	  for (k=0;k<=11;k++)
	    {
	      start_time_tm.tm_mon = k;
	      time_current_tm (&time_tm);
	      time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
	      time_tm_to_string (&local_time_tm, &buffer1 [0],
				 sizeof(buffer1));
	      printf ("month %d at %s\n", k, buffer1);
	      trigger = 1;
	      
	      for (l=1;l<=31;l++)
		{
		  start_time_tm.tm_mday = l;
		  
		  for (i=-60;i<=(86400+60);i++)
		    {
		      time_copy_tm (&start_time_tm, &time_tm);
		      time_UTC_add_seconds (&time_tm, i, INT_MIN);
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
				}
			    }
			}
		      
		      time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
		      time_copy_tm (&extra_time_tm, &intermediate_time_tm);
		      time_local_add_seconds (&extra_time_tm, 2, INT_MIN);
		      time_local_add_seconds (&extra_time_tm, -4, INT_MIN);
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
    }

  if (test_mode == 4)
    {
      time_copy_tm (&time_tm, &start_time_tm);
      start_time_tm.tm_year = 1800 - 1900;
      start_time_tm.tm_mon = 0;
      start_time_tm.tm_mday = 1;
      start_time_tm.tm_hour = 0;
      start_time_tm.tm_min = 0;
      start_time_tm.tm_sec = 0;

      for (i=0;i<=250;i++)
	{
	  time_copy_tm (&start_time_tm, &time_tm);
	  time_UTC_add_years (&time_tm, i, 1, INT_MIN);
	  time_UTC_to_local (&time_tm, &extra_time_tm, INT_MIN);
	  time_local_to_UTC (&extra_time_tm, &intermediate_time_tm, INT_MIN);
	  seconds1 = time_diff (&time_tm, &intermediate_time_tm, INT_MIN);
	  if (1 || (seconds1 != 0))
	    {
	      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
	      time_tm_to_string (&extra_time_tm, &buffer3 [0],
				 sizeof (buffer3));
	      time_tm_to_string (&intermediate_time_tm, &buffer1 [0],
				 sizeof (buffer1));
	      printf ("%s -> %s -> %s.\n", buffer2, buffer3, buffer1);
	    }
	}
    }
  
  if (test_mode == 3)
    {
      time_copy_tm (&local_time_tm, &start_time_tm);
      start_time_tm.tm_year = 1886 - 1900;
      start_time_tm.tm_mon = 11;
      start_time_tm.tm_mday = 31;
      start_time_tm.tm_hour = 18;
      start_time_tm.tm_min = 59;
      start_time_tm.tm_sec = 59;
      time_copy_tm (&start_time_tm, &time_tm);
      time_local_add_years (&time_tm, -1, -1, INT_MIN);
      time_copy_tm (&time_tm, &extra_time_tm);
      time_local_add_years (&extra_time_tm, 1, -1, INT_MIN);
      time_tm_to_string (&start_time_tm, &buffer1 [0], sizeof (buffer1));
      time_tm_to_string (&time_tm, &buffer2 [0], sizeof (buffer2));
      time_tm_to_string (&extra_time_tm, &buffer3 [0], sizeof (buffer3));
      seconds1 = time_diff (&start_time_tm, &time_tm, INT_MIN);
      seconds2 = time_diff (&start_time_tm, &extra_time_tm, INT_MIN);
      printf ("%s->%s is %lld seconds\n",
	      buffer1, buffer2, seconds1);
      printf ("%s->%s->%s is %lld seconds\n",
	      buffer1, buffer2, buffer3, seconds2);
    }

  if (test_mode == 2)
    {
      memset (&start_time_tm, 0, sizeof(start_time_tm));
      start_time_tm.tm_year = 1883 - 1900;
      start_time_tm.tm_mon = 0;
      start_time_tm.tm_mday = 31;
      start_time_tm.tm_hour = 0;
      start_time_tm.tm_min = 0;
      start_time_tm.tm_sec = 0;
      time_UTC_normalize (&start_time_tm, 0, INT_MIN);
      time_UTC_add_months (&start_time_tm, 1, -1, INT_MIN);
      time_UTC_add_days (&start_time_tm, 16, 1, INT_MIN);
      time_copy_tm (&start_time_tm, &prev_time_tm);
      
      for (i=0; i<200; i++)
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
	      ((seconds1 + i) != 2007600) && ((seconds1 + i) != 2094000))
	    {
	      printf ("%d: %s->%s is %lld seconds\n",
		      i, buffer2, buffer4, i+seconds1);
	    }
	  if ((((seconds2 - i) != -2266800) && ((seconds2 - i) != -2180400) &&
	       ((seconds2 - i) != -2007600) && ((seconds2 - i) != -2094000)) ||
	      (seconds3 != 0))
	    {
	      printf ("%d: %s->%s->%s is %lld, %lld seconds\n",
		      i, buffer2, buffer4, buffer3, seconds2 - i, seconds3);
	    }

	  time_copy_tm (&time_tm, &prev_time_tm);
	}
    }

  if (test_mode == 1)
    {
      local_time_tm.tm_year = 2016 - 1900;
      local_time_tm.tm_mon = 2;
      local_time_tm.tm_mday = 13;
      local_time_tm.tm_hour = 1;
      local_time_tm.tm_min = 59;
      local_time_tm.tm_sec = 59;
      nanoseconds = 0;
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("local time: %s\n", &buffer1 [0]);

      time_local_to_UTC (&local_time_tm, &time_tm, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("converted to UTC: %s\n", &buffer1 [0]);
  
      time_UTC_add_seconds (&time_tm, 1, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("forward one second: %s\n", &buffer1 [0]);

      time_UTC_to_local (&time_tm, &local_time_tm, INT_MIN);
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("converted to local time: %s\n", &buffer1 [0]);

      time_local_add_seconds (&local_time_tm, -1, INT_MIN);
      time_tm_to_string (&local_time_tm, &buffer1 [0], sizeof (buffer1));
      printf ("back one second: %s.\n", &buffer1 [0]);
    }
  
  if (test_mode == 0)
    {
      time_tm.tm_year = 2016 - 1900;
      time_tm.tm_mon = 11;
      time_tm.tm_mday = 31;
      time_tm.tm_hour = 23;
      time_tm.tm_min = 59;
      time_tm.tm_sec = 55;
      nanoseconds = 0;

      for (i=1; i<10; i++)
	{
	  add_seconds = 1;
	  add_nanoseconds = 0;
	  time_UTC_add_seconds_ns (&time_tm, &nanoseconds,
				   add_seconds, add_nanoseconds, INT_MIN);
	  time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
	  printf ("%s\n", buffer1);
	}
      
      for (i=1; i<10; i++)
	{
	  add_seconds = -1;
	  add_nanoseconds = 0;
	  time_UTC_add_seconds_ns (&time_tm, &nanoseconds,
				   add_seconds, add_nanoseconds, INT_MIN);
	  time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
	  printf ("%s\n", buffer1);
	}
      
      time_tm.tm_year = 2015 - 1900;
      time_tm.tm_mon = 11;
      time_tm.tm_mday = 31;
      time_tm.tm_hour = 23;
      time_tm.tm_min = 59;
      time_tm.tm_sec = 55;
      nanoseconds = 0;
      
      for (i=1; i<10; i++)
	{
	  add_seconds = 1;
	  add_nanoseconds = 0;
	  time_UTC_add_seconds_ns (&time_tm, &nanoseconds,
				   add_seconds, add_nanoseconds, INT_MIN);
	  time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
	  printf ("%s\n", buffer1);
	}

      time_tm.tm_year = 2016 - 1900;
      time_tm.tm_mon = 11;
      time_tm.tm_mday = 31;
      time_tm.tm_hour = 23;
      time_tm.tm_min = 59;
      time_tm.tm_sec = 60;
      nanoseconds = 0;
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
      printf ("%s\n", buffer1);
      time_copy_tm (&time_tm, &backup_tm);
      time_UTC_add_years (&time_tm, 1, -1, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
      printf ("%s\n", buffer1);
      time_copy_tm (&backup_tm, &time_tm);
      time_UTC_add_years (&time_tm, 1, 1, INT_MIN);
      time_tm_to_string (&time_tm, &buffer1 [0], sizeof(buffer1));
      printf ("%s\n", buffer1);
      
    }
  
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
	       "test_add\n"
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
 
  do_test ();
  exit (EXIT_SUCCESS);

  return 0;
}
