/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Material
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: GL Material
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MATERIAL - Builds Material class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2002.07.03:
 * Mongoose - My newer coding style applied during refactoring
 *
 *            This is also useful for tracking down how it's used
 *            since it breaks the API when I remove the adapter
 *
 * 2002.01.21:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_MATERIAL_H_
#define GUARD__FREYJA_MONGOOSE_MATERIAL_H_


#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#endif


class Material
{
 public:

	typedef enum {
		fEnable_Blending         = 1,
		fEnable_Texture          = 2,
		fEnable_DetailTexture    = 4,
		fEnable_Normalize        = 8   /* Normals corrected for glScale use */
	} MaterialFlags_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Material();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Material
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Material();
	/*------------------------------------------------------
	 * Pre  : Material object is allocated
	 * Post : Deconstructs an object of Material
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

   static int getBlendIndex(int blend);
	/*------------------------------------------------------
	 * Pre  : Pass blend value, Built with HAVE_GL
	 * Post : Returns -1 if not used, or index if used
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Made static member
	 *
	 * 2002.02.14: 
	 * Mongoose - Created, to allow GL values to be mapped
	 *            outside of this class where GL isn't used
	 ------------------------------------------------------*/

	static unsigned int getCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of unique materials
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.22:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getFlags();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get currently set flags
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int getId();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns unique material id ( 1..N, or 0 if invalid )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.22:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Material's name or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getTextureName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's texture filename or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	const char *getDetailTextureName();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets Material's detail texture filename or NULL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setFlag(MaterialFlags_t flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets passed flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clearFlag(MaterialFlags_t flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unsets passed flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	void setTextureName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's texture filename
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	void setDetailTextureName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's detail texture filename
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	int saveFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid with full path
	 * Post : Saves material to file
	 *
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.22:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	int loadFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid with full path
	 * Post : Loads material from file
	 *
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.22:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

	void applyEffectGL();
	/*------------------------------------------------------
	 * Pre  : Built with USING_GL
	 * Post : Sets up this material for use in GL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.07.03:
	 * Mongoose - Created, from old FreyjaRender code
	 ------------------------------------------------------*/


	float ambient[4];          /* Ambient color */

	float diffuse[4];          /* Diffuse color */

	float specular[4];         /* Specular color */

	float emissive[4];         /* Emissive color */

	float shininess;           /* Specular exponent */

	unsigned int texture;      /* Texture id */

	unsigned int texture2;     /* Detail Texture id */

	unsigned int blend_src;    /* Blend source factor */

	unsigned int blend_dest;   /* Blend destination factor */

private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	unsigned int m_id;          /* Unique identifier of this material */

	unsigned int m_flags;       /* Rendering and special flags */

	char *m_name;               /* Material name */

	char *m_textureFilename;    /* Filename of texture */

	char *m_textureDetailFilename;    /* Filename of detail texture */

  	/* Id system use */
	static unsigned int m_counter;
};

#endif
