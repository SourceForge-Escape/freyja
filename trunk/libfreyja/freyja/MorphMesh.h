/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : MorphMesh
 * Comments: This is the freyja::Mesh and class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.10:
 * Mongoose - XML serializer, doc update, new clean methods to 
 *            shrink sparse buffers, etc.
 *
 * 2004.10.22:
 * Mongoose - Created, expanded from Freyja class
 ==========================================================================*/

#ifndef GUARD__FREYJA_MORPHMESH_H__
#define GUARD__FREYJA_MORPHMESH_H__

#include <hel/Ray.h>
#include "Weight.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Plane.h"

#include "SceneNode.h"
#include "VertexArray.h"

namespace freyja {

class MorphMesh :
		public Mesh
{
public:	
	
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MorphMesh( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	MorphMesh( const MorphMesh& mesh );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy constructor.
	 *
	 ------------------------------------------------------*/

	~MorphMesh( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool GetBlendShadowVolume(mstl::Vector<vec_t> &shadowVolume,
							  vec3_t lightPos, vec3_t pos, vec_t cinf);
	/*------------------------------------------------------
	 * Pre  : <cinf> > 0.0f - Then apply max occlusion distance.
	 * 
	 * Post : Rebuilds shadow volume for this mesh.
	 *        Uses currently past in shadow volume buffer.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	bool GetShadowVolume(mstl::Vector<vec_t> &shadowVolume,
						 vec3_t lightPos, vec3_t pos, vec_t cinf);
	/*------------------------------------------------------
	 * Pre  : <cinf> > 0.0f - Then apply max occlusion distance.
	 * 
	 * Post : Rebuilds shadow volume for this mesh.
	 *        Uses currently past in shadow volume buffer.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

	index_t CreateVertexKeyframeFromImport(index_t track, vec_t time, Vector<vec_t>& vertices);
	/*------------------------------------------------------
	 * Pre  : List must have the same number of vertices as mesh.
	 *
	 * Post : The imported vertices are converted to a 
	 *        vertex morph keyframe.
	 *
	 *        Returns valid index on success. 
	 *
	 ------------------------------------------------------*/	


	//////////////////////////////////////////////////////////////////////////
	// Animation tracks interface
	// Testing embedded keyframing, so bare with me...
	// these methods only support one 'animation' atm, but enforcing interface
	// API usage makes it a transparent fix to API users later.
	//////////////////////////////////////////////////////////////////////////

	uint32 GetVertexAnimTrackCount() {return 1;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetVertexAnimKeyframeCount(uint32 track) 
	{ return mVertexAnimTrack.GetKeyframeCount();}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	index_t NewVertexAnimTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	VertexAnimTrack &GetVertexAnimTrack(uint32 track) {return mVertexAnimTrack;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual const char* GetType() const
	{ return "MorphMesh"; } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag string (often just class name). 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetVersion() const
	{ return 0; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag version (often just serial format version). 
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize(XMLSerializerNode parent) const;
	/*------------------------------------------------------
	 * Pre  : <parent> is this object's parent in XML tree.
	 * Post : Serializes this object to XML.
	 *
	 ------------------------------------------------------*/

	virtual bool Unserialize(XMLSerializerNode node);
	/*------------------------------------------------------
	 * Pre  : <node> is this object in XML tree.
	 * Post : Unserializes node from XML.
	 *
	 ------------------------------------------------------*/

protected:

	////////////////////////////////////////////////////////////
	// Protected 
	////////////////////////////////////////////////////////////

	VertexAnimTrack mVertexAnimTrack; /* Mesh vertex animation track  */

	vec_t mTime;                      /* Last time this was updated */

	FloatArray mTransformedVertices;  /* Vertex morphing use  */
};

} // namespace freyja

#endif // GUARD__FREYJA_MORPHMESH_H__
