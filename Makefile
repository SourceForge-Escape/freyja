
lazybuild:
	@-printf "Installing mstl\n"
	@-cd mstl; make install
	@-cd ..

	@-printf "Installing libhel\n"
	@-cd libhel; ./autogen.sh; make; make install
	@-cd ..

	@-printf "Installing libfreyja\n"
	@-cd libfreyja; ./autogen.sh; make; make install
	@-cd ..
	@-cd libfreyja; make plugins; make install-plugins
	@-cd ..

	@-printf "Installing libmgtk\n"
	@-cd libmgtk; ./autogen.sh; make; make install
	@-cd ..

	@-printf "Installing Freyja\n"
	@-cd Freyja; ./autogen.sh; make && make install
	@-cd ..

	@-printf "\n\n o If your build failed:\n"
	@-printf "       * Make sure you have a complete glext.h header\n"
	@-printf "       * Look in Freyja/Makefile for options to disable\n"
	@-printf "\n\n o Now 'make user-install' as a user\n\n"


clean:
	@-cd mstl; make clean
	@-cd ..

	@-cd libhel; make clean
	@-cd ..

	@-cd libfreyja; make clean
	@-cd ..

	@-cd Freyja; make clean
	@-cd ..

	@-cd egg_plugins; make clean
	@-cd ..

tarball:
	@-cd mstl; make tarball
	@-cd ..

	@-cd libhel; make tarball
	@-cd ..

	@-cd libfreyja; make tarball
	@-cd ..

	@-cd Freyja; make tarball
	@-cd ..

	@-cd egg_plugins; make tarball
	@-cd ..

user-install:
	@-printf "Installing freyja user files\n"
	@-cd Freyja; setup-freyja.sh
	@-cd ..

	@-printf "Installing freyja user plugins\n"
	@-cd egg_plugins; make; make install
	@-cd ..
