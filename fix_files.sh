#!/bin/bash
# File: fix_files.sh, author: John Sauter, date: April 14, 2020.
#
# Imbedding files in a PDF loses their subdirectory and permissions.
# Restore them.

chmod +x configure
chmod +x autogen.sh
mkdir src
mv int128_to_string.c src/
mv time_copy.c src/
mv time_current_tm.c src/
mv time_current_tm_nano.c src/
mv time_diff.c src/
mv time_length.c src/
mv time_local_add.c src/
mv time_local_normalize.c src/
mv time_local_to_utc.c src/
mv time_sleep_until.c src/
mv time_test_for_disabled_adjtimex.c src/
mv time_tm_nano_to_integer.c src/
mv time_tm_nano_to_string.c src/
mv time_tm_to_integer.c src/
mv time_tm_to_string.c src/
mv time_utc_add.c src/
mv time_utc_normalize.c src/
mv time_utc_to_local.c src/
mkdir examples
mv example_02.c examples/
mv example_03.c examples/
mv example_04.c examples/
mv example_05.c examples/
mv example_06.py examples/
mkdir tests
mv test_add.c tests/
mv test_diff.c tests/
mv test_ep.c tests/
mv test_for_disabled_adjtimex.c tests/
mv test_JDN.c tests/
mv test_local.c tests/
mv test_python_interface.py tests/
mv test_time.c tests/
mv POSIX_epoch.c tests/
mv powers_of_two.c tests/
mv print_DTAI.c tests/
mv sleep_until_midnight.c tests/
mv demo_python.py tests/
mkdir m4
mv libtool.m4 m4/
mv lt~obsolete.m4 m4/
mv ltoptions.m4 m4/
mv ltsugar.m4 m4/
mv ltversion.m4 m4/

# End of file fix_files.sh
