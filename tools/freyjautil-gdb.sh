#!/bin/sh
# Local libs used to run local bin tests
ARGS=`pwd`/$@

if [ ! $1 ]
then
	ARGS=""
fi

cd /home/mongoose/Projects/freyja/freyja_0.9.5/libfreyja
TEST=../bin/test
SELECT=debug
BIN=../bin/tools/test/freyjautil.test

mkdir -p ${TEST}
export LD_LIBRARY_PATH=${TEST}

cp -f ../bin/lib*/${SELECT}/*.so ${TEST}

ldd ${BIN} | grep ${TEST}

gdb --args ${BIN} ${ARGS} 

