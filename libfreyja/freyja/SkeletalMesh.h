/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Object  : SkeletalMesh
 * Comments: 
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

#ifndef GUARD__FREYJA_SKELETALMESH_H__
#define GUARD__FREYJA_SKELETALMESH_H__

#include <hel/Ray.h>
#include "Mesh.h"

namespace freyja {

class SkeletalMesh : 
	public Mesh
{
public:	
	
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	SkeletalMesh( freyja::Skeleton* skeleton, const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	SkeletalMesh( const SkeletalMesh& mesh );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy constructor.
	 *
	 ------------------------------------------------------*/

	~SkeletalMesh( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Weight methods
	////////////////////////////////////////////////////////////

	freyja::Weight* CreateWeight( freyja::Bone* bone, freyja::Vertex* vertex );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	freyja::Weight* GetWeight( index_t w ) 
	{ return ( w < mWeights.size() ) ? mWeights[w] : NULL; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetWeightCount() 
	{ return mWeights.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void RemoveWeight( freyja::Weight* weight );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool CopyVertexBlendBuffer(mstl::Vector<vec_t> &buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies vertex blend buffer to external Vector.
	 *        Returns true on success.
	 *
	 ------------------------------------------------------*/

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


	////////////////////////////////////////////////////////////
	// Move to external algorithms.
	////////////////////////////////////////////////////////////

	Mesh* CopyWithBlendedVertices() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies this mesh, except the blend vertices
	 *        replace the unweighted vertices.
	 *
	 *        Returns mesh outside of pool control. 
	 *
	 ------------------------------------------------------*/

	void Merge(Mesh *mesh);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        The passed <mesh> will be duplicated entirely
	 *        into this mesh.  There is no 'partial merge'
	 *        anymore.  If you want partial do a Split() first.
	 *
	 *        Also this doesn't make a 3rd mesh, but adds
	 *        to this existing one.  Copy() if you want that.
	 *
	 *        If you want <mesh> to not exist after merge
	 *        then you'll have to delete it yourself.
	 ------------------------------------------------------*/

	Mesh* PartialCopy() const;
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        A new mesh is made from this mesh based on the
	 *        currently fSelected flagged Faces.
	 *
	 *        This mesh exists outside of the scene.
	 *        If you want to add use the AddToPool() method.
	 ------------------------------------------------------*/

	Mesh *Split();
	/*------------------------------------------------------
	 * Pre  : If <trim> is true remove the selected faces
	 *        from this mesh after splitting off to another mesh.
	 * 
	 * Post : Not like the old school 2000.11.30 API, Read
	 *
	 *        A new mesh is made from this mesh based on the
	 *        currently fSelected flagged Faces.
	 *
	 *        This mesh exists outside of the gobal pool.
	 *        If you want to add use the AddToPool() method.
	 ------------------------------------------------------*/

	void RemoveWeightSelectedVertices(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetWeightSelectedVertices(index_t bone, vec_t weight);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const vec_t* GetBlendVerticesArray() const
	{ return mBlendVertices.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetBlendVerticesCount() const 
	{ return mBlendVertices.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void ResetBlendVertices();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	void SyncBlendVertices();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Interfaces
	////////////////////////////////////////////////////////////

	static freyja::SkeletalMesh* Cast( freyja_ptr ptr )
	{ return (freyja::SkeletalMesh*)ptr; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME Add RTTI checking.
	 *
	 ------------------------------------------------------*/

	FREYJA_NODE_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Node implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_RENDERABLE_INTERFACE
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renderable implementation.
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected.
	////////////////////////////////////////////////////////////

	VertexAnimTrack mVertexAnimTrack;   /* Mesh vertex animation track  */

	vec_t mBlendVerticesTime;           /* Last time this was updated */

	FloatArray mBlendVertices;          /* Skeletal vertex blending use  */

	Vector<freyja::Weight*> mWeights;
};


inline
const char* SkeletalMesh::GetType() const
{ return "SkeletalMesh"; }

 
inline
uint32 SkeletalMesh::GetVersion() const
{ return 0; }


inline
freyja::Node* SkeletalMesh::Duplicate() const
{ return new SkeletalMesh(*this); }


inline
freyja::Material* SkeletalMesh::GetMaterial() const
{ return NULL; }


} // namespace freyja

#endif // GUARD__FREYJA_SKELETALMESH_H__
