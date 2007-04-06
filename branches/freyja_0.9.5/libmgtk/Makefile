################################################################
# Mongoose <mongooseichiban@gmail.com>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
################################################################

NAME=mgtk
MAJOR_VERSION=0
MINOR_VERSION=1
MICRO_VERSION=0
PRE=
BUILD_ID=20050724
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)$(PRE)
EXT=.so
TREE_DIR=libmgtk
TARBALL_NAME=lib$(NAME)-$(VERSION).$(BUILD_ID)
BUILD_HOST=`uname -n -m -s`
URL=http://icculus.org/freyja
EMAIL=mongoose@icculus.org

################################################################
# Directory Setup
################################################################

INSTALL_DIR=/usr/local/bin
#$(NAME)-$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
SRC_DIR=mgtk
PACKAGE_DIR=/tmp/package/$(NAME)

BUILD_SELECT=debug
BUILD_ROOT_DIR=../bin/libmgtk
BUILD_PROF_DIR=$(BUILD_ROOT_DIR)/prof
BUILD_DEBUG_DIR=$(BUILD_ROOT_DIR)/debug
BUILD_RELEASE_DIR=$(BUILD_ROOT_DIR)/release
BUILD_WIN32_DIR=$(BUILD_ROOT_DIR)/win32
BUILD_TEST_DIR=$(BUILD_ROOT_DIR)/test
BUILD_INSTALL_DIR=$(BUILD_ROOT_DIR)/$(BUILD_SELECT)
INSTALL_BIN=/usr/local/bin
#INSTALL_LIB=/usr/local/lib
#INSTALL_INCLUDE=/usr/local/include
PLUGIN_INSTALL_DIR=$(INSTALL_LIB)/$(NAME)/modules
LOCAL_INCLUDES=-I../mstl/ -I../libhel/
LOCAL_LIBS=-L../bin/libhel/$(BUILD_SELECT)/ 
LOCAL_WIN32_LIBS=-L../bin/libhel/win32/ 


################################################################
# Configure support was removed, but options are set here
################################################################
# DEBUG_GTK_MOUSE    
# DEBUG_GTK_KEYS
# DISABLE_GTK_COLORBUTTON
# USE_OLD_FILE_SELECTION_WIDGET
# HAVE_GTKGLEXT
# HAVE_GTKGLAREA
CONFIGURE_CFLAGS=-DUSING_GTK_2_0 -DHAVE_FREYJA_IMAGE -DHAVE_OPENGL \
		-DHAVE_GTKGLEXT -DFORCE_ICON_TOOLBAR -DMGTKEXPORTSCLASS \
		$(LOCAL_INCLUDES)

CONFIGURE_LDFLAGS=$(LOCAL_LIBS)

DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL


################################################################
# PKG_CONFIG Setup
################################################################
# gtkglext-win32-1.0
# gtkgl-2.0
PKG_CONFIG_CFLAGS="gtk+-2.0 gtkglext-1.0"
PKG_CONFIG_LIBS="gtk+-2.0 gtkglext-1.0"


################################################################
# Compiler and Cross Enviroment
################################################################
CC=gcc
DEPS=../deps.sh
-include ../win32.env


################################################################

# -Weffc++
BASE_DEFS= \
	-I$(SRC_DIR) $(CONFIGURE_CFLAGS) -Wno-deprecated \
	$(shell pkg-config --cflags $(PKG_CONFIG_CFLAGS))

BASE_CFLAGS=-fPIC -Wall -I$(SRC_DIR) \
		-DPROJECT_URL=\"\\\"$(URL)\\\"\" \
		-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
		-DVERSION=\"\\\"$(VERSION)\\\"\" \
		-DBUILD_ID=\"\\\"$(BUILD_ID)\\\"\" \
		-DPROGRAM_NAME=\"\\\"$(NAME)\\\"\" \
		-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\" \
		-DBUILD_HOST=\"\\\"$(BUILD_HOST)\\\"\" 

LD_FLAGS=$(CONFIGURE_LDFLAGS) -lm -ldl -lGL -lGLU -lstdc++
LD_FLAGS += -Wl,-z,defs
LD_FLAGS += -Wl,-O1

################################################################

GTK2_CFLAGS=$(shell pkg-config --cflags $(PKG_CONFIG_CFLAGS))

GTK2_LDFLAGS=$(shell pkg-config --libs $(PKG_CONFIG_LIBS))

RELEASE_CFLAGS=$(BASE_CFLAGS) $(CONFIGURE_CFLAGS) -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations -O2

DEBUG_CFLAGS=$(BASE_CFLAGS) $(CONFIGURE_CFLAGS) -g $(DEBUG_DEFS)

SHLIBEXT=$(EXT).$(VERSION)

SHLIBLDFLAGS=$(LDFLAGS) -fPIC -shared


################################################################
# Main build targets
################################################################

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<
DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

TARGETS=$(BUILDDIR)/lib$(NAME)$(SHLIBEXT)
W32TARGETS=$(BUILDDIR)/$(MODULE)$(SHLIBEXT)
OSXTARGETS=$(BUILDDIR)/$(FRAMEWORK)$(SHLIBEXT)
TESTS=$(BUILDDIR)/$(NAME).test


