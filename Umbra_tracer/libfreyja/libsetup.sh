#!/bin/sh
# A script for caching libs in given path


if [ ! $1 ]
then
    LIB_PATH="/usr/local/lib"
else
    LIB_PATH=$1
fi

echo " "
echo "libsetup.sh: Caching libs in given path"
echo " "
echo -n "Checking to see if you are root                          "

if whoami | grep root > /dev/null
then
    echo OK
else
    echo ERROR
    echo "   You must be root to edit your library data"
    echo "   User install support will come out later"
    exit 0
fi

echo -n "Checking for '${LIB_PATH}' in '/etc/ld.so.conf'       "

if grep "${LIB_PATH}" /etc/ld.so.conf > /dev/null
then
    echo OK 
else
    echo ERROR
    echo Adding ${LIB_PATH} to /etc/ld.so.conf
    echo ${LIB_PATH} >> /etc/ld.so.conf
fi

echo -n "Running ldconfig                                         "
if /sbin/ldconfig -n ${LIB_PATH} > /dev/null
then 
    echo OK
else
    echo ERROR
fi

echo " "
echo " "
