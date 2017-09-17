#!/bin/bash
rm time2.txt
ntpq -n -p >ntpq21.txt
ntpq -c rv >ntpq22.txt
export LD_LIBARY_PATH=.:$LD_LIBRARY_PATH
./test_time -r 62 -d 0.1 -o time2.txt
ntpq -n -p >ntpq23.txt
ntpq -c rv >ntpq24.txt

# End of file record.sh
