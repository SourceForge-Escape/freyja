printf "Installing mstl\n"
cd mstl; make install
cd ..

printf "Installing libfreyja_image\n"
cd libfreyja_image; make; make install
cd ..

printf "Installing libhel\n"
cd libhel; make; make install
cd ..

printf "Installing libfreyja_model\n"
cd libfreyja_model; make; make install
cd ..

printf "Installing Freyja\n"
cd Freyja; make; make install
cd ..

if [ $USER = "root" ]
then
	su $USERNAME
fi

printf "Installing freyja user files\n"
cd Freyja; setup-freyja.sh

printf "Installing freyja user plugins\n"
cd egg_plugins; make; make install
