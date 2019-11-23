#!/bin/bash
# File: verify_files_template.sh, author: John Sauter, date: November 20, 2019. 
# This file is executed as verify_files.sh to check libtime.

diff check_time_output.txt check_time_expected_output.txt
diff_result=$?
if [[ $diff_result -ne 0 ]]; then
  exit $diff_result
fi

# End of file verify_files_template.sh
