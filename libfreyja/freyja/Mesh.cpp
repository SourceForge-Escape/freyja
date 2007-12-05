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
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created
 ==========================================================================*/

#include "Mesh.h"


using namespace freyja;
using namespace hel;

Vector<Mesh *> Mesh::mGobalPool;
//index_t Mesh::mNextUID = 0;

#define GROUP_FACE_WITHOUT_BITMAPS 0


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Mesh::Mesh() :
	mTrack(),
	mVertexAnimTrack(),
	mBlendVerticesTime(-1.0f),
	mBlendVertices(),
	mUID(INDEX_INVALID),
	mInitBoundingVol(false),
	mPacked(false),
	mFlags(0),
	mMaterialIndex(0),
	mPosition(),
	mRotation(),  // Store as Euler Angles for 'Size' interface
	mScale(),
	mBoundingVolume(),
	mVertexPool(),
	mFreedVertices(),
	mNormalPool(),
	mFreedNormals(),
	mColorPool(),
	mFreedColors(),
	mTexCoordPool(),
	mFreedTexCoords(),
	mFaces(),
	mVertices(),
	mWeights(),
	mPlanes(),
	mEdges()
{
	mVertexPool.SetFlag(mstl::Vector<vec_t>::fNonClass);
	mNormalPool.SetFlag(mstl::Vector<vec_t>::fNonClass);
	mColorPool.SetFlag(mstl::Vector<vec_t>::fNonClass);
	mTexCoordPool.SetFlag(mstl::Vector<vec_t>::fNonClass);

	mName.Set("Mesh%i", mUID);
	
	mFlags = 0;
	mMaterialIndex = 0;
	mPosition = Vec3(0.0f, 0.0f, 0.0f);
	mRotation = Vec3(0.0f, 0.0f, 0.0f);
	mScale = Vec3(1.0f, 1.0f, 1.0f);

	// FIXME: Define larger intial boundary? 
	//mBoundingVolume = ...;
}


