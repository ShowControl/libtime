#!/bin/bash
# File: build_RPMs.sh, author: John Sauter, date: December 2, 2019
# Build the RPMs for libtime

pushd ~/rpmbuild
# Set the umask so files created will not have strange permissions.
umask 022
# Clean out any old versions of libtime.
rm -f SOURCES/*
rm -f SRPMS/*
rm -f RPMS/x86_64/*
# Copy in the new tarball.
popd
cp libtime-*.tar.gz ~/rpmbuild/SOURCES/
pushd ~/rpmbuild/SOURCES
chmod 0644 libtime-*.tar.gz
# Build and test the source RPM.
rpmbuild -ta libtime-*.tar.gz
# Copy back the source RPM so it can be copied from github.
popd
cp ~/rpmbuild/SRPMS/libtime-*.src.rpm .
# Make sure libtime will build from the source RPM.
pushd ~/rpmbuild/SRPMS
mock -r opensuse-tumbleweed-x86_64 libtime-*.src.rpm
copr-cli build test libtime-*.src.rpm
# Perform validity checking on the RPMs.
rpmlint libtime-*.src.rpm
cd ../RPMS/x86_64/
rpmlint libtime-*.rpm

# End of file build_RPMs.sh
