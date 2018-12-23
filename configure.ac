#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([time], [2018.12.23], [John_Sauter@systemeyescomputerstore.com],
[libtime],["github@github.com:ShowControl/libtime.git"])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([time_diff.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

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

# Libtool
LT_PREREQ([2.2])
LT_INIT()

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_RANLIB

AM_PATH_PYTHON ([/usr/bin/python3])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_MKTIME
AC_CHECK_FUNCS([clock_gettime localtime_r memset strerror])

AC_CONFIG_FILES([Makefile libtime.pc])
AC_OUTPUT
