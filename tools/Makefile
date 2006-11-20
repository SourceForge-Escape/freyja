################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
################################################################

NAME=tools
BASE_NAME=tools
MAJOR_VERSION=0
MINOR_VERSION=9
MICRO_VERSION=3
BRANCH=
BUILD_ID=20060505
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)$(BRANCH)
BUILD_HOST=`uname -n -m -s`
WEBSITE="http://icculus.org/freyja"
EMAIL="mongoose@icculus.org"
EXT=.so
TREE_DIR=libfreyja
TARBALL_NAME=$(NAME)-$(VERSION).$(BUILD_ID)$(BRANCH)
LICENSE=LGPL


################################################################
# Build options
################################################################

BUILD_SELECT=tools
BASE_DEFS=-DMODEL_PLUGINS 
DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL
CONFIGURE_DEF=-DFREYJAIMAGE_PLUGINS -DFREYJA_PLUGINS
CONFIGURE_LNK=
BUILD_ROOT_DIR=../bin/tools
BUILD_DEBUG_DIR=$(BUILD_ROOT_DIR)/debug
BUILD_RELEASE_DIR=$(BUILD_ROOT_DIR)/release
BUILD_PROF_DIR=$(BUILD_ROOT_DIR)/prof
BUILD_WIN32_DIR=$(BUILD_ROOT_DIR)/win32
BUILD_OSX_DIR=$(BUILD_ROOT_DIR)/osx
BUILD_TEST_DIR=$(BUILD_ROOT_DIR)/test
BUILD_INSTALL_DIR=$(BUILD_ROOT_DIR)/$(BUILD_SELECT)
SRC_DIR=src
LOCAL_INCLUDES=-I../mstl -I../libhel/ -I../libfreyja/
LOCAL_LIBS=-L../bin/libhel/debug/ -L../bin/libfreyja/debug/ 


################################################################
# Compiler and Cross Enviroments
################################################################

CC=gcc
DEPS=../deps.sh
-include ../win32.env


################################################################
# Build options
################################################################

DESTDIR =
NO_DEB=
NO_DEB=local/
INSTALL_BIN=$(DESTDIR)/usr/$(NO_DEB)bin
INSTALL_LIB=$(DESTDIR)/usr/$(NO_DEB)lib
INSTALL_DOC=$(DESTDIR)/usr/share/doc/$(NAME)
INSTALL_SHARE=$(DESTDIR)/usr/share/$(NAME)
INSTALL_INCLUDE=$(DESTDIR)/usr/$(NO_DEB)include
INSTALL_PLUGIN_DIR=$(INSTALL_LIB)/$(BASE_NAME)/modules


################################################################
# Debian package setup
################################################################

DEB_DIR=/tmp/$(NAME).deb
NAME_DEB=$(NAME)
VERSION_DEB=$(VERSION).$(BUILD_ID)
# Package types: -s single bin, -l single lib, -m multiple bins
FLAGS_DEB=-s


################################################################
# Compiler flags
################################################################

BASE_CFLAGS=-fPIC -Wall -I$(SRC_DIR) $(LOCAL_INCLUDES) \
	$(CONFIGURE_DEF) \
	-DPLUGIN_IMAGE_DIR=\"\\\"$(INSTALL_PLUGIN_DIR)/image\\\"\" \
	-DVERSION=\"\\\"$(VERSION)-$(BUILD_ID)\\\"\" \
	-DPROJECT_URL=\"\\\"$(WEBSITE)\\\"\" \
	-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
	$(BASE_DEFS)

#BASE_CFLAGS += -Weffc++ -std=c++98
#BASE_CFLAGS += -pedantic

LDFLAGS=$(CONFIGURE_LNK) -lm -ldl $(LOCAL_LIBS)
#LDFLAGS += -Wl,-z,defs
#LDFLAGS += -Wl,-O1
RELEASE_CFLAGS=$(BASE_CFLAGS) -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations -O2
DEBUG_CFLAGS=$(BASE_CFLAGS) -g $(DEBUG_DEFS)

