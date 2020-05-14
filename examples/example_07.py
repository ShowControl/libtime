#!python3
#  -*- coding: utf-8

# Copyright © 2020 by John Sauter <John_Sauter@systemeyescomputerstore.com>

#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.

#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

#   The author's contact information is as follows:
#     John Sauter
#     System Eyes Computer Store
#     20A Northwest Blvd.  Ste 345
#     Nashua, NH  03063-4066
#     telephone: (603) 424-1188
#     e-mail: John_Sauter@systemeyescomputerstore.com
  
# Example 7: Compute the arrival time of a flight to Tehran.

import ctypes
import time
import time_subroutines

def example_7 (flight_time):
        
    # Construct the departure date of December 31, 1978, at 12:01 local time.
    time1_tm = time_subroutines.tm()
    time_subroutines.time_current_tm(time1_tm)
    time1_tm.tm_year = 1978 - 1900
    time1_tm.tm_mon = 12 - 1
    time1_tm.tm_mday = 31
    time1_tm.tm_hour = 12
    time1_tm.tm_min = 1
    time1_tm.tm_sec = 0
    time5 = time.struct_time((time1_tm.tm_year + 1900, time1_tm.tm_mon + 1,
                              time1_tm.tm_mday, time1_tm.tm_hour,
                              time1_tm.tm_min, time1_tm.tm_sec,
                              (time1_tm.tm_wday + 6) % 7,
                              time1_tm.tm_yday + 1, time1_tm.tm_isdst))
    buffer1 = time.strftime ("%A, %B %d, %Y, %I:%M:%S %p %Z", time5)
    print ("The flight leaves here on " + buffer1 + ".")
    
    # Convert to UTC.
    time2_tm = time_subroutines.tm()
    time_subroutines.time_local_to_UTC (time1_tm, time2_tm,
                                        time_subroutines.int_min)
    # Add the flight time.
    time3_tm = time_subroutines.tm()
    time_subroutines.time_copy_tm (time2_tm, time3_tm)
    time_subroutines.time_UTC_add_seconds (time3_tm, flight_time,
                                           time_subroutines.int_min)

    # Convert to local time in Tehran.  Tehran's time zone is +3:30.
    time4_tm = time_subroutines.tm()
    time_subroutines.time_UTC_to_foreign_local (time3_tm,
                                                ((3 * 3600) + (30 * 60)),
                                                time4_tm,
                                                time_subroutines.int_min)

    time5 = time.struct_time((time4_tm.tm_year + 1900, time4_tm.tm_mon + 1,
                              time4_tm.tm_mday, time4_tm.tm_hour,
                              time4_tm.tm_min, time4_tm.tm_sec,
                              (time4_tm.tm_wday + 6) % 7,
                              time4_tm.tm_yday + 1, time4_tm.tm_isdst))
    
    # Convert to a string using strftime and print.
    buffer1 = time.strftime ("%A, %B %d, %Y, %I:%M:%S %p IRST", time5)
    print ("The flight arrives in Tehran on " + buffer1 + ".")
    return

# The flight time is quoted as 14 hours and 44 minutes, but that assumes
# that all minutes are 60 seconds in length.  In fact, the flight time
# is the same no matter what the clocks say, so we express the flight
# time as a number of seconds.
example_7 ((14 * 3600) + (44 * 60))

# End of file example_07.py
