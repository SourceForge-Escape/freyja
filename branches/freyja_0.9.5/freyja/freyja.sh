#!/bin/bash
# Local libs used to run local bin tests
SELECT=debug
LIB_PATH=../bin/linux/${SELECT}
BIN=../bin/linux/${SELECT}/freyja

if [ -d  ~/Projects/freyja/freyja_0.9.5/freyja ]
then
	cd ~/Projects/freyja/freyja_0.9.5/freyja
fi

mkdir -p ${LIB_PATH}
export LD_LIBRARY_PATH=${LIB_PATH}:../tinyxml:../libhel:../libfreyja:../libmgtk

# Copy the bins and libraries from their build directories.
cp -f ../bin/freyja/linux/${SELECT}/freyja ${LIB_PATH}
cp -f ../bin/lib*/linux/${SELECT}/*.so ${LIB_PATH}

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
	cd ../bin && gdb ${BIN}

elif [ ${ARG1} = "ddd" ]
then
	cd ../bin && ddd ${BIN}

elif [ ${ARG1} = "alleyoop" ]
then
	# Run alleyoop with any options given.
	cd ../bin && $* ${BIN}

elif [ ${ARG1} = "valgrind" ]
then
	# Run valgrind with any options given.
	cd ../bin && $* ${BIN}

elif [ ${ARG1} = "ldd" ]
then
	ldd -v ${BIN}

else
	cd ../bin && ${BIN} $@
fi