SHLIBEXT=$(EXT).$(VERSION)
SHLIBLDFLAGS=$(LDFLAGS) -fPIC -shared

MODULE_CFLAGS=$(BASE_CFLAGS) -g -DFREYJA_PLUGINS
MODULE_LD_FLAGS=-lfreyja -lstdc++ -shared -fPIC -ldl -rdynamic 
#MODULE_LD_FLAGS += --warn-unresolved-symbols -Wl -z defs
MODULE_LD_FLAGS += --warn-unresolved-symbols -Wl

################################################################

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<
DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

TARGETS=$(BUILDDIR)/$(NAME)$(SHLIBEXT)
TESTS=$(BUILDDIR)/$(NAME).test


################################################################
# Build rules
################################################################

tools: freyjautil.test raw2riff.test cull.test freyjaimageutil.test
#bsa.test 

auto: $(BUILD_SELECT)

all: debug release prof

debug:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) \
	CFLAGS="$(DEBUG_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS)"

prof:
	@-mkdir -p $(BUILD_PROF_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_PROF_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) -pg" \
	LD_FLAGS="$(LD_FLAGS) -pg"

release:
	@-mkdir -p $(BUILD_RELEASE_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_RELEASE_DIR) \
	CFLAGS="$(RELEASE_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS)"

win32:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_WIN32_DIR) \
	NAME="freyja9" \
	MODULE="freyja9" \
	SHLIBEXT=".dll" \
	CC="$(WIN32_CC)" \
	CPP="$(WIN32_CPP) " \
	LD="$(WIN32_LD) " \
	CFLAGS="$(BASE_CFLAGS) $(WIN32_INC_DIR)" \
	LD_FLAGS="$(CONFIGURE_LNK) -lm" \
	LIBS="-lstdc++ -lm $(WIN32_LIB_DIR) $(WIN32_LIBHEL)"

osx:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) \
	NAME="freyja9" \
	MODULE="freyja9" \
	SHLIBEXT=".dynlib" \
	CFLAGS="$(DEBUG_CFLAGS)" \
	LD_FLAGS="$(CONFIGURE_LNK) -lm -ldl"


################################################################
# Include Dependences
################################################################

depend:
	@-echo -n Making deps...
	@-echo "# Autogenerated dependency file" > depend
	@-find $(SRC_DIR) -name "*.cpp" -exec $(DEPS) $(BASE_DEFS) {} \; >> depend
	@-echo "       [done]"



################################################################
# Main targets and objects
################################################################

targets: $(TARGETS)

PROG_OBJS = \
	$(BUILDDIR)/Action.o \
	$(BUILDDIR)/Bone.o \
	$(BUILDDIR)/CopyModel.o \
	$(BUILDDIR)/Egg.o \
	$(BUILDDIR)/FreyjaFSM.o \
	$(BUILDDIR)/FreyjaPluginABI.o \
	$(BUILDDIR)/FreyjaPlugin.o \
	$(BUILDDIR)/FreyjaCamera.o \
	$(BUILDDIR)/FreyjaPrinter.o \
	$(BUILDDIR)/FreyjaFileReader.o \
	$(BUILDDIR)/FreyjaFileWriter.o \
	$(BUILDDIR)/FreyjaImage.o \
	$(BUILDDIR)/FreyjaLight.o \
	$(BUILDDIR)/FreyjaMesh.o \
	$(BUILDDIR)/FreyjaMaterial.o \
	$(BUILDDIR)/FreyjaPakReader.o \
	$(BUILDDIR)/FreyjaTexture.o \
	$(BUILDDIR)/FreyjaUtil.o \
	$(BUILDDIR)/KeyFrame.o \
	$(BUILDDIR)/PerlinNoise.o \
	$(BUILDDIR)/RenderModel.o \
	$(BUILDDIR)/RigidBody.o \
	$(BUILDDIR)/Skeleton.o

#	$(BUILDDIR)/FreyjaScene.o 


