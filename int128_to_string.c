/*
 * File: int128_to_string.c, author: John Sauter, date: May 7, 2017.
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

#include "time_subroutines.h"

/* Convert a 128-bit integer to a string.  
 * Return value is the number of characters written into the string,
 * not counting the trailing NUL character.  
 * Value is the integer to be converted.
 * Result is a pointer to a string; result_size is its length.  
 * The string should be at least 41 characters long, to hold a
 * maximum-sized result.  */

/* This subroutine is dedicated to Steve Russell, who wrote the
 * original recursive decimal print subroutine for the DEC PDP-1
 * in 1964.  */
int
int128_to_string (__int128 value, char *result, int result_size)
{
  __int128 positive_value;
  __int128 remaining_digits;
  int size_remaining;
  int character_count;
  int remaining_count;
  int current_digit;
  char *result_pointer;
  volatile __int128 volatile_value;
  
  size_remaining = result_size;
  result_pointer = result;
  character_count = 0;

  /* If the value is negative, produce a minus sign
   * and then produce its absolute value.  */
  if (value < 0)
    {
      positive_value = -value;
      if (size_remaining > 0)
	{
	  *result_pointer = '-';
	  result_pointer = result_pointer + 1;
	  size_remaining = size_remaining - 1;
	  character_count = character_count + 1;
	}
    }
  else
    {
      positive_value = value;
    }

  /* If the value is zero, produce a 0 and return.  */
  if (positive_value == 0)
    {
      if (size_remaining > 1)
	{
	  *result_pointer = '0';
	  result_pointer = result_pointer + 1;
	  size_remaining = size_remaining - 1;
	  character_count = character_count + 1;
	  *result_pointer = 0;
	  size_remaining = size_remaining - 1;
	}
      return (character_count);
    }

  /* Separate the positive value into its low-order digit and
   * the remaining high-order digits.  We must handle -2**127
   * as a special case because it does not negate.  */
  
  /* If the positive value is less than 0, we have -2**127.  
   * We use a volatile variable to force the compiler to execute
   * the test, because otherwise the compiler might assume
   * that a negative value, after being negated, is necessarily 
   * positive.  */
  volatile_value = positive_value;
  if (volatile_value < 0) 
    {
      current_digit = 8;
      /* The compiler will not let us say
      remaining_digits = 
	(__int128) 17014118346046923173168730371588410572;
	so we build the constant from parts.  */
      remaining_digits = ((__int128) 1701411834604692317 *
			  (__int128) 1e19)
	+ (__int128) 3168730371588410572;
    }
  else
    {
      current_digit = positive_value % 10;
      remaining_digits = positive_value / 10;
    }
  
  /* If we have any high-order digits, produce them.  We can call 
   * this subroutine recursively because we know the value we are 
   * passing is positive and is less than the positive value passed 
   * to us, so the recursion will terminate.  */
  if (remaining_digits != 0)
    {
      remaining_count =
	int128_to_string (remaining_digits,
			  result_pointer, size_remaining);
      result_pointer = result_pointer + remaining_count;
      size_remaining = size_remaining - remaining_count;
      character_count = character_count + remaining_count;
    }

  /* Having produced the digits of higher order than the low-order
   * digit of this value, append that low-order digit to the string.  
   */
  if (size_remaining > 1)
    {
      *result_pointer = '0' + current_digit;
      result_pointer = result_pointer + 1;
      size_remaining = size_remaining - 1;
      character_count = character_count + 1;

      /* If we are being called recursively, the following NUL byte
       * will be overwritten by the next digit.  */
      *result_pointer = 0;
      size_remaining = size_remaining - 1;
    }

  /* By returning the number of characters we wrote into the string,
   * not counting the trailing NUL byte, we make the recursive call
   * simpler since it has to find the end of the string.  */
  return (character_count);
}
