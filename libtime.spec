Name:           libtime
Version:        2019.11.23
Release:        1%{?dist}
Summary:        Manipulate time values

License:        GPLv3+
URL:            https://github.com/ShowControl/libtime/
Source0:        https://github.com/ShowControl/libtime/blob/master/libtime-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  python3 >= 3.5, python3-jdcal
BuildRequires:  git

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
Requires: libtime = %{version}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.  Use libtime to manipulate
time values stored in a POSIX tm structure, thus avoiding the use of
POSIX time_t, which does not support leap seconds.

%files
%defattr(-,root,root)
%{_exec_prefix}/lib/python3.*/site-packages/__pycache__/time_subroutines.cpython-3*.pyc
%{_exec_prefix}/lib/python3.*/site-packages/time_subroutines.py
%{_libdir}/libtime.la
%{_libdir}/libtime.so.*
%exclude /usr/share/doc/%{name}/AUTHORS
%exclude /usr/share/doc/%{name}/COPYING
%exclude /usr/share/doc/%{name}/ChangeLog
%exclude /usr/share/doc/%{name}/INSTALL
%exclude /usr/share/doc/%{name}/NEWS
%exclude /usr/share/doc/%{name}/README
%exclude /usr/share/doc/%{name}/LICENSE
%license LICENSE

%files devel
%defattr(-,root,root)
%{_includedir}/time_subroutines.h
%{_libdir}/libtime.a
%{_libdir}/libtime.so
%{_libdir}/pkgconfig/libtime.pc
%{_mandir}/man3/libtime.3.gz
%doc AUTHORS COPYING ChangeLog NEWS README
%doc avoid_time_t.pdf
%license LICENSE

%changelog
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
