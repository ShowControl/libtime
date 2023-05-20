#!python3
#  -*- coding: utf-8

# Copyright © 2023 by John Sauter <John_Sauter@systemeyescomputerstore.com>

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
  
# Demonstrate the time_subroutines interface.
import ctypes
import time_subroutines

# Make sure the adjtimex function has not been disabled.
if (time_subroutines.time_test_for_disabled_adjtimex() != 0):
    print ('The current time will not be correct during a leap second')
    print ('because the Linux adjtimex function is not working.')
    raise SystemExit
    
# Define two variables of type tm.
tm_1 = time_subroutines.tm()
tm_2 = time_subroutines.tm()

# Fetch the current time.
time_subroutines.time_current_tm(tm_1)

# Express the current time as a string.
string_buffer = ctypes.create_string_buffer(256)
time_subroutines.time_tm_to_string(tm_1, string_buffer,
                                   ctypes.sizeof(string_buffer))
the_time = string_buffer.value.decode("utf-8")

# Print the current time.
print (the_time)

# Convert the current time to the local time zone.
time_subroutines.time_UTC_to_local (tm_1, tm_2, time_subroutines.int_min)

# Express it as a string using the str method, and print it along with
# the day of the week and the day of the year.
weekday_names = ('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday',
                 'Friday', 'Saturday')
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")

# Print the date and time as an integer.
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Create a time value of December 31, 2016, at 23:59:59 UTC.
tm_3 = time_subroutines.tm (tm_year = 2016 - 1900, tm_mon = 12 - 1,
                            tm_mday = 31, tm_hour = 23, tm_min = 59,
                            tm_sec = 59, tm_isdst = 0, tm_gmtoff = 0,
                            tm_zone = None)

# Calculate the day of the week and the day of the month.
time_subroutines.time_UTC_normalize (tm_3, tm_3.tm_sec,
                                     time_subroutines.int_min)

# Print it.
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Add one second.
time_subroutines.time_UTC_add_seconds (tm_3, 1, time_subroutines.int_min)

# And print.  The result should be 2016-12-31T23:59:60.
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# One more second should get us to the next year.
time_subroutines.time_UTC_add_seconds (tm_3, 1, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# End of file demo_python.py
