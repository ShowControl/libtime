Name:           libtime
Version:        2021.07.09
Release:        1%{?dist}
Summary:        Manipulate time values

License:        GPLv3+
URL:            https://github.com/ShowControl/libtime/
Source0:        https://github.com/ShowControl/libtime/blob/master/libtime-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  python3 >= 3.5
BuildRequires:  git
BuildRequires:  proleptic_utc_with_leap_seconds

%global _hardened_build 1

# Do not rebuild the PDF file on RHEL, since it lacks TeX.
%if 0%{?rhel}
%global rebuild_pdf 0
%else
%global rebuild_pdf 1
%endif

%description
Manipulate time values stored in a POSIX tm structure.

%prep
%autosetup -S git

%build
# Tell configure to rebuild the PDF only if TeX is available.
%if %{rebuild_pdf}
%configure --enable-pdf
%else
%configure
%endif

%make_build 

%install
%make_install

%check
make check VERBOSE=1

%package devel
Summary: Manipulate time values stored in a POSIX tm structure
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.  Use libtime to manipulate
time values stored in a POSIX tm structure, thus avoiding the use of
POSIX time_t, which does not support leap seconds.

%package doc
Summary: Comprehensive documentation for %{name}
Requires: %{name}%{?_isa} = %{version}-%{release}

%if %{rebuild_pdf}
# gnuplot and texlive-scheme-full are needed only to rebuild the PDF file.
BuildRequires:  gnuplot
BuildRequires:  texlive-scheme-full
%endif

%description doc
The %{name}-doc package contains the documentation for %{name}-devel
in the form of a PDF file which describes the motivation for having
the package, then goes through each entry point, explaining what it
does and exhibiting the source code.  This is followed by some examples
of its use.  Included in the PDF file using embedding are all of the
files and instructions needed to create the source tarball, which
includes the RPM spec file.

%files
%defattr(-,root,root)
# The library soname changes frequently, up to once a week, but there is
# no need to recompile dependencies if the first number does not change,
# since the changes are usually just an update to the prediction of future
# leap seconds.
%{_libdir}/libtime.so.3
%{_libdir}/libtime.so.3.*
%exclude %{_libdir}/libtime.a
%exclude %{_libdir}/libtime.la
%exclude /usr/share/doc/%{name}/AUTHORS
%exclude /usr/share/doc/%{name}/COPYING
%exclude /usr/share/doc/%{name}/ChangeLog
%exclude /usr/share/doc/%{name}/INSTALL
%exclude /usr/share/doc/%{name}/NEWS
%exclude /usr/share/doc/%{name}/README
%exclude /usr/share/doc/%{name}/LICENSE
%exclude /usr/share/doc/%{name}/avoid_time_t.pdf
%license LICENSE
%license COPYING

%files devel
%defattr(-,root,root)
%{_exec_prefix}/lib/python3.*/site-packages/__pycache__/time_subroutines.cpython-3*.pyc
%{_exec_prefix}/lib/python3.*/site-packages/time_subroutines.py
%{_includedir}/time_subroutines.h
%{_libdir}/libtime.so
%{_libdir}/pkgconfig/libtime.pc
%{_mandir}/man3/libtime.3.*
%doc AUTHORS ChangeLog NEWS README
%license LICENSE
%license COPYING

%files doc
%defattr(-,root,root)
%doc avoid_time_t.pdf
%license LICENSE
%license COPYING

