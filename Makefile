################################################################
# Terry Hendrix (Mongoose) <mongooseichban@gmail.com>
################################################################

ARCH = $(shell uname -m)
APT_DIR = apt_repo/$(ARCH)
DEB_DIR = /tmp/$(ARCH)
DATE=`date +%Y%m%d`
UDATE=`date +%Y%m%d%H%M%S`
VERSION=freyja_0.9.5rc3-$(DATE)-$(ARCH)
LINUX_SANDBOX=linux-$(ARCH)

WIN32_BUNDLE_DIR=bin/freyja-win32
WIN32_MODEL_MOD_DIR=bin/freyja-win32/modules/model
WIN32_IMAGE_MOD_DIR=bin/freyja-win32/modules/image


#################################################################

info:
	@-printf "\n"
	@-printf " To do an auto build choose either:\n"
	@-printf "\tmake native\n"
	@-printf "\tmake win32\n"
	@-printf "\tmake osx\n"
	@-printf "\n"
	@-printf " Packaging options:\n"
	@-printf "\tmake debian\n"
	@-printf "\tmake ubuntu\n"
	@-printf "\tmake snapshot\n"
	@-printf "\tmake redhat\n"
	@-printf "\tmake apt\n"
	@-printf "\n"


native-old:
	@-printf "Building libhel\n"
	@-cd libhel && ./autogen.sh && make

	@-printf "Building libfreyja\n"
	@-cd libfreyja && ./autogen.sh && make

	@-printf "Building libmgtk\n"
	@-cd libmgtk && ./autogen.sh && make

	@-printf "Building freyja\n"
	@-cd freyja && ./autogen.sh && make

	@-printf "Building freyja plugins\n"
	@-cd plugins && make

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"


native:
	@-printf "Building libhel\n" && \
	cd libhel && ./autogen.sh && make && cd .. &&  \
	\
	printf "Building libfreyja\n" && \
	cd libfreyja && ./autogen.sh && make && cd .. &&  \
	\
	printf "Building libmgtk\n" && \
	cd libmgtk && ./autogen.sh && make && cd .. &&  \
	\
	printf "Building plugins\n" && \
	cd plugins && make && cd .. &&  \
	\
	printf "Building freyja\n" && \
	cd freyja && ./autogen.sh && make

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"


osx:
	@-printf "Building libhel\n"
	@-cd libhel && ./autogen.sh && make osx

	@-printf "Building libfreyja\n"
	@-cd libfreyja && ./autogen.sh && make osx

	@-printf "Building libfreyja plugins\n"
	@-cd plugins && make osx

	@-printf "Building libmgtk\n"
	@-cd libmgtk && ./autogen.sh && make osx

	@-printf "Building freyja\n"
	@-cd freyja && ./autogen.sh && make osx && make plugins-osx

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"

.PHONY : win32

win32:
	@-printf "Building libhel\n" 
	cd libhel && rm -f depend.win32 && make -f Makefile.Win32

	@-printf "Building libtinyxml\n" 
	cd tinyxml && rm -f depend.win32 && make -f Makefile.Win32 

	@-printf "Building libfreyja\n" 
	cd libfreyja && rm -f depend.win32 && make -f Makefile.Win32 
	
	@-printf "Building libmgtk\n" 
	cd libmgtk && rm -f depend.win32 && make -f Makefile.Win32 
	
	@-printf "Building freyja\n" 
	cd freyja && rm -f depend.win32 && make -f Makefile.Win32 
	
	@-printf "Building plugins\n" 
	cd plugins && make win32

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"

win32-bundle:
	@-printf "Building win32 bundle...\n"
	mkdir -p $(WIN32_BUNDLE_DIR)
	cp freyja/data/freyja-dev.mlisp $(WIN32_BUNDLE_DIR)
	cp freyja/data/freyja.ico $(WIN32_BUNDLE_DIR)
	cp freyja/data/freyja.lnk $(WIN32_BUNDLE_DIR)
	mkdir -p $(WIN32_BUNDLE_DIR)/icons
	cp freyja/data/icons/*.png $(WIN32_BUNDLE_DIR)/icons
	mkdir -p $(WIN32_BUNDLE_DIR)/materials
	cp freyja/data/materials/*.mat $(WIN32_BUNDLE_DIR)/materials
	mkdir -p $(WIN32_BUNDLE_DIR)/palettes
	cp freyja/data/palettes/*.pal $(WIN32_BUNDLE_DIR)/palettes
	cp bin/freyja/win32/freyja.exe $(WIN32_BUNDLE_DIR)
	mkdir -p $(WIN32_MODEL_MOD_DIR)
	cp bin/plugins/model/*/win32/*.dll $(WIN32_MODEL_MOD_DIR)
	mkdir -p $(WIN32_IMAGE_MOD_DIR)
	cp bin/plugins/image/win32/*.dll $(WIN32_IMAGE_MOD_DIR)
	cp bin/libfreyja/win32/freyja9.dll $(WIN32_BUNDLE_DIR)
	cp bin/libmgtk/win32/mgtk1.dll $(WIN32_BUNDLE_DIR)
	cp bin/libhel/win32/hel0.dll $(WIN32_BUNDLE_DIR)
	zip -r freyja-win32.zip  $(WIN32_BUNDLE_DIR) 


clean:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[done]"
	@-cd mstl && make clean
	@-cd libhel && make clean
	@-cd libmgtk && make clean
	@-cd libfreyja && make clean
	@-cd freyja && make clean

tarball:
	@-cd mstl && make tarball
	@-cd libhel && make tarball
	@-cd libfreyja && make tarball
	@-cd libmgtk && make tarball
	@-cd freyja && make tarball

user-install:
	@-printf "Installing freyja user files\n"
	@-cd freyja; ./setup-freyja.sh
	@-cd ..
	@-cd plugins; $(MAKE) user-install
	@-cd ..


#################################################################

snapshot:
	tar zcvf $(VERSION).tar.gz \
	--exclude=.svn \
	--exclude=*~ \
	--exclude=*.xcf \
	--exclude=*.o \
	--exclude=*.dll \
	--dereference $(LINUX_SANDBOX)

#################################################################


ubuntu: debian

debian:
	$(MAKE) deb
	$(MAKE) apt


deb:
	@-cd tinyxml; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd lua; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd libhel; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd libmgtk; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd libfreyja; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd plugins; fakeroot dpkg-buildpackage 
	@-cd ..
	@-cd freyja; fakeroot dpkg-buildpackage 
	@-cd ..

apt:
	@-mkdir -p $(APT_DIR)
	@-for i in *.deb; do mv $${i%_*.deb}.tar.gz $(APT_DIR)/; done
	@-mv *.deb $(APT_DIR)/
	@-mv *.dsc $(APT_DIR)/
	@-mv *.changes $(APT_DIR)/
	cd $(APT_DIR) && dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
	cd $(APT_DIR) && dpkg-scansources . /dev/null | gzip -9c > Sources.gz

apt-upload:
	scp $(APT_DIR)/Packages.gz \
		$(APT_DIR)/Sources.gz \
		$(APT_DIR)/*.deb  \
		$(APT_DIR)/*.dsc \
		$(APT_DIR)/*.changes \
		$(APT_DIR)/*.tar.gz \
	icculus.org:~/freyja/files/debian/$(ARCH)/


#################################################################

redhat:
	for i in *.deb; \
	do \
		alien --to-rpm $i.deb; \
	done


