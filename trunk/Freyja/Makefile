################################################################
# Mongoose <stu7440@westga.edu>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install
################################################################

NAME=Freyja
MAJOR_VERSION=0
MINOR_VERSION=3
MICRO_VERSION=5
PRE=
BUILD_ID=20040801
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)$(PRE)
BUILD_HOST=`uname -n -m -s`
URL=http://icculus.org/freyja
EMAIL=mongoose@icculus.org

################################################################

INSTALL_DIR=/usr/local/bin
TREE_DIR=$(NAME)
#$(NAME)-$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
SRC_DIR=src
PACKAGE_DIR=/tmp/package/$(NAME)

BUILD_SELECT=debug
BUILD_PROF_DIR=bin/prof
BUILD_DEBUG_DIR=bin/debug
BUILD_RELEASE_DIR=bin/release
BUILD_TEST_DIR=bin/test
BUILD_INSTALL_DIR=bin/$(BUILD_SELECT)

################################################################

# DEBUG_GTK_MOUSE    
# DEBUG_GTK_KEYS
# DEBUG_SCREEN_TO_WORLD
# HAVE_HEL
# DEBUG_EVENT_PRINT
CONFIGURE_CFLAGS=-DPROTOTYPE_02 -DPLANE_NOTIFY_WITH_AXIS \
		-DHAVE_FREYJA_IMAGE -DFORCE_GLAREA_GTK_2_0 \
		-DHAVE_OPENGL -DHAVE_GTKGL
#-DUSE_ARB_MULTITEXTURE 

CONFIGURE_LDFLAGS=

################################################################
CC=gcc
#CC=i586-mingw32msvc-g++
#CPP=i586-mingw32msvc-cpp
#LD=i586-mingw32msvc-ld

DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL

# -Weffc++
BASE_DEFS=-Isrc/common -Isrc/gnome $(CONFIGURE_CFLAGS) -Wno-deprecated \
		-DUSING_GTK_2_0 \
		$(shell pkg-config --cflags gtkgl-2.0 gtk+-2.0)

BASE_CFLAGS=$(CONFIGURE_CFLAGS) \
		-Wall -Isrc/common -Isrc/gnome \
		-DPROJECT_URL=\"\\\"$(URL)\\\"\" \
		-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
		-DVERSION=\"\\\"$(VERSION)\\\"\" \
		-DBUILD_ID=\"\\\"$(BUILD_ID)\\\"\" \
		-DPROGRAM_NAME=\"\\\"$(NAME)\\\"\" \
		-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\" \
		-DBUILD_HOST=\"\\\"$(BUILD_HOST)\\\"\"

LD_FLAGS=-L/usr/X11R6/lib $(CONFIGURE_LDFLAGS) \
		-lstdc++ \
		-lGL -lGLU -lm -lz \
		-lhel0 -lfreyja_model -lfreyja_image
LD_FLAGS += -Wl,-z,defs
LD_FLAGS += -Wl,-O1

GTK2_CFLAGS=-DUSING_GTK_2_0 -I/usr/X11R6/include -I/usr/include/gtkgl-2.0/ \
		$(shell pkg-config --cflags gtk+-2.0 gtkgl-2.0)
GTK2_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)


GTK_GLAREA2_CFLAGS=-DGTK_GLAREA_GL -I/usr/local/include/gtkgl-2.0
GTK_GLAREA2_LD_FLAGS=-lgtkgl-2.0


GTK_GLAREA_CFLAGS=-DGTK_GLAREA_GL -I/usr/local/include/gtkgl
GTK_GLAREA_LD_FLAGS=-lgtkgl

################################################################

RELEASE_CFLAGS=$(BASE_CFLAGS) \
	-ffast-math -funroll-loops -DNDEBUG \
	-fomit-frame-pointer -fexpensive-optimizations -O2

DEBUG_CFLAGS=$(BASE_CFLAGS) -g $(DEBUG_DEFS)

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<

################################################################

TARGETS=$(BUILD_DIR)/$(NAME)

auto: $(BUILD_SELECT)

################################################################
# GTK+ 2.0 
debug:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_DEBUG_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GTK2_CFLAGS) $(GTK_GLAREA2_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS) $(GTK_GLAREA2_LD_FLAGS)"

release:
	@-mkdir -p $(BUILD_RELEASE_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_RELEASE_DIR) \
	CFLAGS="$(RELEASE_CFLAGS) $(GTK2_CFLAGS) $(GTK_GLAREA2_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS) $(GTK_GLAREA2_LD_FLAGS)"

prof: 
	@-mkdir -p $(BUILD_PROF_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_PROF_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GTK2_CFLAGS) $(GTK_GLAREA2_CFLAGS) -DNDEBUG -pg" \
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS) $(GTK_GLAREA2_LD_FLAGS) -pg"

