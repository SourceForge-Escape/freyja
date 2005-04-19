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
 *-- History ------------------------------------------------- 
 *
 * 2004.10.23:
 * Mongoose - Created
 ==========================================================================*/

#include "freyja_events.h"
#include "MaterialManager.h" // temp

#include "OpenGLFreyjaScene.h"


vec4_t OpenGLFreyjaScene::mRed             = {  1.0,  0.0,  0.0, 1.0 };
vec4_t OpenGLFreyjaScene::mGreen           = {  0.0,  1.0,  0.0, 1.0 };
vec4_t OpenGLFreyjaScene::mBlue            = {  0.0,  0.0,  1.0, 1.0 };
vec4_t OpenGLFreyjaScene::mCyan            = {  0.0,  1.0,  1.0, 1.0 };
vec4_t OpenGLFreyjaScene::mOrange          = {  1.0,  0.0,  7.0, 1.0 };
vec4_t OpenGLFreyjaScene::mYellow          = {  1.0,  1.0,  0.0, 1.0 };
vec4_t OpenGLFreyjaScene::mBlack           = {  0.0f,  0.0,  0.0, 1.0 };
vec4_t OpenGLFreyjaScene::mWhite           = {  1.0f,  1.0,  1.0, 1.0 };
vec4_t OpenGLFreyjaScene::mNextPurple      = {  0.3f,  0.3,  0.5, 1.0 };
vec4_t OpenGLFreyjaScene::mLightNextPurple = { 0.4f, 0.4f, 0.6f, 1.0f};

vec4_t OpenGLFreyjaScene::mColorBackground;
vec4_t OpenGLFreyjaScene::mColorText;
vec4_t OpenGLFreyjaScene::mColorWireframe;
vec4_t OpenGLFreyjaScene::mColorWireframeHighlight;
vec4_t OpenGLFreyjaScene::mColorBoundingBox;
vec4_t OpenGLFreyjaScene::mColorGridSeperator;
vec4_t OpenGLFreyjaScene::mColorGridLine;
vec4_t OpenGLFreyjaScene::mColorVertex;
vec4_t OpenGLFreyjaScene::mColorVertexHighlight;
vec4_t OpenGLFreyjaScene::mColorAxisX;
vec4_t OpenGLFreyjaScene::mColorAxisY;
vec4_t OpenGLFreyjaScene::mColorAxisZ;
vec4_t OpenGLFreyjaScene::mColorAnimationGrid;

vec_t OpenGLFreyjaScene::mLineWidthBone = 3.0f;
vec_t OpenGLFreyjaScene::mLineWidthDefault = 1.0;
vec_t OpenGLFreyjaScene::mPointSizeJoint = 5.0f;
vec_t OpenGLFreyjaScene::mPointSizeVertex = 3.5f;
vec_t OpenGLFreyjaScene::mPointSizeDefault = 3.5f;


unsigned int OpenGLFreyjaScene::mRenderMode = 0;
unsigned int OpenGLFreyjaScene::mCurrentModel = 0;
OpenGLFreyjaLight OpenGLFreyjaScene::mMaterialLight;



