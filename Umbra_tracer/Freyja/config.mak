# Author: Mongoose
CC=gcc
BUILD_DIR=bin/test
CFLAGS=-g -Wall
DEBUG_DEFS=-DDEBUG

## Test builds ##########################################################

cpp:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <iostream.h>" > $(BUILD_DIR)/cpp.cpp
	@-echo "" >> $(BUILD_DIR)/cpp.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/cpp.cpp
	@-echo "{" >> $(BUILD_DIR)/cpp.cpp
	@-echo "   cout << \"success\" << endl;" >> $(BUILD_DIR)/cpp.cpp
	@-echo "}" >> $(BUILD_DIR)/cpp.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) -lstdc++ \
	-o $(BUILD_DIR)/cpp.test $(BUILD_DIR)/cpp.cpp

	@-echo -n "<<< C++ test reports >>>   "
	@-$(BUILD_DIR)/cpp.test

########################################################################

zlib:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <stdio.h>" > $(BUILD_DIR)/zlib.cpp
	@-echo "#include <zlib.h>" >> $(BUILD_DIR)/zlib.cpp
	@-echo "" >> $(BUILD_DIR)/zlib.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/zlib.cpp
	@-echo "{" >> $(BUILD_DIR)/zlib.cpp
	@-echo "   printf(\"zlib %s\n\", ZLIB_VERSION);" >> $(BUILD_DIR)/zlib.cpp
	@-echo "}" >> $(BUILD_DIR)/zlib.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) -lz \
	-o $(BUILD_DIR)/zlib.test $(BUILD_DIR)/zlib.cpp

	@-echo -n "<<< zlib test reports >>>   "
	@-$(BUILD_DIR)/zlib.test

########################################################################

gnome:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <gnome.h>" > $(BUILD_DIR)/gnome.cpp
	@-echo "" >> $(BUILD_DIR)/gnome.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/gnome.cpp
	@-echo "{" >> $(BUILD_DIR)/gnome.cpp
	@-echo "   gnome_init(0, 0, 0, 0);" >> $(BUILD_DIR)/gnome.cpp
	@-echo "   printf(\"gnome found\n\");" >> $(BUILD_DIR)/gnome.cpp
	@-echo "}" >> $(BUILD_DIR)/gnome.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) $(shell "gnome-config --cflags") \
	$(shell "gnome-config --libs") \
	-o $(BUILD_DIR)/gnome.test $(BUILD_DIR)/gnome.cpp

	@-echo -n "<<< gnome test reports >>>   "
	@-$(BUILD_DIR)/gnome.test

########################################################################

#include <gtkgl/gtkglarea.h>
gtk:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <gtk/gtk.h>" > $(BUILD_DIR)/gtk.cpp
	@-echo "#include <gdk/gdkkeysyms.h>" > $(BUILD_DIR)/gtk.cpp
	@-echo "" >> $(BUILD_DIR)/gtk.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/gtk.cpp
	@-echo "{" >> $(BUILD_DIR)/gtk.cpp
	@-echo "   gnome_init(\"0, 0, 0, 0);" >> $(BUILD_DIR)/gtk.cpp
	@-echo "   printf(\"gnome found\n\");" >> $(BUILD_DIR)/gtk.cpp
	@-echo "}" >> $(BUILD_DIR)/gtk.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) $(shell ) \
	-o $(BUILD_DIR)/gtk.test $(BUILD_DIR)/gtk.cpp

	@-echo -n "<<< gtk test reports >>>   "
	@-$(BUILD_DIR)/gtk.test

########################################################################

glut:
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) \
	-L/usr/X11R6/lib \
	-lGL -lGLU -lm -lglut -lXmu -lXt -lSM -lICE -lXext -lX11 -lXi \
	-DTEST_GLUT -o $(BUILD_DIR)/glut.test src/test/glut.cpp

lib3ds:
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) \
	-l3ds -o $(BUILD_DIR)/3ds.test src/test/3ds.c

png:
	@-mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEBUG_DEFS) \
	-lpng -DLIB_PNG \
	-DTEST_PNG -o $(BUILD_DIR)/png.test src/test/png.cpp

jpeg:
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) \
	-ljpeg -DLIB_JPEG \
	-DTEST_JPEG -o $(BUILD_DIR)/jpeg.test src/test/jpeg.cpp

python:
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) -I/usr/include/python1.5 \
	-DTEST_PYTHON -lpython1.5 \
	-o $(BUILD_DIR)/python.test src/test/python.cpp

dl:
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) \
	-ldl -rdynamic -DIMAGE_MODULES \
	-o $(BUILD_TEST_DIR)/dl.test src/test/dl.cpp
