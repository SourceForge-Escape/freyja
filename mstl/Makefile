########################################################

NAME=mstl
TREE=mstl
VERSION=0.0.1
BUILD_ID=20040403
TARBALL_NAME=$(NAME)-$(VERSION).$(BUILD_ID)

########################################################
BUILD_RELEASE_DIR=bin/release
BUILD_DEBUG_DIR=bin/debug
BUILD_PROF_DIR=bin/prof
BUILD_TEST_DIR=bin/test
SRC_DIR=mstl

INSTALL_INCLUDE=/usr/local/include

########################################################

all: Vector.test

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
	@-echo "[DONE]"

########################################################

CFLAGS=-Wall -g -I. 
LDFLAGS=-lstdc++

Vector.test:
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-o $(BUILD_TEST_DIR)/Vector.test $(SRC_DIR)/test_vector.cpp
	$(BUILD_TEST_DIR)/Vector.test

list.test:
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-o $(BUILD_TEST_DIR)/list.test $(SRC_DIR)/list.cpp
	$(BUILD_TEST_DIR)/list.test

########################################################

tarball:
	cd .. && tar zcvf $(TARBALL_NAME).tar.gz --exclude .svn $(TREE)

install: install-headers

install-headers:
	@-printf "Installing headers                           "
	@-mkdir -p $(INSTALL_INCLUDE)/$(NAME)
	@-cp -f $(SRC_DIR)/*.h $(INSTALL_INCLUDE)/$(NAME)
	@-printf "[DONE]\n"
