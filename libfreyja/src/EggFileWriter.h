/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : EggFileWriter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides disk file write access
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_EGGFILEWRITER - Builds EggFileWriter class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.21:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_EGGFILEWRITER_H_
#define GUARD__FREYJA_MONGOOSE_EGGFILEWRITER_H_

#include <stdio.h>
#include <string.h>


class EggFileWriter
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	EggFileWriter();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of EggFileWriter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~EggFileWriter();
	/*------------------------------------------------------
	 * Pre  : EggFileWriter object is allocated
	 * Post : Deconstructs an object of EggFileWriter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

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


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

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

	bool openFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Opens a file for writing to disk
	 * Post : Returns true if sucessful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void print(const char *s, ...);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Writes printf formatted text to text file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printFloat(double d);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Writes float to text file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printInteger(int i);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Writes integer to text file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printString(const char *s);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Writes string to text file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeBufferUnsignedChar(unsigned int length, unsigned char *buffer);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of buffer to file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeCharString(unsigned int length, char *buffer);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of string into buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	float writeFloat32();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit float
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char writeInt8();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 8bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned char writeInt8U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 8bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	short writeInt16();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 16bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned short writeInt16U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 16bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int writeInt32();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int writeInt32U();
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit unsigned int
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



	FILE *mFileHandle;                /* File I/O cruft */
	char *mTempBufferHack;
	unsigned int mTempBufferHackSz;
};

#endif
