#!python3
#  -*- coding: utf-8
   
# Test the Python interface to libtime.
# John Sauter, April 28, 2019

import ctypes
import time_subroutines
import pprint

# The TM structure on GNU/Linux is 56 bytes in length.  If your
# computer uses a different size modify the definition in time_subroutines.py
# to conform.
print("tm length = " + str(ctypes.sizeof(time_subroutines.tm())))

# Similarly, the size and offset of each field must match the definition
# used by the C compiler.
for field_name, field_type in time_subroutines.tm._fields_:
    print (field_name, getattr(time_subroutines.tm, field_name))

# Here are the lengths of the various types used in ctypes.
print("int length = " + str(ctypes.sizeof(ctypes.c_int)))
print("long length = " + str(ctypes.sizeof(ctypes.c_long)))
print("long long length = " + str(ctypes.sizeof(ctypes.c_longlong)))

# The values of int_min and int_max must match the values of INT_MIN
# and INT_MAX used by the C compiler.
print ("int_min = " + str(time_subroutines.int_min))
print ("int_max = " + str(time_subroutines.int_max))

# Define a 128-bit integer.
i128 = time_subroutines.int128()

# Set its value.
i128.low_bits = 0
i128.high_bits = 1

# print it.
print("2**64 = " + str(i128))

# Define two variables of type tm.
tm_1 = time_subroutines.tm()
tm_2 = time_subroutines.tm()

# Fetch the current time.
time_subroutines.time_current_tm(tm_1)

# Copy it to the other variable.
time_subroutines.time_copy_tm(tm_1, tm_2)

# Express the current time as a string.
string_buffer = ctypes.create_string_buffer(256)
time_subroutines.time_tm_to_string(tm_2, string_buffer,
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

# Print the current time with nanoseconds.
return_value, nanoseconds = time_subroutines.time_current_tm_nano (tm_1)
print (str(tm_1) + " and " + str(nanoseconds) + " nanoseconds.")

# How many seconds has it been since December 31, 2016 at 23:59:60?

# Create a time value of December 31, 2016, at 23:59:60 UTC.
tm_3 = time_subroutines.tm (tm_year = 2016 - 1900, tm_mon = 12 - 1,
                            tm_mday = 31, tm_hour = 23, tm_min = 59,
                            tm_sec = 60, tm_isdst = 0, tm_gmtoff = 0,
                            tm_zome = None)

print ("It has been " +
       str(time_subroutines.time_diff (tm_3, tm_1, time_subroutines.int_min)) +
           " seconds since " + str(tm_3) + ".")

time_subroutines.time_UTC_to_local (tm_3, tm_2, time_subroutines.int_min)
print ("That minute is " +
       str(time_subroutines.time_length_local_minute (tm_2,
                                                    time_subroutines.int_min)) +
       " seconds long.")
print ("That month is " +
       str(time_subroutines.time_length_month (tm_2)) +
       " days long.")
print ("The previous minute is " +
       str(time_subroutines.time_length_prev_local_minute (tm_2,
                                                    time_subroutines.int_min)) +
       " seconds long.")
print ("The previous month is " +
       str(time_subroutines.time_length_prev_month (tm_2)) +
       " days long.")
print ("The minute, computed from the UTC representation, is " +
       str(time_subroutines.time_length_UTC_minute (tm_3,
                                                    time_subroutines.int_min)) +
       " seconds long.")
print ("The previous minute, computed from the UTC representation, is " +
       str(time_subroutines.time_length_prev_UTC_minute (tm_3,
                                                    time_subroutines.int_min)) +
       " seconds long.")

# Calculate a Julian Day Number.
JDN = time_subroutines.time_Julian_day_number (2016, 12, 31)
print ("December 31, 2016, is Julian day number " + str(JDN) + ".")

# Now compute DTAI for that day and for the next.
print ("DTAI for that day is " +
       str(time_subroutines.time_DTAI (JDN, time_subroutines.int_min)) +
       " and for the next is " +
       str(time_subroutines.time_DTAI (JDN+1, time_subroutines.int_min)) +
       ".")
                                                                 
# Calculate the day of the week and the day of the month.
time_subroutines.time_UTC_normalize (tm_3, tm_3.tm_sec,
                                     time_subroutines.int_min)

# Print it.
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")

# Print the date and time as an integer.
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Subtract one second.
time_subroutines.time_UTC_add_seconds (tm_3, -1, time_subroutines.int_min)

# And print.  The result should be 2016-12-31T23:59:59.
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Add two seconds to get us to the next year.
time_subroutines.time_UTC_add_seconds (tm_3, 2, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Back up one day.
time_subroutines.time_UTC_add_days (tm_3, -1, 1, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Back up one hour.
time_subroutines.time_UTC_add_hours (tm_3, -1, 1, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Forward 120 minutes.
time_subroutines.time_UTC_add_minutes (tm_3, 120, 1, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Forward 1 second.
time_subroutines.time_UTC_add_seconds (tm_3, 1, time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")

# Add some nanoseconds.
return_value, result_nanoseconds = time_subroutines.time_UTC_add_seconds_ns (
    tm_3, 1, int(5.5*(10**9)), time_subroutines.int_min)
print (str(tm_3) + ", day of week = " + weekday_names[tm_3.tm_wday] +
       ", day of year = " + str(tm_3.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_3)
print ("Corresponding integer = " + str(now_int) + ".")
print ("And " + str(result_nanoseconds) + " nanoseconds.")

# Convert to local time.
time_subroutines.time_UTC_to_local (tm_3, tm_2, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Add one day.
time_subroutines.time_local_add_days (tm_2, 1, 1, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Subtract one hour..
time_subroutines.time_local_add_hours (tm_2, -1, 1, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Subtract one minute.
time_subroutines.time_local_add_minutes (tm_2, -1, 1, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Add 52 seconds.
time_subroutines.time_local_add_seconds (tm_2, 52, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Add (10**9)-1 nanoseconds.
return_value, result_nanoseconds = time_subroutines.time_local_add_seconds_ns (
    tm_2, 0, (10**9)-1, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")
print ("And " + str(result_nanoseconds) + " nanoseconds.")

# Add two more nanoseconds.
return_value, result_nanoseconds = time_subroutines.time_local_add_seconds_ns (
    tm_2, 0, result_nanoseconds+2, time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")
print ("And " + str(result_nanoseconds) + " nanoseconds.")
return_value, i128 = time_subroutines.time_tm_nano_to_integer (tm_2,
                                                            result_nanoseconds)
print ("Corresponding integer which includes nanoseconds = " + str(i128) + ".")

# Normalize.
time_subroutines.time_local_normalize (tm_2, tm_2.tm_sec,
                                       time_subroutines.int_min)
print (str(tm_2) + ", day of week = " + weekday_names[tm_2.tm_wday] +
       ", day of year = " + str(tm_2.tm_yday) + ".")
return_value, now_int = time_subroutines.time_tm_to_integer (tm_2)
print ("Corresponding integer = " + str(now_int) + ".")

# Sleep for five seconds.
time_subroutines.time_current_tm (tm_1)
time_subroutines.time_UTC_add_seconds (tm_1, 5, time_subroutines.int_min)
print ("Sleeping until " + str(tm_1) + ".")
time_subroutines.time_sleep_until (tm_1, 0, time_subroutines.int_min)

# End of file test_python_interface.py
