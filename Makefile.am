# File: Makefile.am for libtime, author: John Sauter, date: November 11, 2018.

lib_LTLIBRARIES = libtime.la
libtime_la_SOURCES = time_copy.c \
	time_current_tm.c \
	time_current_tm_nano.c \
	time_diff.c \
	time_length.c \
	time_local_add.c \
	time_local_normalize.c \
	time_local_to_utc.c \
	time_sleep_until.c \
	time_tm_nano_to_integer.c \
	time_tm_nano_to_string.c \
	time_tm_to_integer.c \
	time_tm_to_string.c \
	time_utc_add.c \
	time_utc_normalize.c \
	time_utc_to_local.c \
	time_subroutines.h \
	int128_to_string.c \
	read_extraordinary_days_table.py \
	extraordinary_days.dat \
	avoid_time_t.tex \
	references.bib \
	legalcode.txt

nodist_libtime_la_SOURCES = dtai_table.h dtai_table.tab
dist_man_MANS = libtime.3
BUILT_SOURCES = dtai_table.h dtai_table.tab example_03_output.txt
CLEANFILES = dtai_table.h dtai_table.tab example_03_output.txt \
avoid_time_t.out avoid_time_t.brf avoid_time_t.log avoid_time_t.aux \
avoid_time_t.blg avoid_time_t.bbl avoid_time_t.pdf check_time_output.txt \
verify_files.sh

clean-local:
	rm -rf _minted-avoid_time_t/

ACLOCAL_AMFLAGS = -I m4

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

noinst_PROGRAMS = example_02 example_03 example_04 test_time test_diff \
test_add test_ep test_local print_DTAI sleep_until_midnight POSIX_epoch \
powers_of_two check_time
example_04_LDADD = libtime.la
example_03_LDADD = libtime.la
example_02_LDADD = libtime.la
test_time_LDADD = libtime.la
test_diff_LDADD = libtime.la
test_add_LDADD = libtime.la
test_ep_LDADD = libtime.la
test_local_LDADD = libtime.la
print_DTAI_LDADD = libtime.la
sleep_until_midnight_LDADD = libtime.la
POSIX_epoch_LDADD = libtime.la
powers_of_two_LDADD = libtime.la
check_time_LDADD = libtime.la

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
# See the NEWS file (and the revision history in the PDF) for the history
# of the version-info.
#
libtime_la_LDFLAGS = -version-info 2:0:0

# fix_files.sh is used to restore directories and permissions when needed
# for files extracted from the PDF.
EXTRA_DIST = fix_files.sh

include_HEADERS = time_subroutines.h

dtai_table.h : read_extraordinary_days_table.py extraordinary_days.dat
	python3 $(srcdir)/read_extraordinary_days_table.py \
$(srcdir)/extraordinary_days.dat --c-output=$(builddir)/dtai_table

# Build the PDF on demand, if requested.
pdf-local: avoid_time_t.pdf
.PHONEY: pdf-local

avoid_time_t.pdf : avoid_time_t.tex avoid_time_t.bbl example_02.c example_03.c \
	example_03_output.txt libtime.pc $(libtime_la_SOURCES)
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex

avoid_time_t.bbl : avoid_time_t.tex references.bib example_02.c example_03.c \
	example_03_output.txt $(libtime_la_SOURCES)
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex
	export BIBINPUTS="$(srcdir):" && kpsewhich -var-value BIBINPUTS && bibtex $(builddir)/avoid_time_t.aux

example_03_output.txt : example_03
	$(builddir)/example_03 >$(builddir)/example_03_output.txt

# Support pgk-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtime.pc

# Support make check and make distcheck
dist_check_DATA = check_time_output.txt check_time_expected_output.txt
dist_check_SCRIPTS = verify_files_template.sh
TESTS = verify_files.sh
verify_files.sh : verify_files_template.sh check_time_output.txt
	cp $(srcdir)/verify_files_template.sh verify_files.sh
	if [ ! -r "check_time_expected_output.txt" ] ; then cp $(srcdir)/check_time_expected_output.txt check_time_expected_output.txt ; touch copied_from_srcdir ; fi
	chmod +x verify_files.sh

check_time_output.txt : check_time
	./check_time > check_time_output.txt
clean-local: clean-local-check
.PHONEY: clean-local-check
clean-local-check:
	if [ -e "copied_from_srcdir" ] ; then rm -f check_time_expected_output.txt ; rm copied_from_srcdir ; fi