################################################################
# GTK+ 1.2

gtk1.2: debug-gtk1.2

debug-gtk1.2:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_DEBUG_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GNOME_CFLAGS) $(GTK_GLAREA_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS) $(GNOME_LDFLAGS) $(GTK_GLAREA_LD_FLAGS)"

release-gtk1.2:
	@-mkdir -p $(BUILD_RELEASE_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_RELEASE_DIR) \
	CFLAGS="$(RELEASE_CFLAGS) $(GNOME_CFLAGS) $(GTK_GLAREA_CFLAGS)" \
	LD_FLAGS="$(LD_FLAGS) $(GNOME_LDFLAGS) $(GTK_GLAREA_LD_FLAGS)"

prof-gtk1.2: 
	@-mkdir -p $(BUILD_PROF_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_PROF_DIR) \
	CFLAGS="$(DEBUG_CFLAGS) $(GNOME_CFLAGS) $(GTK_GLAREA_CFLAGS) -pg" \
	LD_FLAGS="$(LD_FLAGS) $(GNOME_LDFLAGS) $(GTK_GLAREA_LD_FLAGS) -pg"

########################################################################

all: debug release prof

########################################################################

targets: $(TARGETS)

OBJS = \
	$(BUILD_DIR)/gnome_resource.o \
	$(BUILD_DIR)/gnome_callbacks.o \
	$(BUILD_DIR)/gnome_interface.o \
	$(BUILD_DIR)/freyja_events.o \
	$(BUILD_DIR)/utils.o \
	$(BUILD_DIR)/MaterialManager.o \
	$(BUILD_DIR)/Material.o \
	$(BUILD_DIR)/Texture.o \
	$(BUILD_DIR)/Resource.o \
	$(BUILD_DIR)/FreyjaRender.o \
	$(BUILD_DIR)/FreyjaControl.o \
	$(BUILD_DIR)/FreyjaModel.o \
	$(BUILD_DIR)/FreyjaResource.o


$(BUILD_DIR)/$(NAME) : $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LD_FLAGS)

################################################################

-include depend

################################################################

depend:
	@-printf "Making deps..."
	@-printf "# Autogenerated dependency file\n" > depend
	@-find ./src -name "*.cpp" -exec ./deps.sh {} $(BASE_DEFS) $(BASE_INC) \; >> depend
	@-printf "       [done]\n"


################################################################

clean-all: clean clean-conf

clean: clean-builds

clean-builds: 
	@-echo -n "Cleaning emacs files                        "
	@-rm -f `find . -name "*~" -print`
	@-echo [done]

	@-echo -n "Cleaning profiling output                   "
	@-rm -f gmon.out
	@-echo [done]

	@-echo -n "Cleaning .xvpics cache                      "
	@-rm -rf `find -name ".xvpics" -print`
	@-echo [done]

	@-echo -n "Cleaning TEST builds                        "
	@-rm -f $(BUILD_TEST_DIR)/*.test
	@-echo [done]

	@-echo -n "Cleaning Gtk+ 2.0 build                     "
	@-rm -f $(BUILD_GTK2_DIR)*/*.o
	@-rm -f $(BUILD_GTK2_DIR)*/$(NAME)
	@-rm -f $(BUILD_GTK2_DIR)*/*.test
	@-echo [done]

	@-echo -n "Cleaning DEBUG builds                       "
	@-rm -f $(BUILD_DEBUG_DIR)*/*.o
	@-rm -f $(BUILD_DEBUG_DIR)*/$(NAME)
	@-rm -f $(BUILD_DEBUG_DIR)*/*.test
	@-echo [done]

	@-echo -n "Cleaning PROF builds                        "
	@-rm -f $(BUILD_PROF_DIR)*/*.o
	@-rm -f $(BUILD_PROF_DIR)*/$(NAME)
	@-rm -f $(BUILD_PROF_DIR)*/*.test
	@-echo [done]

	@-echo -n "Cleaning RELEASE builds                     "
	@-rm -f $(BUILD_RELEASE_DIR)*/*.o
	@-rm -f $(BUILD_RELEASE_DIR)*/$(NAME)
	@-echo [done]

clean-conf:
	@-echo -n "Cleaning autotools                          "
	@-rm -f mkinstalldirs system.info configure \
		config.status config.log \
		autoscan.log config.cache stamp-* \
		missing install-sh \
		Makefile.in confdefs.h
	@-echo [done]

################################################################

install:
	@-echo -n "Installing                                  "
	@-cp $(BUILD_INSTALL_DIR)/$(NAME) $(INSTALL_DIR)
	@-echo [done]

install-debug:
	@-echo -n "Installing debug                            "
	@-cp $(BUILD_DEBUG_DIR)/$(NAME) $(INSTALL_DIR)
	@-echo [done]


################################################################

