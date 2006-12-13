
BUILD_DIR=../bin/libfreyja/conf
DEBUG_DEFS=-g -Wall -DUSING_PYTHON
DEBUG_LIBS=-lpython2.4

python:	
	@-mkdir -p $(BUILD_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) $(DEBUG_LIBS) -lstdc++ \
	-o $(BUILD_DIR)/python python.cpp
