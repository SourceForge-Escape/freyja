
lazybuild:
	@-printf "Installing mstl\n"
	@-cd mstl && make install

	@-printf "Installing libhel\n"
	@-cd libhel && ./autogen.sh && make && make install

	@-printf "Installing libfreyja\n"
	@-cd libfreyja && ./autogen.sh && make && make install
	@-cd libfreyja && make plugins && make install-plugins

	@-printf "Installing libmgtk\n"
	@-cd libmgtk && ./autogen.sh && make && make install

	@-printf "Installing Freyja\n"
	@-cd Freyja && ./autogen.sh && make && make install

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in Freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"


clean:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[done]"
	@-cd mstl && make clean
	@-cd libhel && make clean
	@-cd libmgtk && make clean
	@-cd libfreyja && make clean
	@-cd Freyja && make clean

tarball:
	@-cd mstl && make tarball
	@-cd libhel && make tarball
	@-cd libfreyja && make tarball
	@-cd libmgtk && make tarball
	@-cd Freyja && make tarball

user-install:
	@-printf "Installing freyja user files\n"
	@-cd Freyja; ./setup-freyja.sh
	@-cd ..


#################################################################

DEB_DIR=/tmp/FreyjaMaster.deb

# Edited for Debian GNU/Linux.
DESTDIR =
INSTALL_BIN=$(DESTDIR)/usr/bin
INSTALL_LIB=$(DESTDIR)/usr/lib
INSTALL_DOC=$(DESTDIR)/usr/share/doc/$(NAME)
INSTALL_SHARE=$(DESTDIR)/usr/share/$(NAME)
INSTALL_INCLUDE=$(DESTDIR)/usr/include


redhat:
	cd $(DEB_DIR) && \
		alien --to-rpm $(NAME_DEB)_$(VERSION_DEB)-1_$(ARCH).deb

debian-package:
	@-cd libmgtk; make debian-package
	@-cd ..
	@-cd libhel; make debian-package
	@-cd ..
	@-cd libfreyja; make debian-package
	@-cd ..
	@-cd Freyja; make debian-package
	@-cd ..

apt:
	@-mkdir -p $(DEB_DIR)
	@-cp $(DEB_DIR)/../*.deb/*.deb $(DEB_DIR)/
	@-cp $(DEB_DIR)/../*.deb/*.dsc $(DEB_DIR)/
	@-cp $(DEB_DIR)/../*.deb/*.changes $(DEB_DIR)/
	@-cp $(DEB_DIR)/../*.deb/*.gz $(DEB_DIR)/
	cd $(DEB_DIR) && dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
	cd $(DEB_DIR) && dpkg-scansources . /dev/null | gzip -9c > Sources.gz

apt-upload:
	scp $(DEB_DIR)/Packages.gz \
		$(DEB_DIR)/Sources.gz \
		$(DEB_DIR)/$(NAME_DEB)*.diff.gz  \
		$(DEB_DIR)/$(NAME_DEB)*.deb \
		$(DEB_DIR)/$(NAME_DEB)*.dsc \
		$(DEB_DIR)/$(NAME_DEB)*.orig.tar.gz \
	icculus.org:~/freyja/files/debian
