/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Sims
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the SIMS plugin module.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SIMS - Builds Sims class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.26:
 * Mongoose - Created, Spec from http://simtech.sourceforge.net/tech/iff.html
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_SIMS_H_
#define GUARD__FREYJA_MONGOOSE_SIMS_H_

#include <mstl/Vector.h>

#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>


/* NOTE: This is a BIG_ENDIAN format */
#define IFF_IDENT1 "IFF FILE 2.5:TYPE FOLLOWED BY SIZE"
#define IFF_IDENT2 " JAMIE DOORNBOS & MAXIS 1"
#define IFF_BCON 0x42434f4e
#define IFF_BHAV 0x42484156
#define IFF_BMP_ 0x424d505f
#define IFF_CTSS 0x43545353
#define IFF_DGRP 0x44475250
#define IFF_FWAV 0x46574156
#define IFF_PALT 0x50414c54
#define IFF_TTAs 0x54544173
#define IFF_TTAB 0x54544142
#define IFF_STRN 0x53545223
#define IFF_OBJD 0x4f424a44
#define IFF_SPR2 0x53505232
#define IFF_XXXX 0x58585858
#define IFF_rsmp 0x72736d70
#define IFF_SLOT 0x534c4f54


class IFFPak
{
public:

	bool check(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if valid IFF header found
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if valid and loads
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

private:

	bool loadChunk(FreyjaFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if valid and loads
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char mId[35];

	char mId2[25];

	long mResourceMapOffset;

	Vector<unsigned char *> mPalettes;
};


class Sims
{
 public:


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Sims();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Sims
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Sims();
	/*------------------------------------------------------
	 * Pre  : Sims object is allocated
	 * Post : Deconstructs an object of Sims
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif
