#!/bin/sh
###################################################
# Mongoose 
#
# Autocrap!!!  run run!
###################################################

ARG1=$1

rm -f mkinstalldirs system.info configure \
		config.status config.log Makefile \
		autoscan.log config.cache stamp-* \
		configure.scan missing install-sh \
		Makefile.in confdefs.h

automake --foreign --add-missing --copy
autoconf 
./configure
rm -f depend

echo
echo "Now do make && make install && ./setup-freyja.sh"
echo "Alternatively you can make debug && make install-debug"
echo "Alternatively you can make gtk1.2 && make install-gtk1.2"
echo

if [ ! $ARG1 ]
then
    exit 0
fi

if [ $ARG1 = "debug" ]
then
    make debug
fi 

if [ $ARG1 = "auto" ]
then
    make debug && make install && ./setup-freyja.sh
fi 

