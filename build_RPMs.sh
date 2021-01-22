#!/bin/bash
# File: build_RPMs.sh, author: John Sauter, date: January 21, 2021.
# Build the RPMs for libtime

# Requires fedora-packager, rpmdevtools, copr-cli.
# Don't forget to tell copr-cli about your copr API token.
# See https://developer.fedoraproject.org/deployment/copr/copr-cli.html.

rm -rf ~/rpmbuild/
mkdir -p ~/rpmbuild
mkdir -p ~/rpmbuild/SOURCES
mkdir -p ~/rpmbuild/SRPMS
mkdir -p ~/rpmbuild/RPMS/x86_64

pushd ~/rpmbuild
# Set the umask so files created will not have strange permissions.
umask 022
# Clean out any old versions of libtime.
rm -f SOURCES/*
rm -f SRPMS/*
rm -f RPMS/x86_64/*
# Copy in the new tarball and spec file.
popd
cp -v libtime-*.tar.gz ~/rpmbuild/SOURCES/
cp -v libtime.spec ~/rpmbuild/SOURCES/
pushd ~/rpmbuild/SOURCES
chmod 0644 libtime-*.tar.gz
# Build and test the source RPM.
rpmbuild -ba libtime.spec
# Copy back the source RPM so it can be copied from github.
popd
cp -v ~/rpmbuild/SRPMS/libtime-*.src.rpm .
# Perform validity checking on the RPMs.
pushd ~/rpmbuild/SRPMS
rpmlint libtime-*.src.rpm
pushd ../RPMS/x86_64/
rpmlint libtime-*.rpm
# Make sure libtime will build from the source RPM.
popd
# Disable building with Mock until I can figure out how to
# add a repository.
#mock -r fedora-33-x86_64 libtime-*.src.rpm
# now that all local tests have passed, see if it builds on copr
#copr-cli build test libtime-*.src.rpm

# End of file build_RPMs.sh