Mesh::Mesh(const Mesh &mesh) :
	mTrack(mesh.mTrack),
	mVertexAnimTrack(mesh.mVertexAnimTrack),
	mBlendVerticesTime(-1.0f),
	mBlendVertices(),
	mUID(INDEX_INVALID),
	mInitBoundingVol(false),
	mPacked(false),
	mFlags(mesh.mFlags),
	mMaterialIndex(mesh.mMaterialIndex),
	mPosition(mesh.mPosition),
	mRotation(mesh.mRotation),
	mScale(mesh.mScale),
	mBoundingVolume(mesh.mBoundingVolume),
	mVertexPool(mesh.mVertexPool),
	mFreedVertices(mesh.mFreedVertices),
	mNormalPool(mesh.mNormalPool),
	mFreedNormals(mesh.mFreedNormals),
	mColorPool(mesh.mColorPool),
	mFreedColors(mesh.mFreedColors),
	mTexCoordPool(mesh.mTexCoordPool),
	mFreedTexCoords(mesh.mFreedTexCoords),
	mFaces(),
	mVertices(),
	mWeights(),
	mPlanes(),
	mEdges()
{
	mName.Set("Mesh%i", mUID);

	uint32 i;

	foreach (((Mesh&)mesh).mVertices, i)
	{
		if (((Mesh&)mesh).mVertices[i])
		{
			mVertices.push_back(new Vertex(*(((Mesh&)mesh).mVertices[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mVertices.push_back(NULL);
		}

		DEBUG_MSG("\t%i - vertices copied\n", i);
	}


	foreach (((Mesh&)mesh).mFaces, i)
	{
		if (((Mesh&)mesh).mFaces[i])
		{
			mFaces.push_back(new Face(*(((Mesh&)mesh).mFaces[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mFaces.push_back(NULL);
		}

		DEBUG_MSG("\t%i - Facees copied\n", i);
	}


	foreach (((Mesh&)mesh).mWeights, i)
	{
		if (((Mesh&)mesh).mWeights[i])
		{
			mWeights.push_back(new Weight(*(((Mesh&)mesh).mWeights[i])));
		}
		else  // Need to NULL pad to exact match if needed
		{
			mWeights.push_back(NULL);
		}

		DEBUG_MSG("\t%i - weights copied\n", i);
	}

	//VertexCleanup();
}


Mesh::~Mesh()
{
	// Force removal in case of bad delete calls
	if (GetMesh(mUID) == this)
		RemoveFromPool();

	uint32 i;

	foreach (mWeights, i)
	{
		if (mWeights[i])
		{
			delete mWeights[i];
			mWeights[i] = NULL;
		}
	}

	foreach (mVertices, i)
	{
		if (mVertices[i])
		{
			delete mVertices[i];
			mVertices[i] = NULL;
		}
	}

	foreach (mFaces, i)
	{
		if (mFaces[i])
		{
			delete mFaces[i];
			mFaces[i] = NULL;
		}
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Mesh::CopyVertexBuffer(mstl::Vector<vec_t> &buffer)
{ 
	if (buffer.size() < mVertexPool.size())
	{
		buffer.resize(mVertexPool.size());
	}
		
	if (buffer.get_array() && 
		buffer.size() >= mVertexPool.size())
	{
		memcpy(buffer.get_array(), 
			   mVertexPool.get_array(), 
			   sizeof(vec_t) * mVertexPool.size() );
		
		return true;
	}
	
	return false;
}


bool Mesh::CopyVertexBlendBuffer(mstl::Vector<vec_t> &buffer)
{ 
	if (buffer.size() < mBlendVertices.size())
	{
		buffer.resize(mBlendVertices.size());
	}
		
	if (buffer.get_array() && 
		buffer.size() >= mBlendVertices.size())
	{
		memcpy(buffer.get_array(), 
			   mBlendVertices.get_array(), 
			   sizeof(vec_t) * mBlendVertices.size() );
		
		return true;
	}
	
	return false;
}


bool Mesh::GetShadowVolume(mstl::Vector<vec_t> &shadowVolume,
					 vec3_t lightPos, vec3_t pos, vec_t cinf)
{
	//TripleVec_Addition(shadowVolume, v);
	//TripleVec_Normalize(shadowVolume)
	//TripleVec_Mulitply(shadowVolume, inf);

	if ( CopyVertexBuffer(shadowVolume) )
	{
		// Translate by pos, and get vector from lightPos.
		vec3_t v = {pos[0] - lightPos[0], 
					pos[1] - lightPos[1], 
					pos[2] - lightPos[2] };

		TripleVec_Addition(shadowVolume, v);

		vec_t *array = shadowVolume.get_array();

		for (uint32 i = 0, size = shadowVolume.size(); i < size; i += 3)
		{
			vec_t n = 1.0f / sqrtf(array[i  ] * array[i  ] +
								   array[i+1] * array[i+1] +
								   array[i+2] * array[i+2]);

			array[i  ] *= n;  
			array[i+1] *= n;  
			array[i+2] *= n;  
		}
				
		// If requested project to Inf, but not beyond.
		if (cinf > 0.0f)
		{
			for (uint32 i = 0, size = shadowVolume.size(); i < size; ++i)
			{
				array[i] *= cinf;
			}
		}

		return true;
	}

	return false;
}


bool Mesh::GetBlendShadowVolume(mstl::Vector<vec_t> &shadowVolume,
								vec3_t lightPos, vec3_t pos, vec_t cinf)
{
	//TripleVec_Addition(shadowVolume, v);
	//TripleVec_Normalize(shadowVolume)
	//TripleVec_Mulitply(shadowVolume, inf);

	if ( CopyVertexBlendBuffer(shadowVolume) )
	{
		// Translate by pos, and get vector from lightPos.
		vec3_t v = {pos[0] - lightPos[0], 
					pos[1] - lightPos[1], 
					pos[2] - lightPos[2] };

		TripleVec_Addition(shadowVolume, v);

		vec_t *array = shadowVolume.get_array();

		for (uint32 i = 0, size = shadowVolume.size(); i < size; i += 3)
		{
			vec_t n = 1.0f / sqrtf(array[i  ] * array[i  ] +
								   array[i+1] * array[i+1] +
								   array[i+2] * array[i+2]);

			array[i  ] *= n;  
			array[i+1] *= n;  
			array[i+2] *= n;  
		}
				
		// If requested project to Inf, but not beyond.
		if (cinf > 0.0f)
		{
			for (uint32 i = 0, size = shadowVolume.size(); i < size; ++i)
			{
				array[i] *= cinf;
			}
		}

		return true;
	}

	return false;
}


void Mesh::GetSelectedVertices(Vector<index_t> &list) 
{
	list.clear();

	for (uint32 i = 0, icount = GetVertexCount(); i < icount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		if (v->mFlags & Vertex::fSelected)
		{
			list.push_back(i);
		}
	}	
}


bool Mesh::UnserializeXML(const char* filename)
{
#if TINYXML_FOUND
	if (!filename)
		return false;

	TiXmlDocument doc(filename);

	if ( !doc.LoadFile() )
	{
		freyjaPrintMessage("XML ERROR: %s, Line %i, Col %i\n", 
		             doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol() );
		return false;
	}

	TiXmlElement *root = doc.RootElement(); 

	if (!root) 
	{
		freyjaPrintMessage("XML ERROR: Couldn't find document root for '%s'!\n", filename );
		return false;
	}

	TiXmlElement *child = root->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		// Only handle the first one encountered given this is a class method.
		if ( s == "mesh" )
		{
			return Unserialize( child );
		}
	}

#endif // TINYXML_FOUND

	return false;
}


bool Mesh::SerializePool(SystemIO::TextFileWriter &w, const char *name,
						 Vector<vec_t> &array, mstl::stack<index_t> &stack)
{
	if (!array.size())
	{
		w.Print("\t%sStack %u\n", name, 0);
		w.Print("\t%sArray %u\n", name, 0);	
		return true;
	}

	{
		w.Print("\t%sStack %u\n", name, stack.size());
		mstl::StackNode<index_t> *cur = stack.top();

		// FIXME: This will cause it to read in reverse, but it doesn't matter 
		while (cur)
		{
			index_t data = cur->Data();
			w.Print("%u ", data);
			cur = cur->Prev();
		}

		w.Print("\n");
	}

	w.Print("\t%sArray %u\n", name, array.size());
	for (uint32 i = 0, n = array.size(); i < n; ++i)
	{
		if (i % 3 == 0) w.Print("\n\t"); // Makes it easier on text editors
		w.Print("%f ", array[i]);
	}
	w.Print("\n");

	return true;
}


bool Mesh::SerializePool(SystemIO::TextFileReader &r, const char *name,
						 Vector<vec_t> &array, mstl::stack<index_t> &stack)
{
	freyjaPrintMessage("Loading %s pool...\n", name);

	{
		r.ParseSymbol(); // name + "Stack"
		int32 count = r.ParseInteger();

		while (count > 0)
		{
			stack.push(r.ParseInteger());
			--count;
		}
	}

	r.ParseSymbol(); // name + "Array"
	int32 count = r.ParseInteger();

	while (count > 0)
	{
		array.push_back(r.ParseFloat());
		--count;
	}

	return true;
}


bool Mesh::Repack()
{
	if (mPacked)
		return false;


	/* Vertices. */
	
	/* Repack Vertices. */
	for (uint32 i = 0; i < mVertices.size(); ++i)
	{
		// Precondition 1: Found a 'gap' vertex.
		if (mVertices[i] == NULL)
		{
			for (uint32 j = i; j < mVertices.size(); ++j)
			{
				// Precondition 2: Found a valid pointer later.
				if (mVertices[j])
				{
					// 1. Swap 'gap' pointer with valid vertex.
					mVertices[i] = mVertices[j];
					mVertices[j] = NULL;
						
					// 2. Now do a vertex weld to update references.
					WeldVertices(j, i);

					Vertex *v = mVertices[i];

					// Only Vertex objects reference vertex buffer directly, so
					// we can 'tear it up and rebuild' here.
					{
						Vec3 pos;
						GetTripleVec(mVertexPool, v->mVertexIndex, pos.mVec);

						v->mVertexIndex = i;
						//( !mFreedVertices.empty() ) ? mFreedVertices.pop() : i;

						SetTripleVec(mVertexPool, v->mVertexIndex, pos.mVec);
					}

					{
						Vec3 pos;
						GetTripleVec(mNormalPool, v->mNormalIndex, pos.mVec);

						v->mNormalIndex = i;
						//( !mFreedVertices.empty() ) ? mFreedVertices.pop() : i;

						SetTripleVec(mNormalPool, v->mNormalIndex, pos.mVec);
					}

					break;
				}
			}		
		}
	}

	/* Purge 'free' state.
	 * Only Vertex objects reference vertex buffer directly.
	 */
	while ( !mFreedVertices.empty() )
	{
		mFreedVertices.pop();
	}

	while ( !mFreedNormals.empty() )
	{
		mFreedNormals.pop();
	}

	/* Reduce the reported size of the vertex object array. */
	for (uint32 i = 0; i < mVertices.size(); ++i)
	{
		if (mVertices[i] == NULL)
		{
			freyjaPrintMessage("Vert resize %i %i\n", mVertices.size(), i);
			mVertices.resize(i, NULL);

			// Only Vertex objects reference vertex buffer directly, so
			// we can 'tear it up and rebuild' here.
			mVertexPool.resize(i*3, 0.0f);
			mNormalPool.resize(i*3, 0.33f);
			break;
		}
	}
   


	/* Repack Faces. */

	for (uint32 i = 0; i < mFaces.size(); ++i)
	{
		if (mFaces[i] == NULL)
		{
			for (uint32 j = i; j < mFaces.size(); ++j)
			{
				if (mFaces[j])
				{
					mFaces[i] = mFaces[j];
					mFaces[j] = NULL;

					// Update vertex face refs... factor this out later.
					for (uint32 k = 0; k < mVertices.size(); ++k)
					{
						if (mVertices[k])
						{
							mstl::Vector<index_t> &refs = mVertices[k]->GetFaceRefs();

							uint32 r;
							foreach (refs, r)
							{
								if (refs[r] == j)
									refs[r] = i;
							}

						}
					}

					break;
				}
			}		
		}
	}

	/* Reduce the reported size of the vertex object array. */
	for (uint32 i = 0; i < mFaces.size(); ++i)
	{
		if (mFaces[i] == NULL)
		{
			freyjaPrintMessage("Face resize %i %i\n", mFaces.size(), i);
			mFaces.resize(i, NULL);
			break;
		}
	}

	// TODO: On repack we could move alpha pass faces
	//       to the end of face array to be a nice guy modeler.
	//       ( This makes it easier to render multipass. )
	

	/* Repack texcoord array here -- noting Faces and Vertices reference it. */
	if (0) // FIXME: need to split up texcoord array for polymapped/uv
	{
		index_t replace, texcoord;

		

		for (uint32 i = mFaces.begin(); i < mFaces.end(); ++i)
		{
			if (mFaces[i])
			{
				mFaces[i]->WeldTexCoords(replace, texcoord);
			}
		}

		{
			unsigned int i;
			foreach (mVertices, i)
			{
				if (mVertices[i])
				{
					mVertices[i]->WeldTexCoords(replace, texcoord);
				}
			}
		}

		vec3_t st;
		GetTripleVec(mTexCoordPool, texcoord, st);
		SetTripleVec(mTexCoordPool, replace, st);
	}


	/* Repack Weights. */
	{
		for (uint32 i = 0; i < mWeights.size(); ++i)
		{
			if (mWeights[i] == NULL)
			{
				for (uint32 j = i; j < mWeights.size(); ++j)
				{
					if (mWeights[j])
					{
						mWeights[i] = mWeights[j];
						mWeights[j] = NULL;
						break;
					}
				}		
			}
		}

		/* Reduce the reported size of the vertex object array. */
		for (uint32 i = 0; i < mWeights.size(); ++i)
		{
			if (mWeights[i] == NULL)
			{
				freyjaPrintMessage("Weight resize %i %i\n", mWeights.size(), i);
				mWeights.resize(i, NULL);
				break;
			}
		}
	}

#if 0
	// Might want to do a 'bugs' compatibilty for external array use.
	// Some projects use Vertex index as the same as buffer index.
	// This means we would copy the values in the:
	// normal/texcoord buffers here also.

	vec3_t v;

	{
		GetTripleVec(mVertexPool, mVertices[gap]->mVertexIndex, v);
		mVertices[gap]->mVertexIndex = 
		( !mFreedVertices.empty() ) ? mFreedVertices.pop() : gap;
		SetTripleVec(mVertexPool, mVertices[gap]->mVertexIndex, v);
	}

	GetTripleVec(mNormalPool, mVertices[gap]->mNormalIndex, v);
	mVertices[gap]->mNormalIndex = 
	( !mFreedNormals.empty() ) ? mFreedNormals.pop() : gap;
	SetTripleVec(mNormalPool, mVertices[gap]->mNormalIndex, v);
			
	GetTripleVec(mTexCoordPool, mVertices[gap]->mTexCoordIndex, v);
	mVertices[gap]->mTexCoordIndex = 
	( !mFreedTexCoords.empty() ) ? mFreedTexCoords.pop() : gap;
	SetTripleVec(mTexCoordPool, mVertices[gap]->mTexCoordIndex, v);

#endif

	//VertexAnimTrack mVertexAnimTrack; /* Mesh vertex animation track  */

	//Vector<vec_t> mBlendVertices;     /* Skeletal vertex blending use  */

	// Planes use pointer references to faces, so there is no need to update.
	//Vector<Plane *> mPlanes;

	// 
	//Vector<Edge *> mEdges;

	//mPacked = true; // FIXME: Once all other fuctions hook into this uncomment
	return true;
}


bool Mesh::WeldTexCoords(index_t replace, index_t texcoord)
{
	// Make all polygons referencing A point to B
	for (uint32 i = mFaces.begin(); i < mFaces.end(); ++i)
	{
		if (mFaces[i])
		{
			mFaces[i]->WeldTexCoords(replace, texcoord);
		}
	}

	{
		unsigned int i;
		foreach (mVertices, i)
		{
			if (mVertices[i])
			{
				mVertices[i]->WeldTexCoords(replace, texcoord);
			}
		}
	}

	vec3_t st = {0,0,0};
	GetTripleVec(mTexCoordPool, texcoord, st);
	SetTripleVec(mTexCoordPool, replace, st);

	// Mark unused index in the texcoord pool
	mFreedTexCoords.push(replace);

	return true;
}


#if TINYXML_FOUND
bool Mesh::SerializeBuffer(TiXmlElement *container, 
						   const char *name, Vector<vec_t> &array)
{
	// NOTE: You lose 'gaps' this way  =/

	if (!array.size())
	{
		// Nothing to write home about, literally...
		return true;
	}

	TiXmlElement *buffer = new TiXmlElement(name);
	buffer->SetAttribute("reserve", array.size() );

	for (uint32 i = 0, n = array.size()/3; i < n; ++i)
	{
		unsigned int idx = i * 3;

		TiXmlElement *element = new TiXmlElement("vec3");
		element->SetAttribute("id", idx/3 );
		element->SetDoubleAttribute("x", array[idx] );
		element->SetDoubleAttribute("y", array[idx+1] );
		element->SetDoubleAttribute("z", array[idx+2] );
		buffer->LinkEndChild(element);
	}

	container->LinkEndChild(buffer);
	return true;
}


bool Mesh::UnserializeBuffer(TiXmlElement *container, 
							 const char *name, Vector<vec_t> &array)
{
	// Have to read which gap type above this level

	if (!container)
		return false;

	int attr;
	container->QueryIntAttribute("reserve", &attr);

	if (attr < 0) attr = 16;

	// attr - this is currently the old array reserve size.
	array.reserve(attr);
	
	TiXmlElement *child = container->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();
		
		// FIXME: should check id's in future, in case of hand edited files.
		if (s == "vec3")
		{
			child->QueryIntAttribute("id", &attr);
			
			float x, y, z;
			x = y = z = 0.0f;
			child->QueryFloatAttribute("x", &x);
			child->QueryFloatAttribute("y", &y);
			child->QueryFloatAttribute("z", &z);

			//array[id*3] = x;
			//array[id*3+1] = y;
			//array[id*3+2] = z;
			
			array.push_back(x);
			array.push_back(y);
			array.push_back(z);
		}
	}

	return true;
}


bool Mesh::SerializeBufferGaps(TiXmlElement *container, 
							   const char *name, mstl::stack<index_t> &s)
{
	if (s.empty())
	{
		// Nothing to write home about, literally...
		return true;
	}

	mstl::stack<index_t> copy; 
	TiXmlElement *gaps = new TiXmlElement(name);

	while ( !s.empty() )
	{
		index_t item = s.pop();
		TiXmlElement *element = new TiXmlElement("gap");
		element->SetAttribute("id", item );	
		gaps->LinkEndChild(element);
		copy.push(item);
	}

	// We don't really care about order
	while ( !copy.empty() )
	{
		s.push(copy.pop());
	}

	container->LinkEndChild(gaps);
	return true;	
}


bool Mesh::UnserializeBufferGaps(TiXmlElement *container, 
								 const char *name, mstl::stack<index_t> &s)
{
	// Have to read which gap type above this level

	if (!container)
		return false;

	TiXmlElement *child = container->FirstChildElement();
	for(int attr; child; child = child->NextSiblingElement() )
	{
		child->QueryIntAttribute("id", &attr);
		s.push(attr);
	}

	return true;
}


bool Mesh::SerializeFaces(TiXmlElement *container)
{
	if (!container)
		return false;	

	TiXmlElement *faces = new TiXmlElement("faces");
	faces->SetAttribute("reserve", mFaces.size() );
	
	unsigned int i;
	foreach (mFaces, i)
	{
		if (mFaces[i])
			mFaces[i]->Serialize(faces);
	}

	container->LinkEndChild(faces);
	return true;
}


bool Mesh::UnserializeFaces(TiXmlElement *faces)
{
	if (!faces)
		return false;

	int reserve;
	faces->QueryIntAttribute("reserve", &reserve);
	//mFaces.reserve(reserve);

	TiXmlElement *child = faces->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		Face *f = new Face();
		f->Unserialize(child);
		mFaces.push_back(f);
	}

	return true;
}


bool Mesh::SerializeWeights(TiXmlElement *container)
{
	if (!container)
		return false;	

	TiXmlElement *weights = new TiXmlElement("weights");
	weights->SetAttribute("reserve", mWeights.size() );

	unsigned int i;
	foreach (mWeights, i)
	{
		if (mWeights[i])
			mWeights[i]->Serialize(weights);
	}

	container->LinkEndChild(weights);
	return true;
}


bool Mesh::UnserializeWeights(TiXmlElement *weights)
{
	if (!weights)
		return false;

	int reserve;
	weights->QueryIntAttribute("reserve", &reserve);
	//mWeights.reserve(reserve, NULL);

	TiXmlElement *child = weights->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		Weight *w = new Weight();
		w->Unserialize(child);
		mWeights.push_back(w);
	}

	return true;
}


bool Mesh::SerializeVertices(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *verts = new TiXmlElement("vertices");
	verts->SetAttribute("reserve", mFaces.size() );

	unsigned int i;
	foreach (mVertices, i)
	{
		if (mVertices[i])
			mVertices[i]->Serialize(verts);
	}

	container->LinkEndChild(verts);
	return true;
}


bool Mesh::UnserializeVertices(TiXmlElement *verts)
{
	if (!verts)
		return false;

	int reserve;
	verts->QueryIntAttribute("reserve", &reserve);
	//mVertices.reserve(reserve, NULL);

	TiXmlElement *child = verts->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		Vertex *v = new Vertex();
		v->Unserialize(child);
		mVertices.push_back(v);
	}

	return true;
}


bool Mesh::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *mesh = new TiXmlElement("mesh");
	container->LinkEndChild(mesh);

	mesh->SetAttribute("version", 3);
	mesh->SetAttribute("name", mName.c_str());
	mesh->SetAttribute("uid", mUID);
	mesh->SetAttribute("flags", mFlags);
	mesh->SetAttribute("material", mMaterialIndex);

	TiXmlElement *pos = new TiXmlElement("loc");
	pos->SetDoubleAttribute("x", mPosition.mX);
	pos->SetDoubleAttribute("y", mPosition.mY);
	pos->SetDoubleAttribute("z", mPosition.mZ);
	mesh->LinkEndChild(pos);

	TiXmlElement *rot = new TiXmlElement("rot");
	rot->SetDoubleAttribute("x", mRotation.mX);
	rot->SetDoubleAttribute("y", mRotation.mY);
	rot->SetDoubleAttribute("z", mRotation.mZ);
	mesh->LinkEndChild(rot);

	TiXmlElement *sz = new TiXmlElement("size");
	sz->SetDoubleAttribute("x", mScale.mX);
	sz->SetDoubleAttribute("y", mScale.mY);
	sz->SetDoubleAttribute("z", mScale.mZ);
	mesh->LinkEndChild(sz);

	TiXmlElement *bbox = new TiXmlElement("bounding_box");
	bbox->SetDoubleAttribute("min-x", mBoundingVolume.mBox.mMin[0]);
	bbox->SetDoubleAttribute("min-y", mBoundingVolume.mBox.mMin[1]);
	bbox->SetDoubleAttribute("min-z", mBoundingVolume.mBox.mMin[2]);
	bbox->SetDoubleAttribute("max-x", mBoundingVolume.mBox.mMax[0]);
	bbox->SetDoubleAttribute("max-y", mBoundingVolume.mBox.mMax[1]);
	bbox->SetDoubleAttribute("max-z", mBoundingVolume.mBox.mMax[2]);
	mesh->LinkEndChild(bbox);

	TiXmlElement *bs = new TiXmlElement("bounding_sphere");
	bs->SetDoubleAttribute("x", mBoundingVolume.mSphere.mCenter[0]);
	bs->SetDoubleAttribute("y", mBoundingVolume.mSphere.mCenter[1]);
	bs->SetDoubleAttribute("z", mBoundingVolume.mSphere.mCenter[2]);
	bs->SetDoubleAttribute("radius", mBoundingVolume.mSphere.mRadius);
	mesh->LinkEndChild(bs);

	/* Buffers */
	SerializeBuffer(mesh, "vertex_buffer", mVertexPool);
	SerializeBuffer(mesh, "texcoord_buffer", mTexCoordPool);
	SerializeBuffer(mesh, "normal_buffer", mNormalPool);

	/* 'Gaps' */
	SerializeBufferGaps(mesh, "vertex_gaps", mFreedVertices);
	SerializeBufferGaps(mesh, "texcoord_gaps", mFreedTexCoords);
	SerializeBufferGaps(mesh, "normal_gaps", mFreedNormals);

	/* Object lits */
	SerializeWeights(mesh);
	SerializeVertices(mesh);
	SerializeFaces(mesh);

	// NOTE: Mesh animations not serialized in XML <mesh>
	return true;
}


bool Mesh::Unserialize(TiXmlElement *mesh)
{
	if (!mesh)
		return false;

	int attr;
	mesh->QueryIntAttribute("version", &attr);

	if ( attr != 3 )
	{
		freyjaPrintError("Wrong mesh XML version");
		return false;
	}

	mName = mesh->Attribute("name");
	//mesh->QueryIntAttribute("uid", &mUID);

	mesh->QueryIntAttribute("flags", &attr);
	mFlags = attr;

	mesh->QueryIntAttribute("material", &attr);
	mMaterialIndex = ( attr < 0 ) ? INDEX_INVALID : attr;

	TiXmlElement *child = mesh->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		printf("! %s\n", s.c_str() );

		if (s == "loc")
		{
			mesh->QueryFloatAttribute("x", &mPosition.mX);
			mesh->QueryFloatAttribute("y", &mPosition.mY);
			mesh->QueryFloatAttribute("z", &mPosition.mZ);
		}
		else if (s == "rot")
		{
			mesh->QueryFloatAttribute("x", &mRotation.mX);
			mesh->QueryFloatAttribute("y", &mRotation.mY);
			mesh->QueryFloatAttribute("z", &mRotation.mZ);
		}
		else if (s == "size")
		{
			mesh->QueryFloatAttribute("x", &mScale.mX);
			mesh->QueryFloatAttribute("y", &mScale.mY);
			mesh->QueryFloatAttribute("z", &mScale.mZ);
		}
		else if (s == "bounding_box")
		{
			mesh->QueryFloatAttribute("min-x", &mBoundingVolume.mBox.mMin[0]);
			mesh->QueryFloatAttribute("min-y", &mBoundingVolume.mBox.mMin[1]);
			mesh->QueryFloatAttribute("min-z", &mBoundingVolume.mBox.mMin[2]);
			mesh->QueryFloatAttribute("max-x", &mBoundingVolume.mBox.mMax[0]);
			mesh->QueryFloatAttribute("max-y", &mBoundingVolume.mBox.mMax[1]);
			mesh->QueryFloatAttribute("max-z", &mBoundingVolume.mBox.mMax[2]);
		}
		else if (s == "bounding_sphere")
		{
			mesh->QueryFloatAttribute("x", &mBoundingVolume.mSphere.mCenter[0]);
			mesh->QueryFloatAttribute("y", &mBoundingVolume.mSphere.mCenter[1]);
			mesh->QueryFloatAttribute("z", &mBoundingVolume.mSphere.mCenter[2]);
			mesh->QueryFloatAttribute("radius", &mBoundingVolume.mSphere.mRadius);
		}
		else if (s == "vertex_buffer")
		{
			UnserializeBuffer(child, "vertex_buffer", mVertexPool);
		}
		else if (s == "texcoord_buffer")
		{
			UnserializeBuffer(child, "texcoord_buffer", mTexCoordPool);
		}
		else if (s == "normal_buffer")
		{
			UnserializeBuffer(child, "normal_buffer", mNormalPool);
		}
		else if (s == "vertex_gaps")
		{
			UnserializeBufferGaps(child, "vertex_gaps", mFreedVertices);
		}
		else if (s == "texcoord_gaps")
		{
			UnserializeBufferGaps(child, "texcoord_gaps", mFreedTexCoords);
		}
		else if (s == "normal_gaps")
		{
			UnserializeBufferGaps(child, "normal_gaps", mFreedNormals);
		}
		else if (s == "vertices")
		{
			UnserializeVertices(child);
		}
		else if (s == "weights")
		{
			UnserializeWeights(child);
		}
		else if (s == "faces")
		{
			UnserializeFaces(child);
		}
	}

	// NOTE: Mesh animations not serialized in XML <mesh>
	return true;
}

#endif


bool Mesh::Serialize(SystemIO::TextFileWriter &w)
{
	w.Print("Mesh\n");
	w.Print("\t mVersion 2\n");
	w.Print("\t mName \"%s\"\n", mName.c_str() );
	w.Print("\t mUID %u\n", mUID);
	w.Print("\t mFlags %u\n", mFlags);
	w.Print("\t mMaterial %u\n", mMaterialIndex);
	w.Print("\t mPosition %f %f %f\n", mPosition[0], mPosition[1], mPosition[2]);
	w.Print("\t mRotation %f %f %f\n", mRotation[0], mRotation[1], mRotation[2]);
	w.Print("\t mScale %f %f %f\n", mScale[0], mScale[1], mScale[2]);
	w.Print("\t mBoundingVolume.mBox %f %f %f %f %f %f\n", 
			mBoundingVolume.mBox.mMin[0],
			mBoundingVolume.mBox.mMin[1],
			mBoundingVolume.mBox.mMin[2],
			mBoundingVolume.mBox.mMax[0],
			mBoundingVolume.mBox.mMax[1],
			mBoundingVolume.mBox.mMax[2]);
	w.Print("\t mBoundingVolume.mSphere %f %f %f %f\n", 
			mBoundingVolume.mSphere.mRadius,
			mBoundingVolume.mSphere.mCenter[0], 
			mBoundingVolume.mSphere.mCenter[1], 
			mBoundingVolume.mSphere.mCenter[2]);

	SerializePool(w, "mVertex", mVertexPool, mFreedVertices);
	SerializePool(w, "mTexCoord", mTexCoordPool, mFreedTexCoords);
	SerializePool(w, "mNormal", mNormalPool, mFreedNormals);

	w.Print("\t mWeights %u\n", mWeights.size());
	for (uint32 i = 0, n = mWeights.size(); i < n; ++i)
	{
		Weight *ww = GetWeight(i);
		if (ww) ww->Serialize(w); else w.Print("\t\tNULL\n");
	}

	w.Print("\t mVertices %u\n", mVertices.size());
	for (uint32 i = 0, n = mVertices.size(); i < n; ++i)
	{
		Vertex *v = GetVertex(i);
		if (v) v->Serialize(w); else w.Print("\t\tNULL\n");
	}

	for (uint32 i = 0, n = mFaces.size(), count = 0; i < n; ++i)
	{
		if (GetFace(i)) ++count;
	}

	w.Print("\t mFaces %u\n", mFaces.size());
	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *f = GetFace(i);
		if (f) f->Serialize(w); else w.Print("\t\tNULL\n");
	}

	w.Print("\t mTracks %u\n", 1); // only have one in test =p
	mTrack.Serialize(w);

	w.Print("\t mVertexAnimTracks %u\n", 1); // only have one in test =p
	mVertexAnimTrack.Serialize(w);

	return true;
}


bool Mesh::Unserialize(SystemIO::TextFileReader &r)
{
	// Currently no parsing/checking done as this is mostly for debugging

	r.ParseSymbol(); // Mesh

	r.ParseSymbol(); // version
	uint32 version = r.ParseInteger();

	r.ParseSymbol(); // name
	const char *s = r.ParseStringLiteral();
	SetName(s);
	if (s) delete [] s;

	r.ParseSymbol(); // uid
	/*mUID =*/ r.ParseInteger();
	
	r.ParseSymbol(); // flags
	mFlags = r.ParseInteger();

	r.ParseSymbol(); // material
	mMaterialIndex = r.ParseInteger();

	r.ParseSymbol(); // loc 
	mPosition[0] = r.ParseFloat();
	mPosition[1] = r.ParseFloat();
	mPosition[2] = r.ParseFloat();

	r.ParseSymbol(); // rot 
	mRotation[0] = r.ParseFloat();
	mRotation[1] = r.ParseFloat();
	mRotation[2] = r.ParseFloat();

	r.ParseSymbol(); // siz
	mScale[0] = r.ParseFloat();
	mScale[1] = r.ParseFloat();
	mScale[2] = r.ParseFloat();

	r.ParseSymbol(); // bbox
	mBoundingVolume.mBox.mMin[0] = r.ParseFloat();
	mBoundingVolume.mBox.mMin[1] = r.ParseFloat();
	mBoundingVolume.mBox.mMin[2] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[0] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[1] = r.ParseFloat();
	mBoundingVolume.mBox.mMax[2] = r.ParseFloat();

	r.ParseSymbol(); // bsphere
	mBoundingVolume.mSphere.mRadius = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[0] = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[1] = r.ParseFloat();
	mBoundingVolume.mSphere.mCenter[2] = r.ParseFloat();

	SerializePool(r, "vertex", mVertexPool, mFreedVertices);
	SerializePool(r, "texcoord", mTexCoordPool, mFreedTexCoords);
	SerializePool(r, "normal", mNormalPool, mFreedNormals);

	freyjaPrintMessage("Loading weights...\n");
	r.ParseSymbol(); // Weight
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Weight *w = new Weight();
			if (w->Serialize(r))
			{
				mWeights.push_back(w);
			}
			else
			{
				delete w;
				mWeights.push_back(NULL);
			}
			--count;
		}
	}

	freyjaPrintMessage("Loading vertices...\n");
	r.ParseSymbol(); // Vertex
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Vertex *v = new Vertex();
			if (v->Serialize(r))
			{
				mVertices.push_back(v);
			}
			else
			{
				delete v;
				mVertices.push_back(NULL);
			}
			--count;
		}
	}

	freyjaPrintMessage("Loading faces...\n");
	r.ParseSymbol(); // Face
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			Face *f = new Face();
			if (f->Serialize(r))
			{
				mFaces.push_back(f);
			}
			else
			{
				delete f;
				mFaces.push_back(NULL);
			}
			--count;
		}
	}

	if (version == 1)
		return true;

	r.ParseSymbol(); // mTracks
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			// only have one in test =p
			mTrack.Serialize(r);
			--count;
		}
	}

	r.ParseSymbol(); // mVertexAnimTracks
	{
		int32 count = r.ParseInteger();
		while (count > 0)
		{
			// only have one in test =p
			mVertexAnimTrack.Serialize(r);
			--count;
		}
	}

	return true;
}


