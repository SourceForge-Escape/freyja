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
MINOR_VERSION=9
MICRO_VERSION=5
BRANCH=
BUILD_ID=20070407
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

CONFIGURE_CFLAGS=-DHAVE_FREYJA_IMAGE -DHAVE_OPENGL -DHAVE_GTKGL -DHAVE_HEL \
		-DPLANE_NOTIFY_WITH_AXIS -DUSING_OPENGL_EXT 
CONFIGURE_LDFLAGS=

PKG_LIB_ADD=gtkglext-1.0


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
PLUGIN_BUILD_DIR=$(BUILD_INSTALL_DIR)/plugins
SRC_DIR=src
LOCAL_INCLUDES=-I$(SRC_DIR) -I../mstl/ -I../libhel/ -I../libfreyja/  -I../libmgtk/
LOCAL_LIBS=-L../bin/libfreyja/$(BUILD_SELECT)/ -L../bin/libhel/$(BUILD_SELECT)/ -L../bin/libmgtk/$(BUILD_SELECT)/
LOCAL_WIN32_LIBS=-L../bin/libfreyja/win32/ -L../bin/libhel/win32/ -L../bin/libmgtk/win32/

DESTDIR =
NO_DEB=
NO_DEB=
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
		$(shell freetype-config --cflags) \
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
		$(shell freetype-config --libs) \
		$(shell pkg-config --libs gtk+-2.0 $(PKG_LIB_ADD)) $(LIB_ADD)
#LD_FLAGS += -Wl,-z,defs
#LD_FLAGS += -Wl,-O1

CG_LD_FLAGS=-lCgGL -lCg

ifdef __darwin__
LD_FLAGS=-L/usr/X11R6/lib $(CONFIGURE_LDFLAGS) $(LOCAL_LIBS) \
		-lstdc++ -lm -lz \
		-lhel0 -lfreyja -lmgtk \
		$(shell pkg-config --libs gtk+-2.0 $(PKG_LIB_ADD)) $(LIB_ADD)
LD_FLAGS += -framework OpenGL
endif

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
	CC=$(WIN32_CC) \
	CPP=$(WIN32_CPP) \
	LD=$(WIN32_LD) \
	CFLAGS="$(LOCAL_INCLUDES) -mwindows $(DEBUG_CFLAGS) $(WIN32_CROSS_CFLAGS)" \
	LD_FLAGS="$(LOCAL_WIN32_LIBS) $(WIN32_CROSS_LIBS) $(WIN32_LIBFREYJA) $(WIN32_LIBHEL) $(WIN32_LIBMGTK)"


########################################################################

all: debug release prof

########################################################################

targets: $(TARGETS)

OBJS = \
	$(BUILD_DIR)/freyja_events.o \
	$(BUILD_DIR)/Control.o \
	$(BUILD_DIR)/ControlPrinter.o \
	$(BUILD_DIR)/MaterialEv.o \
	$(BUILD_DIR)/Texture.o \
	$(BUILD_DIR)/FreyjaRender.o \
	$(BUILD_DIR)/FreyjaControl.o \
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

deb:
	dpkg-buildpackage -rfakeroot
	$(MAKE) deb-info

deb-info:
	@-printf "================================================================================\n"
	dpkg --info ../freyja_*.deb
	@-printf " -------------------------------------------------------------------------------\n"
	dpkg --contents ../freyja_*.deb
	@-printf "================================================================================\n"

################################################################

install:
	@-echo -n "Installing                                  "
	@-mkdir -p  $(INSTALL_BIN)
	@-cp $(BUILD_INSTALL_DIR)/freyja $(INSTALL_BIN)
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
	@-cp $(PLUGIN_BUILD_DIR)/*.so $(DESTDIR)/usr/share/freyja/plugins/
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
	@-cp $(PLUGIN_BUILD_DIR)/*.dll $(DESTDIR)/usr/share/freyja/plugins/
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
PLUGIN_CFLAGS=-fPIC -Wall -g -Isrc -Iplugins $(BASE_CFLAGS) $(LOCAL_INCLUDES)
PLUGIN_LD_FLAGS=-lm -ldl $(LOCAL_LIBS) -lfreyja -lhel0 -lmgtk -lstdc++ -shared -fPIC -ldl -rdynamic 
PLUGIN_LD_FLAGS += --warn-unresolved-symbols -Wl
#PLUGIN_LD_FLAGS += -z defs

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

plugins: pakreader.so perlinnoise.so reverseengineer.so lineageIIextract.so pythonplug.so

plugins-win32: pakreader.dll perlinnoise.dll reverseengineer.dll lineageIIextract.dll pythonplug.dll

plugins-osx:
	@-printf "Not implemented\n"

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
		CC=$(WIN32_CC) \
		CPP=$(WIN32_CPP) \
		LD=$(WIN32_LD) \
		CFLAGS="$(PLUGIN_CFLAGS) $(LOCAL_INCLUDES)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ $(LOCAL_WIN32_LIBS) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA) $(WIN32_LIBMGTK)"

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
		CC=$(WIN32_CC) \
		CPP=$(WIN32_CPP) \
		LD=$(WIN32_LD) \
		CFLAGS="$(PLUGIN_CFLAGS) $(LOCAL_INCLUDES)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ $(LOCAL_WIN32_LIBS) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA) $(WIN32_LIBMGTK)"

pythonplug.so:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=pythonplug \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/pythonplug.o" \
		CFLAGS="$(PLUGIN_CFLAGS)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)"

pythonplug.dll:
	@-mkdir -p $(PLUGIN_BUILD_DIR)
	$(MAKE) plug-ins-win32 BUILD_DIR=$(PLUGIN_BUILD_DIR) \
		MOD_NAME=pythonplug \
		MOD_OBJS="$(PLUGIN_BUILD_DIR)/pythonplug.o" \
		CC=$(WIN32_CC) \
		CPP=$(WIN32_CPP) \
		LD=$(WIN32_LD) \
		CFLAGS="$(PLUGIN_CFLAGS) $(LOCAL_INCLUDES)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS) $(WIN32_LIB_DIR)" \
		LIBS="-lm -lstdc++ $(LOCAL_WIN32_LIBS) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA) $(WIN32_LIBMGTK)  $(WIN32_LIB_DIR) -lpython24"

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
		CC=$(WIN32_CC) \
		CPP=$(WIN32_CPP) \
		LD=$(WIN32_LD) \
		CFLAGS="$(PLUGIN_CFLAGS) $(LOCAL_INCLUDES)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ $(LOCAL_WIN32_LIBS) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA) $(WIN32_LIBMGTK)"

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
		CC=$(WIN32_CC) \
		CPP=$(WIN32_CPP) \
		LD=$(WIN32_LD) \
		CFLAGS="$(PLUGIN_CFLAGS) $(LOCAL_INCLUDES)" \
		LD_FLAGS="$(PLUGIN_LD_FLAGS)" \
		LIBS="-lm -lstdc++ $(LOCAL_WIN32_LIBS) $(WIN32_LIBHEL) $(WIN32_LIBFREYJA) $(WIN32_LIBMGTK)"
