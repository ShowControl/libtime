#!/bin/bash
# File: verify_files_template.sh, author: John Sauter, date: November 18, 2017. 
# This file is executed as verify_files.sh to check libtime.

diff -q check_time_output.txt check_time_expected_output.txt
diff_result=$?
if [[ $diff_result -ne 0 ]]; then
  exit $diff_result
fi

# End of file verify_files_template.sh
