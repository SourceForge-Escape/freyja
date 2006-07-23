################################################################
# Mongoose <mongoose@icculus.org>
################################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
################################################################

NAME=freyja
BASE_NAME=freyja
MAJOR_VERSION=0
MINOR_VERSION=3
MICRO_VERSION=5
BRANCH=
BUILD_ID=20050822
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)$(BRANCH)
BUILD_HOST=`uname -n -m -s`
WEBSITE="http://icculus.org/freyja"
EMAIL="mongoose@icculus.org"
EXT=
TREE_DIR=freyja
TARBALL_NAME=$(NAME)-$(VERSION).$(BUILD_ID)$(BRANCH)
LICENSE=

################################################################

BUILD_SELECT=debug

# USE_ARB_MULTITEXTURE 
# DISABLE_MULTITEXTURE
# DISABLE_GTK_COLORBUTTON
CONFIGURE_CFLAGS=-DPROTOTYPE_02 -DPLANE_NOTIFY_WITH_AXIS \
		-DHAVE_FREYJA_IMAGE -DHAVE_OPENGL -DHAVE_GTKGL -DHAVE_HEL \
		-DUSE_ARB_MULTITEXTURE
CONFIGURE_LDFLAGS=

# Uncomment PKG_LIB_ADD and comment out LIB_ADD to use gtkglext
PKG_LIB_ADD=gtkglext-1.0
#LIB_ADD=-lgtkgl-2.0


################################################################
# Compiler and Cross Enviroments
################################################################

CC=gcc
DEPS=./deps.sh
-include ../win32.env


################################################################

BUILD_ROOT_DIR=../bin/freyja
BUILD_DEBUG_DIR=$(BUILD_ROOT_DIR)/debug
BUILD_RELEASE_DIR=$(BUILD_ROOT_DIR)/release
BUILD_PROF_DIR=$(BUILD_ROOT_DIR)/prof
BUILD_WIN32_DIR=$(BUILD_ROOT_DIR)/win32
BUILD_OSX_DIR=$(BUILD_ROOT_DIR)/osx
BUILD_TEST_DIR=$(BUILD_ROOT_DIR)/test
BUILD_INSTALL_DIR=$(BUILD_ROOT_DIR)/$(BUILD_SELECT)
SRC_DIR=src
LOCAL_INCLUDES=-I$(SRC_DIR) -I../mstl/ -I../libhel/ -I../libfreyja/  -I../libmgtk/
LOCAL_LIBS=-L../bin/libfreyja/$(BUILD_SELECT)/ -L../bin/libhel/$(BUILD_SELECT)/ -L../bin/libmgtk/$(BUILD_SELECT)/

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

################################################################

DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL


BASE_DEFS=-Isrc $(CONFIGURE_CFLAGS) -Wno-deprecated -Weffc++ -Wall -pedantic
#BASE_DEFS += -Weffc++ -Wall -pedantic -std=c99

BASE_CFLAGS=$(CONFIGURE_CFLAGS) \
		-fPIC -Wall $(LOCAL_INCLUDES) \
		-DPROJECT_URL=\"\\\"$(WEBSITE)\\\"\" \
		-DEMAIL_ADDRESS=\"\\\"$(EMAIL)\\\"\" \
		-DVERSION=\"\\\"$(VERSION)\\\"\" \
		-DBUILD_ID=\"\\\"$(BUILD_ID)\\\"\" \
		-DPROGRAM_NAME=\"\\\"$(NAME)\\\"\" \
		-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\" \
		-DBUILD_HOST=\"\\\"$(BUILD_HOST)\\\"\"

LD_FLAGS=-L/usr/X11R6/lib $(CONFIGURE_LDFLAGS) $(LOCAL_LIBS) \
		-lstdc++ -lGL -lGLU -lm -lz \
		-lhel0 -lfreyja -lmgtk \
		$(shell pkg-config --libs gtk+-2.0 $(PKG_LIB_ADD)) $(LIB_ADD)
#LD_FLAGS += -Wl,-z,defs
#LD_FLAGS += -Wl,-O1

CG_LD_FLAGS=-lCgGL -lCg


