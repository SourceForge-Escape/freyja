# CMakeLists files in this project can refer to the root source directory
# of the project as ${FREYJA_SOURCE_DIR} and to the root binary directory
# of the project as ${FREYJA_BINARY_DIR}.
PROJECT( freyja )

#set( PLUGIN_DIR plugins/ )

# Recurse into the project subdirectories.
SUBDIRS( lua tinyxml libhel libfreyja libmgtk freyja plugins )
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

#apt:
#	@-mkdir -p $(APT_DIR)
#	@-for i in *.deb; do mv $${i%_*.deb}.tar.gz $(APT_DIR)/; done
#	@-mv *.deb *.dsc *.changes $(APT_DIR)/
#	cd $(APT_DIR) && dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
#	cd $(APT_DIR) && dpkg-scansources . /dev/null | gzip -9c > Sources.gz


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