bool Mesh::Serialize(SystemIO::FileWriter &w)
{
	freyja_file_chunk_t chunk;

	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = 0;
	chunk.flags = 0x0;
	chunk.version = 10;

	// FIXME: Screw this -- 
	// just use file seek to come back and write the value =P
	/* Compute byte size here */
	chunk.size += 4;  // index_t mUID;
	chunk.size += 4;  // uint32 mFlags;
	chunk.size += 4;  // index_t mMaterialIndex;
	chunk.size += 12; // vec3_t mPosition;
	chunk.size += 12; // vec3_t mRotation;
	chunk.size += 12; // vec3_t mScale;
	chunk.size += 4 + mVertexPool.size() * 4;
	chunk.size += 4 + mFreedVertices.size() * 4;
	chunk.size += 4 + mNormalPool.size() * 4;
	chunk.size += 4 + mFreedNormals.size() * 4;
	chunk.size += 4 + mColorPool.size() * 4;
	chunk.size += 4 + mFreedColors.size() * 4;
	chunk.size += 4 + mTexCoordPool.size() * 4;
	chunk.size += 4 + mFreedTexCoords.size() * 4;
	chunk.size += 4 + mFaces.size() * 0;//Face::SerializedSize();
	chunk.size += 4 + mVertices.size() * Vertex::SerializedSize();
	chunk.size += 4 + mWeights.size() * Weight::SerializedSize();

	/* Write chunk header to diskfile */
	w.WriteLong(FREYJA_CHUNK_MESH);
	w.WriteLong(chunk.size);
	w.WriteLong(chunk.flags);
	w.WriteLong(chunk.version);

	/* Write chunk data to diskfile */
	w.WriteLong(mUID);
	w.WriteLong(mFlags);
	w.WriteLong(mMaterialIndex);
	w.WriteFloat32(mPosition.mVec[0]);
	w.WriteFloat32(mPosition.mVec[1]);
	w.WriteFloat32(mPosition.mVec[2]);
	w.WriteFloat32(mRotation.mVec[0]);
	w.WriteFloat32(mRotation.mVec[1]);
	w.WriteFloat32(mRotation.mVec[2]);
	w.WriteFloat32(mScale.mVec[0]);
	w.WriteFloat32(mScale.mVec[1]);
	w.WriteFloat32(mScale.mVec[2]);

	SerializePool(w, mVertexPool, mFreedVertices);
	SerializePool(w, mNormalPool, mFreedNormals);
	SerializePool(w, mColorPool, mFreedColors);
	SerializePool(w, mTexCoordPool, mFreedTexCoords);

	w.WriteLong(mFaces.size());
	for ( uint32 i = 0; i < mFaces.size(); ++i )
	{
		if ( mFaces[i] ) 
			mFaces[i]->Serialize(w);
	}

	w.WriteLong(mVertices.size());
	for ( uint32 i = 0; i < mVertices.size(); ++i )
	{
		if ( mVertices[i] ) 
			mVertices[i]->Serialize(w);
	}

	w.WriteLong(mWeights.size());
	for ( uint32 i = 0; i < mWeights.size(); ++i )
	{
		if ( mWeights[i] ) 
			mWeights[i]->Serialize(w);
	}

	return true;
}


