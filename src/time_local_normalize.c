/*
 * File: time_local_normalize.c, author: John Sauter, date: November 11, 2018.
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

#include "time_subroutines.h"

/* Make a modified tm structure containing local time have all of 
 * its elements in range.  We carry the tm_sec field separately as 
 * a long long int so the caller can make large adjustments to the 
 * field.  */
int
time_local_normalize (struct tm *time_tm, long long int in_seconds,
		      int variable_length_seconds_before_year)
{
  int return_value = 0;
  long long int seconds;
  struct tm local_tm;

  seconds = in_seconds;
  
  /* The body of time_local_normalize is an infinite loop so we can
   * say "continue" to restart the algorithm.  The loop terminates
   * only when all of the tests have passed without manipulating
   * any of the fields.  */
  for (;;)
    {
      /* We test the fields from high-order to low-order, because
       * sometimes the valid range of a field depends on the values 
       * of the higher-order fields.  */

      /* Years has no limits.  */

      /* Months are limited to 0 to 11.  If the month is 
       * out of range, adjust it and compensate by adjusting 
       * the year.  */
      if (time_tm->tm_mon < 0)
	{
	  time_tm->tm_mon = time_tm->tm_mon + 12;
	  time_tm->tm_year = time_tm->tm_year - 1;
	  continue;
	}
      
      if (time_tm->tm_mon > 11)
	{
	  time_tm->tm_mon = time_tm->tm_mon - 12;
	  time_tm->tm_year = time_tm->tm_year + 1;
	  continue;
	}

      /* The valid range of the day of the month depends
       * on the month and the year.  We walk forward or back 
       * through the months one at a time because we need to 
       * measure their lengths.  */
      if (time_tm->tm_mday < 1)
	{
	  time_tm->tm_mday = time_tm->tm_mday +
	    time_length_prev_month (time_tm);
	  time_tm->tm_mon = time_tm->tm_mon - 1;
	  continue;
	}

      if (time_tm->tm_mday > time_length_month (time_tm))
	{
	  time_tm->tm_mday =
	    time_tm->tm_mday - time_length_month (time_tm);
	  time_tm->tm_mon = time_tm->tm_mon + 1;
	  continue;
	}

      /* Hours.  There are always 24 hours in a day.  */
      if (time_tm->tm_hour < 0)
	{
	  time_tm->tm_hour = time_tm->tm_hour + 24;
	  time_tm->tm_mday = time_tm->tm_mday - 1;
	  continue;
	}

      if (time_tm->tm_hour > 23)
	{
	  time_tm->tm_hour = time_tm->tm_hour - 24;
	  time_tm->tm_mday = time_tm->tm_mday + 1;
	  continue;
	}

      /* Minute.  There are always 60 minutes in an hour.  */
      if (time_tm->tm_min < 0)
	{
	  time_tm->tm_min = time_tm->tm_min + 60;
	  time_tm->tm_hour = time_tm->tm_hour - 1;
	  continue;
	}

      if (time_tm->tm_min > 59)
	{
	  time_tm->tm_min = time_tm->tm_min - 60;
	  time_tm->tm_hour = time_tm->tm_hour + 1;
	  continue;
	}

      /* Seconds.  There are 59, 60 or 61 seconds in a minute,
       * depending on the year, month, day, hour and minute.
       * All of those fields are now in their valid range,
       * so we can compute the number of seconds in this
       * minute.  Note that seconds is not carried in the tm
       * structure but passed in separately, so it can be a
       * long long int.  When time_local_normalize completes, 
       * tm_sec has been set to a valid value.  */
      if (seconds < 0)
	{
	  seconds = seconds +
	    time_length_prev_local_minute (time_tm,
			   variable_length_seconds_before_year);
	  time_tm->tm_min = time_tm->tm_min - 1;
	  continue;
	}

      if (seconds >=
	  time_length_local_minute (time_tm,
			    variable_length_seconds_before_year))
	{
	  seconds = seconds -
	    time_length_local_minute (time_tm,
			    variable_length_seconds_before_year);
	  time_tm->tm_min = time_tm->tm_min + 1;
	  continue;
	}
      break;
    }

  /* Now that the seconds value is in its proper range
   * we can store it in the tm structure.  */
  time_tm->tm_sec = seconds;

  /* Use mktime to compute the other fields, given
   * tm_year, tm_mon, tm_mday, tm_hour, tm_min,
   * tm_sec and tm_gmtoff.  We set the seconds to 0
   * so that mktime won't be presented with a leap
   * second.  */
  time_copy_tm (time_tm, &local_tm);
  local_tm.tm_sec = 0;
  mktime (&local_tm);

  /* Copy back the information returned by mktime.
   * If the time zone changed, the hour likely changed,
   * and possibly the year, month, day and minute.
   * Daylight Saving Time always occurs on minute
   * boundaries, so the seconds won't have changed.  */
  time_tm->tm_year = local_tm.tm_year;
  time_tm->tm_mon = local_tm.tm_mon;
  time_tm->tm_mday = local_tm.tm_mday;
  time_tm->tm_hour = local_tm.tm_hour;
  time_tm->tm_min = local_tm.tm_min;
  time_tm->tm_yday = local_tm.tm_yday;
  time_tm->tm_wday = local_tm.tm_wday;
  time_tm->tm_isdst = local_tm.tm_isdst;
  time_tm->tm_zone = local_tm.tm_zone;
  time_tm->tm_gmtoff = local_tm.tm_gmtoff;
  
  return (return_value);
}
