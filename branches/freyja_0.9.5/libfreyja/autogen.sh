#!/bin/sh
# What the wolf you say?  Just use these simple make 
# rules to install libfreyja and the test plugins.
#
# Or if you don't want to type them, use this as a
# shell script as root.
# 
# /bin/sh INSTALL

# No autoconf please
rm -f python.env
echo "#################################################" > python.env
echo "# Python environment" >> python.env
echo "#################################################" >> python.env
echo  "" >> python.env
echo "#CONFIGURE_DEF += -DUSING_PYTHON" >> python.env
echo "#CONFIGURE_LNK += -lpython2.4" >> python.env
echo  "" >> python.env
make -f python.mak
if [ -e ../bin/libfreyja/conf/python ]
then
	../bin/libfreyja/conf/python
fi

cat python.env

#./UpdateBindings.py > freyja/PythonABI.cpp

rm -f depend
make debug
#make install
echo
echo Now do make install
# && make plugins && make install-plugins
#make plugins
#make install-plugins