void Mesh::GetVertexArrayPos(index_t vertexIndex, vec3_t xyz)
{	
	if (vertexIndex < mVertexPool.size())
		GetTripleVec(mVertexPool, vertexIndex, xyz);	
}


#if 0
void Mesh::GetVertexPos(index_t vertexIndex, vec3_t xyz)
{
	Vertex *vert = GetVertex(vertexIndex);

	if ( vert ) 
	{
		GetVertexArrayPos(vert->mVertexIndex, xyz);
	}
}
#endif


Vertex *Mesh::GetVertex(index_t vertexIndex)
{
	if (vertexIndex < mVertices.size())
		return mVertices[vertexIndex];

	return NULL;
}


hel::Vec3 Mesh::GetVertexPosition(index_t idx)
{
	Vec3 v;

	if (idx < mVertices.size())
	{
		Vertex *vert = mVertices[idx];

		if ( vert ) 
		{
			GetVertexArrayPos(vert->mVertexIndex, v.mVec);
		}
	}

	return v;
}


hel::Vec3 Mesh::GetVertexNormal(index_t idx)
{
	Vec3 v;
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		GetNormal(vert->mNormalIndex, v.mVec);
	}

	return v;
}


void Mesh::SetVertexNormal(index_t idx, hel::Vec3 n)
{
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		SetNormal(vert->mNormalIndex, n.mVec);
	}
}


hel::Vec3 Mesh::GetVertexTexCoord(index_t idx)
{
	Vec3 v;
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		GetTexCoord(vert->mTexCoordIndex, v.mVec);
	}

	return v;
}


void Mesh::SetVertexTexCoord(index_t idx, hel::Vec3 uv)
{
	Vertex *vert = mVertices[idx];
	if ( vert ) 
	{
		SetTexCoord(vert->mTexCoordIndex, uv.mVec);
	}
}


bool Mesh::IntersectPerFace(hel::Ray &r, vec_t &t)
{
	if (!Intersect(r, t))
		return false;

	vec_t bestDist = 99999.0f;
	r.Norm();
	int32 face0 = -1;
	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);
		
		if (!f) 
			continue;
	
		// Quick and dirty hit test that pretends you can 
		// pusedo tesselate a face and get as good results
		bool intersect = false;
					
		if (f->mIndices.size() > 2)
		{
			uint32 jCount = f->mIndices.size();
			Vec3 a, b, c, tuv;

			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);


			for (uint32 j = 2; j < jCount; ++j)
			{
				GetVertexPos(f->mIndices[j], c.mVec);
				intersect = r.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec);

				if (intersect)
				{
					break;
				}
			
				a = b;
				b = c;
			}

			if (intersect)
			{
				vec_t dist = tuv.mVec[0];

				if (face0 == -1 || dist < bestDist)
				{
					t = bestDist = dist;
					face0 = i;
				}
			}
		}
	}

	// Did we have a 'best hit'?
	return (face0 > -1);
}


bool Mesh::Intersect(hel::Ray &r, vec_t &t)
{
	if (GetFlags() & fHidden)
		return false;

	r.Norm();

	// Sphere test
	if (!r.IntersectSphere(mBoundingVolume.mSphere.mCenter, 
						   mBoundingVolume.mSphere.mRadius, t))
	{
		return false;
	}

	// Bbox test
#if 0
	vec_t bestDist = 99999.0f;
	vec3_t min, max;

	GetBBox(min, max);

	const uint32 count = 6;
	Vec3 v[count];

	v[0] = Vec3(max[0], max[1], max[2]);
	v[1] = Vec3(min[0], min[1], min[2]);
	v[2] = Vec3(max[0], min[1], max[2]);
	v[3] = Vec3(min[0], max[1], max[2]);
	v[4] = Vec3(max[0], max[1], min[2]);
	v[5] = Vec3(min[0], min[1], max[2]);
	v[6] = Vec3(min[0], max[1], min[2]);
	v[7] = Vec3(max[0], min[1], min[2]);

	bool intersect = false;
	Vec3 tuv;

	// Quick and dirty hit test that assumes you can pusedo tesselate 
	// a quad here and always get as good results...
	for (uint32 i = 0, a, b, c, d; i < count; ++i)
	{
		switch (i)
		{
		case 0:    
			a = 1, b = 6, c = 4, d = 7;
			break;

		case 1:
			a = 6, b = 3, c = 0, d = 4;
			break;

		case 2:
			a = 1, b = 5, c = 2, d = 7;
			break;

		case 3:
			a = 1, b = 6, c = 3, d = 5;
			break;

		case 4:
			a = 4, b = 0, c = 2, d = 7;
			break;

		case 5:
			a = 3, b = 0, c = 2, d = 5;
			break;
		}

		if (r.IntersectTriangle(v[a].mVec, v[b].mVec, v[c].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mVec[0] < bestDist)
			{
				t = bestDist = tuv.mVec[0];
			}
		}
		else if (r.IntersectTriangle(v[c].mVec, v[d].mVec, v[a].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mVec[0] < bestDist)
			{
				t = bestDist = tuv.mVec[0];
			}
		}
	}
#else
	Vec3 min, max;
	GetBBox(min.mVec, max.mVec);
	bool intersect = IntersectHitBox(r, min, max, t);
#endif

	return intersect;
}


bool Mesh::IntersectUVFaces(hel::Ray &r, int &face0, bool markAll,
							index_t material)
{
	vec_t bestDist = 99999.0f;
	r.Norm();
	face0 = -1;

	for (uint32 i = 0, n = GetFaceCount(); i < n; ++i)
	{
		Face *f = GetFace(i);
			
		if (!f || f->mMaterial != material) 
			continue;

		mstl::Vector<index_t> &indices = 
		((f->mFlags & Face::fPolyMappedTexCoords) ? 
		 f->mTexCoordIndices : f->mIndices);

		Vec3 a, b, c, tuv;
		
		GetTexCoord(indices[0], a.mVec);
		GetTexCoord(indices[1], b.mVec);

		bool intersect = false;

		for (uint32 j = 2, jn = indices.size(); j < jn; ++j)
		{
			GetTexCoord(indices[j], c.mVec);
			intersect = r.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec);

			if (intersect)
			{
				break;
			}
			
			b = c;
		}

		// Clear old hit flags
		f->mFlags &= ~Face::fRayHit;

		if (intersect)
		{
			const vec_t dist = tuv.mX;
			
			if (face0 == -1 || dist < bestDist)
			{
				bestDist = dist;
				face0 = i;
			}
			
			// Only mark hit flags for every face hit when: markAll = true
			if (markAll) f->mFlags |= Face::fRayHit;
		}
	}

	return (face0 > -1);
}


bool Mesh::IntersectHitBox(hel::Ray &r, 
						   const hel::Vec3 &min, const hel::Vec3 &max, vec_t &t)
{
	Vec3 v[8];
	v[0] = max;
	v[1] = min;
	v[2] = Vec3(max.mX, min.mY, max.mZ);
	v[3] = Vec3(min.mX, max.mY, max.mZ);
	v[4] = Vec3(max.mX, max.mY, min.mZ);
	v[5] = Vec3(min.mX, min.mY, max.mZ);
	v[6] = Vec3(min.mX, max.mY, min.mZ);
	v[7] = Vec3(max.mX, min.mY, min.mZ);

	vec_t bestDist = 99999.0f;
	bool intersect = false;
	Vec3 tuv;

	r.Norm();

	// Quick and dirty hit test that assumes you can pusedo tesselate 
	// a quad here and always get as good results...
	for (uint32 i = 0, a, b, c, d, count = 6; i < count; ++i)
	{
		switch (i)
		{
		case 0:    
			a = 1, b = 6, c = 4, d = 7;
			break;

		case 1:
			a = 6, b = 3, c = 0, d = 4;
			break;

		case 2:
			a = 1, b = 5, c = 2, d = 7;
			break;

		case 3:
			a = 1, b = 6, c = 3, d = 5;
			break;

		case 4:
			a = 4, b = 0, c = 2, d = 7;
			break;

		case 5:
			a = 3, b = 0, c = 2, d = 5;
			break;
		}

		if (r.IntersectTriangle(v[a].mVec, v[b].mVec, v[c].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mX < bestDist)
			{
				t = bestDist = tuv.mX;
			}
		}
		else if (r.IntersectTriangle(v[c].mVec, v[d].mVec, v[a].mVec, tuv.mVec))
		{
			intersect = true;
			
			if (tuv.mX < bestDist)
			{
				t = bestDist = tuv.mX;
			}
		}
	}

	return intersect;
}


int Mesh::PickEdge(hel::Ray &r, vec_t &t)
{
	vec_t bestDist = 99999.0f;
	r.Norm();
	int best = -1;
	const Vec3 n(0.2f, 0.2f, 0.2f); // FIXME: Allow external mutator for size here

	for (uint32 i = 0, iCount = mEdges.size(); i < iCount; ++i)
	{
		Edge *e = mEdges[i];
		
		if (!e) 
			continue;
	
		Vec3 a = GetVertexPosition(e->mA) + n;
		Vec3 b = GetVertexPosition(e->mB) - n;

		if ( IntersectHitBox(r, a, b, t) && t < bestDist )
		{
			best = i;
			bestDist = t;
		}
	}

	return best;
}


int Mesh::PickFace(Face::Flags flag, hel::Ray &r, hel::Vec3 &tuv)
{
	vec_t bestDist = 99999.0f;
	r.Norm();
	int face0 = -1;
	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);
		
		if (!f || f->mFlags & flag) 
			continue;
	
		// Quick and dirty hit test that pretends you can 
		// pusedo tesselate a face and get as good results
		bool intersect = false;
					
		if (f->mIndices.size() > 2)
		{
			uint32 jCount = f->mIndices.size();
			Vec3 a, b, c, tmp;

			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);

			for (uint32 j = 2; j < jCount; ++j)
			{
				GetVertexPos(f->mIndices[j], c.mVec);
				intersect = 
				r.IntersectTriangle(a.mVec, b.mVec, c.mVec, tmp.mVec);

				if (intersect)
				{
					break;
				}

				b = c;
			}

			if (intersect && (tmp.mX < bestDist) )
			{
				tuv = tmp;
				bestDist = tmp.mX;
				face0 = i;
			}
		}
	}

	return face0;
}


bool Mesh::IntersectFaces(hel::Ray &r, int &face0, bool markAll)
{
	//no 'editor side' scale support vec_t t; if (!Intersect(r, t)) return false;

	vec_t bestDist = 99999.0f;
	r.Norm();
	face0 = -1;
	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);
		
		if (!f) 
			continue;
	
		// Quick and dirty hit test that pretends you can 
		// pusedo tesselate a face and get as good results
		bool intersect = false;
					
		if (f->mIndices.size() > 2)
		{
			uint32 jCount = f->mIndices.size();
			Vec3 a, b, c, tuv;

			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);

			for (uint32 j = 2; j < jCount; ++j)
			{
				GetVertexPos(f->mIndices[j], c.mVec);
				intersect = r.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec);

				if (intersect)
				{
					break;
				}

				//a = b;
				b = c;
			}

			// Clear old hit flags
			f->mFlags &= ~Face::fRayHit;

			if (intersect)
			{
				vec_t dist = tuv.mVec[0];

				if (face0 == -1 || dist < bestDist)
				{
					freyjaPrintMessage("--- t = %f, uv = %f, %f", tuv.mVec[0],tuv.mVec[1],tuv.mVec[2]);
					bestDist = dist;
					face0 = i;
				}

				// Only mark hit flags for every face hit with markAll==true
				if (markAll) f->mFlags |= Face::fRayHit;
			}
		}
	}

	// Did we have a 'best hit'?
	if (face0 > -1)
	{
		Face *f = GetFace(face0);
		
		if (f) 
		{
			f->mFlags |= Face::fRayHit;
		}
	}

	return (face0 > -1);
}