uninstall:
	@-echo -n "Uninstalling                                "
	@-rm -f $(INSTALL_DIR)/$(NAME) 
	@-echo [done]

################################################################

package.tgz:
	@-rm -rf $(PACKAGE_DIR)
	@-mkdir -p $(PACKAGE_DIR)/common
	@-mkdir -p $(PACKAGE_DIR)/common/doc
	@-mkdir -p $(PACKAGE_DIR)/common/data
	@-mkdir -p $(PACKAGE_DIR)/common/data/icons
	@-mkdir -p $(PACKAGE_DIR)/common/data/palettes
	@-mkdir -p $(PACKAGE_DIR)/common/plugins
	@-mkdir -p $(PACKAGE_DIR)/common/libs
	@-cp install-package.sh $(PACKAGE_DIR)/
	@-cp doc/FAQ ChangeLog TODO README AUTHORS $(PACKAGE_DIR)/common/doc
	@-cp setup-freyja.sh $(PACKAGE_DIR)/common
	@-cp ~/.freyja/palettes/*.pal $(PACKAGE_DIR)/common/data/palettes
	@-cp data/freyja.menu $(PACKAGE_DIR)/common/data
	@-cp data/icons/*.xpm $(PACKAGE_DIR)/common/data/icons
	@-cp data/icons/*.png $(PACKAGE_DIR)/common/data/icons
	@-cp ~/.freyja/plugins/* $(PACKAGE_DIR)/common/plugins
	@-cp /usr/local/lib/libfreyja_egg.so.0.8.12 $(PACKAGE_DIR)/common/libs
	@-cp /usr/local/lib/libmtk_image.so.0.1.1 $(PACKAGE_DIR)/common/libs
	@-cd $(PACKAGE_DIR) && cd .. && \
		tar zcvf $(NAME)-$(VERSION)-common.tgz $(NAME)/
	@-echo "====================================================="
	@-echo "Common package complete"
	@-echo "====================================================="
	@-rm -rf $(PACKAGE_DIR)
	@-mkdir -p $(PACKAGE_DIR)/bin
	@-cp $(BUILD_DEBUG_DIR)/$(NAME) $(PACKAGE_DIR)/bin/$(NAME).gtk2
	@-cd $(PACKAGE_DIR) && cd .. && \
		tar zcvf $(NAME)-$(VERSION)-gtk2_0.tgz $(NAME)/
	@-echo "====================================================="
	@-echo "Gtk+ 2.0 binary package complete"
	@-echo "====================================================="
	@-rm -rf $(PACKAGE_DIR)
	@-mkdir -p $(PACKAGE_DIR)/bin
	@-cp $(BUILD_DEBUG_DIR)/$(NAME) $(PACKAGE_DIR)/bin/$(NAME).gnome
	@-cd $(PACKAGE_DIR) && cd .. && \
		tar zcvf $(NAME)-$(VERSION)-gnome.tgz $(NAME)/
	@-echo "====================================================="
	@-echo "Gnome binary package complete"
	@-echo "====================================================="
	@-rm -rf $(PACKAGE_DIR)
	@-ls -l $(PACKAGE_DIR)

tarball:
	@-cd .. && tar zcvf $(NAME)-$(VERSION)-$(BUILD_ID).tar.gz \
		$(TREE_DIR) \
		--exclude bin --exclude *~ --exclude cvs  \
		--exclude CVS --exclude *.notar --exclude spec \
		--exclude *.log --exclude *.tga --exclude contrib \
		--exclude $(NAME)/data/models \
		--exclude $(NAME)/data/textures \
		--exclude $(NAME)/data/animations \
		--exclude $(NAME)/data/maps \
		--exclude $(NAME)/data/tags \
		--exclude $(NAME)/data/prefab \
		--exclude $(NAME)/utils \
		--exclude $(NAME)/doc/rtf \
		--exclude $(NAME)/system.info \
		--totals

backup:
	@-$(MAKE) clean
	@-cd .. && tar jcvf $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 \
	$(TREE_DIR) \
	--exclude bin --exclude *~ --totals 
	@-cd .. && cp $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 ~/Backup
	@-cd .. && cp $(NAME)-$(VERSION)-$(BUILD_ID).tar.bz2 /mnt/smb/share/www/files/tmp/



################################################################
# Unit tests
################################################################

Mlisp.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -ggdb -DUNIT_TEST_MLISP -lstdc++ \
	src/common/Mlisp.cpp -o $(BUILD_TEST_DIR)/Mlisp.test

Material.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_MATERIAL -DHAVE_OPENGL \
	src/common/Material.cpp -o $(BUILD_TEST_DIR)/Material.test

Resource.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_RESOURCE -DDEBUG -Isrc/common \
	-lstdc++ \
	src/common/Resource.cpp -o $(BUILD_TEST_DIR)/Resource.test   

