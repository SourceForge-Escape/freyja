/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Light
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
 ================================================================*/


#ifndef __FREYJA_MONGOOSE_LIGHT_H_
#define __FREYJA_MONGOOSE_LIGHT_H_

typedef enum
{
	FREYJA_LIGHT_POINT        = 1,
	FREYJA_LIGHT_SPOT         = 2,
	FREYJA_LIGHT_DIRECTIONAL  = 3

} freyja_light_type_t;


class Light
{
 public:

	Light();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Light
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Light();
	/*------------------------------------------------------
	 * Pre  : Light object is allocated
	 * Post : Deconstructs an object of Light
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	float ambient[4];          /* Ambient color */

	float diffuse[4];          /* Diffuse color */

	float specular[4];         /* Specular color */

	vec3_t pos;                /* Light position in 3 space */

	vec3_t dir;	               /* Light direction */

 private:

	unsigned int _id;                /* Unique identifier of this light */

	char *_name;                     /* Light name */

	freyja_light_type_t _type;	      /* Type of light */
};

#endif
