# File: Makefile.am for libtime, author: John Sauter, date: November 6, 2019.

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
	demo_python.py \
	test_python_interface.py \
	example_06.py \
	extraordinary_days.dat \
	avoid_time_t.tex \
	references.bib \
	legalcode.txt

nodist_libtime_la_SOURCES = dtai_table.h dtai_table.tab
dist_man_MANS = libtime.3
#BUILT_SOURCES = dtai_table.h dtai_table.tab 
time_diff.lo: dtai_table.h dtai_table.tab
test_local.${OBJEXT}: dtai_table.h dtai_table.tab
CLEANFILES = dtai_table.h dtai_table.tab example_03_output.txt \
avoid_time_t.out avoid_time_t.brf avoid_time_t.log avoid_time_t.aux \
avoid_time_t.blg avoid_time_t.bbl avoid_time_t.pdf check_time_output.txt \
verify_files.sh

clean-local:
	rm -rf _minted-avoid_time_t/
	rm -rf __pycache__
	rm -rf autom4te.cache
	rm -rf *~

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

noinst_PROGRAMS = example_02 example_03 example_04 example_05 \
test_time test_diff test_JDN \
test_add test_ep test_local print_DTAI sleep_until_midnight POSIX_epoch \
powers_of_two check_time

example_05_LDADD = libtime.la
example_04_LDADD = libtime.la
example_03_LDADD = libtime.la
example_02_LDADD = libtime.la
test_time_LDADD = libtime.la
test_JDN_LDADD = libtime.la
test_diff_LDADD = libtime.la
test_add_LDADD = libtime.la
test_ep_LDADD = libtime.la
test_local_LDADD = libtime.la
print_DTAI_LDADD = libtime.la
sleep_until_midnight_LDADD = libtime.la
POSIX_epoch_LDADD = libtime.la
powers_of_two_LDADD = libtime.la
check_time_LDADD = libtime.la

python_PYTHON = time_subroutines.py

# The Libtool version-info integers are defined in configure.ac.
libtime_la_LDFLAGS = -version-info @libtool_version_current@:@libtool_version_revision@:@libtool_version_age@

# The spec file is used to build twp RPMs
# for Fedora and OpenSUSE distributions.
EXTRA_DIST = libtime.spec

# fix_files.sh is used to restore directories and permissions when needed
# for files extracted from the PDF.
EXTRA_DIST += fix_files.sh

include_HEADERS = time_subroutines.h

dtai_table.tab: read_extraordinary_days_table.py extraordinary_days.dat
	python3 $(srcdir)/read_extraordinary_days_table.py \
$(srcdir)/extraordinary_days.dat --c-output-tab=$(builddir)/dtai_table.tab

dtai_table.h: read_extraordinary_days_table.py extraordinary_days.dat
	python3 $(srcdir)/read_extraordinary_days_table.py \
$(srcdir)/extraordinary_days.dat --c-output-h=$(builddir)/dtai_table.h

# Build the PDF on demand, if requested.
pdf-local: avoid_time_t.pdf
.PHONEY: pdf-local

avoid_time_t.pdf : avoid_time_t.tex avoid_time_t.bbl dtai_table.h \
	dtai_table.tab \
	example_02.c example_03.c example_06.py \
	example_03_output.txt libtime.pc time_subroutines.py \
	$(libtime_la_SOURCES)
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex
	export TEXINPUTS="$(srcdir):" && pdflatex -shell-escape $(srcdir)/avoid_time_t.tex

avoid_time_t.bbl : avoid_time_t.tex references.bib example_02.c example_03.c \
	dtai_table.h dtai_table.tab example_03_output.txt time_subroutines.py \
	$(libtime_la_SOURCES)
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
	rm -f *.pyc

# End of file Makefile.AM
