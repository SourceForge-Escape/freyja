#!/bin/bash
# Local libs used to run local bin tests
FBIN=freyja
FPATH=..

if [ -d  ~/Projects/freyja/freyja_0.9.5/freyja ]
then
	FPATH=/home/mongoose/Projects/freyja_0.9.5
	FBIN=${FPATH}/freyja/freyja
fi

export LD_LIBRARY_PATH=${FPATH}/tinyxml:${FPATH}/libhel:${FPATH}/libfreyja:${FPATH}/libmgtk

# Show which libraries we're linking to from here.
ldd ${FBIN} | grep ${FPATH}
echo "  ========================================================="

# Handle special script arguments for debugging.
ARG1="none"

if [ $1 ]
then
	ARG1=$1
fi

if [ ${ARG1} = "gdb" ]
then
	$* ${FBIN}

elif [ ${ARG1} = "ddd" ]
then
	ddd ${FBIN}

elif [ ${ARG1} = "alleyoop" ]
then
	# Run alleyoop with any options given.
	$* ${FBIN}

elif [ ${ARG1} = "valgrind" ]
then
	# Run valgrind with any options given.
	$* ${FBIN}

elif [ ${ARG1} = "ldd" ]
then
	ldd -v ${FBIN}

else
	${FBIN} $@
fi