########################################################################
# Main build rules
########################################################################

auto: $(BUILD_SELECT)

all: debug release prof

debug:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GTK2_CFLAGS)" \
	SHLIBLDFLAGS="$(SHLIBLDFLAGS)"
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS)"

release:
	@-mkdir -p $(BUILD_RELEASE_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_RELEASE_DIR) \
	CFLAGS="$(RELEASE_CFLAGS) $(GTK2_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS)"

prof: 
	@-mkdir -p $(BUILD_PROF_DIR)
	$(MAKE) targets BUILDDIR=$(BUILD_PROF_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GTK2_CFLAGS) -DNDEBUG -pg" \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS) -pg"


################################################################

win32: debug-win32

debug-win32:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) w32targets BUILDDIR=$(BUILD_WIN32_DIR) \
	CC=$(WIN32_CC) \
	CPP=$(WIN32_CPP) \
	LD=$(WIN32_LD) \
	MODULE="mgtk1" \
	SHLIBEXT=".dll" \
	CFLAGS="$(BASE_CFLAGS) $(WIN32_CROSS_CFLAGS) $(CONFIGURE_CFLAGS)" \
	LD_FLAGS="$(WIN32_CROSS_LIBS) $(LD_FLAGS)" \
	LIBS="$(WIN32_LIB_DIR) -lm -lstdc++ $(WIN32_CROSS_LIBS)"


################################################################

osx: debug-osx

debug-osx:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) osxtargets BUILDDIR=$(BUILD_DEBUG_DIR) \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS)" \
	SHLIBLDFLAGS="$(LD_FLAGS) -dynamiclib -fPIC" \
	CFLAGS="$(DEBUG_CFLAGS) $(GTK2_CFLAGS)"


########################################################################

osxtargets: $(OSXTARGETS)

w32targets: $(W32TARGETS)

targets: $(TARGETS)

PROG_OBJS = \
	$(BUILDDIR)/ConfirmationDialog.o \
	$(BUILDDIR)/MLisp.o \
	$(BUILDDIR)/Resource.o \
	$(BUILDDIR)/ResourceEvent.o \
	$(BUILDDIR)/mgtk_callbacks.o \
	$(BUILDDIR)/mgtk_interface.o \
	$(BUILDDIR)/mgtk_linker.o \
	$(BUILDDIR)/mgtk_resource.o \
	$(BUILDDIR)/mgtk_filedialog.o \
	$(BUILDDIR)/mgtk_tree.o

$(BUILDDIR)/lib$(NAME)$(SHLIBEXT) : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(PROG_OBJS)
	@-cp $(BUILDDIR)/lib$(NAME)$(SHLIBEXT) $(BUILDDIR)/lib$(NAME).so

$(BUILDDIR)/$(MODULE).dll : $(PROG_OBJS)
	$(CC) -shared -o $(BUILDDIR)/$(MODULE).dll \
    -Wl,--out-implib=$(BUILDDIR)/lib$(MODULE).a \
    -Wl,--export-all-symbols \
    -Wl,--enable-auto-import \
    -Wl,--whole-archive $(PROG_OBJS) \
    -Wl,--no-whole-archive $(LIBS)


##########################################################################
# Unit tests
##########################################################################

TEST=$(BUILDDIR)/$(NAME).test

test: $(TEST)

raw2riff.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(MAKE) test NAME=raw2riff BUILDDIR=$(BUILD_TEST_DIR) \
	OBJS="$(BUILD_TEST_DIR)/raw2riff.o" \
	CFLAGS="-Wall -g -O0 -DUNIT_TEST_EGG $(CONFIGURE_CFLAGS) $(BASE_CFLAGS)" \
	LD_FLAGS="-lm -ldl -lstdc++ -lhel0 -lfreyja8"

$(BUILDDIR)/$(NAME).test : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LD_FLAGS)


##########################################################################
# Dependency rule and depend include 
########################################################################

