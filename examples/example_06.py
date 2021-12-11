#!python3
#  -*- coding: utf-8

# Copyright © 2021 by John Sauter <John_Sauter@systemeyescomputerstore.com>

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
  
# Example 6: Display PTP time stamps using Python.

import ctypes
import time
import time_subroutines

def example_6 (time_stamp_value):
    
    # Construct the base date of January 1, 1972, at midnight.
    time1_tm = time_subroutines.tm (
        tm_year = 1972 - 1900, tm_mon = 1 - 1, tm_mday = 1,
        tm_hour = 0, tm_min = 0, tm_sec = 0, tm_isdst = 0,
        tm_gmtoff = 0, tm_zone = None)

    # Add the time stamp, which is an integer.  Note that the epoch
    # for SMPTE ST-2059-2 is 63,072,010 seconds before the base date.
    # PTP always uses fixed-length (SI) seconds.
    time2_tm = time_subroutines.tm()
    time_subroutines.time_copy_tm (time1_tm, time2_tm)
    time_subroutines.time_UTC_add_seconds (
        time2_tm, time_stamp_value - 63072010,
        time_subroutines.int_min)

    # Convert to local time.
    time3_tm = time_subroutines.tm()
    time_subroutines.time_UTC_to_local (
        time2_tm, time3_tm, time_subroutines.int_min)

    # Convert the POSIX tm structure to the Python struct_time structure.
    # Add 1900 to tm_year and 1 to tm_mon.  For tm_wday, struct_time has
    # 0 meaning Monday whereas POSIX has 0 meaning Sunday, thus add 6
    # modulo 7.  For tm_yday, struct_time is 1-366 whereas POSIX is 0-365,
    # so add 1.
    time4 = time.struct_time(
        (time3_tm.tm_year + 1900, time3_tm.tm_mon + 1,
         time3_tm.tm_mday, time3_tm.tm_hour,
         time3_tm.tm_min, time3_tm.tm_sec,
         (time3_tm.tm_wday + 6) % 7,
         time3_tm.tm_yday + 1, time3_tm.tm_isdst))
    
    # Convert to a string using strftime and print.
    buffer1 = time.strftime ("%A, %B %d, %Y, %I:%M:%S %p %Z", time4)
    print ("PTP " + str(time_stamp_value) + " displays as " +
           buffer1 + ".")
    return

example_6 (0)
example_6 (15638408)
example_6 (47174409)
example_6 (63072010)

# End of file example_06.py
