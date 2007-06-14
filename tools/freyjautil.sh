#!/bin/sh
# Local libs used to run local bin tests
TEST=../bin/test
SELECT=debug
BIN=../bin/tools/test/freyjautil.test

mkdir -p ${TEST}
export LD_LIBRARY_PATH=../libhel:../libfreyja

cp -f ../bin/lib*/${SELECT}/*.so ${TEST}

ldd ${BIN} | grep ${TEST}

${BIN} $@ 

