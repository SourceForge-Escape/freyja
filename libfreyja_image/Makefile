###################################################
# Mongoose <stu7440@westga.edu>
###################################################
# + Cleaner clean
# + Add new include tree
# + Add new flags
# + Install/Uninstall
# + Lots of misc new features
###################################################

NAME=freyja_image
TREE_NAME=lib$(NAME)
MAJOR_VERSION=0
MINOR_VERSION=1
MICRO_VERSION=1
BUILD_ID=20040403
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
EXT=.so

TARBALL_NAME=lib$(NAME)-$(VERSION).$(BUILD_ID)

PLUGIN_LIST=tga.so ppm.so pcx.so jpeg.so png.so
MONOLITHIC_PLUGINS=$(BUILDDIR)/mtk_tga.o $(BUILDDIR)/mtk_ppm.o $(BUILDDIR)/mtk_pcx.o $(BUILDDIR)/mtk_jpeg.o $(BUILDDIR)/mtk_png.o
###################################################

BUILD_SELECT=release

# MTKIMAGE_MODULES - Build plugins as modules
#  you also have to disable MONOLITHIC_PLUGINS

BASE_DEFS=

# DEBUG 
# EXPERIMENTAL 
#FIXME
# OBSOLETE
DEBUG_DEFS=-DDEBUG -DEXPERIMENTAL

###################################################

CC=gcc

###################################################

BUILD_DEBUG_DIR=bin/debug
BUILD_RELEASE_DIR=bin/release
BUILD_PROF_DIR=bin/prof
BUILD_TEST_DIR=bin/test
BUILD_INSTALL_DIR=bin/$(BUILD_SELECT)

INSTALL_BIN=/usr/local/bin
INSTALL_LIB=/usr/local/lib
INSTALL_INCLUDE=/usr/local/include

###################################################

CONFIGURE_DEF=`grep -e "-D" ./config.cache`
CONFIGURE_LNK=`grep -e "-l" ./config.cache`

###############################################################

BASE_CFLAGS=-Wall -Isrc -Isrc/plugins \
	$(CONFIGURE_DEF) \
	-DVERSION=\"\\\"$(VERSION)-$(BUILD_ID)\\\"\" \
	$(BASE_DEFS)

LDFLAGS=$(CONFIGURE_LNK) -lm -ldl
RELEASE_CFLAGS=$(BASE_CFLAGS) -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations -O2
DEBUG_CFLAGS=$(BASE_CFLAGS) -g $(DEBUG_DEFS)

SHLIBEXT=$(EXT).$(VERSION)
SHLIBLDFLAGS=$(LDFLAGS) -fPIC -shared

MODULE_CFLAGS=$(BASE_CFLAGS) -g
MODULE_LD_FLAGS=-l$(NAME) -shared -fPIC -ldl -rdynamic

################################################################

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<
DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

TARGETS=$(BUILDDIR)/lib$(NAME)$(SHLIBEXT)
PLUGINS=$(BUILDDIR)/lib$(NAME)-$(MOD_NAME).so
TESTS=$(BUILDDIR)/$(NAME).test

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
	@-echo
	@-echo
	@-echo Now do a 'make install' to install
	@-echo

map:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(CC) -o $(BUILD_TEST_DIR)/map.test src/ds/Map.cpp 

################################################################

plug-ins: $(PLUGINS)

tga.so:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) plug-ins BUILDDIR=$(BUILD_DEBUG_DIR) \
	MOD_NAME=tga \
	MOD_OBJS="$(BUILD_DEBUG_DIR)/mtk_tga.o" \
	CFLAGS="$(MODULE_CFLAGS)" \
	LD_FLAGS="$(MODULE_LD_FLAGS)"

ppm.so:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) plug-ins BUILDDIR=$(BUILD_DEBUG_DIR) \
	MOD_NAME=ppm \
	MOD_OBJS="$(BUILD_DEBUG_DIR)/mtk_ppm.o" \
	CFLAGS="$(MODULE_CFLAGS)" \
	LD_FLAGS="$(MODULE_LD_FLAGS)"

png.so:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) plug-ins BUILDDIR=$(BUILD_DEBUG_DIR) \
	MOD_NAME=png \
	MOD_OBJS="$(BUILD_DEBUG_DIR)/mtk_png.o" \
	CFLAGS="$(MODULE_CFLAGS) -DLIB_PNG" \
	LD_FLAGS="$(MODULE_LD_FLAGS) -lpng -lz"

pcx.so:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) plug-ins BUILDDIR=$(BUILD_DEBUG_DIR) \
	MOD_NAME=pcx \
	MOD_OBJS="$(BUILD_DEBUG_DIR)/mtk_pcx.o" \
	CFLAGS="$(MODULE_CFLAGS)" \
	LD_FLAGS="$(MODULE_LD_FLAGS)"

jpeg.so:
	@-mkdir -p $(BUILD_DEBUG_DIR)
	$(MAKE) plug-ins BUILDDIR=$(BUILD_DEBUG_DIR) \
	MOD_NAME=jpeg \
	MOD_OBJS="$(BUILD_DEBUG_DIR)/mtk_jpeg.o" \
	CFLAGS="$(MODULE_CFLAGS) -DLIB_JPEG" \
	LD_FLAGS="$(MODULE_LD_FLAGS) -ljpeg"

