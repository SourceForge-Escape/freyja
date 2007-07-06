#!/bin/sh
# Local libs used to run local bin tests
TEST=../bin/test
SELECT=debug
BIN_DIR=/home/mongoose/Projects/freyja_0.9.5/bin
BIN=../bin/tools/test/freyjautil.test

if [ -d ${BIN_DIR} ] 
then
	BIN=${BIN_DIR}/tools/test/freyjautil.test
else
	BIN_DIR=../bin
	BIN=${BIN_DIR}/tools/test/freyjautil.test
fi

export LD_LIBRARY_PATH=${BIN_DIR}/../libhel:${BIN_DIR}/../libfreyja:${BIN_DIR}/../tinyxml

#ldd ${BIN} | grep "../" 

${BIN} $@ 

