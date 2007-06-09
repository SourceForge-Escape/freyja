#!/bin/bash
# Local libs used to run local bin tests
BIN=freyja
PATH=..

if [ -d  ~/Projects/freyja/freyja_0.9.5/freyja ]
then
	PATH=/home/mongoose/Projects/freyja_0.9.5
	BIN=${PATH}/freyja/freyja
fi

export LD_LIBRARY_PATH=${PATH}/tinyxml:${PATH}/libhel:${PATH}/libfreyja:${PATH}/libmgtk

# Show which libraries we're linking to from here.
ldd ${BIN} | grep ${LIB_PATH}
echo "  ========================================================="

# Handle special script arguments for debugging.
ARG1="none"

if [ $1 ]
then
	ARG1=$1
fi

if [ ${ARG1} = "gdb" ]
then
	$* ${BIN}

elif [ ${ARG1} = "ddd" ]
then
	$* ${BIN}

elif [ ${ARG1} = "alleyoop" ]
then
	# Run alleyoop with any options given.
	$* ${BIN}

elif [ ${ARG1} = "valgrind" ]
then
	# Run valgrind with any options given.
	$* ${BIN}

elif [ ${ARG1} = "ldd" ]
then
	ldd -v ${BIN}

else
	${BIN} $@
fi