$(BUILDDIR)/$(NAME)$(SHLIBEXT) : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(PROG_OBJS)
	@-cp $(BUILDDIR)/$(NAME)$(SHLIBEXT) $(BUILDDIR)/$(NAME).so

$(BUILDDIR)/$(MODULE).dll : $(PROG_OBJS)
	$(CC) -shared -o $(BUILDDIR)/$(MODULE).dll \
	-Wl,--out-implib=$(BUILDDIR)/lib$(MODULE).a \
	-Wl,--export-all-symbols \
	-Wl,--enable-auto-import \
	-Wl,--whole-archive $(PROG_OBJS) \
	-Wl,--no-whole-archive $(LIBS)

$(BUILDDIR)/$(MODULE).dynlib : $(PROG_OBJS)
	$(CC) $(LD_FLAGS) -L/usr/local/lib -lhel0 \
	-L/usr/lib -lstdc++ -fPIC -dynamiclib \
	-o $(BUILDDIR)/$(MODULE).dynlib \
	$(PROG_OBJS)


##########################################################################
# Unit tests
##########################################################################

TEST=$(BUILDDIR)/$(NAME).test

test: $(TEST)

##########################################################################

SDLUNIT_CFLAGS=$(BASE_CFLAGS) $(shell sdl-config --cflags) \
		-DHAVE_SDL -DHAVE_OPENGL -DHAVE_SDL_TTF -DHAVE_FREYJA
SDLUNIT_LDFLAGS=$(shell sdl-config --libs) -lSDL_ttf -lGL -lGLU -lm -lstdc++ \
		-lhel0 -lfreyja
SDLUNIT_OBJS=$(BUILD_TEST_DIR)/SDLUnitTest.o

##########################################################################

Md3AnimModel.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=Md3AnimModel BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(SDLUNIT_OBJS) $(BUILD_TEST_DIR)/Md3.o $(BUILD_TEST_DIR)/Md3AnimModel.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_MD3ANIMMODEL $(SDLUNIT_CFLAGS)" \
	LD_FLAGS="$(SDLUNIT_LDFLAGS)"

PSKModel.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=PSKModel BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(SDLUNIT_OBJS) $(BUILD_TEST_DIR)/PSKModel.o $(BUILD_TEST_DIR)/UTPackage.o $(BUILD_TEST_DIR)/URTexture.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_PSKMODEL -DHAVE_UTPACKAGE $(SDLUNIT_CFLAGS)" \
	LD_FLAGS="$(SDLUNIT_LDFLAGS)"

MDLModel.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=MDLModel BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(SDLUNIT_OBJS) $(BUILD_TEST_DIR)/MDLModel.o $(BUILD_TEST_DIR)/MathLib.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_MDLMODEL $(SDLUNIT_CFLAGS)" \
	LD_FLAGS="$(SDLUNIT_LDFLAGS)"

##########################################################################

ase.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=ase BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/Ase.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_ASE $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"

bsa.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=bsa BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/BSAPakReader.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_BSAPAKREADER $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"

glm.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=glm BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/GLMModel.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_GLMMODEL $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"

md5.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=md5 BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/Md5.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_MD5 $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"

nod.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=nod BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/Nod.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_NOD $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"

jpeg.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test BUILDDIR=$(BUILD_TEST_DIR) \
	NAME=jpeg \
	OBJS="$(BUILD_TEST_DIR)/image_test.o $(BUILD_TEST_DIR)/image_jpeg.o" \
	CFLAGS="-Wall -g -O0 -DLIB_JPEG" \
	LD_FLAGS="-lm -lstdc++ -lhel0 -lfreyja -ljpeg"

freyjaimageutil.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=freyjaimageutil BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/freyjaimageutil.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_DDS2TGA $(BASE_CFLAGS)" \
	LD_FLAGS="$(LOCAL_LIBS) -lm -ldl -lstdc++ -lhel0 -lfreyja"

cull.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=cull BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/cull.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_CULL $(BASE_CFLAGS)" \
	LD_FLAGS="$(LOCAL_LIBS) -lm -ldl -lstdc++ -lhel0 -lfreyja"