void Mesh::SelectFacesByBox(hel::Vec3 &min, hel::Vec3 &max)
{
	Vec3 mina(min), maxa(max);

	// Reorder as needed, since we don't trust input
	for (int32 i = 2; i >= 0; --i)
	{
		if (min[i] > max[i])
		{
			mina[i] = max[i];
			maxa[i] = min[i];
		}
	}

	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		Vec3 p;
		bool inside = true;
		v->mFlags &= ~Vertex::fSelected2;
		GetVertexArrayPos(v->mVertexIndex, p.mVec);
		
		for (int32 j = 2; inside && j >= 0; --j)
		{
			if (p[j] > maxa[j] || p[j] < mina[j])
			{
				inside = false;
			}
		}

		if (inside)
		{
			v->mFlags |= Vertex::fSelected2;
		}
	}

	
	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);

		if (f)
		{
			bool select = true;
			uint32 j;
			foreach (f->mIndices, j)
			{
				Vertex *v = GetVertex(f->mIndices[j]);
				if (!(v->mFlags & Vertex::fSelected2))
				{
					select = false;
				}
			}

			if (select)
				f->mFlags |= Face::fSelected;
		}
	}
}


void Mesh::SelectVerticesByBox(hel::Vec3 &min, hel::Vec3 &max)
{
	Vec3 mina(min), maxa(max);

	// Reorder as needed, since we don't trust input
	for (int32 i = 2; i > -1; --i)
	{
		if (min[i] > max[i])
		{
			mina[i] = max[i];
			maxa[i] = min[i];
		}
	}

	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		Vec3 p;
		bool inside = true;
		GetVertexArrayPos(v->mVertexIndex, p.mVec);
		
		for (int32 j = 2; inside && j >= 0; --j)
		{
			if (p[j] > maxa[j] || p[j] < mina[j])
			{
				inside = false;
			}
		}

		if (inside)
		{
			v->mFlags |= Vertex::fSelected;
		}
	}	
}


bool Mesh::IntersectClosestVertex(hel::Ray &r, int &vertex0, vec_t radius)
{
	// No 'editor side' scale support if (!Intersect(r, t)) return false;
	vec_t bestDist = 99999.0f;
	r.Norm();
	vertex0 = -1;
	
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		// Clear old ray hit results
		v->mFlags &= ~Vertex::fRayHit;

		Vec3 u;
		GetVertexArrayPos(v->mVertexIndex, u.mVec);

		vec_t t;
		if (r.IntersectSphere(u.mVec, radius, t))
		{
			if (vertex0 == -1 || t < bestDist)// && t > 0)
			{
				bestDist = t;
				vertex0 = i;
			}
		}
	}

	// Did we have a 'best hit'?
	if (vertex0 > -1)
	{
		Vertex *v = GetVertex(vertex0);
		
		if (v) 
		{
			v->mFlags |= Vertex::fRayHit;
		}
	}

	return (vertex0 > -1);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Mesh::ApplyLoopSubDiv()
{
	// FIXME: Clean Up And Optimize!
	// Use the vertices' tempRefs to store shared edges in UpdateEdgeGraph();
	// for much more speed by avoiding search loops.


	// Forced in 0.9.5, since it didn't start with edge support ( not ensured )
	UpdateEdgeGraph();

	Vector<index_t> vertices; // 'edge' vertices
	Vector<index_t> centroids; // 'face' vertices
	Vector<index_t> purge;

	uint32 i;

	// 1. Compute centroid for each face. ( vertex in the 'middle' of the facet )
	centroids.reserve( mFaces.size() );
	purge.reserve(mFaces.size()+1);
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f)
		{
			purge.push_back(i);

			Vec3 centroid, uv;

			uint32 j;
			foreach (f->mIndices, j)
			{
				uv += GetVertexTexCoord(f->mIndices[j]);
				centroid += GetVertexPosition(f->mIndices[j]);
			}

			// Set it's 'pretty' vertex ( normals, uvs, etc are 'correct' )
			uv /= f->mIndices.size();
			centroid /= f->mIndices.size();
			index_t vertex = CreateVertex(centroid);
			SetVertexNormal(vertex, f->mNormal);
			SetVertexTexCoord(vertex, uv);

			centroids.push_back(vertex);
		}		
	}

	// 2. Compute midpoint vertices for each edge.
	vertices.reserve( mVertices.size() / 2 );
	foreach (mEdges, i)
	{
		Edge *e = mEdges[i];

		if (!e)
			continue;

		// Take a lot of midpoints to generate pretty vertices.
		Vec3 a = GetVertexPosition(e->mA);
		Vec3 b = GetVertexPosition(e->mB);
		index_t vertex = CreateVertex( ( a + b ) * 0.5f );

		{
			Vec3 n0 = GetVertexNormal(e->mA);
			Vec3 n1 = GetVertexNormal(e->mB);
			SetVertexNormal(vertex, (n0 + n1) * 0.5f);
		}

		{
			Vec3 uv0 = GetVertexTexCoord(e->mA);
			Vec3 uv1 = GetVertexTexCoord(e->mB);
			SetVertexTexCoord(vertex, (uv0 + uv1) * 0.5f);
		}

		vertices.push_back( vertex );
	}

	// 3. Make new facets with these vertices on a per face pass.
	foreach (mEdges, i)
	{
		Edge *e = mEdges[i];

		if (!e)
			continue;

		// 1. Get our shared midpoint for this edge. 
		index_t mid = vertices[i];

		// 2. Generate new facets for each facet using this edge.
		uint32 j;
		foreach (e->mFaceRefs, j)
		{
			Face *f = mFaces[ e->mFaceRefs[j] ];
			if (!f) continue;

			index_t centroid = centroids[ e->mFaceRefs[j] ];
			
			// 3. We need to *search for the neighbouring edges given no 
			//    connected graph in this API version.  =(

			uint32 k, count = 0;
			foreach (mEdges, k)
			{
				Edge *e2 = mEdges[k];
				if (!e2 || e == e2) continue;
		
				bool found = false;
				uint32 l;
				foreach (e2->mFaceRefs, l)
				{
					if (e2->mFaceRefs[l] == e->mFaceRefs[j])
					{
						found = true;
						break;
					}
				}

				if (!found)
					continue;
				
				if (e->mA == e2->mA || e->mA == e2->mB)
				{
					// centroid, mid, e->mA, e2's mid
					index_t face = CreateFace();
					Face *f2 = GetFace(face);

					if (f2)
					{
						// FIXME: Handle material, etc
						f2->mMaterial = f->mMaterial;
						f2->mNormal = f->mNormal;
#if 0
						f2->AppendVertex(centroid);
						f2->AppendVertex(mid);
						f2->AppendVertex(e->mA);
						f2->AppendVertex( vertices[k] );
#else
						AppendVertexToFace(face, centroid);
						AppendVertexToFace(face, mid);
						AppendVertexToFace(face, e->mA);
						AppendVertexToFace(face, vertices[k]);
#endif
					}

					++count;
				}
				else if (e->mB == e2->mA || e->mB == e2->mB)
				{
					// centroid, mid, e->mB, e2's mid
					index_t face = CreateFace();
					Face *f2 = GetFace(face);

					if (f2)
					{
						// FIXME: Handle material, etc
#if 0
						f2->AppendVertex(centroid);
						f2->AppendVertex(mid);
						f2->AppendVertex(e->mB);
						f2->AppendVertex( vertices[k] );
#else
						AppendVertexToFace(face, centroid);
						AppendVertexToFace(face, mid);
						AppendVertexToFace(face, e->mB);
						AppendVertexToFace(face, vertices[k]);
#endif
					}

					++count;
				}

				// Up to 2 correct faces can be generated per pass per
				// edge this way... aka the hard way.

				//if (count == 2)
				//	break;
			}
		}
	}

	// 4. Purge old faces.
	foreach(purge, i)
	{
		DeleteFace( purge[i] );
	}

	// 5. This is needed for alignment if called again.
	//Repack();
}


void Mesh::ApplyTrianglarTesselation()
{
	// This isn't needed to avoid the issues related to 'gaps' in mFaces[].
	// However it ensures the mesh is 'squeaky clean', which should see
	// the new faces appended to the end of the array at count offset.
	//Repack();

	for (uint32 i = 0, polygonCount = mFaces.size(); i < polygonCount; ++i)
	{
		Face* face = mFaces[i];

		// This shouldn't happen with a repacked mesh in between valid faces.
		// If this wasn't repacked a tesselated face would be skipped anyway.
		if ( !face )
			continue;

		// For the sake of compatibility only support quads for now
		switch ( face->mIndices.size() )
		{
		case 0: // Unallocated or empty
		case 1: // Point?
		case 2: // Edge?
		case 3: // Already a triangle
			break;

		case 4: // Quad
			{
				Face* face2 = GetFace( CreateFace() );
				FREYJA_ASSERTMSG( face2, "Face allocation failed!" );

#warning "FIXME: Fix up vertex references and neighbours."

				if ( face2 )
				{
					face2->mFlags = face->mFlags;                      
					face2->mSmoothingGroup = face->mSmoothingGroup;
					face2->mColor = face->mColor;                              
					face2->mMaterial = face->mMaterial;                         
					face2->mNormal = face->mNormal;
				
					// Split: abcd -> abc, cda ( keep winding )
					{
						index_t a = face->mIndices[0];
						index_t b = face->mIndices[1];
						index_t c = face->mIndices[2];
						index_t d = face->mIndices[3];

						face->mIndices.clear();
						face->mIndices.push_back(a);
						face->mIndices.push_back(b);
						face->mIndices.push_back(c);

						face2->mIndices.push_back(c);
						face2->mIndices.push_back(d);
						face2->mIndices.push_back(a);
					}

					if ( face->mTexCoordIndices.size() )
					{
						index_t a = face->mTexCoordIndices[0];
						index_t b = face->mTexCoordIndices[1];
						index_t c = face->mTexCoordIndices[2];
						index_t d = face->mTexCoordIndices[3];

						face->mTexCoordIndices.clear();
						face->mTexCoordIndices.push_back(a);
						face->mTexCoordIndices.push_back(b);
						face->mTexCoordIndices.push_back(c);

						face2->mTexCoordIndices.push_back(c);
						face2->mTexCoordIndices.push_back(d);
						face2->mTexCoordIndices.push_back(a);
					}

					if ( face->mNormalsIndices.size() )
					{
						index_t a = face->mNormalsIndices[0];
						index_t b = face->mNormalsIndices[1];
						index_t c = face->mNormalsIndices[2];
						index_t d = face->mNormalsIndices[3];

						face->mNormalsIndices.clear();
						face->mNormalsIndices.push_back(a);
						face->mNormalsIndices.push_back(b);
						face->mNormalsIndices.push_back(c);

						face2->mNormalsIndices.push_back(c);
						face2->mNormalsIndices.push_back(d);
						face2->mNormalsIndices.push_back(a);
					}
				}
			}
			break;

		default:
			;
#warning "FIXME: Didn't add a case for general polygons, which should tesselate on import anyway."
		}
	}
}


Mesh* Mesh::CopyWithBlendedVertices()
{
	if ( mBlendVertices.size() )
	{
		Mesh* mesh = new Mesh(*this);
		mesh->mVertexPool = mBlendVertices;
		mesh->UpdateBoundingVolume();
		return mesh;
	}

	return NULL;
}


index_t Mesh::CreateVertexKeyframeFromBlended(index_t track, vec_t time)
{
	if ( mBlendVertices.size() )
	{
		VertexAnimTrack& t = GetVertexAnimTrack(track);
		index_t key = t.NewKeyframe(time);
		VertexAnimKeyFrame* k = t.GetKeyframe(key);

		if (k)
		{
			// Resize in vertex count, copy in float count.
			k->ArrayResize( GetVertexCount() ); //mBlendVertices.size() );
			vec_t* array = k->GetVertexArray();
			memcpy( array, mBlendVertices.get_array(), mBlendVertices.size()*4 );

			return key;
		}
	}

	return INDEX_INVALID;
}


index_t Mesh::CreateVertexKeyframeFromImport(index_t track, vec_t time, Vector<vec_t>& vertices)
{
	if ( vertices.size() )
	{
		VertexAnimTrack& t = GetVertexAnimTrack(track);
		index_t key = t.NewKeyframe(time);
		VertexAnimKeyFrame* k = t.GetKeyframe(key);

		if (k)
		{
			// Resize in vertex count, copy in float count.
			k->ArrayResize( GetVertexCount() ); //vertices.size() );
			vec_t* array = k->GetVertexArray();
			memcpy( array, vertices.get_array(), vertices.size()*4 );

			return key;
		}
	}

	return INDEX_INVALID;
}


