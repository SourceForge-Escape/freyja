
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

	@-printf "Installing Freyja\n"
	@-cd Freyja; make; make install
	@-cd ..

	@-printf "\n\n o If your build failed make sure you have a complete glext.h header\n"
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

user-install:
	@-printf "Installing freyja user files\n"
	@-cd Freyja; setup-freyja.sh

	@-printf "Installing freyja user plugins\n"
	@-cd egg_plugins; make; make install
	@-cd ..
