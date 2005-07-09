/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaPakReader
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is a facade for generic pak browsing
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_PAKBROWSER - Builds FreyjaPakReader class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.07.28:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_PAKBROWSER_H_
#define GUARD__FREYJA_MONGOOSE_PAKBROWSER_H_


#include <mstl/Vector.h>


class FreyjaPakFile
{
 public:

	char *getName();

	unsigned int getDataSize();

	unsigned char *getDataPointer();

	unsigned char *getCopyOfData();
};


class FreyjaPakDirectory
{
 public:

	FreyjaPakDirectory *getDir(unsigned int index);

	FreyjaPakFile *getFile(unsigned int index);

	char *getDirName(unsigned int index);

	char *getFileName(unsigned int index);

	unsigned int getDirCount();

	unsigned int getFileCount();

	char *getName();

 protected:

	Vector <FreyjaPakDirectory *> mDir;

	Vector <FreyjaPakFile *> mFiles;
};


class FreyjaPakReader
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaPakReader();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaPakReader();
	/*------------------------------------------------------
	 * Pre  : FreyjaPakReader object is allocated
	 * Post : Deconstructs an object of FreyjaPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void viewFile(char *name);
	void viewFile(unsigned int index);

	void dumpFile(char *name);
	void dumpFile(unsigned int index);

	void changeDir(char *name);
	void changeDir(unsigned int index);

	// Factor these out to FreyjaPakWriter
	void mkdirDir(char *name);
	void removeDir(char *name);
	void removeDir(unsigned int index);


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	FreyjaPakDirectory mRoot;        /* Virtual root directory */
};

#endif