index_t Mesh::CreateVertex(const vec3_t xyz, const vec3_t uvw, const vec3_t nxyz)
{
	Vertex **array = mVertices.get_array();
	index_t vertex = AddTripleVec(mVertexPool, mFreedVertices, (vec_t*)xyz);
	index_t texcoord = AddTripleVec(mTexCoordPool, mFreedTexCoords, (vec_t*)uvw);
	index_t normal = AddTripleVec(mNormalPool, mFreedNormals, (vec_t*)nxyz);
	Vertex *vert = new Vertex(vertex, texcoord, normal);
	
	for ( uint32 i = 0, count = mVertices.size(); i < count; ++i )
	{
		if (array[i] == NULL)
		{
			array[i] = vert;
			return i;
		}
	}
	
	// Adjust bounding volume
	if (!mInitBoundingVol)
	{
		mInitBoundingVol = true;
		SetBBox(xyz, xyz);
	}
	else
	{
		vec3_t min;
		vec3_t max;
		bool update = false;

		GetBBox(min, max);
		
		for (uint32 i = 0; i < 3; ++i)
		{
			if (xyz[i] < min[i])
			{
				min[i] = xyz[i];
				update = true;
			}
			else if (xyz[i] > max[i])
			{
				max[i] = xyz[i];
				update = true;
			}
		}

		if (update)
		{
			SetBBox(min, max);
		}
	}

	mVertices.push_back(vert);
	return mVertices.size() - 1;
}


void Mesh::DeleteVertexHelper(Vertex **array, index_t vertex)
{
	Vertex *vert = array[vertex]; 

	// You have to remove vertex from the list to check for OTHER deps...
	array[vertex] = NULL;
	
#if 0
	// Do ref checks for faces that share indices
	if (!CheckVertexArrayRefs(vert->mVertexIndex))
		mFreedVertices.push(vert->mVertexIndex);
	
	if (!CheckTexCoordArrayRefs(vert->mTexCoordIndex))
		mFreedTexCoords.push(vert->mTexCoordIndex);
	
	if (!CheckNormalArrayRefs(vert->mNormalIndex))
		mFreedNormals.push(vert->mNormalIndex);
#elif 1
	bool v, n, t;
	CheckArrayRefs(vert->mVertexIndex, v, 
				   vert->mNormalIndex, n, 
				   vert->mTexCoordIndex, t);

	if (!v)
		mFreedVertices.push(vert->mVertexIndex);
	
	if (!t)
		mFreedTexCoords.push(vert->mTexCoordIndex);
	
	if (!n)
		mFreedNormals.push(vert->mNormalIndex);
#endif

	// Now you can finally delete the vertex
	delete vert;
}


void Mesh::VertexCleanup()
{
	RebuildVertexPolygonReferences();
	Vertex **array = mVertices.get_array();
	Vertex *vert;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
	{
		vert = array[v];

		if ( vert && vert->GetFaceRefs().size() == 0)
		{
			DeleteVertexHelper(array, v);
		}
	}
}


bool Mesh::DeleteVertex(index_t vertex)
{
	bool result = false;

	// We don't trust anyone calling this method...
	RebuildVertexPolygonReferences(vertex);

	Vertex **array = mVertices.get_array();
	Vertex *v = GetVertex(vertex);

	if ( v && v->GetFaceRefs().size() == 0)
	{
		DeleteVertexHelper(array, vertex);
		result = true;
	}

	return result;
}


void Mesh::MeldVertices(index_t a, index_t b)
{
	Vertex *va = GetVertex(a);
	Vertex *vb = GetVertex(b);

	if (va && vb)
		va->Meld(*vb);
}


bool Mesh::WeldVertices(index_t replace, index_t vertex)
{
	Vertex *vertReplace = GetVertex(replace);
	Vertex *vertNew = GetVertex(vertex);

	if (!vertNew)
		return false;

	// Make all objects referencing REPLACE point to VERTEX
	{
		unsigned int i;
		foreach (mWeights, i)
		{
			if (mWeights[i])
				mWeights[i]->WeldVertices(replace, vertex);
		}
	}

	{
		unsigned int i;
		foreach (mEdges, i)
		{
			if (mEdges[i])
				mEdges[i]->WeldVertices(replace, vertex);
		}
	}


	{
		unsigned int i;
		foreach (mFaces, i)
		{
			if (mFaces[i])
				mFaces[i]->WeldVertices(replace, vertex);
		}
	}

	if (vertReplace)
	{
		// Delete REPLACE from the vertex pool
		vertReplace->GetFaceRefs().clear();
		return DeleteVertex(replace);
	}

	// We now allow replacing 'NULL' vertices
	return true;
}


void Mesh::WeldVerticesByDistance(vec_t tolerance)
{
	for (uint32 i = 0, iCount = GetVertexCount(), jCount = GetVertexCount();
		 i < iCount; ++i)
	{
		for (uint32 j = 0; j < jCount; ++j)
		{
			vec_t dist = helDist3v(GetVertexPosition(i).mVec, 
								   GetVertexPosition(j).mVec);
			
			if (dist < tolerance)
				WeldVertices(i, j);
		}
	}
}


void Mesh::CheckArrayRefs(index_t vertex, bool &v, 
						  index_t normal, bool &n,
						  index_t texcoord, bool &t)
{
	v = n = t = false;

	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *vert = GetVertex(i);

		if (vert)
		{
			if (vert->mNormalIndex == normal)
				n = true;
			
			if (vert->mVertexIndex == vertex)
				v = true;

			if (vert->mTexCoordIndex == texcoord)
				t = true;

			if (n && t && v)
				break;
		}
	}	
}


bool Mesh::CheckNormalArrayRefs(index_t arrayIndex)
{
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (v && v->mNormalIndex == arrayIndex)
			return true;
	}

	return false;
}


bool Mesh::CheckTexCoordArrayRefs(index_t arrayIndex)
{
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (v && v->mTexCoordIndex == arrayIndex)
			return true;
	}

	for (uint32 i = 0, iCount = GetFaceCount(); i < iCount; ++i)
	{
		Face *f = GetFace(i);

		if (f && f->mFlags & Face::fPolyMappedTexCoords)
		{
			uint32 j;
			foreach (f->mIndices, j)
			{
				if (f->mIndices[j] == arrayIndex)
					return true;
			}
		}
	}

	return false;
}


bool Mesh::CheckVertexArrayRefs(index_t arrayIndex)
{
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (v && v->mVertexIndex == arrayIndex)
			return true;
	}

	return false;
}


void Mesh::Merge(Mesh *mesh)
{
	if (mesh == NULL)
		return;

	// FIXME: Merge doesn't merge animations...
	//TransformTrack mTrack;
	//VertexAnimTrack mVertexAnimTrack;

	Vector<index_t> transV; // need to translate ids to 'this' mesh ids

	uint32 i;

	foreach (mesh->mVertices, i)
	{
		if (mesh->mVertices[i])
		{
			Vec3 n = mesh->GetVertexNormal(i);
			Vec3 p = mesh->GetVertexPosition(i);
			Vec3 t = mesh->GetVertexTexCoord(i);

			// pos, uvw, normal order
			index_t idx = CreateVertex(p.mVec, t.mVec, n.mVec);

			// transV[i] <= idx
			transV.push_back(idx);

			Vertex *a =	mesh->GetVertex(i);
			Vertex *b = GetVertex(idx);
			b->mFlags = a->mFlags;
		}
		else  // Need to NULL pad to exact match if needed
		{
			transV.push_back(INDEX_INVALID);
		}
	}


	foreach (mesh->mFaces, i)
	{
		Face *face = mesh->GetFace(i);
		if (face)
		{
			Face *newFace = GetFace(CreateFace());

			newFace->mFlags = face->mFlags;
			newFace->mColor = face->mColor;
			newFace->mMaterial = face->mMaterial;
			newFace->mSmoothingGroup = face->mSmoothingGroup;

			{
				uint32 j;
				foreach (face->mIndices, j)
				{
					newFace->AppendVertex(transV[face->mIndices[j]]);
				}
			}

#if 0 // atm only vertex normals allowed
			if (face->mFlags & Face::fPolyMappedNormals)
			{
				uint32 j;
				foreach (face->mNormalIndices, j)
				{
					vec3_t uvw;
					index_t idx = face->mNormalIndices[j];
					mesh->GetNormal(idx, uvw);
					newFace->AppendNormal(CreateNormal(uvw));
				}
			}
#endif

			if (face->mFlags & Face::fPolyMappedTexCoords)
			{
				uint32 j;
				foreach (face->mTexCoordIndices, j)
				{
					Vec3 uvw;
					index_t idx = face->mTexCoordIndices[j];
					mesh->GetTexCoord(idx, uvw.mVec);
					newFace->AppendTexCoord(CreateTexCoord(uvw.mVec));
				}
			}
		}
	}

	foreach (mesh->mWeights, i)
	{
		Weight *w = mesh->mWeights[i];

		if (w)
		{
			// vertex, weight, bone order
			AddWeight(transV[w->mVertexIndex], w->mWeight, w->mBoneIndex);
		}
	}

	UpdateBoundingVolume();
}


void Mesh::DeleteFace(index_t face)
{
	Face **array = mFaces.get_array();
	Face *f = GetFace(face);

	if ( f )
	{
		array[face] = NULL; // this has to marked NULL before cleanup is called
		FaceRemovalCleanup(f);
		delete f;
	}
}


void Mesh::DeleteSelectedFaces()
{
	Face **array = mFaces.get_array();

	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *face = array[i];

		if (face && face->mFlags & Face::fSelected)
		{
			array[i] = NULL;
			//FaceRemovalCleanup(face);
			delete face;
		}
	}

	VertexCleanup();
	UpdateBoundingVolume();
}


void Mesh::DeleteUnSelectedFaces()
{
	Face **array = mFaces.get_array();

	for (uint32 i = 0, n = mFaces.size(); i < n; ++i)
	{
		Face *face = array[i];

		if (face && !(face->mFlags & Face::fSelected))
		{
			array[i] = NULL;
			//FaceRemovalCleanup(face);
			delete face;
		}
	}

	VertexCleanup();
	UpdateBoundingVolume();
}


Mesh *Mesh::Split(bool trim)
{
	Mesh *mesh = new Mesh(*this);
	mesh->DeleteUnSelectedFaces();

	if (trim)
	{
		DeleteSelectedFaces();
	}

	return mesh;
}


void Mesh::CollapseEdge(index_t faceIndex, 
						uint32 a, uint32 b, uint32 c, uint32 d)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	for (uint32 i = 0, count = face->mIndices.size(); i < count; ++i)
	{
		if (face->mIndices[i] == a) face->mIndices[i] = c;
		if (face->mIndices[i] == d) face->mIndices[i] = d;
	}


	BUG_ME("Can't be implemented properly until vertex refs are checked in.");

	// Edge #1 AB
	// Edge #2 CD
}


void Mesh::ExtrudeFace(index_t faceIndex, vec3_t displacement)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	Vector<index_t> faceWind;
	Vec3 pos, uvw, norm;
	index_t A, B, C, D;

	uint32 i;
	foreach (face->mIndices, i)
	{
		/* 1. Make duplicate vertices with same wind for 'face' */
		A = face->mIndices[i];
		GetVertexPos(A, pos.mVec);
		uvw = GetVertexTexCoord(A);
		norm = GetVertexNormal(A);

		// Push vertices along 'displacement'
		pos[0] += displacement[0];
		pos[1] += displacement[1];
		pos[2] += displacement[2];

		B = CreateVertex(pos.mVec, uvw.mVec, norm.mVec);
		faceWind.push_back(A);

		/* 2. Replace all references to A with B in old face */
		face->mIndices[i] = B;
	}

	foreach (face->mIndices, i)
	{
		// 3. Generate new quad ABCD connecting 'face' and ploygonIndex vertices

		// Make edge #1
		A = faceWind[i];
		B = face->mIndices[i];

		// Make edge #2
		if (i+1 < face->mIndices.size())
		{
			C = face->mIndices[i+1];
			D = faceWind[i+1];
		}
		else
		{
			C = face->mIndices[0];
			D = faceWind[0];
		}

		// Make the quad face from #1 and #2
		Face *genFace = GetFace(CreateFace());
		genFace->mMaterial = face->mMaterial;
		genFace->mIndices.push_back(A);
		genFace->mIndices.push_back(B);
		genFace->mIndices.push_back(C);
		genFace->mIndices.push_back(D);

		{
			Vec3 a, b, c;
			GetVertexPos(genFace->mIndices[0], a.mVec);
			GetVertexPos(genFace->mIndices[1], b.mVec);
			GetVertexPos(genFace->mIndices[2], c.mVec);
		
			genFace->mNormal = -Vec3::Cross(a - b, c - b);
			genFace->mNormal.Norm();
		}

		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when ploymapping changes
			genFace->mTexCoordIndices.push_back(A);
			genFace->mTexCoordIndices.push_back(B);
			genFace->mTexCoordIndices.push_back(C);
			genFace->mTexCoordIndices.push_back(D);
		}
	}
}


void Mesh::SetFaceSmoothingGroup(index_t face, uint32 group)
{
	Face *f = GetFace(face);

	if (f)
	{
		f->SetSmoothingGroup(group);
	}	
}


