/*
 * File: test_for_disabled_adjtimex.c, author: John Sauter, 
 * date: December 14, 2019
 * Make sure the Linux adjtimex function works.
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

#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>

int
time_test_for_disabled_adjtimex ()
{
  struct timex current_timex;
  int adjtimex_result;

  /* Fetch time information from the kernel.  */
  current_timex.status = 0;
  current_timex.modes = 0;
  adjtimex_result = adjtimex (&current_timex);

  if (adjtimex_result == -1)
    {
      /* Some high security environments disable the adjtimex function,
       * even when, as here, it is just fetching information.  
       * The adjtimex function is the only way to determine that
       * there is a leap second in progress, so return 1 if it
       * doesn't work.  */
      return (1);
    }

  /* Return 0 to indicate that adjtimex has not been disabled.  */
  return (0);
}
