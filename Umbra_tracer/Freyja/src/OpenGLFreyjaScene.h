/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : OpenGLFreyjaScene
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Renderer for FreyjaScene data model.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_OPENGLFREYJASCENE - Builds OpenGLFreyjaScene console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.23:
 * Mongoose - Created, Replaces aging FreyjaRender class
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_OPENGLFREYJASCENE_H_
#define GUARD__FREYJA_MONGOOSE_OPENGLFREYJASCENE_H_

#include <freyja/Freyja.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include "OpenGLFreyjaLight.h"


class OpenGLFreyjaMesh : public FreyjaMesh
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	OpenGLFreyjaMesh();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/

	~OpenGLFreyjaMesh();
	/*------------------------------------------------------
	 * Pre  : FreyjaMesh object is allocated
	 * Post : Deconstructs an object of FreyjaMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created, from FreyjaMesh in Freyja
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void renderPolygon(FreyjaPolygon &polygon);

	void render()
	{
		FreyjaPolygon *polygon;
		unsigned int i;


		glEnable(GL_TEXTURE_2D); // FIXME: Should use material.texture
		glBindTexture(GL_TEXTURE_2D, material+1);

		// Make all polygons referencing A point to B
		for (i = polygons.begin(); i < polygons.end(); ++i)
		{
			polygon = polygons[i];

			if (polygon)
				renderPolygon(*polygon);
		}
	}

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};


class OpenGLFreyjaScene : public FreyjaScene
{
 public:

	typedef enum {          /* Rendering modes */
		fWireframe          = 1,
		fTexture            = 2,
		fFace               = 4,
		fGrid               = 8,
		fLighting           = 16,       /* Render with GL lighting */
		fParticles          = 32,
		fBones              = 64,
		fMaterial           = 128,
		fCamera             = 256,      /* Enable panning  */
		fBoundingBox        = 512,      /* Rendering bounding box */
		fVisualNormals      = 1024, 
		fNormals            = 2048, 
		fHightlightPolygon  = 4096, 
		fRenderSelection    = 8192,
		fReserved16384      = 16384,
		fVertices           = 32768,
	} render_mode_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	OpenGLFreyjaScene();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGLFreyjaScene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~OpenGLFreyjaScene();
	/*------------------------------------------------------
	 * Pre  : OpenGLFreyjaScene object is allocated
	 * Post : Deconstructs an object of OpenGLFreyjaScene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void getSceneTranslation(vec3_t translation)
	{
		translation[0] = mPos[0];
		translation[1] = mPos[1];
		translation[2] = mPos[2];
	}


	void render(FreyjaMesh &mesh, FreyjaPolygon &polygon);

	void render(FreyjaMesh &mesh)
	{
		unsigned int i;

		for (i = mesh.polygons.begin(); i < mesh.polygons.end(); ++i)
		{
			if (mesh.polygons[i])
				render(mesh, *mesh.polygons[i]);
		}
	}


	void render(FreyjaModel &model)
	{
		unsigned int i;
		
		for (i = model.meshes.begin(); i < model.meshes.end(); ++i)
		{
			if (meshes[model.meshes[i]])
				render(*meshes[model.meshes[i]]);
		}
	}


	/* Mongoose 2004.10.30, 
	 * Should use scene graph to handle duped models */
	void render()
	{
		unsigned int i;

		if (mRenderMode & fLighting) // FIXME: testing before scene lights implemented
			OpenGLFreyjaScene::mMaterialLight.enable();

		for (i = models.begin(); i < models.end(); ++i)
		{
			if (models[i])
				render(*models[i]);
		}
	}


	void renderCurrentModel()
	{
		if (!models.empty() && models[mCurrentModel])
			render(*models[mCurrentModel]);
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	/* Some sample colors */
	static vec4_t mRed;
	static vec4_t mGreen;
	static vec4_t mBlue;
	static vec4_t mCyan;
	static vec4_t mOrange;
	static vec4_t mYellow;
	static vec4_t mBlack;
	static vec4_t mWhite;
	static vec4_t mNextPurple;
	static vec4_t mLightNextPurple;

	/* Custom color support */
	static vec4_t mColorBackground;
	static vec4_t mColorText;
	static vec4_t mColorWireframe;
	static vec4_t mColorWireframeHighlight;
	static vec4_t mColorBoundingBox;
	static vec4_t mColorGridSeperator;
	static vec4_t mColorGridLine;
	static vec4_t mColorVertex;
	static vec4_t mColorVertexHighlight;
	static vec4_t mColorAxisX;
	static vec4_t mColorAxisY;
	static vec4_t mColorAxisZ;
	static vec4_t mColorAnimationGrid;

	/* Custom artifact size */
	static vec_t mLineWidthBone;
	static vec_t mLineWidthDefault;
	static vec_t mPointSizeJoint;
	static vec_t mPointSizeVertex;
	static vec_t mPointSizeDefault;

	static unsigned int mCurrentModel;

	static unsigned int mRenderMode;

	static OpenGLFreyjaLight mMaterialLight;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	vec3_t mPos;

	vec_t mZoom;                               /* Scaling of rendered scene */
};

class MeshEditorWindow
{
public:

	vec4_t mColorBackground;
};

#endif