raw2riff.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=raw2riff BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/raw2riff.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_EGG $(BASE_CFLAGS)" \
	LD_FLAGS="$(LOCAL_LIBS) -lm -ldl -lstdc++ -lhel0 -lfreyja"

freyjautil.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test BUILDDIR=$(BUILD_TEST_DIR) \
	NAME=freyjautil \
	OBJS="$(BUILD_TEST_DIR)/freyjautil.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_FREYJAUTIL" \
	LD_FLAGS="$(LOCAL_LIBS) -lm -lstdc++ -lhel0 -lfreyja"

cycle-win32: win32 install-win32 test-win32

test-win32: freyjautil-win32.test

freyjautil-win32.test:
	mkdir -p $(BUILD_TEST_DIR)
	CC="i586-mingw32msvc-g++ -mms-bitfields" \
	CPP="i586-mingw32msvc-cpp " \
	LD="i586-mingw32msvc-ld " \
	CFLAGS="$(DEBUG_CFLAGS) -I/usr/local/include/" \
	i586-mingw32msvc-g++ -mms-bitfields \
		-Wall -g -pipe -O0 -I/usr/local/include/ \
		tools/freyjautil.cpp \
		-o $(BUILD_TEST_DIR)/freyjautil-win32.test \
		-lstdc++ -L/usr/local/lib/ -lhel0 -lfreyja9 

python.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test BUILDDIR=$(BUILD_TEST_DIR) \
	NAME=python \
	OBJS="$(BUILD_TEST_DIR)/python.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_PYTHON" \
	LD_FLAGS="-lm -lhel0 -lfreyja -lpython2.3 -lstdc++"

UTPackage.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=UTPackage BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/UTPackage.o $(BUILD_TEST_DIR)/URTexture.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_UTPACKAGE -DHAVE_FREYJA $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja"


UTPackage-win32.test:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) test NAME=UTPackage.exe BUILDDIR=$(BUILD_WIN32_DIR) \
	OBJS="$(BUILD_WIN32_DIR)/UTPackage.o $(BUILD_WIN32_DIR)/URTexture.o" \
	CC="i586-mingw32msvc-g++ -mms-bitfields" \
	CPP="i586-mingw32msvc-cpp " \
	LD="i586-mingw32msvc-ld " \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_UTPACKAGE -DHAVE_FREYJA -I/usr/local/include $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9"
	@-mv $(BUILD_WIN32_DIR)/UTPackage.exe.test $(BUILD_WIN32_DIR)/UTPackage.exe


lineageIIextract.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=lineageIIextract BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/extractFace.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_UTPACKAGE -DHAVE_FREYJA $(BASE_CFLAGS)" \
	LD_FLAGS="-lstdc++"


lineageIIextract-win32.test:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) test NAME=lineageIIextract.exe BUILDDIR=$(BUILD_WIN32_DIR) \
	OBJS="$(BUILD_WIN32_DIR)/extractFace.o" \
	CC="i586-mingw32msvc-g++ -mms-bitfields" \
	CPP="i586-mingw32msvc-cpp " \
	LD="i586-mingw32msvc-ld " \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_UTPACKAGE -DHAVE_FREYJA -I/usr/local/include $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9"
	@-mv $(BUILD_WIN32_DIR)/lineageIIextract.exe.test $(BUILD_WIN32_DIR)/lineageIIextract.exe


PerlinNoise.test:
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) -Wall -g -pipe -O0 -lstdc++ -Isrc \
		-DUNIT_TEST_PERLINNOISE \
		plugins/image/tga.cpp \
		src/PerlinNoise.cpp -o $(BUILD_TEST_DIR)/PerlinNoise.test

#################################################################

$(BUILDDIR)/$(NAME).test : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LD_FLAGS)


##########################################################################
# Object dependency include
##########################################################################

-include depend

##########################################################################

clean: clean-emacs clean-build clean-test clean-obj clean-win32