// polygon.id
// polygon.flags
// polygon.material
// polygon.vertices
// polygon.texcoords
void OpenGLFreyjaScene::render(FreyjaMesh &mesh, FreyjaPolygon &polygon)
{
	vec_t *vertexArray = mesh.vertices.getVectorArray();
	vec_t *texcoordArray = mesh.texcoords.getVectorArray();
	vec_t *normalArray = mesh.normals.getVectorArray();
	unsigned int i, vertex, texcoord;


	if (polygon.vertices.size() < 3)
	{
		freyja_print("renderPolygon> Polygon %u has less than 3 vertices\n",
					 polygon.id);
		return;
	}

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);		

	/* Render wireframe with color as polylines */
	if (mRenderMode & fWireframe)
	{
		/* Update wireframe color with either highlight or normal color */
		if (mRenderMode & fHightlightPolygon)
		{
			glColor4fv(mColorWireframe);    
		}
		else
		{
			glColor4fv(mColorWireframeHighlight);
		}

		glLineWidth(mLineWidthDefault);

		glBegin(GL_LINE_LOOP);

		for (i = polygon.vertices.begin(); i < polygon.vertices.end(); ++i)
		{
			vertex = polygon.vertices[i];

			/* Mongoose: 2001.10.28 Scaling replaced by mul ops */
			glVertex3f(vertexArray[vertex*3] * 1.0001, 
					   vertexArray[vertex*3+1] * 1.0001, 
					   vertexArray[vertex*3+2] * 1.0001);
		}
	  
		glEnd();
	}


	/* Render vertices */
	if (mRenderMode & fVertices)
	{
		glPointSize(mPointSizeVertex);

		/* Update wireframe color with either highlight or normal color */
		if (mRenderMode & fHightlightPolygon)
		{
			glColor3fv(mColorVertex);
		}
		else
		{
			glColor4fv(mColorVertexHighlight);
		}

		glBegin(GL_POINTS);
	  
		for (i = polygon.vertices.begin(); i < polygon.vertices.end(); ++i)
		{
			vertex = polygon.vertices[i];

			/* Mongoose: 2001.10.28 Scaling replaced by mul ops */
			glVertex3f(vertexArray[vertex*3] * 1.0001, 
					   vertexArray[vertex*3+1] * 1.0001, 
					   vertexArray[vertex*3+2] * 1.0001);
		}

		glEnd();
	}

	/* Render vertex normals */
	if (mRenderMode & fNormals && normalArray)
	{
		glBegin(GL_LINES);
		glColor3f(0.2, 0.2, 0.8);
		
		for (i = polygon.vertices.begin(); i < polygon.vertices.end(); ++i)
		{
			vertex = polygon.vertices[i] * 3;

			glVertex3fv(vertexArray + vertex);
			glVertex3f(vertexArray[vertex  ] + normalArray[vertex  ]*2*1/mZoom, 
					   vertexArray[vertex+1] + normalArray[vertex+1]*2*1/mZoom, 
					   vertexArray[vertex+2] + normalArray[vertex+2]*2*1/mZoom);
		}
		
		glEnd();
	}

	glPopAttrib();


	/* Render face */
	if (mRenderMode & fFace)
	{
		/* Call material binding ( no shader support yet ) */
		if (mRenderMode & fMaterial)
		{
			//glPushAttrib(GL_ENABLE_BIT);
			gMaterialManager->applyEffectGL(polygon.material);
		}

		if (mRenderMode & fTexture)// && polygon.material != mesh.material)
		{


			// FIXME: make it material.texture when using FreyjaMaterial
			glBindTexture(GL_TEXTURE_2D, polygon.material+1);
			// BindTexture(polygon.material+1);
		}
		else
		{
			// FIXME: make it COLOR_TEXTURE_ID when using FreyjaMaterial
			glBindTexture(GL_TEXTURE_2D, 0); // Colored
		}

		glColor3fv(mWhite);

		switch (polygon.vertices.size())
		{
		case 3:
			glBegin(GL_TRIANGLES);
			break;
		case 4:
			glBegin(GL_QUADS);
			break;
		default:
			glBegin(GL_POLYGON);
		}

		for (i = polygon.vertices.begin(); i < polygon.vertices.end(); ++i)
		{
			vertex = polygon.vertices[i];
			texcoord = polygon.texcoords[i];
			
			if (normalArray)
				glNormal3fv(normalArray+vertex*3);
				
			if (texcoordArray)
			{
				if (mRenderMode & fTexture)
				{
					glTexCoord2fv(texcoordArray+texcoord*2);
				}
				else
				{
					glColor4f(texcoordArray[texcoord*2], 
							  texcoordArray[texcoord*2+1],
							  0.5, 1.0);
				}
			}

			if (vertexArray)
				glVertex3fv(vertexArray+vertex*3);
		}
	 
		glEnd();

		//if (mRenderMode & fMaterial)
		//{
		//	  glPopAttrib();
		//}
	}
}



////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGLFreyjaScene::OpenGLFreyjaScene()
{
	vec4_t pos = {128.0, 128.0, 128.0, 1.0};

	mPos[0] = 0.0f;
	mPos[1] = -18.0f;
	mPos[2] = 0.0f;

	OpenGLFreyjaScene::mMaterialLight.setPosition(pos);
}


OpenGLFreyjaScene::~OpenGLFreyjaScene()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGLFREYJASCENE
int runOpenGLFreyjaSceneUnitTest(int argc, char *argv[])
{
	OpenGLFreyjaScene test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[OpenGLFreyjaScene class test]\n");

	return runOpenGLFreyjaSceneUnitTest(argc, argv);
}
#endif