################################################################

plugins: $(PLUGIN_LIST)

$(BUILDDIR)/lib$(NAME)-$(MOD_NAME).so : $(MOD_OBJS)
	$(CC) $(CFLAGS) $(LD_FLAGS) -o $@ $(MOD_OBJS)

$(BUILDDIR)/mtk_tga.o :        src/plugins/mtk_tga.cpp
	$(DO_CC)

$(BUILDDIR)/mtk_ppm.o :        src/plugins/mtk_ppm.cpp
	$(DO_CC)

$(BUILDDIR)/mtk_png.o :        src/plugins/mtk_png.cpp
	$(DO_CC)

$(BUILDDIR)/mtk_pcx.o :        src/plugins/mtk_pcx.cpp
	$(DO_CC)

$(BUILDDIR)/mtk_jpeg.o :        src/plugins/mtk_jpeg.cpp
	$(DO_CC)

################################################################

targets: $(TARGETS)


PROG_OBJS = \
	$(BUILDDIR)/MtkImage.o \
	$(MONOLITHIC_PLUGINS)


$(BUILDDIR)/lib$(NAME)$(SHLIBEXT) : $(PROG_OBJS)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) -o $@ $(PROG_OBJS)

$(BUILDDIR)/MtkImage.o :         src/MtkImage.cpp
	$(DO_CC)

#################################################################

clean: clean-emacs clean-build clean-test clean-obj

clean-test:
	@-echo -n "Cleaning test builds                         "
	@-rm -f $(BUILD_TEST_DIR)/*.test
	@-echo "[DONE]"

clean-obj:
	@-echo -n "Cleaning objects                             "
	@-rm -f $(BUILD_PROF_DIR)/*.o
	@-rm -f $(BUILD_DEBUG_DIR)/*.o
	@-rm -f $(BUILD_RELEASE_DIR)/*.o
	@-echo "[DONE]"

clean-emacs:
	@-echo -n "Cleaning emacs files                         "
	@-rm -f `find . -name "*~" -print`
	@-echo "[DONE]"

clean-build:
	@-echo -n "Cleaning builds                              "
	@-rm -f $(BUILD_PROF_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_DEBUG_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_RELEASE_DIR)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(BUILD_PROF_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.so
	@-rm -f $(BUILD_RELEASE_DIR)/*.so
	@-rm -f $(BUILD_DEBUG_DIR)/*.test
	@-rm -f config.cache
	@-rm -f system.info
	@-echo "[DONE]"

######################################

install-plugins:
	@-mkdir -p $(HOME)/.freyja/plugins/
	@cp -f $(BUILD_INSTALL_DIR)/libfreyja_loader* ~/.freyja/plugins/

install:
	@-echo -n "Installing selected build                     "
	@-cp $(BUILD_INSTALL_DIR)/lib$(NAME)$(SHLIBEXT) $(INSTALL_LIB)
	@-mkdir -p $(INSTALL_LIB)/$(NAME)
	@-cp $(BUILD_INSTALL_DIR)/lib$(NAME)$(SHLIBEXT) $(INSTALL_LIB)/$(NAME)
	@-echo "[DONE]"
	@-$(MAKE) install-link
	@-$(MAKE) install-headers
	@-./libsetup.sh $(INSTALL_LIB)

install-link:
	@-echo -n "New soft links                               "
	@-rm -f $(INSTALL_LIB)/lib$(NAME)$(EXT)
	@-ln -s $(INSTALL_LIB)/lib$(NAME)$(SHLIBEXT) \
	$(INSTALL_LIB)/lib$(NAME)$(EXT)
	@-echo "[DONE]"

install-headers:
	@-echo -n "Installing headers                           "
	@-mkdir -p $(INSTALL_INCLUDE)/$(NAME)
	@-cp -f src/*.h $(INSTALL_INCLUDE)/$(NAME)
	@-echo "[DONE]"

######################################

uninstall:
	@-echo -n "Uninstalling                                 " 
	@-rm -f $(INSTALL_LIB)/lib$(NAME)$(EXT)
	@-rm -f $(INSTALL_LIB)/lib$(NAME)$(SHLIBEXT)
	@-rm -f $(INSTALL_BIN)/$(NAME)
	@-rm -rf $(INSTALL_LIB)/$(NAME)
	@-rm -rf $(INSTALL_INCLUDE)/$(NAME)
	@-echo "[DONE]"

######################################

tarball:
	@-echo "Making tarball                                  " 
	@-cd .. && tar zcvf $(TARBALL_NAME).tar.gz $(TREE_NAME) \
		--exclude bin --exclude CVS --exclude *~ --exclude contrib
	@-echo "[DONE]"

backup:
	@-echo "Making backup tarball                           " 
	@-cd .. && tar zcvf $(TARBALL_NAME).tar.gz $(TREE_NAME) \
		--exclude bin
	@-echo "[DONE]"


########################################################################
# Test module builds
#
# These test plugins and componets as console programs
########################################################################

