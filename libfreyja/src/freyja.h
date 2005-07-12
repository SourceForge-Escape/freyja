/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaMesh and classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAMESH - Builds FreyjaMesh class as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJA__H_
#define GUARD__FREYJA_MONGOOSE_FREYJA__H_

#include <hel/math.h>
//#include <hel/Vector3d.h>
//#include <mstl/Vector.h>

#define FREYJA_API_VERSION   "Freyja 0.10.0"

/* FSM uses index_t with extentions by these error/states */
#define FREYJA_NEXT        (UINT_MAX - 1)
#define FREYJA_RESET       (UINT_MAX - 2)
#define FREYJA_CURRENT     (UINT_MAX - 3)
#define FREYJA_SIZE        (UINT_MAX - 4)
#define FREYJA_ERROR       (UINT_MAX - 5)

/* UINT_MAX 32bit */
#define INDEX_INVALID      4294967295U

typedef uint32 index_t;

typedef unsigned char byte;

extern "C" {

	void freyjaPrintError(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stderr or gPrinter
	 ------------------------------------------------------*/

	void freyjaPrintMessage(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout or gPrinter
	 ------------------------------------------------------*/
}

#endif
