################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
################################################################

NAME=libfreyja
BASE_NAME=freyja
MAJOR_VERSION=0
MINOR_VERSION=9
MICRO_VERSION=5
BRANCH=
BUILD_ID=20061124
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

BUILD_SELECT=debug
BASE_DEFS=-DMODEL_PLUGINS
DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL
CONFIGURE_DEF=-DFREYJAIMAGE_PLUGINS -DFREYJA_PLUGINS
CONFIGURE_LNK=
BUILD_ROOT_DIR=../bin/libfreyja
BUILD_DEBUG_DIR=$(BUILD_ROOT_DIR)/debug
BUILD_RELEASE_DIR=$(BUILD_ROOT_DIR)/release
BUILD_PROF_DIR=$(BUILD_ROOT_DIR)/prof
BUILD_WIN32_DIR=$(BUILD_ROOT_DIR)/win32
BUILD_OSX_DIR=$(BUILD_ROOT_DIR)/osx
BUILD_TEST_DIR=$(BUILD_ROOT_DIR)/test
BUILD_INSTALL_DIR=$(BUILD_ROOT_DIR)/$(BUILD_SELECT)
SRC_DIR=freyja
LOCAL_INCLUDES=-I../mstl -I../libhel/
LOCAL_LIBS=-L../bin/libhel/$(BUILD_SELECT)/ 
LOCAL_WIN32_LIBS=-L../bin/libhel/win32/ 


################################################################
# Compiler and Cross Enviroments
################################################################

CC=gcc
DEPS=./deps.sh
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

BASE_CFLAGS=-Wall -I. -I$(SRC_DIR) $(LOCAL_INCLUDES) \
	$(CONFIGURE_DEF) \
	-DPLUGIN_IMAGE_DIR=\"\\\"$(INSTALL_PLUGIN_DIR)/image\\\"\" \
	-DVERSION=\"\\\"$(VERSION)-$(BUILD_ID)\\\"\" \
	-DPROJECT_URL=\"\\\"$(WEBSITE)\\\"\" \
	-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
	$(BASE_DEFS) \
	-fPIC

BASE_CFLAGS += -Weffc++ -std=c++98
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
	MODULE="freyja9" \
	NAME="freyja9" \
	SHLIBEXT=".dll" \
	CC=$(WIN32_CC) \
	CPP=$(WIN32_CPP) \
	LD=$(WIN32_LD) \
	CFLAGS="-DWIN32 $(DEBUG_CFLAGS) $(WIN32_INC_DIR) " \
	LD_FLAGS="$(WIN32_LIB_DIR) $(LOCAL_WIN32_LIBS) " \
	LIBS="$(WIN32_LIB_DIR) $(LOCAL_WIN32_LIBS) -lstdc++ -lm $(WIN32_LIBHEL)"

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
	$(BUILDDIR)/freyja.o \
	$(BUILDDIR)/Bone.o \
	$(BUILDDIR)/BoneABI.o \
	$(BUILDDIR)/Camera.o \
	$(BUILDDIR)/FreyjaFSM.o \
	$(BUILDDIR)/FreyjaPrinter.o \
	$(BUILDDIR)/FreyjaImage.o \
	$(BUILDDIR)/FreyjaPakReader.o \
	$(BUILDDIR)/FreyjaTexture.o \
	$(BUILDDIR)/KeyFrame.o \
	$(BUILDDIR)/KeyFrameABI.o \
	$(BUILDDIR)/LegacyABI.o \
	$(BUILDDIR)/Light.o \
	$(BUILDDIR)/LightABI.o \
	$(BUILDDIR)/Material.o \
	$(BUILDDIR)/MaterialABI.o \
	$(BUILDDIR)/Mesh.o \
	$(BUILDDIR)/MeshABI.o \
	$(BUILDDIR)/Model.o \
	$(BUILDDIR)/ModelABI.o \
	$(BUILDDIR)/PakABI.o \
	$(BUILDDIR)/PerlinNoise.o \
	$(BUILDDIR)/PluginABI.o \
	$(BUILDDIR)/Plugin.o \
	$(BUILDDIR)/PythonABI.o \
	$(BUILDDIR)/RigidBody.o \
	$(BUILDDIR)/Skeleton.o \
	$(BUILDDIR)/Track.o 



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
# Object dependency include
##########################################################################

-include depend

##########################################################################
# Make clean
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


#################################################################

clean-deb:
	@-echo -n "Cleaning DEBs                                "
	@-rm -rf $(DEB_DIR)
	@-echo "[DONE]"

debian-package:
	$(MAKE) clean
	$(MAKE) clean-deb
	$(MAKE) tarball
	mkdir -p $(DEB_DIR)
	cp ../$(NAME_DEB)-$(VERSION_DEB).tar.gz $(DEB_DIR)
	cd $(DEB_DIR) && tar zxvf $(NAME_DEB)-$(VERSION_DEB).tar.gz && mv $(TREE_DIR) $(NAME_DEB)-$(VERSION_DEB) && cd $(NAME_DEB)-$(VERSION_DEB) && make deb-init && make deb

deb-init:
	yes | dh_make -c $(LICENSE) $(FLAGS_DEB) -e $(EMAIL) -f ../$(NAME_DEB)-$(VERSION_DEB).tar.gz
	@-cp debian/control debian/control.old
	@-sed -e "s/<insert up to 60 chars description>/Freyja backbone/g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/control debian/control.old
	@-sed -e "s/<insert long description, indented with spaces>/3d Modeler./g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/rules debian/rules.old
	@-sed -e "s/MAKE) install/MAKE) install plugins install-plugins tools install-tools/g" debian/rules.old > debian/rules
	@-rm -f debian/rules.old

deb:
	dpkg-buildpackage -rfakeroot

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
	@-cp -f ${SRC_DIR}/*.h $(INSTALL_INCLUDE)/$(BASE_NAME)
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

