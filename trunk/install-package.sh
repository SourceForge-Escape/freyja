#!/bin/sh
# A script for installing Freyja, using part of my old libsetup script

LIB_PATH="/usr/local/lib"

######################################################################

echo -n "Checking to see if you are root                          "

if whoami | grep root > /dev/null
then
    echo OK
else
    echo ERROR
    echo "   You must be root to install and setup libraries and bins"
#    echo "   User install support will come out later"
    exit 0
fi

######################################################################

echo Installing bins..
cp bin/* /usr/local/bin/

######################################################################

echo Installing libs...
cp common/libs/* /usr/local/lib/
rm -f /usr/local/lib/libfreyja_egg.so
ln -s /usr/local/lib/libfreyja_egg.so.0.8.11 /usr/local/lib/libfreyja_egg.so
rm -f /usr/local/lib/libmtk_image.so
ln -s /usr/local/lib/libmtk_image.so.0.1.1 /usr/local/lib/libmtk_image.so

######################################################################

echo Setting up libs...

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

######################################################################

echo Now cd common and run freyja-setup.sh as the user who will run Freyja
