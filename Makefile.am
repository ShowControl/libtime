# File: Makefile.am for libtime, author: John Sauter, date: March 18, 2021.
#
# Copyright © 2021 by John Sauter.
# Licensed under the Creative Commons Attribution-ShareAlike 4.0 International
# license.  See https://creativecommons.org/license/by-sa/4.0/.

# Place object files in the same subdirectory as their source files.
AUTOMAKE_OPTIONS = subdir-objects

# We are building libtime.
lib_LTLIBRARIES = libtime.la

# These are the sources from which we will build the library.
libtime_la_SOURCES = src/time_copy.c \
	src/time_current_tm.c \
	src/time_current_tm_nano.c \
	src/time_diff.c \
	src/time_length.c \
	src/time_local_add.c \
	src/time_local_normalize.c \
	src/time_local_to_utc.c \
	src/time_sleep_until.c \
	src/time_test_for_disabled_adjtimex.c \
	src/time_tm_nano_to_string.c \
	src/time_tm_to_integer.c \
	src/time_tm_to_string.c \
	src/time_utc_add.c \
	src/time_utc_normalize.c \
	src/time_utc_to_local.c 

# Conditionally include the subroutines which require 128-bit integers.
if USE_int128
libtime_la_SOURCES += src/time_tm_nano_to_integer.c \
	src/int128_to_string.c 
endif

# In addition to the files named above, also include the following files
# in the distribution.  Note that we include extraordinary_days.dat
# even though it won't be used, to document the source of our data.
EXTRA_DIST = read_extraordinary_days_table.py \
	tests/demo_python.py \
	tests/test_python_interface.py \
	examples/example_06.py \
	examples/example_07.py \
	extraordinary_days.dat \
	avoid_time_t.tex \
	references.bib \
	legalcode.txt \
	autogen.sh John_Sauter_public_key.asc

# Also distribute the text for the man command.
dist_man_MANS = libtime.3

# The files dtai_table.h and dtai_table.tab are built from
# file extraordinary_days.dat.  They are used by time_diff.
# The following line is commented out because the two lines
# following it are sufficient to build the dtai_table files
# before they are needed.
#BUILT_SOURCES = dtai_table.h dtai_table.tab 
src/time_diff.${OBJEXT}: dtai_table.h dtai_table.tab
src/time_diff.lo: dtai_table.h dtai_table.tab

# When removing files, also remove the following:
CLEANFILES = dtai_table.h dtai_table.tab example_03_output.txt \
avoid_time_t.out avoid_time_t.brf avoid_time_t.log avoid_time_t.aux \
avoid_time_t.blg avoid_time_t.bbl avoid_time_t.pdf check_time_output.txt \
verify_files.sh extraordinary_days.dat

clean-local:
	rm -rf _minted-avoid_time_t/
	rm -rf __pycache__
	rm -rf autom4te.cache
	rm -f *~

# Place support files in the m4 directory.
ACLOCAL_AMFLAGS = -I m4

# Include the following documentation files in the distribution.
dist_doc_DATA = \
	README \
	COPYING \
	LICENSE \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

# Conditionally build the PDF.
if ENABLE_PDF
  dist_doc_DATA += avoid_time_t.pdf
endif

# Build but do not distribute the following programs:
noinst_PROGRAMS = example_02 example_03 example_04 example_05 \
test_time test_diff test_JDN \
test_for_disabled_adjtimex \
test_add test_ep test_local print_DTAI sleep_until_midnight POSIX_epoch \
check_time

# The powers of 2 program requires 128-bit integers
if USE_int128
noinst_PROGRAMS += powers_of_two
endif

# Here is the location of the source file, the dependency information
# and the linking process for the above programs.
example_02_SOURCES = examples/example_02.c
example_03_SOURCES = examples/example_03.c
example_04_SOURCES = examples/example_04.c
example_05_SOURCES = examples/example_05.c
example_05_LDADD = libtime.la
example_04_LDADD = libtime.la
example_03_LDADD = libtime.la
example_02_LDADD = libtime.la
POSIX_epoch_SOURCES = tests/POSIX_epoch.c
print_DTAI_SOURCES = tests/print_DTAI.c
test_add_SOURCES = tests/test_add.c
test_ep_SOURCES = tests/test_ep.c
test_local_SOURCES = tests/test_local.c
tests/test_local.lo: dtai_table.h dtai_table.tab
tests/test_local.${OBJEXT}: dtai_table.h dtai_table.tab
test_struct_time_SOURCES = tests/test_struct_time.py
powers_of_two_SOURCES = tests/powers_of_two.c
sleep_until_midnight_SOURCES = tests/sleep_until_midnight.c
test_diff_SOURCES = tests/test_diff.c
test_JDN_SOURCES = tests/test_JDN.c
test_for_disabled_adjtimex_SOURCES = tests/test_for_disabled_adjtimex.c
test_for_disabled_adjtimex_LDADD = libtime.la
test_python_interface_SOURCES = tests/test_python_interface.py
test_time_SOURCES = tests/test_time.c
test_time_LDADD = libtime.la
test_JDN_LDADD = libtime.la
test_diff_LDADD = libtime.la
test_add_LDADD = libtime.la
test_ep_LDADD = libtime.la
print_DTAI_LDADD = libtime.la
sleep_until_midnight_LDADD = libtime.la
POSIX_epoch_LDADD = libtime.la
powers_of_two_LDADD = libtime.la
check_time_LDADD = libtime.la

# Distribute the python interface code.
python_PYTHON = time_subroutines.py

# The Libtool version-info integers are defined in configure.ac.
libtime_la_LDFLAGS = -version-info @libtool_version_current@:@libtool_version_revision@:@libtool_version_age@

