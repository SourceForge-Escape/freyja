/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : OpenGLFreyjaLight
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: This is the GL light class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.01.27:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_LIGHT_H_
#define GUARD__FREYJA_MONGOOSE_LIGHT_H_

#include <freyja/FreyjaLight.h>


class OpenGLFreyjaLight : public FreyjaLight
{
 public:

	OpenGLFreyjaLight();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Light
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~OpenGLFreyjaLight();
	/*------------------------------------------------------
	 * Pre  : Light object is allocated
	 * Post : Deconstructs an object of Light
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static void renderSymbol(FreyjaLight *light);

	void enable();

	void enableGobalAmbient();


 private:
	//vec4_t mProjectionColor; /* unused for now */

	bool mEnabled;                    /* Is this light enabled? */

	static vec4_t mGobalAmbient;      /* Gobal Ambient color */
};

#endif
