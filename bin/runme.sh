#!/bin/sh
# Local libs used to run local bin tests
TEST=test
SELECT=debug

cd ~/Projects/freyja/freyja_0.9.3/bin
mkdir -p ${TEST}
export LD_LIBRARY_PATH=${TEST}

cp -f lib*/${SELECT}/*.so ${TEST}

ldd ./freyja/${SELECT}/freyja | grep ${TEST}

export LD_LIBRARY_PATH=${TEST}
$@ ./freyja/${SELECT}/freyja