CROSSDUMP_INC=-I/home/mongoose/Projects/win32/include/gtk-2.0 -I/home/mongoose/Projects/win32/lib/gtk-2.0/include -I/home/mongoose/Projects/win32/include/atk-1.0 -I/home/mongoose/Projects/win32/include/pango-1.0 -I/home/mongoose/Projects/win32/include/glib-2.0 -I/home/mongoose/Projects/win32/lib/glib-2.0/include -I/home/mongoose/Projects/win32/include/gtkglext-1.0 -I/home/mongoose/Projects/win32/lib/gtkglext-1.0/include

CROSSDUMP_LIBS=-lgtkglext-win32-1.0 -lgdkglext-win32-1.0 -lglu32 -lopengl32 -luser32 -lkernel32 -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lgdi32 -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -liconv

WIN32_CROSS_DIR=/home/mongoose/Projects/win32
CROSS_FLAGS=-I$(WIN32_CROSS_DIR)/include $(CROSSDUMP_INC)
CROSS_LIBS=-L$(WIN32_CROSS_DIR)/lib/ $(CROSSDUMP_LIBS)

LD_FLAGS_WIN32=$(CONFIGURE_LDFLAGS) -lstdc++ -lm \
		$(CROSS_LIBS) \
		-L/usr/local/lib -lhel0 -lfreyja9 -lmgtk1 

LD_FLAGS_OSX=-framework OpenGL

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
	LD_FLAGS="$(LD_FLAGS) $(GTK2_LDFLAGS) $(GTK_GLAREA2_LD_FLAGS) -Wl,-z,defs"

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

win32:
	@-mkdir -p $(BUILD_WIN32_DIR)
	$(MAKE) targets BUILD_DIR=$(BUILD_WIN32_DIR) \
	NAME=$(NAME).exe \
	CC="i586-mingw32msvc-g++ " \
	CPP="i586-mingw32msvc-cpp " \
	LD="i586-mingw32msvc-ld " \
	CFLAGS="-mwindows $(DEBUG_CFLAGS) $(GTK2_CFLAGS) $(GTK_GLAREA2_CFLAGS) -I/usr/local/include/ $(CROSS_FLAGS)" \
	LD_FLAGS="$(LD_FLAGS_WIN32) $(GTK2_LDFLAGS) $(GTK_GLAREA2_LD_FLAGS)"


########################################################################

all: debug release prof

########################################################################

targets: $(TARGETS)

OBJS = \
	$(BUILD_DIR)/freyja_events.o \
	$(BUILD_DIR)/Texture.o \
	$(BUILD_DIR)/FreyjaRender.o \
	$(BUILD_DIR)/FreyjaControl.o \
	$(BUILD_DIR)/FreyjaModel.o \
	$(BUILD_DIR)/FreyjaState.o \
	$(BUILD_DIR)/FreyjaOpenGL.o


$(BUILD_DIR)/$(NAME) : $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LD_FLAGS)

################################################################

-include depend

################################################################

depend-plugins:
	@-find ./plugins -name "*.cpp" -exec ./deps.sh $(BASE_DEFS) {} \; >> depend

depend:
	@-printf "Making deps..."
	@-printf "# Autogenerated dependency file\n" > depend
	@-find ./src -name "*.cpp" -exec ./deps.sh {} $(BASE_DEFS) $(BASE_INC) \; >> depend
	$(MAKE) depend-plugins
	@-printf "       [done]\n"


################################################################

clean-all: clean clean-conf clean-plugins

clean: clean-builds	clean-plugins

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

	@-echo -n "Cleaning Win32 build                        "
	@-rm -f $(BUILD_WIN32_DIR)*/*.o
	@-rm -f $(BUILD_WIN32_DIR)*/$(NAME).exe
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

#################################################################

clean-deb:
	@-echo -n "Cleaning DEBs                                "
	@-rm -rf $(DEB_DIR)
	@-echo "[DONE]"

debian-package:
	$(MAKE) clean-deb
	$(MAKE) tarball
	mkdir -p $(DEB_DIR)
	cp ../$(NAME_DEB)-$(VERSION_DEB).tar.gz $(DEB_DIR)
	cd $(DEB_DIR) && tar zxvf $(NAME_DEB)-$(VERSION_DEB).tar.gz && mv $(TREE_DIR) $(NAME_DEB)-$(VERSION_DEB) && cd $(NAME_DEB)-$(VERSION_DEB) && make deb-init && make deb