void Mesh::SelectedFacesMarkSmoothingGroup(uint32 group, bool t)
{
	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (!face || !(face->mFlags & Face::fSelected))
			continue;

		if (t)
		{
			face->SetSmoothingGroup(group);
		}
		else
		{
			face->ClearSmoothingGroup();
		}
	}
}


void Mesh::AssignGroupToSelectedFaces(uint32 group)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (face && face->mFlags & Face::fSelected)
		{
			face->mSmoothingGroup = group; 
		}
	}
}


void Mesh::ClearGroupsFaceSelected(uint32 groups)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group
		if (face && 
#if GROUP_FACE_WITHOUT_BITMAPS
			// No longer use bitflaged groups
			groups & (1<<face->mSmoothingGroup))
#else
			groups & face->mSmoothingGroup)
#endif
		{
			face->mFlags &= ~Face::fSelected;
		}
	}
}


void Mesh::SetGroupsFaceSelected(uint32 groups)
{
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (face &&
#if GROUP_FACE_WITHOUT_BITMAPS
			// No longer use bitflaged groups
			groups & (1<<face->mSmoothingGroup))
#else
			groups & face->mSmoothingGroup)
#endif
		{
			face->mFlags |= Face::fSelected;
		}
	}
}


void Mesh::FaceRemovalCleanup(Face *face)
{
	if (face && face->mIndices.size())
	{
		uint32 i;
		foreach (face->mIndices, i)
		{
			DeleteVertex(face->mIndices[i]);
		}
	}
}


void Mesh::RebuildVertexPolygonReferences(index_t vertex)
{
	Vertex *v = GetVertex(vertex);

	if (!v)
		return;

	v->GetFaceRefs().clear();

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (!face)
			continue;
		
		uint32 i;
		foreach (face->mIndices, i)
		{
			if (face->mIndices[i] == vertex)
			{
				v->GetFaceRefs().push_back(f);	
			}
		}
	}	
}


void Mesh::RebuildVertexPolygonReferences()
{
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetFaceRefs().clear();
		}
	}	

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		if (face)
		{
			uint32 i;
			foreach (face->mIndices, i)
			{
				Vertex *vertex = GetVertex(face->mIndices[i]);
				if (vertex)
				{
					vertex->GetFaceRefs().push_back(f);
				}	
			}
		}
	}	
}


void Mesh::RecomputeFaceNormal(index_t face, bool full)
{
	Face *f = GetFace(face);

	if (f)
	{
		if (full) // expensive!
		{
			Vector<index_t> faces;

			faces.reserve(16);

			// 1. Find all faces affected by this update.
			uint32 i;
			foreach (f->mIndices, i)
			{
				Vertex *v = GetVertex(f->mIndices[i]);

				if (!v)
					continue;

				uint32 j;
				foreach (v->GetFaceRefs(), j)
				{
					bool found = false;
					uint32 k;
					foreach (faces, k)
					{
						if (faces[k] == v->GetFaceRefs()[j])
						{
							found = true;
							break;
						}
					}

					// 2. Compute new face normal per face. Skip if seen again.
					if (!found)
					{
						RecomputeFaceNormal(v->GetFaceRefs()[j], false);
						faces.push_back(v->GetFaceRefs()[j]);
					}
				} 
			}


			// 3. Compute new vertex normals.
			foreach (f->mIndices, i)
			{
				Vertex *v = GetVertex(f->mIndices[i]);

				if (!v)
					continue;

				Vec3 n;
				uint32 j;
				foreach (v->GetFaceRefs(), j)
				{
					Face *f2 = GetFace( v->GetFaceRefs()[j] );
					n += f2->mNormal;
				} 

				n.Norm();
				SetNormal(f->mIndices[i], n.mVec);
			}		
		}
		else
		{
			Vec3 a, b, c;
			GetVertexPos(f->mIndices[0], a.mVec);
			GetVertexPos(f->mIndices[1], b.mVec);
			GetVertexPos(f->mIndices[2], c.mVec);
		
			f->mNormal = -Vec3::Cross(a - b, c - b);
			f->mNormal.Norm();
		}
	}
}


void Mesh::UpdateVertexReferenceWithSelectedBias()
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(face->mFlags & Face::fSelected))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().push_back(f);
			}
		}
	}	
}


void Mesh::UpdateVertexReferenceWithColorBias(uint32 color)
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(color == face->mColor))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().push_back(f);
			}
		}
	}
}


void Mesh::UpdateVertexReferenceWithSmoothingGroupBias(uint32 groupFilter)
{
	// FIXME: Add some kind of conditional flag here to avoid
	//        uneeded updates.  This isn't so important *yet, since
	//        only 'one-offs' call this for now.
	//
	//        You need one flag for 'mesh not modified' and one for prev filter.

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
		{
			vertex->GetTmpRefs().clear();
		}
	}

	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider facets in given group(s)...
		if (!face || !(groupFilter & (1 << face->mSmoothingGroup)))
		{
			continue;
		}

		// Very expensive
		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
			{
				vertex->GetTmpRefs().push_back(f);
			}
		}
	}
}

 
void Mesh::UVMapSelectedFaces_Spherical()
{
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uvw;

	UpdateVertexReferenceWithSelectedBias();

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			longitude = atan2((float)-xyz[0], xyz[2]);
			latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));
			
			longitude = 1.0 - longitude / (HEL_2_PI);
			latitude = fabs(0.5 - latitude / HEL_PI);
			
			uvw[0] = longitude - floor(longitude);
			uvw[1] = latitude;
			uvw[2] = 0.0f;

			SetTexCoord(vertex->mTexCoordIndex, uvw);
		}
	}
}


void Mesh::UVMapSelectedFaces_Cylindrical()
{
	UpdateVertexReferenceWithSelectedBias();
	vec_t longitude, latitude, ysize;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			for (uint32 j = 0; j < 3; ++j)
			{
				if (xyz[j] < min[j])
					min[j] = xyz[j];

				if (xyz[j] > max[j])
					max[j] = xyz[j];
			}
		}
	}

	if (max[1] >= 0)
	{
		if (min[1] >= 0)
		{
			ysize = max[1] - min[1];
		}
		else
		{
			ysize = max[1] + -min[1];
		}
	}
	else
	{
		ysize = -max[1] + min[1];
	}
	
	if (ysize < 0.0001 && ysize > -0.0001)
		ysize = 1.0f;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;
			vec3_t uvw;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);

			longitude = atan2((float)-xyz[0], xyz[2]);
			latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

			longitude = 1.0 - longitude / (HEL_2_PI);
			latitude = fabs(0.5 - latitude / HEL_PI);

			uvw[0] = longitude - floor(longitude);
			uvw[1] = xyz[1] / ysize;
			uvw[2] = 0.0f;

			SetTexCoord(vertex->mTexCoordIndex, uvw);

			// Force face to drop polymapping
			uint32 i;
			foreach (vertex->GetTmpRefs(), i)
			{
				Face *f = GetFace(vertex->GetTmpRefs()[i]);
				if (f)
				{
					SetTexCoord(f->FindCorrespondingTexCoord(v), uvw);
					//f->PurgePolyMappedTexCoords();
				}
			}
		}
	}
}


void Mesh::UVMapSelectedFaces_Plane()
{
	UpdateVertexReferenceWithSelectedBias();

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		// Was this allocated and also marked by group bias?
		if (vertex && vertex->GetTmpRefs().size() > 0)
		{
			vec3_t xyz;
			vec3_t uvw;

			GetVertexArrayPos(vertex->mVertexIndex, xyz);
	
			uvw[0] = (xyz[0] > 0) ? xyz[0] : -xyz[0];	
			vec_t s = 0.025;
  
			while (uvw[0] > 1.0)
			{
				uvw[0] *= s;
				s *= 0.01;
			}
  
			uvw[1] = (xyz[1] > 0) ? xyz[1] : -xyz[1];
			s = 0.025;
  
			while (uvw[1] > 1.0)
			{
				uvw[1] *= s;
				s *= 0.01;
			}
  
			uvw[0] = 1.0 - uvw[0];
			uvw[1] = 1.0 - uvw[1];
			uvw[2] = 0.0f;	

			SetTexCoord(vertex->mTexCoordIndex, uvw);
		}
	}
}


void Mesh::GroupedFacesGenerateVertexNormals(uint32 group)
{
	// FIXME: Cached face refs would be smart for all the connectivty use
	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
			vertex->GetTmpRefs().clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider selected facets here... you filter updates here
		if (!face || !(group & (1<<face->mSmoothingGroup)))
		{
			faceNormals.push_back(normal);  // For alignment purposes push a 'V0'
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->GetTmpRefs().push_back(f);
		}

		GetVertexPos(face->mIndices[0], a.mVec);
		GetVertexPos(face->mIndices[1], b.mVec);
		GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vec3::Cross(a - b, c - b);
		normal.Norm();
		faceNormals.push_back(normal);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		normal.Zero();

		if (vertex->GetTmpRefs().size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
		}

		normal.Norm();

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		SetNormal(v, normal.mVec);
    }
}


void Mesh::SelectedFacesFlipVertexNormals()
{
	UpdateVertexReferenceWithSelectedBias();  

	for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		if (vertex->GetTmpRefs().size())
		{
			Vec3 normal;
			GetNormal(vertex->mNormalIndex, normal.mVec);
			normal = -normal;
			SetNormal(vertex->mNormalIndex, normal.mVec);
		}
	}
}


void Mesh::SelectedFacesGenerateVertexNormals()
{
	// FIXME: Cached face refs would be smart for all the connectivty use
	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (vertex)
			vertex->GetTmpRefs().clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = GetFaceCount(); f < fn; ++f)
	{
		Face *face = GetFace(f);

		// We only consider selected facets here... you filter updates here
		if (!face || !(face->mFlags & Face::fSelected))
		{
			faceNormals.push_back(normal);  // For alignment purposes push a 'V0'
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->GetTmpRefs().push_back(f);
		}

		GetVertexPos(face->mIndices[0], a.mVec);
		GetVertexPos(face->mIndices[1], b.mVec);
		GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vec3::Cross(a - b, c - b);
		normal.Norm();
		faceNormals.push_back(normal);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = GetVertex(v);

		if (!vertex)
			continue;

		normal.Zero();

		if (vertex->GetTmpRefs().size() == 0)
		{
			// This means it's not selected for update, or
			// has no faces containing this vertex
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("%s> ERROR Bad face reference\n", __func__);
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
		}

		normal.Norm();

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		SetNormal(v, normal.mVec);
    }
}


void Mesh::SelectVerticesOfSelectedFaces()
{
	uint32 i;
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f && f->mFlags & Face::fSelected)
		{
			unsigned int j;
			foreach (f->mIndices, j)
			{
				Vertex *v = GetVertex( f->mIndices[j] );

				if (v)
				{
					v->SetFlag( Vertex::fSelected );
				}
			}
		}
	}	
}


void Mesh::SplitFace(index_t faceIndex)
{
	Face *face = GetFace(faceIndex);

	if (!face || face->mIndices.size() == 0)
		return;

	Face *genFace;
	int32 A, B, C, D, M1, M2;
	Vec3 m1, m2, a, b, c, d, uv, norm;
	bool gap1 = false, gap2 = false;

 
	switch (face->mIndices.size())
	{
	case 4:

		// 1. Generate midpoint vertices
		A = face->mIndices[0];
		GetVertexPos(A, a.mVec);
		B = face->mIndices[1];
		GetVertexPos(B, b.mVec);
		C = face->mIndices[2];
		GetVertexPos(C, c.mVec);
		D = face->mIndices[3];
		GetVertexPos(D, d.mVec);

		// M1
		helMidpoint3v(a.mVec, b.mVec, m1.mVec);
		uv = GetVertexTexCoord(A) + GetVertexTexCoord(A);
		uv *= 0.5f;
		norm = GetVertexNormal(A) + GetVertexNormal(B);
		norm.Norm();
		M1 = CreateVertex(m1.mVec, uv.mVec, norm.mVec);

		// M2
		helMidpoint3v(c.mVec, d.mVec, m2.mVec);
		uv = GetVertexTexCoord(C) + GetVertexTexCoord(D);
		uv *= 0.5f;
		norm = GetVertexNormal(C) + GetVertexNormal(D);
		norm.Norm();
		M2 = CreateVertex(m2.mVec, uv.mVec, norm.mVec);

		// 2. Generate a new quad face, and reuse the old one with new vertices
		genFace = GetFace(CreateFace());
		genFace->mMaterial = face->mMaterial;
		genFace->mIndices.push_back(A);
		genFace->mIndices.push_back(M1);
		genFace->mIndices.push_back(M2);
		genFace->mIndices.push_back(D);
		
		// This won't be correct
		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when polymapping changes
			genFace->mTexCoordIndices.push_back(A);
			genFace->mTexCoordIndices.push_back(M1);
			genFace->mTexCoordIndices.push_back(M2);
			genFace->mTexCoordIndices.push_back(D);
		}

		// Reuse old face here
		face->mIndices[0] = B;
		face->mIndices[1] = M1;
		face->mIndices[2] = M2;
		face->mIndices[3] = C;

		// This won't be correct
		if (face->mFlags & Face::fPolyMappedTexCoords)
		{
			// This will have to change when polymapping changes
			genFace->mTexCoordIndices.push_back(B);
			genFace->mTexCoordIndices.push_back(M1);
			genFace->mTexCoordIndices.push_back(M2);
			genFace->mTexCoordIndices.push_back(C);
		}

		// 3. Use vertex references to check for gaps
		BUG_ME("No vertex refs to check for gaps, assuming closed topology...");
		gap1 = gap2 = true;

		// 4. Fill in the two gaps with triangles
		if (gap1)
		{
			genFace = GetFace(CreateFace());
			genFace->mMaterial = face->mMaterial;
			genFace->mIndices.push_back(A);
			genFace->mIndices.push_back(M1);
			genFace->mIndices.push_back(B);
		}

		if (gap2)
		{
			genFace = GetFace(CreateFace());
			genFace->mMaterial = face->mMaterial;
			genFace->mIndices.push_back(D);
			genFace->mIndices.push_back(M2);
			genFace->mIndices.push_back(C);
		}
		break;

	default:
		;
	}
}


void Mesh::ClearFlagForSelectedFaces(Face::Flags flag)
{
	uint32 i;
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f && f->mFlags & Face::fSelected)
		{
			f->mFlags &= ~flag;
		}
	}
}


void Mesh::SetFlagForSelectedFaces(Face::Flags flag)
{
	uint32 i;
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f && f->mFlags & Face::fSelected)
		{
			f->mFlags |= flag;
		}
	}
}


void Mesh::SetFaceFlags(index_t face, uint32 flags)
{
	Face *f = GetFace(face);
		
	if (f)
	{
		// Set flags
		f->mFlags |= flags;
	}
}


void Mesh::SetVertexArrayPos(index_t vertexIndex, const vec3_t xyz)
{	
	if (vertexIndex < mVertexPool.size())
		SetTripleVec(mVertexPool, vertexIndex, xyz);	
}


void Mesh::ClearFaceFlags(index_t face, uint32 flags)
{
	Face *f = GetFace(face);
		
	if (f)
	{
		// Clear flags
		f->mFlags |= flags;
		f->mFlags ^= flags;
	}
}


void Mesh::SetVertexFlags(index_t face, uint32 flags)
{
	Vertex *f = GetVertex(face);
		
	if (f)
	{
		// Set flags
		f->mFlags |= flags;
	}
}


void Mesh::ClearVertexFlags(index_t face, uint32 flags)
{
	Vertex *f = GetVertex(face);
		
	if (f)
	{
		// Clear flags
		f->mFlags |= flags;
		f->mFlags ^= flags;
	}
}


void Mesh::UpdateBoundingVolume()
{
	vec3_t min;
	vec3_t max;
	vec3_t xyz;
	bool update = false;


	for ( uint32 i = 0, count = mVertices.size(); i < count; ++i )
	{
		if (!mVertices[i])
			continue;

		GetVertexArrayPos(mVertices[i]->mVertexIndex, xyz);

		// Adjust bounding volume in loop to handle gaps
		if (!mInitBoundingVol)
		{
			mInitBoundingVol = true;
			SetBBox(xyz, xyz);
			continue;
		}

		GetBBox(min, max);

		for (uint32 i = 0; i < 3; ++i)
		{
			if (xyz[i] < min[i])
			{
				min[i] = xyz[i];
				update = true;
			}
			else if (xyz[i] > max[i])
			{
				max[i] = xyz[i];
				update = true;
			}
		}

		if (update)
		{
			update = false;
			SetBBox(min, max);
		}
	}

	SetPosition(GetBoundingVolumeCenter());
}


void Mesh::AppendVertexToFace(index_t face, index_t vertex)
{
	Face *f = GetFace(face);
	Vertex *v = GetVertex(vertex);

	if (f && v)
	{
		f->AppendVertex(vertex);
		Vector<index_t> &refs = v->GetFaceRefs();
		refs.push_back(face);
	}
}



void Mesh::UpdateEdgeGraph()
{
	mEdges.erase();
	Repack(); // This is needed for alignment

	uint32 i;
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f)
		{
			// Iterate over indices list, and find edges
			uint32 j = 0;
			uint32 j2 = f->mIndices.size()-1;
			uint32 jcount = f->mIndices.size();
			for (; j < jcount; ++j)
			{
				Edge *e = new Edge(f->mIndices[j], f->mIndices[j2]);

				uint32 k;
				foreach (mEdges, k)
				{
					if (*(mEdges[k]) == *e)
					{
						mEdges[k]->mFaceRefs.push_back(i);
						delete e;
						e = NULL;
						break;
					}
				}

				if (e)
				{
					e->mFaceRefs.push_back(i);
					mEdges.push_back(e);
				}

				j2 = j;
			}

			// 2007.06: Face neighbours should be obsoleted by edge refs?

			// Could use vertex references to find facet neighbours
		}
	}	
}


void Mesh::UpdatePlaneGeometry()
{	
	mPlanes.erase();

	uint32 i;
	foreach (mFaces, i)
	{
		Face *f = mFaces[i];
		
		if (f)
		{
			// Iterate over indices list, and find edges
			for (uint32 j = 2, jn = f->mIndices.size(); j < jn; ++j)
			{
				Vec3 a = GetVertexPosition(f->mIndices[j-2]);
				Vec3 b = GetVertexPosition(f->mIndices[j-1]);
				Vec3 c = GetVertexPosition(f->mIndices[j]);

				// Make a new plane, or append if exists
			}

			// Could use vertex references to find facet neighbours
		}
	}
}


////////////////////////////////////////////////////////////
// Transforms
////////////////////////////////////////////////////////////

void Mesh::Rotate(const hel::Vec3 &v)
{
	RotateAboutPoint(GetBoundingVolumeCenter(), v);
}


void Mesh::RotateAboutPoint(const hel::Vec3 &point, const hel::Vec3 &v)
{
	hel::Mat44 t, r, t2, mat;
	// Rotate about bounding volume center instead of origin
	t.Translate(point.mVec);
	r.Rotate(v.mVec);
	t2.Translate((-Vec3(point)).mVec);
	
	// FIXME: Fix the damn matrix backend to avoid such expensive
	//        processing here ( only want to transform once )
	TransformVertices(t2);
	TransformVertices(r);
	TransformVertices(t);
	
	// Transform normals by inverted rotation to stay correct
	hel::Mat44 nr;
	nr.Rotate(v.mVec);
	nr.Invert();
	TransformNormals(nr);
}


void Mesh::Scale(const hel::Vec3 &v)
{
	ScaleAboutPoint(GetPosition(), v);
}


void Mesh::ScaleAboutPoint(const hel::Vec3 &point, const hel::Vec3 &v)
{
	hel::Mat44 t, s, t2, mat;

	// Scale about bounding volume center instead of origin
	t.Translate(point.mVec);
	s.Scale(v.mVec);
	t2.Translate((-Vec3(point)).mVec);

	// FIXME: Fix the damn matrix backend to avoid such expensive
	//        processing here ( only want to transform once )
	TransformVertices(t2);
	TransformVertices(s);
	TransformVertices(t);
}


void Mesh::Translate(const hel::Vec3 &v) 
{
	// Position (world)
	SetPosition(Vec3(v)+GetPosition());
	
	// Vertices
	TripleVec_Addition(mVertexPool, v.mVec);
	
	// Bounding Vol
	mBoundingVolume.mSphere.mCenter[0] += v.mVec[0];
	mBoundingVolume.mSphere.mCenter[1] += v.mVec[1];
	mBoundingVolume.mSphere.mCenter[2] += v.mVec[2];
	mBoundingVolume.mBox.mMax[0] += v.mVec[0];
	mBoundingVolume.mBox.mMax[1] += v.mVec[1];
	mBoundingVolume.mBox.mMax[2] += v.mVec[2];
	mBoundingVolume.mBox.mMin[0] += v.mVec[0];
	mBoundingVolume.mBox.mMin[1] += v.mVec[1];
	mBoundingVolume.mBox.mMin[2] += v.mVec[2];
}


void Mesh::SelectedFacesRotateUVMap(vec_t z)
{
	MSTL_MSG("FIXME");
}

void Mesh::SelectedFacesScaleUVMap(vec_t x, vec_t y)
{
	MSTL_MSG("FIXME");
}


void Mesh::SelectedFacesTranslateUVMap(vec_t x, vec_t y)
{
	MSTL_MSG("FIXME");
}


void Mesh::TransformFacesWithFlag(Face::Flags flag, hel::Mat44 &mat)
{
	MarkVerticesOfFacesWithFlag(flag, Vertex::fSelected2, true);
	TransformVerticesWithFlag(Vertex::fSelected2, mat);
}


void Mesh::GetUniqueVerticesInFaces(const Vector<index_t> &faces, 
									Vector<index_t>& vertices)
{
	for (uint32 i = 0, n = faces.size(); i < n; ++i)
	{
		Face *f = GetFace(faces[i]);
		
		if (f)
		{
			// These lists must retain order, so please don't sort.
			uint32 j;
			foreach (f->mIndices, j)
			{
				bool found = false;
				uint32 vertex = f->mIndices[j]; 

				uint32 k;
				foreach (vertices, k)
				{
					if (vertex == vertices[k])
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					vertices.push_back(vertex);
				}
			}
		}
	}
}


void Mesh::TransformFacesInList(Vector<index_t> &faces, hel::Mat44 &mat)
{
	Vector<index_t> vertices; 
	GetUniqueVerticesInFaces(faces, vertices);
	TransformVerticesInList(vertices, mat);
}


void Mesh::TransformVerticesWithFlag(Vertex::Flags flag, hel::Mat44 &mat)
{
	vec_t *array = mVertexPool.get_array();

	for (uint32 i = 0, n = GetVertexCount(); i < n; ++i)
	{
		Vertex *v = GetVertex(i);
		
		if (v && v->mFlags & flag)
		{			
			mat.Multiply3fv(array + v->mVertexIndex*3);
		}
	}
}


void Mesh::TransformVerticesInList(Vector<index_t> &vertices, hel::Mat44 &mat)
{
	vec_t *array = mVertexPool.get_array();

	for (uint32 i = 0, n = vertices.size(); i < n; ++i)
	{
		Vertex *v = GetVertex(vertices[i]);
		
		if (v)
		{
			//Debug v->SetFlag(Vertex::fSelected);
			mat.Multiply3fv(array + v->mVertexIndex*3);
		}
	}
}


void Mesh::MarkVerticesOfFacesWithFlag(Face::Flags flag, 
									   Vertex::Flags mark, bool clear)
{
	for (uint32 i = 0, iCount = GetVertexCount(); i < iCount; ++i)
	{
		Vertex *v = GetVertex(i);

		if (!v) 
			continue;

		if (clear)
		{
			v->mFlags &= ~mark;
		}

		for (int32 j = 0, jCount = v->GetFaceRefs().size(); j < jCount; ++j)
		{
			Face *f = GetFace(v->GetFaceRefs()[j]);
			
			if (f && (f->mFlags & flag))
			{
				v->mFlags |= mark;
				break;
			}
		}
	}
}


void Mesh::ConvertAllFacesToTexCoordPloymapping()
{
	for (uint32 i = 0, n = GetFaceCount(); i < n; ++i)
	{
		ConvertFaceToTexCoordPloymapping(i);
	}
}


void Mesh::ConvertFaceToTexCoordPloymapping(index_t face)
{
	Face *f = GetFace(face);

	if (f)
	{
		if (f->mFlags & Face::fPolyMappedTexCoords &&
			f->mIndices.size() == f->mTexCoordIndices.size())
		{
		}
		else
		{
			f->mTexCoordIndices.clear();

			for (uint32 i = 0, n = f->mIndices.size(); i < n; ++i)
			{
				Vec3 t = GetVertexTexCoord(f->mIndices[i]);
				f->mTexCoordIndices.push_back(CreateTexCoord(t.mVec));
			}
		}
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Mesh::SerializePool(SystemIO::FileWriter &w, 
						 Vector<vec_t> &v, mstl::stack<index_t> &s)
{
	mstl::stack<index_t> copy; // We don't really care about order

	w.WriteLong(v.size());

	for ( uint32 i = 0; i < v.size(); ++i )
	{
		w.WriteFloat32(v[i]);
	}
		
	w.WriteLong(s.size());

	for ( uint32 i = 0; i < s.size(); ++i )
	{
		index_t item = s.pop();
		copy.push(item);
		w.WriteLong(item);
	}

	while ( !copy.empty() )
	{
		s.push(copy.pop());
	}

	return true;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAMESH
int runFreyjaMeshUnitTest(int argc, char *argv[])
{
	FreyjaMesh test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaMesh class test]\n");

	return runFreyjaMeshUnitTest(argc, argv);
}
#endif