%changelog
* Fri Jul 09 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.07.09-1 Adjust future leap seconds starting in 2412.
* Fri Jul 02 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.07.02-1 Adjust future leap seconds starting in 2088.
* Fri Jun 25 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.06.25-1 Adjust future leap seconds starting in 2091.
* Fri Jun 18 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.06.18-1 Adjust future leap seconds starting in 2353.
* Fri Jun 11 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.06.11-1 Adjust future leap seconds starting in 2287.
* Fri Jun 04 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.06.04-1 Adjust future leap seconds starting in 2063.
* Sun May 30 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.28-4 Rebuild PDF file except on RHEL.
* Sat May 29 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.28-2 Allow for a future change in the man file compression.
* Fri May 28 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.28-1 Adjust future leap seconds starting in 2182.
* Fri May 21 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.21-1 Adjust future leap seconds starting in 2029.
* Fri May 14 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.14-1 Adjust future leap seconds starting in 2073.
* Fri May 07 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.05.07-1 Adjust future leap seconds starting in 2357.
* Fri Apr 23 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.04.23-1 Adjust future leap seconds starting in 2029.
* Fri Apr 16 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.04.16-1 Adjust future leap seconds starting in 2245.
* Fri Apr 09 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.04.09-1 Adjust future leap seconds starting in 2063.
* Fri Apr 02 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.04.02-1 Adjust future leap seconds starting in 2029.
* Fri Mar 26 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.03.26-1 Adjust future leap seconds starting in 2030.
* Fri Mar 19 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.03.19-1 Adjust future leap seconds starting in 2068.
* Fri Mar 12 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.03.12-1 Adjust future leap seconds starting in 2030.
* Fri Mar 05 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.03.05-1 Include the latest research on historical values of delta T.
* Fri Feb 26 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.02.26-1 Adjust future leap seconds starting in 2104.
* Fri Feb 19 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.02.19-1 Adjust future leap seconds starting in 2051.
* Fri Feb 12 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.02.12-1 Adjust future leap seconds starting in 2103.
* Fri Feb 05 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.02.05-1 Adjust future leap seconds starting in 2055.
* Fri Jan 29 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.29-1 Adjust future leap seconds starting in 2089.
* Mon Jan 25 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.25-1 Do not send random bits to mktime.
* Fri Jan 22 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.22-1 Adjust future leap seconds starting in 2070.
* Fri Jan 15 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.15-1 Adjust future leap seconds starting in 2039.
* Fri Jan 08 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.08-1 There will be no leap second in June of 2021.
- Adjust future leap seconds starting in 2105.
* Fri Jan 01 2021 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2021.01.01-1 Adjust future leap seconds starting in 2064.
* Fri Dec 25 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.12.25-1 Adjust future leap seconds starting in 2077.
* Fri Dec 18 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.12.18-1 Adjust future leap seconds starting in 2044.
* Fri Dec 04 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.12.04-1 Adjust future leap seconds starting in 2070.
* Fri Nov 27 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.11.27-1 Adjust future leap seconds starting in 2097.
* Fri Nov 20 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.11.20-1 Adjust future leap seconds starting in 2067.
* Fri Nov 13 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.11.13-1 Delay the December 2093 leap second by six months to June 2094.
* Fri Nov 06 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.11.06-1 Adjust future leap seconds starting in 2033.
* Tue Nov 03 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.10.30-2 Depend on the package, not on a file it provides.
* Fri Oct 30 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.10.30-1 Adjust future leap seconds starting in 2033.
* Fri Oct 23 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.10.23-1 Adjust future leap seconds starting in 2033.
* Fri Oct 16 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.10.16-1 Adjust future leap seconds starting in 2062.
* Fri Oct 02 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.10.02-1 Adjust future leap seconds starting in 2059.
* Fri Sep 25 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.09.25-1 Adjust almost all future leap seconds starting in 2032.
* Fri Sep 04 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.09.04-1 Adjust almost all future leap seconds.
* Fri Aug 28 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.08.28-1 Adjust many future leap seconds starting in 2089.
* Fri Aug 21 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.08.21-1 Adjust many future leap seconds starting in 2031.
* Fri Aug 14 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.08.14-1 Adjust many future leap seconds starting in 2073.
* Fri Aug 07 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.08.07-1 Adjust many future leap seconds starting in 2114.
* Fri Jul 31 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.07.31-1 Adjust many future leap seconds starting in 2120.
* Fri Jul 24 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.07.24-1 Adjust many future leap seconds starting in 2055.
* Fri Jul 17 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.07.17-1 Adjust many future leap seconds starting in 2080.
* Fri Jul 10 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.07.10-1 Update to IERS Bulletin C 60.
               Adjust many future leap seconds starting in 2066.