deb-init:
	yes | dh_make -c lgpl -s -e mongoose@icculus.org -f ../$(NAME_DEB)-$(VERSION_DEB).tar.gz
	@-cp debian/control debian/control.old
	@-sed -e "s/<insert up to 60 chars description>/Freyja 3d modeler/g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/control debian/control.old
	@-sed -e "s/<insert long description, indented with spaces>/3d Modeler./g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/control debian/control.old
	@-sed -e "s/[$][{]misc:Depends[}]/libmgtk, libhel, libfreyja/g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/control debian/control.old
	@-sed -e "s/[$][{]shlibs:Depends[}]//g" debian/control.old > debian/control
	@-rm -f debian/control.old
	@-cp debian/rules debian/rules.old
	@-sed -e "s/MAKE) install/MAKE) plugins install/g" debian/rules.old > debian/rules
	@-rm -f debian/rules.old

deb:
	dpkg-buildpackage -rfakeroot

################################################################

install:
	@-echo -n "Installing                                  "
	@-mkdir -p  $(INSTALL_BIN)
	@-cp bin/debug/freyja $(INSTALL_BIN)
	@-mkdir -p $(DESTDIR)/usr/share/freyja/
	@-cp data/*.mlisp $(DESTDIR)/usr/share/freyja/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/materials/
	@-cp data/materials/*.mat $(DESTDIR)/usr/share/freyja/materials/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/icons/
	@-cp data/icons/*.png $(DESTDIR)/usr/share/freyja/icons/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/palettes/
	@-cp data/palettes/*.pal $(DESTDIR)/usr/share/freyja/palettes/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/
	@-cp plugins/*.mlisp $(DESTDIR)/usr/share/freyja/plugins/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/
	@-cp bin/plugins/*.so $(DESTDIR)/usr/share/freyja/plugins/
	@-echo "[done]"

install-win32:
	@-echo -n "Installing                                  "
	@-mkdir -p  $(INSTALL_BIN)
	@-cp $(BUILD_WIN32_DIR)/freyja*.exe $(INSTALL_BIN)/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/
	@-cp data/*.mlisp $(DESTDIR)/usr/share/freyja/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/materials/
	@-cp data/materials/*.mat $(DESTDIR)/usr/share/freyja/materials/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/icons/
	@-cp data/icons/*.png $(DESTDIR)/usr/share/freyja/icons/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/palettes/
	@-cp data/palettes/*.pal $(DESTDIR)/usr/share/freyja/palettes/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/
	@-cp plugins/*.mlisp $(DESTDIR)/usr/share/freyja/plugins/
	@-mkdir -p $(DESTDIR)/usr/share/freyja/plugins/
	@-cp bin/plugins/*.dll $(DESTDIR)/usr/share/freyja/plugins/
	@-echo "[done]"

install-debug:
	@-echo -n "Installing debug                            "
	@-cp $(BUILD_DEBUG_DIR)/$(NAME) $(INSTALL_BIN)
	@-echo "[done]"


################################################################

uninstall:
	@-echo -n "Uninstalling                                "
	@-rm -f $(INSTALL_DIR)/$(NAME) 
	@-echo [done]

################################################################

tarball:
	@-cd .. && tar zcvf $(NAME)-$(VERSION).$(BUILD_ID).tar.gz \
		--exclude=bin --exclude=*~ --exclude=.svn  \
		--exclude=*.log --exclude=*.tga --exclude=contrib \
		--totals \
		$(TREE_DIR) 



################################################################
# Unit tests
################################################################

Material.test:
	@-mkdir -p $(BUILD_TEST_DIR)
	gcc -Wall -g -DUNIT_TEST_MATERIAL -DHAVE_OPENGL \
	src/common/Material.cpp -o $(BUILD_TEST_DIR)/Material.test


################################################################
# Plugins
################################################################
PLUGIN_CFLAGS=-fPIC -Wall -g -Isrc -Iplugins
PLUGIN_LD_FLAGS=-lm -ldl -lfreyja -lhel0 -lmgtk -lstdc++ -shared -fPIC -ldl -rdynamic 
PLUGIN_LD_FLAGS += --warn-unresolved-symbols -Wl
#PLUGIN_LD_FLAGS += -z defs
PLUGIN_BUILD_DIR=bin/plugins

PLUGINS=$(BUILD_DIR)/$(MOD_NAME).so
PLUGINSWIN32=$(BUILD_DIR)/$(MOD_NAME).dll

plug-ins: $(PLUGINS)

plug-ins-win32: $(PLUGINSWIN32)

$(BUILD_DIR)/$(MOD_NAME).so : $(MOD_OBJS)
	$(CC) $(CFLAGS) $(LD_FLAGS) -o $@ $(MOD_OBJS)

$(BUILD_DIR)/$(MOD_NAME).dll : $(MOD_OBJS)
	$(CC) -shared -o $(BUILD_DIR)/$(MOD_NAME).dll \
    -Wl,--out-implib=$(BUILD_DIR)/lib$(MOD_NAME).a \
    -Wl,--export-all-symbols \
    -Wl,--enable-auto-import \
    -Wl,--whole-archive $(MOD_OBJS) \
    -Wl,--no-whole-archive $(LIBS)

clean-plugins:
	@-echo -n "Cleaning plugins                            "
	@-rm -f $(PLUGIN_BUILD_DIR)*/*.o
	@-rm -f $(PLUGIN_BUILD_DIR)*/*.so
	@-rm -f $(PLUGIN_BUILD_DIR)*/*.a
	@-rm -f $(PLUGIN_BUILD_DIR)*/*.dll
	@-echo [done]

plugins: pakreader.so perlinnoise.so reverseengineer.so lineageIIextract.so

plugins-win32: pakreader.dll perlinnoise.dll reverseengineer.dll lineageIIextract.dll

pakreader.so:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=pakreader \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/pakreader.o" \
		CFLAGS="$(PLUGIN_CFLAGS)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)"

pakreader.dll:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins-win32 BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=pakreader \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/pakreader.o" \
		CC="i586-mingw32msvc-g++ -mms-bitfields" \
		CPP="i586-mingw32msvc-cpp " \
		LD="i586-mingw32msvc-ld " \
		CFLAGS="$(PLUGIN_CFLAGS) -I/usr/local/include/" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9 -lmgtk1"

perlinnoise.so:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=perlinnoise \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/perlinnoise.o" \
		CFLAGS="$(PLUGIN_CFLAGS)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)"

perlinnoise.dll:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins-win32 BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=perlinnoise \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/perlinnoise.o" \
		CC="i586-mingw32msvc-g++ -mms-bitfields" \
		CPP="i586-mingw32msvc-cpp " \
		LD="i586-mingw32msvc-ld " \
		CFLAGS="$(PLUGIN_CFLAGS) -I/usr/local/include/" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9 -lmgtk1"

reverseengineer.so:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=reverseengineer \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/reverseengineer.o" \
		CFLAGS="$(PLUGIN_CFLAGS)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)"

reverseengineer.dll:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins-win32 BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=reverseengineer \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/reverseengineer.o" \
		CC="i586-mingw32msvc-g++ -mms-bitfields" \
		CPP="i586-mingw32msvc-cpp " \
		LD="i586-mingw32msvc-ld " \
		CFLAGS="$(PLUGIN_CFLAGS) -I/usr/local/include/" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9 -lmgtk1"

lineageIIextract.so:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=lineageIIextract \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/lineageIIextract.o" \
		CFLAGS="$(PLUGIN_CFLAGS)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)"

lineageIIextract.dll:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins-win32 BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=lineageIIextract \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/lineageIIextract.o" \
		CC="i586-mingw32msvc-g++ -mms-bitfields" \
		CPP="i586-mingw32msvc-cpp " \
		LD="i586-mingw32msvc-ld " \
		CFLAGS="$(PLUGIN_CFLAGS) -I/usr/local/include/" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ -L/usr/local/lib -lhel0 -lfreyja9 -lmgtk1"
