#!/bin/sh
# Local libs used to run local bin tests
TEST=test
SELECT=debug
BIN=./tools/test/freyjautil.test

mkdir -p ${TEST}
export LD_LIBRARY_PATH=${TEST}

cp -f lib*/${SELECT}/*.so ${TEST}

ldd ${BIN} | grep ${TEST}

${BIN} $@ 