clean-test:
	@-echo -n "Cleaning test builds                         "
	@-rm -f $(BUILD_TEST_DIR)/*.test
	@-echo "[done]"

clean-obj:
	@-echo -n "Cleaning objects                             "
	@-rm -f $(BUILD_PROF_DIR)/*.o
	@-rm -f $(BUILD_DEBUG_DIR)/*.o
	@-rm -f $(BUILD_RELEASE_DIR)/*.o
	@-rm -f $(BUILD_TEST_DIR)/*.o
	@-echo "[done]"

clean-win32:
	@-echo -n "Cleaning WIN32 builds                        "
	@-rm -f $(BUILD_WIN32_DIR)/*.o
	@-rm -f $(BUILD_WIN32_DIR)/*.a
	@-rm -f $(BUILD_WIN32_DIR)/*.dll
	@-echo "[done]"

clean-emacs:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[done]"

clean-build:
	@-echo -n "Cleaning builds                              "
	@-rm -f $(BUILD_PROF_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_DEBUG_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_RELEASE_DIR)/$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_PROF_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.so
	@-rm -f $(BUILD_RELEASE_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.test
	@-echo "[done]"


################################################################

install:
	@-echo -n "Installing selected build                     "
	@-mkdir -p $(INSTALL_LIB)
	@-mkdir -p $(INSTALL_LIB)/$(BASE_NAME)
	@-cp $(BUILD_INSTALL_DIR)/$(NAME)$(SHLIBEXT) $(INSTALL_LIB)
	@-echo "[done]"
	@-$(MAKE) install-link
	@-$(MAKE) install-headers
	@-./libsetup.sh $(INSTALL_LIB)
	@-echo $(MAKE) plugins
	@-echo $(MAKE) install-plugins
	@-echo $(MAKE) tools
	@-echo $(MAKE) install-tools

install-win32:
	@-mkdir -p $(INSTALL_LIB)
	@-mkdir -p $(INSTALL_LIB)/$(BASE_NAME)
	@-echo -n "Installing win32 build                   "
	@-cp $(BUILD_WIN32_DIR)/*.a $(BUILD_WIN32_DIR)/*.dll $(INSTALL_LIB)
	@-echo "[done]"
	@-$(MAKE) install-link
	@-$(MAKE) install-headers
	@-./libsetup.sh $(INSTALL_LIB)
	@-echo $(MAKE) win32-plugins
	@-echo $(MAKE) win32-install-plugins
	@-echo $(MAKE) win32-tools
	@-echo $(MAKE) win32-install-tools

install-link:
	@-echo -n "New soft links                               "
	@-cd $(INSTALL_LIB) && ln -sf $(NAME)$(SHLIBEXT) $(NAME)$(EXT)
	@-echo "[done]"

install-headers:
	@-echo -n "Installing headers                           "
	@-mkdir -p $(INSTALL_INCLUDE)/$(BASE_NAME)
	@-cp -f src/*.h $(INSTALL_INCLUDE)/$(BASE_NAME)
	@-echo "[done]"


################################################################

uninstall:
	@-echo -n "Uninstalling                                 " 
	@-rm -f $(INSTALL_LIB)/$(NAME)$(EXT)
	@-rm -f $(INSTALL_LIB)/$(NAME)$(SHLIBEXT)
	@-rm -f $(INSTALL_BIN)/$(BASE_NAME)
	@-rm -rf $(INSTALL_LIB)/$(BASE_NAME)
	@-rm -rf $(INSTALL_INCLUDE)/$(BASE_NAME)
	@-echo "[done]"


################################################################

tarball:
	@-echo "Making tarball                                  " 
	@-cd .. && tar zcvf $(TARBALL_NAME).tar.gz  \
		--exclude=bin --exclude=.svn --exclude=*~ --exclude=contrib \
		--totals \
		$(TREE_DIR)
	@-echo "[done]"

backup:
	@-$(MAKE) clean
	@-cd .. && tar Icvf $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 \
	$(TREE_DIR) \
	--exclude bin --exclude *~ --totals \
	&& cp $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 ~/Backup


##########################################################################

