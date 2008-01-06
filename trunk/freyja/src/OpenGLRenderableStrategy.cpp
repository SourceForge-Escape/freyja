/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : OpenGLRenderableStrategy
 * License : No use w/o permission (c) 2007 Mongoose
 * Comments: OpenGL renderer implementation for freyja::Renderable.
 *
 *-- History ------------------------------------------------- 
 *
 * 2007.12.31:
 * Mongoose - Created.
 ==========================================================================*/

#define TEST_OGL_STRATEGY 0

#include "FreyjaOpenGL.h" /* Includes windows.h, so avoid header interaction. */
#include "OpenGLRenderableStrategy.h"

using namespace freyja3d;
using namespace freyja;


void OpenGLRenderableStrategy::ApplyMaterial( freyja::Material* mat )
{
	if ( !mat )
	{
		/* Default to 'color' texture. */
		glColor3fv( WHITE );
		glBindTexture( GL_TEXTURE_2D, 0 );
		return;
	}

	{
		// Id 0 disables ( no weird index scheme here )
		int programId = ( mat->GetShaderId() == -1 ) ? 0 : mat->GetShaderId();
		freyja3d::OpenGL::BindFragmentGLSL( programId );
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat->GetAmbientColor() );
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->GetDiffuseColor() );
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat->GetSpecularColor() );
	glMaterialfv(GL_FRONT, GL_EMISSION, mat->GetEmissiveColor() );
	glMaterialf(GL_FRONT, GL_SHININESS, mat->GetShininess() );

	/* Texture maps. */
	if ( mat->GetDecalMapId() != -1 )
	{
		freyja3d::OpenGL::Bind( GL_TEXTURE0, mat->GetDecalMapId() );
	}
	else if ( 0 ) // flags & fTextured )
	{
		/* Texture mapped ( 1 unit ), ids are always ( id + 1). */
		//glBindTexture(GL_TEXTURE_2D, texture_id + 1);
	}
	else
	{
		/* Colored, first texture is a generated WHITE 32x32. */
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	/* Multitexture, Normal maps, etc... */
	if ( mat->GetSpecularMapId() != -1 )
		freyja3d::OpenGL::Bind( GL_TEXTURE1, mat->GetSpecularMapId() );
	
	if ( mat->GetNormalMapId() != -1 )
		freyja3d::OpenGL::Bind( GL_TEXTURE2, mat->GetNormalMapId() );
	
	if ( mat->GetHeightMapId() != -1 )
		freyja3d::OpenGL::Bind( GL_TEXTURE3, mat->GetHeightMapId() );
	
	if ( mat->GetEmissiveMapId() != -1 )
		freyja3d::OpenGL::Bind( GL_TEXTURE4, mat->GetEmissiveMapId() );

#warning FIXME: Test rendering implementation in tracer.

	if ( mat->IsBlendingEnabled() )
	{
		uint32 blendSrc = OpenGL::BlendStringToInt( mat->GetBlendSrc() );
		uint32 blendDest = OpenGL::BlendStringToInt( mat->GetBlendDest() );
		glBlendFunc( blendSrc, blendDest );
		glEnable( GL_BLEND );
	}
	else
	{
		glDisable( GL_BLEND );
	}
}


void OpenGLRenderableStrategy::DrawIcon( const char* name  )
{
#warning FIXME: No implementation in tracer.
}


void OpenGLRenderableStrategy::DrawTriangles(  )
{
#if TEST_OGL_STRATEGY

	if ( mVertexArrayPtr )
	{
		glColor3fv( GREEN );
		glBegin( GL_POINTS );

		for ( uint32 i = 0; i < mVertexArraySize; i+=3 )
		{
			glVertex3fv( mVertexArrayPtr + i );
		}

		glEnd( );
	}

	if ( mVertexArrayPtr && mIndexArrayPtr )
	{
		// Faces ( fill )
		glColor3fv( PINK );
		for ( uint32 i = 0; i < mIndexArraySize; )
		{
			glBegin( GL_TRIANGLES );
			index_t idx = mIndexArrayPtr[i++] * 3;
			glVertex3fv( mVertexArrayPtr+idx );
			idx = mIndexArrayPtr[i++] * 3;
			glVertex3fv( mVertexArrayPtr+idx );
			idx = mIndexArrayPtr[i++] * 3;
			glVertex3fv( mVertexArrayPtr+idx );
			glEnd( );
		}

		// Faces ( 'wireframe' )
		glColor3fv( RED );
		for ( uint32 i = 0; i < mIndexArraySize; )
		{
			glBegin( GL_LINE_LOOP );
			index_t idx = mIndexArrayPtr[i++];
			glVertex3fv( mVertexArrayPtr+idx );
			idx = mIndexArrayPtr[i++];
			glVertex3fv( mVertexArrayPtr+idx );
			idx = mIndexArrayPtr[i++];
			glVertex3fv( mVertexArrayPtr+idx );
			glEnd( );
		}

	}

	glPushAttrib(GL_ENABLE_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

#endif // TEST_OGL_STRATEGY

#warning FIXME: Test rendering implementation in tracer.
	/* Vertex array. */
	if ( mVertexArrayPtr && mIndexArrayPtr )
	{
		// NOTE: Once we switch to more advanced arrays we have to
		//       start locking the dynamic reallocation.
		//glPushClientAttrib(GL_NORMAL_ARRAY);
		//glEnableClientState(GL_NORMAL_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glNormalPointer(GL_FLOAT, sizeof(vec_t)*3, normalArray);
		//glTexCoordPointer(3, GL_FLOAT, sizeof(vec_t)*3, texcoordArray);
		
		glEnableClientState( GL_VERTEX_ARRAY );

		glVertexPointer( mVertexWidth, GL_FLOAT, 0, mVertexArrayPtr );

		glColor3fv( WHITE );
		glDrawElements( GL_TRIANGLES, mIndexArraySize, GL_UNSIGNED_INT, mIndexArrayPtr );

		glColor3fv( BLUE );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDrawElements( GL_TRIANGLES, mIndexArraySize, GL_UNSIGNED_INT, mIndexArrayPtr );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glColor3fv( YELLOW );
		glDrawElements( GL_POINTS, mIndexArraySize, GL_UNSIGNED_INT, mIndexArrayPtr );

		//glPopClientAttrib( );
	}

#if TEST_OGL_STRATEGY
	glPopAttrib( );
#endif // TEST_OGL_STRATEGY
}


