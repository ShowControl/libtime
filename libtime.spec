Name:           libtime
Version:        2019.11.09
Release:        1%{?dist}
Summary:        Manipulate time values

License:        GPLv3+
URL:            https://github.com/ShowControl/libtime/
Source0:        https://github.com/ShowControl/libtime/blob/master/libtime-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  python3 >= 3.5, python3-jdcal
BuildRequires:  git

ExcludeArch:    i386
# Needs 128-bit integer support in gcc

%description
Manipulate time values stored in a POSIX tm structure.

%prep
%autosetup -S git

%build
%configure
%make_build

%install
%make_install

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
* Thu Nov  7 2019 John Sauter <John_Sauter@sytemeyescomputerstore.com>
- 2019.11.07-1 remove dependency on version-info using wildcards

* Wed Nov  6 2019 John Sauter <John_Sauter@sytemeyescomputerstore.com>
- 2019.11.06-1 divide into libtime and libtime-devel

* Tue Nov  5 2019 John Sauter <John_Sauter@sytemeyescomputerstore.com>
- 2019.11.05-1 initial version of the spec file
