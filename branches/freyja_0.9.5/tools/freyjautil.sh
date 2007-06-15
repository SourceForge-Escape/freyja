#!/bin/sh
# Local libs used to run local bin tests
TEST=../bin/test
SELECT=debug
BIN=../bin/tools/test/freyjautil.test

export LD_LIBRARY_PATH=../libhel:../libfreyja

#ldd ${BIN} | grep "../" 

${BIN} $@ 

