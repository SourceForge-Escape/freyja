/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaFileWriter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides disk file write access
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAFILEWRITER - Builds FreyjaFileWriter class unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.14: 
 * Mongoose - Fixed generated code for binary file I/O
 *
 * 2004.08.21:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAFILEWRITER_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAFILEWRITER_H_

#include <stdio.h>
#include <string.h>

// FIXME: Quick hack until new namespaces are set
#define FileWriter FreyjaFileWriter

class FreyjaFileWriter
{
 public:

	typedef enum {

		BIG = 1,
		LITTLE

	} byte_order_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaFileWriter();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaFileWriter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaFileWriter();
	/*------------------------------------------------------
	 * Pre  : FreyjaFileWriter object is allocated
	 * Post : Deconstructs an object of FreyjaFileWriter
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

	static bool createDirectory(const char *dir);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static void getBasename(const char *filename, char *basename, long size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool copyFileToPath(const char *filename, const char *path);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.01:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void bigEndianMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reads in big endian order
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26:
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

	void littleEndianMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reads in little endian order
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26:
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

	void writeBuffer(unsigned int length, unsigned char *buffer);
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

	void writeFloat32(float r);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit float
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt8(char i);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 8bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt8U(unsigned char u);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 8bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt16(short i);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 16bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt16U(unsigned short u);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 16bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt32(int i);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeInt32U(unsigned int u);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeLong(long l);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit signed int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.14:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void writeLongU(unsigned long l);
	/*------------------------------------------------------
	 * Pre  : openFile must have been sucessful, etc
	 * Post : Binary write of small endian 32bit unsigned int
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.14:
	 * Mongoose - Created
	 ------------------------------------------------------*/



 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	byte_order_t mOrder;

	FILE *mFileHandle;                /* File I/O cruft */
	char *mTempBufferHack;
	unsigned int mTempBufferHackSz;
};

#endif
