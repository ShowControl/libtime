#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# Copyright © 2022 by John Sauter.
# Licensed under the Creative Commons Attribution-ShareAlike 4.0 International
# license.  See https://creativecommons.org/license/by-sa/4.0/.

AC_PREREQ([2.69])
AC_INIT([libtime], [2022.03.11],
[John_Sauter@systemeyescomputerstore.com],
[libtime], ["github@github.com:ShowControl/libtime.git"])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/time_diff.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_PATH_PYTHON([3.5])

# Use maintainer mode because files extracted from a PDF file
# lose their modification dates, and we don't want the end user
# to need all the Autoconfigure software.
AM_MAINTAINER_MODE

# On the other hand, building the PDF requires LaTeX and lots of
# its packages.  Build it only if --enable-pdf is specified at
# configure time, or if building the distribution kit.
# This won't be a problem if the sources have been extracted from the PDF,
# since the PDF will already be present.
#
AC_ARG_ENABLE([pdf],
  [AS_HELP_STRING([--enable-pdf],
    [Create the PDF file, with embedded sources])],
  [enable_pdf=yes],
  [enable_pdf=no])

AM_CONDITIONAL([ENABLE_PDF], [test "$enable_pdf" = "yes"])

# We get the extraordinary_days.dat file from the
# proleptic_utc_with_leap_seconds package.  We need to know where
# that package installed the file.  By default, it used the
# same prefix as we are using.
#
AC_ARG_WITH([extraordinary_days],
  [AS_HELP_STRING([--with-extraordinary_days=location of file \
  extraordinary_days.dat])],
  [extraordinary_days_location=${withval}],
  [extraordinary_days_location=${datadir}/proleptic_utc_with_leap_seconds/data])

AC_SUBST(extraordinary_days_location, [${extraordinary_days_location}])

# Libtool
LT_PREREQ([2.2])
LT_INIT()

#
# The version-info below is used by libtool's versioning system.
# It specifies three integers: current, revision and age.
# Each release of a library might change the global functions that
# it supports, by adding new entry points, removing old entry points,
# or changing the arguments of existing entry points.
# The values change on each release, depending on the nature of the changes
# to the library.  Here are the change rules from the GNU Libtool
# documentation:
#
#  1. Start with version information of ‘0:0:0’ for each libtool library.
#  2. Update the version information only immediately before a public
#     release of your software. More frequent updates are unnecessary,
#     and only guarantee that the current interface number gets larger faster.
#  3. If the library source code has changed at all since the last update,
#     then increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
#  4. If any interfaces have been added, removed, or changed since the last
#     update, increment current, and set revision to 0.
#  5. If any interfaces have been added since the last public release,
#     then increment age.
#  6. If any interfaces have been removed or changed since the last public
#     release, then set age to 0.
#
# The following explanation may help to understand the above rules a bit
# better: consider that there are three possible kinds of reactions from
# users of your library to changes in a shared library:
#
#  1. Programs using the previous version may use the new version as drop-in
#     replacement, and programs using the new version can also work with the
#     previous one. In other words, no recompiling nor relinking is needed.
#     In this case, bump revision only, don’t touch current nor age.
#  2. Programs using the previous version may use the new version as drop-in
#     replacement, but programs using the new version may use APIs not present
#     in the previous one. In other words, a program linking against the new
#     version may fail with “unresolved symbols” if linking against the old
#     version at runtime: set revision to 0, bump current and age.
#  3. Programs may need to be changed, recompiled, and relinked in order to
#     use the new version. Bump current, set revision and age to 0.
#
# See the NEWS file (and the update history in the PDF) for the history
# of the version-info.  When these values are changed, the parallel values
# in the libtime.spec file must also be changed.
#
AC_SUBST(libtool_version_current,"5")
AC_SUBST(libtool_version_revision, "70")
AC_SUBST(libtool_version_age, "2")

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_RANLIB

AM_PATH_PYTHON ([/usr/bin/python3])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h sys/timex.h unistd.h stdio.h \
   limits.h getopt.h stdarg.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_MKTIME
AC_CHECK_FUNCS([clock_gettime localtime_r memset strerror adjtimex])

# Check for support of 128-bit integers.
# Without such support, two subroutines are omitted from the library.
# The user can tell configure to omit the 128-bit subroutines,
# but by detault they are included if the C compiler supports the data type.
AC_ARG_ENABLE([int128],
	[AS_HELP_STRING([--enable-int128], [use 128-bit integers])],
	[case "${enableval}" in
	yes) try_int128=TRUE ;;
	no) try_int128=FALSE ;;
	*) AC_MSG_ERROR([bad value ${enableval} for --enable-int128]) ;;
	esac],[try_int128=default])
AS_IF([test "x$try_int128" = xdefault],
	    [AC_CHECK_TYPE([__int128],
		[try_int128=TRUE], [try_int128=FALSE])])
AC_DEFINE_UNQUOTED([USE_INT128], [$try_int128],
				 [Define as TRUE to use 128-bit integers])
AM_CONDITIONAL([USE_int128], [test $try_int128 = TRUE])

# In time_subroutines.h, comment out the definitions that depend on
# 128-bit integers if the type is not supported or not wanted.
AS_IF([test $try_int128 = TRUE],
	    [AC_SUBST(USE_int128_C,[])],
	    [AC_SUBST(USE_int128_C,[//])])
# Do the same for time_subroutines.py.
AS_IF([test $try_int128 = TRUE],
	    [AC_SUBST(USE_int128_Python,[])],
	    [AC_SUBST(USE_int128_Python,[#])])
	    
AC_CONFIG_FILES([Makefile libtime.pc time_subroutines.h time_subroutines.py])
AC_OUTPUT

# End of file configure.ac
