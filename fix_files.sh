#!/bin/bash
# File: fix_files.sh, author: John Sauter, date: June 26, 2017
#
# Imbedding files in a PDF loses their subdirectory and permissions.
# Restore them.

chmod +x configure
mkdir m4
mv libtool.m4 m4/
mv lt~obsolete.m4 m4/
mv ltoptions.m4 m4/
mv ltsugar.m4 m4/
mv ltversion.m4 m4/

# End of file fix_files.sh
