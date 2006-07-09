#!/bin/sh
# Local libs used to run local bin tests
TEST=../bin/test
SELECT=debug
BIN=../bin/freyja/${SELECT}/freyja

mkdir -p ${TEST}
export LD_LIBRARY_PATH=${TEST}

cp -f ../bin/lib*/${SELECT}/*.so ${TEST}

ldd ${BIN} | grep ${TEST}

ARG1="no"


if [ $1 ]
then
	ARG1=$1
fi


if [ ${ARG1} = "gdb" ]
then
	cd ../bin && gdb ${BIN}
	exit 0
else
	cd ../bin && ${BIN} $@
fi
