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

jpeg:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <stdio.h>" > $(BUILD_DIR)/jpeg.cpp
	@-echo "#include <ctype.h>" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "#include <string.h>" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "#include <unistd.h>" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "#include <jpeglib.h>" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "{" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "   printf(\"jpeg %i\n\",JPEG_LIB_VERSION);" >> $(BUILD_DIR)/jpeg.cpp
	@-echo "}" >> $(BUILD_DIR)/jpeg.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) -lz \
	-o $(BUILD_DIR)/jpeg.test $(BUILD_DIR)/jpeg.cpp

	@-echo -n "<<< jpeg test reports >>>   "
	@-$(BUILD_DIR)/jpeg.test

########################################################################

png:	
	@-mkdir -p $(BUILD_DIR)
	@-echo "#include <stdio.h>" > $(BUILD_DIR)/png.cpp
	@-echo "#include <png.h>" >> $(BUILD_DIR)/png.cpp
	@-echo "" >> $(BUILD_DIR)/png.cpp
	@-echo "int main(int argc, char *argv[])" >> $(BUILD_DIR)/png.cpp
	@-echo "{" >> $(BUILD_DIR)/png.cpp
	@-echo "   printf(\"png %s\n\", PNG_LIBPNG_VER_STRING);" >> $(BUILD_DIR)/png.cpp
	@-echo "}" >> $(BUILD_DIR)/png.cpp

	@-$(CC) -g -Wall $(DEBUG_DEFS) -lz \
	-o $(BUILD_DIR)/png.test $(BUILD_DIR)/png.cpp

	@-echo -n "<<< png test reports >>>   "
	@-$(BUILD_DIR)/png.test

########################################################################