* Fri Jul 03 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.07.03-1 Adjust many future leap seconds starting in 2025.
* Fri Jun 26 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.06.26-1 Adjust many future leap seconds starting in 2068.
* Fri Jun 19 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.06.19-1 Adjust many future leap seconds starting in 2063.
* Fri Jun 12 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.06.12-1 Adjust many future leap seconds starting in 2098.
* Fri Jun 05 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.06.05-1 Adjust many future leap seconds starting in 2087.
* Fri May 29 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.05.29-1 Adjust many future leap seconds starting in 2106.
* Thu May 21 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.05.21-1 Adjust several distant future leap seconds.
* Thu May 14 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.05.14-1 Add time_utc_to_foreign_local.  Adjust five distant
               future leap seconds.
* Thu May 07 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.05.07-1 Adjust five distant future leap seconds.
* Thu Apr 30 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.30-1 Convert the PDF to Libertine fonts and
               adjust future leap seconds.
* Thu Apr 23 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.23-1 Advance three distant future leap seconds by three or six months.
* Thu Apr 16 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.16-1 Advance seven distant future leap seconds by three or six months.
* Wed Apr 15 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.15-1 Recompute all future leap seconds.
* Tue Apr 14 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.14-1 Recompute all future leap seconds.
* Fri Apr 10 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.10-1 Delay the next five leap seconds.
* Fri Apr 03 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.04.03-1 Update the README file to include Fedora 32.
* Fri Mar 20 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.03.20-1 Delay the December 2028 leap second by six months to June 2029.
* Fri Feb 21 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.02.21-1 Delay the December 2023 leap second by six months to June 2024.
* Sat Feb 15 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.02.15-1 Delay the June 2033 leap second by six months to December 2033.
* Fri Jan 31 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.01.31-1 Delay the December 2042 leap second by six months to June 2043.
* Sat Jan 25 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.01.25-1 Delay the June 2023 and June 2028 leap seconds by six months.
* Sat Jan 11 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.01.11-1 Update to IERS Bulletin C 59, issued January 7, 2020.
* Fri Jan 03 2020 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2020.01.02-1 Delay the December 2022 leap second by six months.
* Fri Dec 20 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.20-1 Delay the December 2032 leap second by six months.
* Thu Dec 19 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.14-2 Fix soname in %%files.
* Sat Dec 14 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.14-1 Add time_test_for_disabled_adjtimex.
* Fri Dec 13 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.13-1 Delay the December 2027 and 2032 leap seconds by six months
* Fri Dec 06 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.06-1 Delay the June 2022 leap second by six months to December 2022.
* Mon Dec 02 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.12.02-1 Put the source RPM on github.
* Fri Nov 29 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.29-2 Fix file attributes.
- 2019.11.29-1 Add a test for adjtimex not working.
- 2019.11.24-2 Add -doc subpackage
* Sun Nov 24 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.24-1 Remove the dependence on jdcal
* Sat Nov 23 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.23-1 Delay the June 2027 leap second by six months to December 2027
* Mon Nov 18 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.18-1 Add make check.
* Sat Nov 16 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.16-1 Make a subset work on 32-bit platforms without 128-bit integers
* Sat Nov  9 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.09-1 remove dependency on version-info using wildcards
* Wed Nov  6 2019 John Sauter <John_Sauter@systemeyescomputerstore.com>
- 2019.11.06-1 divide into libtime and libtime-devel
* Tue Nov  5 2019 John Sauter <John_Sauter@sytemeyescomputerstore.com>
- 2019.11.05-1 initial version of the spec file