depend:
	@-echo -n Making deps...
	@-echo "# Autogenerated dependency file" > depend
	@-find $(SRC_DIR) -name "*.cpp" -exec $(DEPS) $(BASE_DEFS) {} \; >> depend
	@-echo "       [done]"


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
	@-rm -f $(BUILD_DEBUG_DIR).gtkglext/*.o
	@-rm -f $(BUILD_RELEASE_DIR)/*.o
	@-rm -f $(BUILD_TEST_DIR)/*.o
	@-echo "[done]"

clean-win32:
	@-echo -n "Cleaning WIN32 builds                        "
	@-rm -f $(BUILD_WIN32_DIR)/*.o
	@-rm -f $(BUILD_WIN32_DIR)/*.dll
	@-rm -f $(BUILD_WIN32_DIR)/*.a
	@-echo "[done]"

clean-emacs:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[done]"

clean-build:
	@-echo -n "Cleaning builds                              "
	@-rm -f $(BUILD_PROF_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_DEBUG_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_DEBUG_DIR).gtkglext/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_RELEASE_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_PROF_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR).gtkglext/*.so
	@-rm -f $(BUILD_RELEASE_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.test
	@-echo "[done]"


#################################################################

DEB_DIR=/tmp/$(NAME).deb
NAME_DEB=libmgtk
VERSION_DEB=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION).$(BUILD_ID)

DESTDIR =
NO_DEB=
INSTALL_BIN=$(DESTDIR)/usr/$(NO_DEB)bin
INSTALL_LIB=$(DESTDIR)/usr/$(NO_DEB)lib
INSTALL_DOC=$(DESTDIR)/usr/share/doc/$(NAME)
INSTALL_SHARE=$(DESTDIR)/usr/share/$(NAME)
INSTALL_INCLUDE=$(DESTDIR)/usr/$(NO_DEB)include
INSTALL_PLUGIN_DIR=$(INSTALL_LIB)/$(BASE_NAME)/modules

deb:
	dpkg-buildpackage -rfakeroot
	$(MAKE) deb-info

deb-info:
	@-printf "================================================================================\n"
	dpkg --info ../libmgtk_*.deb
	@-printf " -------------------------------------------------------------------------------\n"
	dpkg --contents ../libmgtk_*.deb
	@-printf "================================================================================\n"

################################################################

install: install-gtkglext

install-gtkglext:
	@-echo -n "Installing selected build                     "
	@-mkdir -p  $(INSTALL_LIB)
	@-cp $(BUILD_DEBUG_DIR)/lib$(NAME)$(SHLIBEXT) $(INSTALL_LIB)/
	@-echo "[done]"
	@-$(MAKE) install-link
	@-$(MAKE) install-headers
	@-./libsetup.sh $(INSTALL_LIB)


install-gtkglarea:
	@-echo -n "Installing selected build                     "
	@-mkdir -p  $(INSTALL_LIB)
	@-cp bin/debug/libmgtk$(SHLIBEXT) $(INSTALL_LIB)/
	@-echo "[done]"
	@-$(MAKE) install-link
	@-$(MAKE) install-headers
	@-./libsetup.sh $(INSTALL_LIB)


install-win32:
	@-echo -n "Installing win32 build                   "
	@-cp $(BUILD_WIN32_DIR)/*.a $(BUILD_WIN32_DIR)/*.dll $(INSTALL_LIB)
	@-echo "[done]"

install-link:
	@-echo -n "New soft links                               "
	@-cd $(INSTALL_LIB) && ln -sf libmgtk$(SHLIBEXT) libmgtk.so
	@-echo "[done]"

install-headers:
	@-echo -n "Installing headers                           "
	@-mkdir -p $(INSTALL_INCLUDE)/$(NAME)
	@-cp -f src/*.h $(INSTALL_INCLUDE)/$(NAME)
	@-echo "[done]"


################################################################

uninstall:
	@-echo -n "Uninstalling                                 " 
	@-rm -f $(INSTALL_LIB)/lib$(NAME)$(EXT)
	@-rm -f $(INSTALL_LIB)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(INSTALL_BIN)/$(NAME)
	@-rm -rf $(INSTALL_LIB)/$(NAME)
	@-rm -rf $(INSTALL_INCLUDE)/$(NAME)
	@-echo "[done]"


################################################################

tarball:
	@-echo "Making tarball                                  " 
	@-cd .. && tar zcvf $(TARBALL_NAME).tar.gz \
		--exclude=bin --exclude=CVS --exclude=*~ --exclude=contrib \
		--exclude=.svn --totals $(TREE_DIR)
	@-echo "[done]"

backup:
	@-$(MAKE) clean
	@-cd .. && tar Icvf $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 \
	$(TREE_DIR) \
	--exclude bin --exclude *~ --totals \
	&& cp $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 ~/Backup


################################################################
# Unit tests
################################################################

MLispWX.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -ggdb -DUNIT_TEST_MLISPWX -lstdc++ `wx-config --libs --cxxflags --gl-libs` \
	mgtk/MLisp.cpp mgtk/MLispWX.cpp -o $(BUILD_TEST_DIR)/MLispWX.test

MLisp.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_MLISP -lstdc++ \
	mgtk/MLisp.cpp -o $(BUILD_TEST_DIR)/MLisp.test

Resource.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_RESOURCE -DDEBUG -Isrc -lstdc++ $(GTK2_LDFLAGS)  $(GTK2_CFLAGS) -DEMAIL_ADDRESS="\"No\"" -DBUILD_NAME="\"unit\"" -I../mstl/ \
	mgtk/Resource.cpp 

Resource2.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_RESOURCE -DDEBUG -Isrc -lstdc++ $(GTK2_LDFLAGS)  $(GTK2_CFLAGS) $(CONFIGURE_CFLAGS) -DNOLAZYCB -DEMAIL_ADDRESS="\"No\"" -DBUILD_NAME="\"unit\"" -I../mstl/ \
	mgtk/Resource.cpp mgtk/ResourceEvent.cpp mgtk/mgtk_callbacks.cpp mgtk/mgtk_interface.cpp mgtk/mgtk_resource.cpp mgtk/mgtk_tree.cpp -o $(BUILD_TEST_DIR)/Resource.test  



