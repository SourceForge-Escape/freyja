
BUILD_TEST_DIR=bin/test
DEBUG_DEFS=-g -Wall

lib3ds:
	@-mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(DEBUG_DEFS) \
	-l3ds -o $(BUILD_TEST_DIR)/3ds.test src/test/3ds.c


zlib:	
	@-mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(DEBUG_DEFS) \
	-lz -o $(BUILD_TEST_DIR)/zlib.test src/test/zlib.cpp


list:	
	@-mkdir -p $(BUILD_TEST_DIR)
	$(CC) -g -Wall $(DEBUG_DEFS) -lstdc++ \
	-DTEST_LIST -o $(BUILD_TEST_DIR)/list.test src/test/list.cpp