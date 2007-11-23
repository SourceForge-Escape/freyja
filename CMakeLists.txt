# CMakeLists files in this project can refer to the root source directory
# of the project as ${FREYJA_SOURCE_DIR} and to the root binary directory
# of the project as ${FREYJA_BINARY_DIR}.
project(freyja)

set( PLUGIN_DIR plugins/ )

# Recurse into the project subdirectories.
SUBDIRS( lua libhel libfreyja libmgtk freyja )
#SUBDIRS( plugins ) 
#SUBDIRS( tools )

#GET_TARGET_PROPERTY(LIB_NAME Foo LOCATION)
#GET_TARGET_PROPERTY(Bar_prefix Foo PREFIX)
#GET_TARGET_PROPERTY(Bar_suffix Foo SUFFIX)
#SET(NEW_LIB_NAME ${Bar_prefix}Bar${Bar_suffix})
# 
#ADD_CUSTOM_COMMAND(
#   TARGET Foo
#   POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy ${LIB_NAME} ${NEW_LIB_NAME}
#)



#ARCH = $(shell uname -m)
#APT_DIR = apt_repo/$(ARCH)
#DEB_DIR = /tmp/$(ARCH)
#DATE=`date +%Y%m%d`
#UDATE=`date +%Y%m%d%H%M%S`
#VERSION=freyja_0.9.5rc3-$(DATE)-$(ARCH)
#LINUX_SANDBOX=linux-$(ARCH)
#
#WIN32_BUNDLE_DIR=bin/freyja-win32
#WIN32_MODEL_MOD_DIR=bin/freyja-win32/modules/model
#WIN32_IMAGE_MOD_DIR=bin/freyja-win32/modules/image


#################################################################

#info:
#	@-printf "\n"
#	@-printf " To do an auto build choose either:\n"
#	@-printf "\tmake native\n"
#	@-printf "\tmake win32\n"
#	@-printf "\tmake osx\n"
#	@-printf "\n"
#	@-printf " Packaging options:\n"
#	@-printf "\tmake debian\n"
#	@-printf "\tmake ubuntu\n"
#	@-printf "\tmake snapshot\n"
#	@-printf "\tmake redhat\n"
#	@-printf "\tmake apt\n"
#	@-printf "\n"


#native-old:
#	@-printf "Building libhel\n"
#	@-cd libhel && ./autogen.sh && make

#	@-printf "Building libfreyja\n"
#	@-cd libfreyja && ./autogen.sh && make

#	@-printf "Building libmgtk\n"
#	@-cd libmgtk && ./autogen.sh && make

#	@-printf "Building freyja\n"
#	@-cd freyja && ./autogen.sh && make

#	@-printf "Building freyja plugins\n"
#	@-cd plugins && make

#	@-printf "\n\n o If your build failed:\n"
#	@-printf "       * Make sure you have a complete glext.h header\n"
#	@-printf "       * Look in freyja/Makefile for options to disable\n"
#	@-printf "\n\n o Now 'make user-install' as a user\n\n"


#native:
#	@-printf "Building libhel\n" && \
#	cd libhel && ./autogen.sh && make && cd .. &&  \
#	\
#	printf "Building libfreyja\n" && \
#	cd libfreyja && ./autogen.sh && make && cd .. &&  \
#	\
#	printf "Building libmgtk\n" && \
#	cd libmgtk && ./autogen.sh && make && cd .. &&  \
#	\
#	printf "Building plugins\n" && \
#	cd plugins && make && cd .. &&  \
#	\
#	printf "Building freyja\n" && \
#	cd freyja && ./autogen.sh && make

#	@-printf "\n\n o If your build failed:\n"
#	@-printf "       * Make sure you have a complete glext.h header\n"
#	@-printf "       * Look in freyja/Makefile for options to disable\n"
#	@-printf "\n\n o Now 'make user-install' as a user\n\n"


#osx:
#	@-printf "Building libhel\n"
#	@-cd libhel && ./autogen.sh && make osx

#	@-printf "Building libfreyja\n"
#	@-cd libfreyja && ./autogen.sh && make osx

#	@-printf "Building libfreyja plugins\n"
#	@-cd plugins && make osx

#	@-printf "Building libmgtk\n"
#	@-cd libmgtk && ./autogen.sh && make osx

#	@-printf "Building freyja\n"
#	@-cd freyja && ./autogen.sh && make osx && make plugins-osx

#	@-printf "\n\n o If your build failed:\n"
#	@-printf "       * Make sure you have a complete glext.h header\n"
#	@-printf "       * Look in freyja/Makefile for options to disable\n"
#	@-printf "\n\n o Now 'make user-install' as a user\n\n"

#.PHONY : win32

#win32:
#	@-printf "Building libtinyxml\n" 
#	cd tinyxml && rm -f depend.win32 && make -f Makefile.Win32 

#	@-printf "Building libluawrapper\n" 
#	cd lua && rm -f depend.win32 && make -f Makefile.Win32 

#	@-printf "Building libhel\n" 
#	cd libhel && rm -f depend.win32 && make -f Makefile.Win32

#	@-printf "Building libfreyja\n" 
#	cd libfreyja && rm -f depend.win32 && make -f Makefile.Win32 
	
#	@-printf "Building libmgtk\n" 
#	cd libmgtk && rm -f depend.win32 && make -f Makefile.Win32 

#endif( FIXME )