# The spec file is used to build RPMs for the Centos, epel, Fedora and
# OpenSUSE-tumbleweed distributions of GNU/Linux: libtime, libtime-devel,
# libtime-doc and two debug RPMs.
EXTRA_DIST += libtime.spec

# fix_files.sh is used to restore directories and permissions when needed
# for files extracted from the PDF.
EXTRA_DIST += fix_files.sh

# Distribute the C interface definitions.
include_HEADERS = time_subroutines.h

# Distribute the shell script which builds the RPMs.
EXTRA_DIST += build_RPMs.sh

# Run it if the developer types "make RPMs".
.phoney: RPMs
RPMs:
	bash build_RPMs.sh

# Here is how to build dtai_table.tab and dtai_table.h.
# Note that the file extraordinary_days.dat is provided by package
# proleptic_utc_with_leap_seconds.
${builddir}/extraordinary_days.dat : \
${extraordinary_days_location}/extraordinary_days.dat
	cp ${extraordinary_days_location}/extraordinary_days.dat \
${builddir}/extraordinary_days.dat

dtai_table.tab: read_extraordinary_days_table.py \
${extraordinary_days_location}/extraordinary_days.dat
	python3 $(srcdir)/read_extraordinary_days_table.py \
$(extraordinary_days_location)/extraordinary_days.dat \
--c-output-tab=$(builddir)/dtai_table.tab

dtai_table.h: read_extraordinary_days_table.py \
${extraordinary_days_location}/extraordinary_days.dat
	python3 $(srcdir)/read_extraordinary_days_table.py \
$(extraordinary_days_location)/extraordinary_days.dat \
--c-output-h=$(builddir)/dtai_table.h

# Build the PDF on demand, if requested.
pdf-local: avoid_time_t.pdf fix_files.sh 
.PHONEY: pdf-local

# Building the PDF depends on the bbl file, which is built below.
# When the bbl file is available, run LaTeX four times to be sure
# the forward references are resolved and the pdf byte count correct.
# There is a slight chance that this process will be unstable,
# with the size of a reference changing the pagination, thus changing
# the size of a reference to that page.
# Perhaps someday we will check that this is not happening
# by verifying that the size of the last and next-to-last runs match.
avoid_time_t.pdf : avoid_time_t.tex avoid_time_t.bbl dtai_table.h \
	dtai_table.tab time_subroutines.h time_subroutines.py \
	examples/example_02.c examples/example_03.c examples/example_06.py \
	examples/example_07.py \
	example_03_output.txt libtime.pc extraordinary_days.dat \
	$(libtime_la_SOURCES)
	export TEXINPUTS="$(srcdir):" && lualatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && lualatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && lualatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && lualatex -shell-escape $(srcdir)/avoid_time_t.tex

# Build the bbl file by running LaTeX without it, then running bibtex.
avoid_time_t.bbl : avoid_time_t.tex references.bib examples/example_02.c \
	examples/example_03.c examples/example_06.py examples/example_07.py \
	dtai_table.h dtai_table.tab time_subroutines.h time_subroutines.py \
	example_03_output.txt libtime.pc extraordinary_days.dat \
	$(libtime_la_SOURCES)
	export TEXINPUTS="$(srcdir):" && lualatex -shell-escape $(srcdir)/avoid_time_t.tex
	export BIBINPUTS="$(srcdir):" && kpsewhich -var-value BIBINPUTS && bibtex $(builddir)/avoid_time_t.aux

# The output of example 3 is presented in the document.
example_03_output.txt : example_03
	$(builddir)/example_03 >$(builddir)/example_03_output.txt

# Include the signature file in the repository
signature : ${PACKAGE}-${VERSION}.tar.gz.asc
.PHONEY: signature

${PACKAGE}-${VERSION}.tar.gz.asc : ${PACKAGE}-${VERSION}.tar.gz
	rm -f ${PACKAGE}-*.tar.gz.asc
	gpg2 --detach-sign --armor ${PACKAGE}-${VERSION}.tar.gz

# Support pgk-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtime.pc

# Support make check and make distcheck
dist_check_DATA = check_time_output.txt check_time_expected_output.txt
dist_check_SCRIPTS = verify_files_template.sh

# Pass along the location of the extraordinary_days.dat file.
AM_DISTCHECK_CONFIGURE_FLAGS = \
--with-extraordinary_days=${extraordinary_days_location}

# Pass along the dependence on 128-bit integers in the C compiler.
if USE_int128
AM_DISTCHECK_CONFIGURE_FLAGS += --enable-int128
else
AM_DISTCHECK_CONFIGURE_FLAGS += --disable-int128
endif

TESTS = verify_files.sh
verify_files.sh : verify_files_template.sh check_time_output.txt \
test_for_disabled_adjtimex${EXEEXT}
	cp $(srcdir)/verify_files_template.sh verify_files.sh
	if [ ! -r "check_time_expected_output.txt" ] ; then cp $(srcdir)/check_time_expected_output.txt check_time_expected_output.txt ; touch copied_from_srcdir ; fi
	chmod +x verify_files.sh
	echo "Bypass ./test_for_disabled_adjtimex${EXEEXT}"

check_time_output.txt : check_time
	TZ='America/New_York' ./check_time | tee check_time_output.txt
	TZ='Australia/Eucla' ./check_time | tee -a check_time_output.txt

clean-local: clean-local-check
.PHONEY: clean-local-check
clean-local-check:
	if [ -e "copied_from_srcdir" ] ; then rm -f check_time_expected_output.txt ; rm copied_from_srcdir ; fi
	rm -f *.pyc

# End of file Makefile.am
