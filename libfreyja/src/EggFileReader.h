/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : EggFileReader
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Reads file data from bin/text files for model parsers
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_EGGFILEREADER - Builds EggFileReader class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.16:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_EGGFILEREADER_H_
#define GUARD__FREYJA_MONGOOSE_EGGFILEREADER_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


class EggFileReader
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	EggFileReader();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of EggFileReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~EggFileReader();
	/*------------------------------------------------------
	 * Pre  : EggFileReader object is allocated
	 * Post : Deconstructs an object of EggFileReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static int compareFilenameExtention(const char *filename, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Compares filename and extention, 
	 *        Returns 0 if the filename has the extention
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.21: 
	 * Mongoose - Created, from old, old GooseEgg function
	 ------------------------------------------------------*/

	static bool doesFileExist(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Opens a disk file to determine if it exists
	 * Post : Returns true if file exists
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isDirectory(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if the filename is actually for a
	 *        directory
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void closeDirectory();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Closes attribute directory if open
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void closeFile();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Closes attribute file if open
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char *getNextDirectoryListing();
	/*------------------------------------------------------
	 * Pre  : Directory must be open
	 * Post : Returns a pointer to a string of the listing
	 *        item ( Could be a file or directory )
	 *
	 *        Each call the listing is cleared, which means
	 *        if you want a copy of this string copy it.
	 *
	 *        Returns 0x0 for end of listing or error.
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool openDirectory(const char *dirname);
	/*------------------------------------------------------
	 * Pre  : Opens a directory on disk
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool openFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Opens a file for reading from disk
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	double parseFloat();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Returns a parsed float
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int parseInteger();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Returns a parsed signed integer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool parseMatchingSymbol(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Returns true if symbol found matches string
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char *parseString();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Returns an allocated C string
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char *parseSymbol();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Returns a parsed symbol 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void readBufferUnsignedChar(unsigned int length, unsigned char *buffer);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read into buffer from file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void readCharString(unsigned int length, char *buffer);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of string into buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	float readFloat32();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 32bit float
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char readInt8();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 8bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned char readInt8U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 8bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	short readInt16();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 16bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned short readInt16U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 16bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int readInt32();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 32bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int readInt32U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary read of small endian 32bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////



	FILE *mFileHandle;                /* File cruft */
	char *mTempBufferHack;
	unsigned int mTempBufferHackSz;

	DIR *mDirectory;                  /* Directory cruft */
	char *mDirectoryName;
	char *mDirectoryListing;
};

#endif
