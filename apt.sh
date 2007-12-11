#!/bin/bash
# This is a shell script to generate an apt directory for upload.
#
# - Terry 'Mongoose' Hendrix II
#####################################################################

APT_DIR=ubuntu/amd64

mkdir -p ${APT_DIR}
dpkg-scanpackages . /dev/null | gzip -9c > ${APT_DIR}/Packages.gz
dpkg-scansources . /dev/null | gzip -9c > ${APT_DIR}/Sources.gz
for i in *.deb; do mv ${i%_*.deb}.tar.gz ${APT_DIR}/; done
mv *.deb *.dsc *.changes ${APT_DIR}/




