Name:           libtime
Version:        2019.11.05
Release:        1%{?dist}
Summary:        A library to manipulate time values

License:        GPLv3+
URL:            https://github.com/ShowControl/libtime/
Source0:        https://github.com/ShowControl/libtime/blob/master/libtime-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  python3 >= 3.5, python3-jdcal
BuildRequires:  git

Provides: libtime

%description
A library to manipulate time values stored in a tm structure.  Using this
library you can avoid using POSIX time_t for telling time.

# The following must match the values in configure.ac.
%global libtool_version_current 3
%global libtool_version_revision 0
%global libtool_version_age 14

%prep
%autosetup -S git

%build
%configure
%make_build

%install
%make_install

%post

%preun

%files
%{_includedir}/time_subroutines.h
%{_exec_prefix}/lib/python3.*/site-packages/__pycache__/time_subroutines.cpython-3*.pyc
%{_exec_prefix}/lib/python3.*/site-packages/time_subroutines.py
%{_libdir}/libtime.a
%{_libdir}/libtime.la
%{_libdir}/libtime.so
%{_libdir}/libtime.so.%{libtool_version_current}
%{_libdir}/libtime.so.%{libtool_version_current}.%{libtool_version_revision}.%{libtool_version_age}
%{_libdir}/pkgconfig/libtime.pc
%{_mandir}/man3/libtime.%{libtool_version_current}.gz
/usr/share/doc/libtime

%doc avoid_time_t.pdf

%changelog
* Tue Nov  5 2019 John Sauter <John_Sauter@sytemeyescomputerstore.com>
- 2018.11.05-1 initial version of the spec file
