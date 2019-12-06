Name:           libtime
Version:        2019.12.06
Release:        1%{?dist}
Summary:        Manipulate time values

License:        GPLv3+
URL:            https://github.com/ShowControl/libtime/
Source0:        https://github.com/ShowControl/libtime/blob/master/libtime-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  python3 >= 3.5
BuildRequires:  git

%global _hardened_build 1

%description
Manipulate time values stored in a POSIX tm structure.

%prep
%autosetup -S git

%build
%configure
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
%{_libdir}/libtime.so.*
%exclude %{_libdir}/libtime.a
%exclude %{_libdir}/libtime.la
%exclude /usr/share/doc/%{name}/AUTHORS
%exclude /usr/share/doc/%{name}/COPYING
%exclude /usr/share/doc/%{name}/ChangeLog
%exclude /usr/share/doc/%{name}/INSTALL
%exclude /usr/share/doc/%{name}/NEWS
%exclude /usr/share/doc/%{name}/README
%exclude /usr/share/doc/%{name}/LICENSE
%license LICENSE
%license COPYING

%files devel
%defattr(-,root,root)
%{_exec_prefix}/lib/python3.*/site-packages/__pycache__/time_subroutines.cpython-3*.pyc
%{_exec_prefix}/lib/python3.*/site-packages/time_subroutines.py
%{_includedir}/time_subroutines.h
%{_libdir}/libtime.so
%{_libdir}/pkgconfig/libtime.pc
%{_mandir}/man3/libtime.3.gz
%doc AUTHORS ChangeLog NEWS README
%license LICENSE
%license COPYING

%files doc
%defattr(-,root,root)
%doc avoid_time_t.pdf
%license LICENSE
%license COPYING

%changelog
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
