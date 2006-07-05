#!/bin/sh
# Local libs used to run local bin tests
TEST=test
SELECT=debug

mkdir -p ${TEST}
export LD_LIBRARY_PATH=${TEST}

cp -f lib*/${SELECT}/*.so ${TEST}

ldd ./freyja/${SELECT}/freyja | grep ${TEST}

$@ ./freyja/${SELECT}/freyja

