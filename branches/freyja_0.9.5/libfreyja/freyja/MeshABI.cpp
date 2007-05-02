/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005,2006 Mongoose
 * Comments: This is the ABI to expose Mesh class and utils to C.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Mesh.cpp
 ==========================================================================*/

#include <math.h>
#include "Mesh.h"
#include "MeshABI.h"

using namespace freyja;
using namespace hel;


////////////////////////////////////////////////////////////////////////
// 0.9.5 C++ ABI
////////////////////////////////////////////////////////////////////////

Mesh *freyjaGetMeshClass(index_t meshUID)
{
	return Mesh::GetMesh(meshUID);
}


Weight *freyjaGetMeshWeightClass(index_t meshUID, index_t weight)
{
	Mesh *m = freyjaGetMeshClass(meshUID);
	return m ? m->GetWeight(weight) : NULL;
}


Vertex *freyjaGetMeshVertexClass(index_t meshUID, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(meshUID);
	return m ? m->GetVertex(vertex) : NULL;
}


Face *freyjaGetMeshFaceClass(index_t meshUID, index_t face)
{
	Mesh *m = freyjaGetMeshClass(meshUID);
	return m ? m->GetFace(face) : NULL;
}


bool freyjaMeshSaveChunkTextJA(SystemIO::TextFileWriter &w, index_t mesh)
{
	freyjaPrintMessage("> Writing out mesh %i...", mesh);
	Mesh *m = freyjaGetMeshClass(mesh);
	return m ? m->Serialize(w) : false;
}


bool freyjaMeshLoadChunkTextJA(SystemIO::TextFileReader &r)
{
	index_t mesh = freyjaMeshCreate();
	freyjaPrintMessage("> Reading in mesh %i...", mesh);
	Mesh *m = freyjaGetMeshClass(mesh);
	return m ? m->Serialize(r) : false;
}


/* This is to fix all the problems with the backend delete corruption
 * It filters and realigns indices to only save valid data, not
 * the entire junky backend cruft like undo data which remains
 *
 * Vector maps are very memory wasteful */
int32 freyjaMeshSaveChunkJA(SystemIO::FileWriter &w, index_t meshIndex)
{
	const int32 version = 1;
	Vector<long> polygons, vertices, texcoords;
	Vector<long> verticesMap, texcoordsMap;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec2_t uv;
	int32 flags, material;
	int32 i, j, k, idx, count, vertex, texcoord;
	int32 polygonCount = freyjaGetMeshPolygonCount(meshIndex);
	int32 byteSize = 0;
	int32 meshFlags = freyjaGetMeshFlags(meshIndex);
	int32 vertexCount = 0;
	int32 vertexWeightCount = 0;
	int32 vertexFrameCount = 0;
	int32 texCoordCount = 0;

	if (polygonCount < 1)
		return -1;

	byteSize += 4; // meshFlags
	byteSize += 4; // vertexCount
	byteSize += 4; // vertexWeightCount
	byteSize += 4; // vertexFrameCount
	byteSize += 4; // texCoordCount
	byteSize += 4; // polygonCount
	byteSize += 4; // vertexGroupCount

	/* Polygons, filtered by mesh */
	for (i = 0, count = 0; i < polygonCount; ++i)
	{
		count = freyjaGetMeshPolygonEdgeCount(meshIndex, i);

		/* Weed out deformed or 'sparse' (NULL) polygons */
		if (count < 3) 
		{
			continue;
		}

		polygons.pushBack(i);
	}

	polygonCount = polygons.end();

	/* Vertices and polymapped TexCoords, filtered by filtered polygons */
	count = freyjaGetMeshVertexCount(meshIndex);
	verticesMap.reserve(count+1);
	for (i = 0; i < count; ++i)
		verticesMap.pushBack(0);

	count = freyjaGetMeshTexCoordCount(meshIndex);
	texcoordsMap.reserve(count+1);
	for (i = 0; i < count; ++i)
		texcoordsMap.pushBack(0);

	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];
		count = freyjaGetMeshPolygonVertexCount(meshIndex, idx);

		byteSize += 4 + 4 + 4 + 4; // flags vertCount texCount material
		byteSize += (freyjaGetMeshPolygonVertexCount(meshIndex, idx) * 4 +
					 freyjaGetMeshPolygonTexCoordCount(meshIndex, idx) * 4);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetMeshPolygonVertexIndex(meshIndex, idx, j);

			for (k = vertices.begin(); k < (long)vertices.end(); ++k)
			{
				if (vertex == vertices[k])
				{
					vertex = -2;
					break;
				}
			}

			if (vertex > -2)
			{
				verticesMap.assign(vertex, vertexCount);
				++vertexCount;
				vertices.pushBack(vertex);
				byteSize += (4 + 12 + 12 + 8); // flags xyz nxyz uv

				uint32 wcount = 0;
				Mesh *m = freyjaGetMeshClass(meshIndex);

				if (m)
				{
					for (uint32 k = 0, n = m->GetWeightCount(); k < n; ++k)
					{
						Weight *w = m->GetWeight(k);
						
						if (w)
						{
							if ((int)w->mVertexIndex == vertex)
							{
								++wcount;
							}
						}
					}
				}

				vertexWeightCount += wcount;
				byteSize += 12 * wcount; // vidx weight bone
				uint32 vertexframeCount = 0;//freyjaGetVertexFrameCount(vertex);
				vertexFrameCount += vertexframeCount;
				byteSize += 20 * vertexframeCount; // vidx frame xyz
			}
		}

		count = freyjaGetMeshPolygonTexCoordCount(meshIndex, idx);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetMeshPolygonTexCoordIndex(meshIndex, idx, j);

			for (k = texcoords.begin(); k < (long)texcoords.end(); ++k)
			{
				if (texcoord == texcoords[k])
				{
					texcoord = -2;
					break;
				}
			}

			if (texcoord > -2)
			{
				texcoordsMap.assign(texcoord, texCoordCount);
				++texCoordCount;
				texcoords.pushBack(texcoord);
				byteSize += 8; // uv
			}
		}
	}


	/* VertexGroups */
	// Where never written to disk anyway, so removed code


	// If chunk.flags & 0x1 ( 0.9.5+ ) we write out extra things
	// appended to the end of mesh chunk ( 0.9.3 readers will just skip it )
	Mesh *m = freyjaGetMeshClass(meshIndex);
	if (m)
	{
		// Note we're starting a versioned subblock, use the C++ serialize ver
		byteSize += 4;
		int32 count = m->GetFaceCount();
		byteSize += 4;
		for (i = 0; i < count; ++i)
		{
			byteSize += 4;
		}
	}


	/* Write to diskfile */
	chunk.type = FREYJA_CHUNK_MESH;
	chunk.size = byteSize;
	chunk.flags = 0x1;      // 0x0 means 0.9.3 version!
	chunk.version = version;

	w.WriteLong(chunk.type);
	w.WriteLong(chunk.size);
	w.WriteLong(chunk.flags);
	w.WriteLong(chunk.version);

	w.WriteLong(meshFlags);
	w.WriteLong(vertexCount);
	w.WriteLong(vertexWeightCount);
	w.WriteLong(vertexFrameCount);
	w.WriteLong(texCoordCount);
	w.WriteLong(polygonCount);
	w.WriteLong(0); // vertexGroupCount, Never used in older format anyway

	/* Vertices */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];

		flags = freyjaGetMeshVertexFlags(meshFlags, vertex);
		w.WriteLong(flags);

		freyjaGetMeshVertexPos3fv(meshFlags, vertex, xyz);
		for (j = 0; j < 3; ++j)
			w.WriteFloat32(xyz[j]);

		freyjaGetMeshVertexNormal3fv(meshFlags, vertex, xyz);

		for (j = 0; j < 3; ++j)
			w.WriteFloat32(xyz[j]);

		freyjaGetMeshVertexTexCoord3fv(meshFlags, vertex, xyz);
		for (j = 0; j < 2; ++j) // U V only in JA v1  =p
			w.WriteFloat32(xyz[j]);
	}

	/* VertexWeights */
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		//count = freyjaGetVertexWeightCount(vertex);
		count = 0;
		m = freyjaGetMeshClass(meshIndex);
		Vector<index_t> lst;

		if (m)
		{
			for (uint32 k = 0, n = m->GetWeightCount(); k < n; ++k)
			{
				Weight *w = m->GetWeight(k);
				
				if (w)
				{
					if ((int)w->mVertexIndex == vertex)
					{
						lst.push_back(k);
						++count;
					}
				}
			}
		}

		vec_t weight;
		index_t bone, vert;
		for (j = 0; j < count; ++j)
		{
			freyjaGetMeshWeight(meshIndex, lst[j], vert, bone, weight);
			//freyjaGetVertexWeight(vertex, j, &bone, &weight);
			w.WriteLong(vert);
			w.WriteLong(bone);
			w.WriteFloat32(weight);
		}
	}


	/* VertexFrames */
#if 0
	for (i = vertices.begin(); i < (long)vertices.end(); ++i)
	{
		vertex = vertices[i];
		count = freyjaGetVertexFrameCount(vertex);

		for (j = 0; j < count; ++j)
		{
			freyjaGetVertexFrame(vertex, j, &frame, xyz);
			w.WriteLong(verticesMap[vertex]);
			w.WriteLong(frame);
			for (k = 0; k < 3; ++k)
				w.WriteFloat32(xyz[k]);
		}
	}
#endif


	/* TexCoords */
	for (i = texcoords.begin(); i < (long)texcoords.end(); ++i)
	{
		texcoord = texcoords[i];

		freyjaGetMeshTexCoord2fv(meshIndex, texcoord, uv);

		for (j = 0; j < 2; ++j)
			w.WriteFloat32(uv[j]);
	}


	/* Polygons */
	for (i = polygons.begin(); i < (long)polygons.end(); ++i)
	{
		idx = polygons[i];

		flags = freyjaGetMeshPolygonFlags(meshIndex, idx);
		w.WriteLong(flags);

		material= freyjaGetMeshPolygonMaterial(meshIndex, idx);
		w.WriteLong(material);

		count = freyjaGetMeshPolygonVertexCount(meshIndex, idx);
		w.WriteLong(count);

		for (j = 0; j < count; ++j)
		{
			vertex = freyjaGetMeshPolygonVertexIndex(meshIndex, idx, j);
			w.WriteLong(verticesMap[vertex]);
		}

		count = freyjaGetMeshPolygonTexCoordCount(meshIndex, idx);
		w.WriteLong(count);

		for (j = 0; j < count; ++j)
		{
			texcoord = freyjaGetMeshPolygonTexCoordIndex(meshIndex, idx, j);
			w.WriteLong(texcoordsMap[texcoord]);
		}
	}

	// If chunk.flags & 0x1 ( 0.9.5+ ) we write out extra things
	// appended to the end of mesh chunk ( 0.9.3 will just skip it )
	m = freyjaGetMeshClass(meshIndex);
	if (m)
	{
		// Note we're starting a versioned subblock, use the C++ serialize ver
		w.WriteLong(Face::GetChunkVersion());
		int32 count = m->GetFaceCount();
		w.WriteLong(count);
		for (i = 0; i < count; ++i)
		{
			Face *f = m->GetFace(i);

			if (f)
			{
				w.WriteInt8U(f->mSmoothingGroup);
				w.WriteInt8U(0);
				w.WriteInt8U(0);
				w.WriteInt8U(0);
			}
			else
			{
				w.WriteInt8U(0);
				w.WriteInt8U(0);
				w.WriteInt8U(0);
				w.WriteInt8U(0);
			}
		}
	}


	return 0;
}


int32 freyjaMeshLoadChunkJA(SystemIO::FileReader &r, freyja_file_chunk_t &chunk)
{
	Vector<long> verticesMap, texcoordsMap;
	vec3_t xyz;
	vec2_t uv;
	vec_t weight;
	int32 bone, frame, material;
	int32 i, j, count, idx, flags;
	int32 polygonCount;
	int32 vertexGroupCount;
	int32 meshFlags;
	int32 vertexCount;
	int32 vertexWeightCount;
	int32 vertexFrameCount;
	int32 texCoordCount;


	/* Read from diskfile */
	meshFlags = r.ReadLong();
	vertexCount = r.ReadLong();
	vertexWeightCount = r.ReadLong();
	vertexFrameCount = r.ReadLong();
	texCoordCount = r.ReadLong();
	polygonCount = r.ReadLong();
	vertexGroupCount = r.ReadLong();

	index_t mesh = freyjaMeshCreate();
	//freyjaBegin(FREYJA_MESH);
	//freyjaBegin(FREYJA_VERTEX_GROUP);
	
	/* Vertices */
	for (i = 0; i < vertexCount; ++i)
	{
		flags = r.ReadLong();

		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();

		idx = freyjaMeshVertexCreate3fv(mesh, xyz);

		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();

		freyjaMeshVertexNormal3fv(mesh, idx, xyz);

		for (j = 0; j < 2; ++j)
			xyz[j] = r.ReadFloat32();
		xyz[2] = 0.0f;

		freyjaMeshVertexTexCoord3fv(mesh, idx, xyz);

		verticesMap.pushBack(idx);
	}

	//freyjaEnd(); // FREYJA_VERTEX_GROUP

	/* VertexWeights */
	for (i = 0; i < vertexWeightCount; ++i)
	{
		idx = r.ReadLong();
		bone = r.ReadLong();
		weight = r.ReadFloat32();
		
		freyjaMeshVertexWeight(mesh, verticesMap[idx], bone, weight);
	}

	/* VertexFrames */
	for (i = 0; i < vertexFrameCount; ++i)
	{
		idx = r.ReadLong();
		frame = r.ReadLong(); // Reserved use
		for (j = 0; j < 3; ++j)
			xyz[j] = r.ReadFloat32();
		
		//freyjaVertexFrame3f(verticesMap[idx], xyz[0], xyz[1], xyz[2]);
	}

	/* TexCoords */
	for (i = 0; i < texCoordCount; ++i)
	{
		for (j = 0; j < 2; ++j)
			uv[j] = r.ReadFloat32();

		idx = freyjaMeshTexCoordCreate2fv(mesh, uv);
		texcoordsMap.pushBack(idx);
	}

	/* Polygons */
	for (i = 0; i < polygonCount; ++i)
	{
		//freyjaBegin(FREYJA_POLYGON);
		index_t face = freyjaMeshPolygonCreate(mesh);

		flags = r.ReadLong();
		freyjaMeshPolygonSetFlag1u(mesh, face, flags);

		material = r.ReadLong();
		//freyjaPolygonMaterial1i(material);
		freyjaMeshPolygonMaterial(mesh, face, material);

		count = r.ReadLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.ReadLong();
			//freyjaPolygonVertex1i(verticesMap[idx]);
			freyjaMeshPolygonAddVertex1i(mesh, face, verticesMap[idx]);
		}

		count = r.ReadLong();

		for (j = 0; j < count; ++j)
		{
			idx = r.ReadLong();
			//freyjaPolygonTexCoord1i(texcoordsMap[idx]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoordsMap[idx]);
		}

		//freyjaEnd(); // FREYJA_POLYGON
	}

	//freyjaEnd(); // FREYJA_MESH

	// If chunk.flags & 0x1 ( 0.9.5+ ) we write out extra things
	// appended to the end of mesh chunk ( 0.9.3 will just skip it )
	if (chunk.flags & 0x1)
	{
		// Note we're starting a versioned subblock, use the C++ serialize ver
		//uint32 faceChunkVersion = 
		r.ReadLong();

		for (i = 0, count = r.ReadLong(); i < count; ++i)
		{
			byte flags = r.ReadInt8U();
			freyjaMeshPolygonGroup1u(mesh, i, flags);

			r.ReadInt8U();
			r.ReadInt8U();
			r.ReadInt8U();
		}
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////
// 0.9.5 ABI
////////////////////////////////////////////////////////////////////////

char freyjaIsMeshAllocated(index_t mesh)
{
	return ( freyjaGetMeshClass(mesh) != NULL );
}


uint32 freyjaGetMeshFlags(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->GetFlags();
	}

	return 0x0;
}


index_t freyjaMeshCreate()
{
	Mesh *mesh = new Mesh();
	return mesh->AddToPool();
}


void freyjaMeshDelete(index_t mesh)
{
	Mesh *m = Mesh::GetMesh(mesh);
	if (m)
	{
		m->RemoveFromPool();
		delete m;
	}
}


index_t freyjaMeshPolygonCreate(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->CreateFace();
	}

	return INDEX_INVALID;
}


void freyjaMeshPolygonDelete(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->DeleteFace(polygonIndex);
	}
}


byte freyjaGetMeshPolygonFlags(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mFlags;
		}
	}

	return 0;	
}


byte freyjaGetMeshPolygonGroup(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mSmoothingGroup;
		}
	}

	return 0;	
}


uint32 freyjaGetMeshPolygonVertexCount(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mIndices.size();
		}
	}

	return 0;
}


index_t freyjaGetMeshPolygonVertexIndex(index_t mesh, index_t polygon, 
										uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mIndices[element];
		}
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshPolygonTexCoordCount(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mTexCoordIndices.size();
		}
	}

	return 0;
}


index_t freyjaGetMeshPolygonTexCoordIndex(index_t mesh, index_t polygon, 
										  uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			if (f->mFlags & Face::fPolyMappedTexCoords &&
				f->mTexCoordIndices.size() > element)
			{
				return f->mTexCoordIndices[element];
			}
			else if (f->mIndices.size() > element)
			{
				return freyjaGetMeshVertexTexCoord(mesh, f->mIndices[element]);
			}
		}
	}

	return INDEX_INVALID;
}


void freyjaMeshPolygonMaterial(index_t mesh, index_t polygon, index_t material)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			f->mMaterial = material;
		}
	}	
}


index_t freyjaGetMeshPolygonMaterial(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mMaterial;
		}
	}	

	return 0;
}


void freyjaMeshPolygonAddTexCoord1i(index_t mesh, index_t polygon, 
									index_t texcoord)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			f->mFlags |= Face::fPolyMappedTexCoords;
			f->mTexCoordIndices.pushBack(texcoord);
		}
	}
}


void freyjaMeshPolygonAddVertex1i(index_t mesh, index_t polygon, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	FREYJA_ASSERTMSG(m != NULL, "Mesh[%i] == NULL", mesh);

	if ( m != NULL )
	{
		Face *face = m->GetFace(polygon);
		Vertex *v = m->GetVertex(vertex);

		FREYJA_ASSERTMSG(face != NULL, "Mesh[%i].Polygon[%i] == NULL", mesh, polygon);
		FREYJA_ASSERTMSG(v != NULL, "vertex == NULL");

		if ( face != NULL && v != NULL )
		{
			for ( uint32 i = 0; i < face->mIndices.size(); ++i )
			{
				if (face->mIndices[i] == vertex)
				{
					MARK_MSGF("%s, Tried to insert duplicate vertex into face[%i].  %i  %i", __func__, polygon, face->mIndices[i], vertex);
					return;
				}
			}

			face->mIndices.pushBack(vertex);
		}
	}
}


void freyjaMeshTransform3fv(index_t mesh, 
							freyja_transform_action_t action, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m == NULL)
		return;

	switch (action)
	{
	case fRotate:
		{
			hel::Mat44 t, r, t2, mat;
			// Rotate about bounding volume center instead of origin
			t.Translate(m->GetBoundingVolumeCenter().mVec);
			r.Rotate(xyz);
			t2.Translate((-m->GetBoundingVolumeCenter()).mVec);
			//mat = t*r*t2;
			//mesh->TransformVertices(mat);

			// FIXME: Fix the damn matrix backend to avoid such expensive
			//        processing here ( only want to transform once )
			m->TransformVertices(t2);
			m->TransformVertices(r);
			m->TransformVertices(t);

			// Transform normals by inverted rotation to stay correct
			hel::Mat44 nr;
			nr.Rotate(xyz);
			nr.Invert();
			m->TransformNormals(nr);
		}
		break;

	case fScale:
		{
#if 0
			hel::Mat44 s;
			s.scale(xyz);
			m->TransformVertices(s);
#else
			hel::Mat44 t, s, t2, mat;

			// Scale about bounding volume center instead of origin
			t.Translate(m->GetPosition().mVec);
			s.Scale(xyz);
			t2.Translate((-Vec3(m->GetPosition())).mVec);

			// FIXME: Fix the damn matrix backend to avoid such expensive
			//        processing here ( only want to transform once )
			m->TransformVertices(t2);
			m->TransformVertices(s);
			m->TransformVertices(t);
#endif
		}
		break;

	case fTranslate:
		{
			m->Translate(Vec3(xyz));
		}
		break;
		
	default:
		BUG_ME("Case '%i' Not Implemented", action);
	}
}


index_t freyjaMeshVertexCreate3fv(index_t mesh, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		vec3_t uvw = {0.5f, 0.5f, 0.0f};
		vec3_t nxyz = {0.0f, 1.0f, 0.0f};

		return m->CreateVertex(xyz, uvw, nxyz);
	}

	return INDEX_INVALID;
}


void freyjaMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->SetVertexArrayPos(v->mVertexIndex, xyz);
		}
	}
}


void freyjaMeshVertexNormal3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->SetNormal(v->mNormalIndex, xyz);
		}
	}
}


void freyjaMeshVertexTexCoord3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->SetTexCoord(v->mTexCoordIndex, xyz);
		}
	}
}


index_t freyjaGetMeshVertexTexCoord(index_t mesh, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			return v->mTexCoordIndex;
		}
	}

	return INDEX_INVALID;
}


byte freyjaGetMeshVertexFlags(index_t mesh, index_t vertex)
{
	Vertex *v = freyjaGetMeshVertexClass(mesh, vertex);

	return v ? v->mFlags : 0x0;
}


void freyjaGetMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		m->GetVertexPos(vertex, xyz);
	}
}


void freyjaGetMeshVertexNormal3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->GetNormal(v->mNormalIndex, xyz);
		}
	}
}


void freyjaGetMeshVertexTexCoord3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->GetTexCoord(v->mTexCoordIndex, xyz);
		}
	}
}


index_t freyjaMeshTexCoordCreate2fv(index_t mesh, vec2_t uv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		vec3_t uvw = {uv[0], uv[1], 0.0f };
		return m->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


index_t freyjaMeshTexCoordCreate2f(index_t meshIndex, vec_t u, vec_t v)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		vec3_t uvw = {u, v, 0.0f };
		return mesh->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


index_t freyjaMeshTexCoordCreate3f(index_t meshIndex, vec_t u, vec_t v, vec_t w)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		vec3_t uvw = {u, v, w };
		return mesh->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


index_t freyjaMeshTexCoordCreate3fv(index_t mesh, vec3_t uvw)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


void freyjaMeshVertexWeight(index_t mesh, index_t vertex, 
							index_t bone, vec_t weight)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		m->AddWeight(vertex, weight, bone);
	}
}


index_t freyjaMeshVertexWeld(index_t mesh, index_t a, index_t b)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		if (m->WeldVertices(a, b))
			return a;
	}	

	return INDEX_INVALID;
}


index_t freyjaMeshVertexTrackNew(index_t mesh, vec_t duration, vec_t rate)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		index_t track = m->NewVertexAnimTrack();
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		t.SetRate((rate < 15.0f) ? 15.0f : rate);
		t.SetDuration((duration < 10.0f) ? 10.0f : duration);

		return track;
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexKeyFrameCount(index_t mesh, index_t track)
{

	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		return t.GetKeyframeCount();
	}

	return 0;
}


index_t freyjaMeshVertexKeyFrameNew(index_t mesh, index_t track, vec_t time)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		index_t key = t.NewKeyframe(time);
		VertexAnimKeyFrame *k = t.GetKeyframe(key);
		
		if (k)
		{
			k->ArrayResize(m->GetVertexCount());
			return key;
		}
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexTrackCount(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		return m->GetVertexAnimTrackCount();
	}

	return 0;
}


void freyjaMeshVertexKeyFrame3f(index_t mesh, index_t track, index_t key,
								uint32 vert, vec_t x, vec_t y, vec_t z)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		VertexAnimKeyFrame *k = t.GetKeyframe(key);

		if (k)
		{
			//MSTL_ASSERTMSG(false, "Debug me");
			k->SetPos(vert, Vec3(x, y, z));
		}
	}
}


void freyjaMeshPolygonSplit(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);
	
	if (mesh)
	{
		mesh->SplitFace(polygonIndex);
	}
}


void freyjaMeshPolygonExtrudeQuad1f(index_t meshIndex, index_t polygonIndex, vec3_t v)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);
	if (mesh) 
		mesh->ExtrudeFace(polygonIndex, v);
}


index_t freyjaMeshTexCoordWeld(index_t mesh, index_t a, index_t b)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		if (m->WeldTexCoords(a, b))
			return a;
	}	

	return INDEX_INVALID;
}


const char *freyjaGetMeshNameString(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		return mesh->GetName();
	}

	return 0x0;
}


void freyjaGetMeshName1s(index_t meshIndex, int32 lenght, char *name)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (name)
	{
		name[0] = 0;
	}

	if (mesh)
	{
		strncpy(name, mesh->GetName(), (lenght < 32) ? lenght : 32);
		name[lenght-1] = 0;
	}
}


void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		Vec3(mesh->GetPosition()).Get(xyz);
	}
}


void freyjaMeshMaterial(index_t meshIndex, uint32 materialIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		for (uint32 i = 0, n = mesh->GetFaceCount(); i < n; ++i)
		{
			Face *f = mesh->GetFace(i);

			if (f)
			{
				f->mMaterial = materialIndex;
			}
		}

		mesh->SetMaterial(materialIndex);
	}
}


void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


void freyjaGetMeshFrameBoundingBox(index_t meshIndex, uint32 frame, 
									vec3_t min, vec3_t max)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetBBox(min, max);
	}
}



void freyjaMeshName1s(index_t meshIndex, const char *name)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetName(name);
	}
}


void freyjaMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


uint32 freyjaGetMeshCount()
{
	return Mesh::GetCount();
}


void freyjaGetMeshBoundingBox(index_t mesh, vec3_t min, vec3_t max)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		m->GetBBox(min, max);
	}
}


void freyjaGetMeshBoundingSphere(index_t mesh, vec3_t origin, vec_t &radius)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vec3 o = m->GetBoundingVolumeCenter();
		o.Get(origin);
		radius = m->GetBoundingVolumeRadius();
	}
}


uint32 freyjaGetMeshWeightCount(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	uint32 count = 0;

	if (m)
	{
		count = m->GetWeightCount();
	}

	return count;
}


void freyjaGetMeshWeight(index_t mesh, index_t weight,
						 index_t &vertex, index_t &bone, vec_t &weightv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Weight *w = m->GetWeight(weight);

		if (w)
		{
			vertex = w->mVertexIndex;
			bone = w->mBoneIndex;
			weightv = w->mWeight;
		}
	}
}


index_t freyjaGetMeshVertexPolygonRefIndex(index_t mesh, 
										   index_t vertex, uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v && element < v->GetFaceRefs().size())
		{
			return v->GetFaceRefs()[element];
		}
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexPolygonRefCount(index_t mesh, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			return v->GetFaceRefs().size();
		}
	}

	return 0;
}


void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		Vec3(mesh->GetPosition()).Get(xyz);
	}
}


uint32 freyjaGetMeshPolygonCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		return mesh->GetFaceCount();
	}

	return 0;
}


void freyjaMeshPolygonGroup1u(index_t mesh, index_t face, uint32 group)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->SetFaceSmoothingGroup(face, group);
	}
}


uint32 freyjaGetMeshNormalCount(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->GetNormalCount();
	}

	return 0;
}


uint32 freyjaGetMeshVertexCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		return mesh->GetVertexCount();
	}

	return 0;
}


uint32 freyjaGetMeshTexCoordCount(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	return m ? m->GetTexCoordCount() : 0;
}


void freyjaMeshPolygonClearFlag1u(index_t mesh, index_t face, byte flag)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->ClearFaceFlags(face, flag);
	}
}


void freyjaMeshPolygonSetFlag1u(index_t mesh, index_t face, byte flag)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->SetFaceFlags(face, flag);
	}
}


void freyjaMeshGenerateVertexNormals(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (!mesh)
		return;

	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = mesh->GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = mesh->GetVertex(v);

		if (vertex)
			vertex->GetTmpRefs().clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = mesh->GetFaceCount(); f < fn; ++f)
	{
		Face *face = mesh->GetFace(f);

		if (!face)
		{
			faceNormals.pushBack(normal);  // For alignment purposes
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = mesh->GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->GetTmpRefs().pushBack(f);
		}

		mesh->GetVertexPos(face->mIndices[0], a.mVec);
		mesh->GetVertexPos(face->mIndices[1], b.mVec);
		mesh->GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vec3::Cross(a - b, c - b);
		normal.Norm();
		faceNormals.pushBack(normal);
		//DEBUG_MSGF("face %i. %f %f %f\n", f, normal.mVec[0], normal.mVec[1], normal.mVec[2]);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = mesh->GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = mesh->GetVertex(v);

		if (!vertex)
			continue;

		normal.Zero();

		//FREYJA_ASSERTMSG(vertex->mPolyRefIndices.size(), "WTF vertex[%i] with no faces...", v);

		if (vertex->GetTmpRefs().size() == 0)
		{
			//DEBUG_MSGF("WTF vertex[%i] with no faces...", v);
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR Bad face reference\n");
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
		}


		normal.Norm();

		//DEBUG_MSGF("vert %i. %f %f %f\n", v, normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		mesh->SetNormal(v, normal.mVec);
    }
}


void freyjaMeshNormalFlip(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (!mesh)
		return;

	Vec3 normal;

    for (uint32 n = 0, nn = mesh->GetNormalCount(); n < nn; ++n)
    {
		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		mesh->GetNormal(n, normal.mVec);
		normal = -normal;
		mesh->SetNormal(n, normal.mVec);
	}
}


uint32 freyjaGetMeshPolygonEdgeCount(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);
	
		if (f)
		{
			return f->mIndices.size();
		}
	}

	return 0;
}


void freyjaMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		vec3_t uvw = { uv[0], uv[1], 0.0f };
		m->SetTexCoord(texcoord, uvw);
	}
}


void freyjaGetMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		vec3_t uvw;
		m->GetTexCoord(texcoord, uvw);
		uv[0] = uvw[0];
		uv[1] = uvw[1];
	}
}


void freyjaMeshUVMapPlanar(index_t meshIndex)
{
	uint32 vertexCount = freyjaGetMeshVertexCount(meshIndex);
	uint32 idx;
	vec3_t xyz;
	vec2_t uv;
	vec_t s;

    for (uint32 i = 0; i < vertexCount; ++i)
    {
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		uv[0] = (xyz[0] > 0) ? xyz[0] : -xyz[0];
		s = 0.025;
  
		while (uv[0] > 1.0)
		{
			uv[0] *= s;
			s *= 0.01;
		}
  
		uv[1] = (xyz[1] > 0) ? xyz[1] : -xyz[1];
		s = 0.025;
		
		while (uv[1] > 1.0)
		{
			uv[1] *= s;
			s *= 0.01;
		}
		
		uv[0] = 1.0 - uv[0];
		uv[1] = 1.0 - uv[1];

		idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


index_t freyjaMeshCopy(index_t meshIndex)
{
	Mesh *m = freyjaGetMeshClass(meshIndex);
	
	if (m)
	{
		Mesh *meshCopy = new Mesh(*m);
		return meshCopy->AddToPool();
	}

	return INDEX_INVALID;
}


void freyjaMeshUVMapSpherical(index_t meshIndex)
{
	int32 i, vertexCount;
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uv;

	Mesh *m = freyjaGetMeshClass(meshIndex);

	if (!m)
		return;

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = latitude;

		//freyjaVertexTexCoord2fv(vertexIndex, uv);
		index_t idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


void freyjaMeshUVMapCylindrical(index_t meshIndex)
{
	int32 i, j, vertexCount;
	vec_t longitude, latitude, ysize;
	vec3_t xyz;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};
	vec2_t uv;

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		for (j = 0; j < 3; ++j)
		{
			if (xyz[j] < min[j])
				min[j] = xyz[j];

			if (xyz[j] > max[j])
				max[j] = xyz[j];
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

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = xyz[1] / ysize;

		//freyjaVertexTexCoord2fv(vertexIndex, uv);
		index_t idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


#include "Bone.h"

void freyjaMeshUpdateBlendVertices(index_t mesh, index_t track, vec_t time)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m)
	{
		// Make sure we have enough blend vertices, and
		// this is 'smart' enough not to reallocate if we do
		m->SyncBlendVertices();
		vec_t *array = m->GetBlendVerticesArray();
		m->ResetBlendVertices();

		// Forget about 'cobbling' random skeletons with reused weights!
		//Skeleton *s = Skeleton::getSkeleton(skeleton);
		
		// Good case for threads if we cache all the uses of a bone and
		// divide among vertices


		/* Matrix animation transform lists */
		for (uint32 i = 0, n = Bone::GetCount(); i < n; ++i)
		{
			Bone *b = Bone::GetBone(i);

			if (b)
			{
				b->UpdateWorldPose(track, time);
			}
		}

		// Looks like I might go with relative transforms for now...

		// FIXME: Decide on which skinning method to enforce.
		// FIXME: Come back and rework this to avoid all this branching.
		for (uint32 i = 0, n = m->GetWeightCount(); i < n; ++i)
		{
			Weight *w = m->GetWeight(i);

			if (!w)
				continue;

			Bone *b = Bone::GetBone(w->mBoneIndex);

			if (!b)
				continue;

			Vertex *v = m->GetVertex(w->mVertexIndex);

			if (!v)
				continue;

			Vec3 p;
			m->GetVertexArrayPos(v->mVertexIndex, p.mVec);


#if 0
			// The 'right' way
			hel::Mat44 combined;	
			helPostMatrixMultiply(b->GetInverseBindPose().mMatrix,
								  b->GetWorldPose().mMatrix,
								  combined.mMatrix);

			p = (combined * p) * w->mWeight;
#else
			p = (b->GetInverseBindPose() * p);
			p = (b->GetWorldPose() * p);
			p *= w->mWeight;
#endif

			array[v->mVertexIndex*3  ] += p.mVec[0]; 
			array[v->mVertexIndex*3+1] += p.mVec[1]; 
			array[v->mVertexIndex*3+2] += p.mVec[2]; 
		}
	}
}


vec_t *freyjaGetMeshBlendVertices(index_t mesh)
{

	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m) return m->GetBlendVerticesArray();

	return NULL;
}


void freyjaDebugMeshBlendVertices(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m && m->GetBlendVerticesArray()) 
	{
		vec_t *array = m->GetBlendVerticesArray();
	
		for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			Vertex *v = m->GetVertex(i);

			if (!v)
				continue;
			
			Vec3 p;
			m->GetVertexArrayPos(v->mVertexIndex, p.mVec);

			printf("%i, mesh, %f, %f, %f\n", 
				   i, p.mVec[0], p.mVec[1], p.mVec[2]);
			printf("%i, blnd, %f, %f, %f\n", 
				   i, array[i*3], array[i*3+1], array[i*3+2]);
		}
	}
}


void freyjaMeshClampTexCoords(index_t meshIndex)
{
	uint32 count = freyjaGetMeshTexCoordCount(meshIndex);
	vec2_t uv;

	for (uint32 i = 0; i < count; ++i)
	{
		freyjaGetMeshTexCoord2fv(meshIndex, i, uv);

		for (int32 j = 1; j >= 0; --j)
		{
			if (uv[j] < 0.0f)
			{
				uv[j] = 0.0f;
			}
			else if (uv[j] > 1.0f)
			{
				uv[j] = 1.0f;
			}
		}
		
		freyjaMeshTexCoord2fv(meshIndex, i, uv);
	}
}


byte freyjaIsMeshSelected(index_t mesh)
{
	Mesh *m = Mesh::GetMesh(mesh);

	if (m)
	{
		return (m->GetFlags() & Mesh::fSelected);
	}

	return 0;
}


index_t freyjaMeshCreateSheet(vec3_t origin, vec_t size, 
							  uint32 rows, uint32 columns)
{
	rows = (rows > 0) ? rows : 1;
	columns = (columns > 0) ? columns : 1;

	++rows;
	++columns;

	const index_t material = 0;
	index_t mesh = freyjaMeshCreate();

	{
		char name[64];
		snprintf(name, 63, "Sheet-%i", mesh);
		name[63] = '\0';
		freyjaMeshName1s(mesh, name);
	}

	/* Generate vertices and texcoords */
	Vec3 p, o(origin), n(0.0f, 1.0f, 0.0f);
	vec_t invRows = 1.0f / rows, invCols = 1.0f / columns;
	vec_t u = 0.0f, v = 0.0f;
	mstl::Vector<index_t> uvs;

	for (uint32 i = 0; i < rows; ++i)
	{
		u = (vec_t)i * invRows;

		for (uint32 j = 0; j < columns; ++j)
		{
			v = (vec_t)j * invCols;

			p = o + Vec3(size * u, 0.0f, size * v);
			index_t vertex = freyjaMeshVertexCreate3fv(mesh, p.mVec);
			freyjaMeshVertexNormal3fv(mesh, vertex, n.mVec);
			index_t idx = freyjaMeshTexCoordCreate2f(mesh, u, v);
			uvs.push_back(idx);
		}
	}


	/* Generate faces */
	index_t a, b, c, d;

	for (uint32 i = 0; i < (rows - 1); ++i)
	{
		for (uint32 j = 1; j < (columns - 1); ++j)
		{
			a = j + i * columns;
			b = j + 1 + i * columns;
			c = b + columns;
			d = a + columns;

			/* Now generate quad, texcoord == vertex index */
			index_t face = freyjaMeshPolygonCreate(mesh);
			freyjaMeshPolygonAddVertex1i(mesh, face, a);
			freyjaMeshPolygonAddVertex1i(mesh, face, b);
			freyjaMeshPolygonAddVertex1i(mesh, face, c);
			freyjaMeshPolygonAddVertex1i(mesh, face, d);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[a]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[b]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[c]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[d]);
			freyjaMeshPolygonMaterial(mesh, face, material);
		}
	}

	for (uint32 i = 0; i < (rows - 1); ++i)
	{
		a = (columns * i);
		b = a + 1;
		c = b + columns;
		d = a + columns;

		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonAddVertex1i(mesh, face, a);
		freyjaMeshPolygonAddVertex1i(mesh, face, b);
		freyjaMeshPolygonAddVertex1i(mesh, face, c);
		freyjaMeshPolygonAddVertex1i(mesh, face, d);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[a]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[b]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[c]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, uvs[d]);
		freyjaMeshPolygonMaterial(mesh, face, material);
	}

	return mesh;
}


index_t freyjaMeshCreateCube(vec3_t origin, vec_t size)
{
	Vector<index_t> vertices, texcoords;
	Vec3 v(origin), n;

	index_t mesh = freyjaMeshCreate();

	{
		char name[64];
		snprintf(name, 63, "Cube%i", mesh);
		name[63] = '\0';
		freyjaMeshName1s(mesh, name);
	}

	index_t vertex;
	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(0.0, size, 0.0)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(-0.33, 0.33, -0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(size, size, 0.0)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(0.33, 0.33, -0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(size, 0.0, 0.0)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(0.33, -0.33, -0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, v.mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(-0.33, -0.33, -0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(0.0, size, size)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(-0.33, 0.33, 0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(size, size, size)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(0.33, 0.33, 0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(size, 0.0, size)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(0.33, -0.33, 0.33)).mVec);
	vertices.push_back(vertex);

	vertex = freyjaMeshVertexCreate3fv(mesh, (v + Vec3(0.0, 0.0, size)).mVec);
	freyjaMeshVertexNormal3fv(mesh, vertex, (Vec3(-0.33, -0.33, 0.33)).mVec);
	vertices.push_back(vertex);


	/* Generate polymapped texcoords */
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.0, 0.0));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.25, 0.0));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.25, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.0, 0.25));

	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.25, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.5, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.5, 0.5));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.25, 0.5));

	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.5, 0.5));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.5));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.75));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.5, 0.75));

	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.75));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 0.75));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 1.0));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 1.0));

	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.0));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 0.0));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.25));

	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 0.25));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 1.0, 0.50));
	texcoords.push_back(freyjaMeshTexCoordCreate2f(mesh, 0.75, 0.50));


	/* Now generate mesh */
	index_t face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[0]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[1]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[2]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[3]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[4]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[5]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[6]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[7]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[8]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[9]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[10]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[11]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[12]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[13]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[14]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[15]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[3]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[16]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[1]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[17]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[2]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[18]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[19]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[20]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[4]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[21]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[5]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[22]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[6]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[23]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[7]);
	freyjaMeshPolygonMaterial(mesh, face, 0);

	return mesh;
}


// 'Pinwheel' circle
index_t freyjaMeshCreateCircle(vec3_t origin, vec_t radius, uint32 count)
{
	if (count < 3)
		count = 3;

	index_t mesh = freyjaMeshCreate();

	/* Generate geometery */
	hel::Vec3 n(0.0f, 1.0f, 0.0f);
	hel::Vec3 o(origin);
	index_t center = freyjaMeshVertexCreate3fv(mesh, o.mVec);
	freyjaMeshVertexNormal3fv(mesh, center, n.mVec);
	index_t centerUV = freyjaMeshTexCoordCreate2f(mesh, 0.5f, 0.5f);
	mstl::Vector<index_t> vertices, texcoords;
	const vec_t invCount = 1.0f / (float)count;

	for (uint32 i = 0; i < count; ++i)
	{
		hel::Vec3 u;
		helSinCosf(helDegToRad(360.0f * ((float)i * invCount)), &u.mZ, &u.mX);

		index_t t = freyjaMeshTexCoordCreate2f(mesh, 
											   0.5f - u.mX * 0.5f, 
											   0.5f - u.mZ * 0.5f);
		texcoords.push_back(t);

		u *= radius;
		u += o;

		index_t v = freyjaMeshVertexCreate3fv(mesh, u.mVec);
		freyjaMeshVertexNormal3fv(mesh, v, n.mVec);
		vertices.push_back(v);
	}


	// i = 0
	index_t face = freyjaMeshPolygonCreate(mesh);
	freyjaMeshPolygonMaterial(mesh, face, 0);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, centerUV);
	freyjaMeshPolygonAddVertex1i(mesh, face, center);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[0]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[0]);
	freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[count-1]);
	freyjaMeshPolygonAddVertex1i(mesh, face, vertices[count-1]);

	for (uint32 i = 1; i < count; ++i)
	{
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, centerUV);
		freyjaMeshPolygonAddVertex1i(mesh, face, center);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[i]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[i]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[i-1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[i-1]);
	}

	return mesh;
}


// 'Quad' circle
index_t freyjaMeshCreateCircleQuad(vec3_t origin, vec_t radius, 
								   uint32 count, uint32 rings)
{
	if (count < 3)
		count = 3;

	index_t mesh = freyjaMeshCreate();

	/* Generate geometery */
	hel::Vec3 n(0.0f, 1.0f, 0.0f);
	hel::Vec3 o(origin);
	mstl::Vector<index_t> vertices[rings], texcoords[rings];
	const vec_t invCount = 1.0f / (float)count;
	const vec_t invRings = 1.0f / (float)rings;

	for (uint32 i = 0; i < count; ++i)
	{
		hel::Vec3 u;
		helSinCosf(helDegToRad(360.0f * ((float)i * invCount)), &u.mZ, &u.mX);

		for (uint32 j = 0; j < rings; ++j)
		{
			vec_t s = 1.0f * ((float)j * invRings);

			if (!j) { s = 1.0f * (0.5f * invRings); }

			index_t t = freyjaMeshTexCoordCreate2f(mesh, 
												   0.5f - u.mX * 0.5f * s, 
												   0.5f - u.mZ * 0.5f * s);
			texcoords[j].push_back(t);

			hel::Vec3 u2(o + u * radius * s);
			index_t v2 = freyjaMeshVertexCreate3fv(mesh, u2.mVec);
			freyjaMeshVertexNormal3fv(mesh, v2, n.mVec);
			vertices[j].push_back(v2);
		}
	}

	//index_t center = freyjaMeshVertexCreate3fv(mesh, o.mVec);
	//freyjaMeshVertexNormal3fv(mesh, center, n.mVec);
	//index_t centerUV = freyjaMeshTexCoordCreate2f(mesh, 0.5f, 0.5f);

	for (uint32 j = 0; j < (rings - 1); ++j)
	{
		// i = 0
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][0]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][0]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][count-1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][count-1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][count-1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][count-1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][0]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][0]);
	
		freyjaPrintMessage("%i = %i?", j, vertices[j][0]);

		for (uint32 i = 1; i < count; ++i)
		{
			index_t face = freyjaMeshPolygonCreate(mesh);
			freyjaMeshPolygonMaterial(mesh, face, 0);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][i]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][i]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][i-1]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][i-1]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][i-1]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][i-1]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][i]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][i]);
		}
	}

	return mesh;
}


index_t freyjaMeshCreateRing(vec3_t origin, vec_t radius, 
							 uint32 count, uint32 rings)
{
	if (count < 3)
		count = 3;

	index_t mesh = freyjaMeshCreate();

	/* Generate geometery */
	hel::Vec3 n(0.0f, 1.0f, 0.0f);
	hel::Vec3 o(origin);
	mstl::Vector<index_t> vertices[rings], texcoords[rings];
	const vec_t invCount = 1.0f / (float)count;
	const vec_t invRings = 1.0f / (float)rings;

	for (uint32 i = 0; i < count; ++i)
	{
		hel::Vec3 u;
		helSinCosf(helDegToRad(360.0f * ((float)i * invCount)), &u.mZ, &u.mX);

		for (uint32 j = 0; j < rings; ++j)
		{
			vec_t s = 1.0f * ((float)j * invRings);

			if (!j) { s = 1.0f * (0.5f * invRings); }

			index_t t = freyjaMeshTexCoordCreate2f(mesh, 
												   0.5f - u.mX * 0.5f * s, 
												   0.5f - u.mZ * 0.5f * s);
			texcoords[j].push_back(t);

			hel::Vec3 u2(o + u * radius * s);
			index_t v2 = freyjaMeshVertexCreate3fv(mesh, u2.mVec);
			freyjaMeshVertexNormal3fv(mesh, v2, n.mVec);
			vertices[j].push_back(v2);
		}
	}


	for (uint32 j = 0; j < (rings - 1); ++j)
	{
		// i = 0
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][0]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][0]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][count-1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][count-1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][count-1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][count-1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][0]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][0]);
	

		for (uint32 i = 1; i < count; ++i)
		{
			index_t face = freyjaMeshPolygonCreate(mesh);
			freyjaMeshPolygonMaterial(mesh, face, 0);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][i]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][i]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j][i-1]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j][i-1]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][i-1]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][i-1]);
			freyjaMeshPolygonAddVertex1i(mesh, face, vertices[j+1][i]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoords[j+1][i]);
		}
	}

	return mesh;
}


index_t freyjaMeshCreateCone(vec3_t origin, vec_t height, vec_t radius,
							 uint32 wedges)
{
	MSTL_MSG("Calling stub function");
	return INDEX_INVALID;
}


index_t freyjaMeshCreateCylinder(vec3_t origin, vec_t height, vec_t radius, 
								 uint32 sides, uint32 rings)
{
	MSTL_MSG("Calling stub function");
	return INDEX_INVALID;
}


index_t freyjaMeshCreateSphere(vec3_t origin, vec_t radius, 
							   int32 sides, int32 rings)
{
	MSTL_MSG("Calling stub function");
	return INDEX_INVALID;
}


index_t freyjaMeshCreateTube(vec3_t origin, vec_t height, vec_t radius, 
							 int32 sides, int32 rings)
{
#if 1
	FREYJA_ASSERTMSG(0, "Calling stub function");
	return INDEX_INVALID;
#else
	(segments < 1) ? segments = 1 : 0;
	(sides < 3) ? sides = 3 : 0;

	index_t mesh = freyjaMeshCreate();

	/* Generate geometery */

	vec_t invCount = 1.0f / (float)sides;
	hel::Vec3 v, n, t, o(origin);

	/* Bottom and top */
	for (uint32 i = 0; i < sides; ++i)
	{
		vec_t s = ((float)i * invCount);
		helSinCosf(helDegToRad(360.0f * s), &v.mZ, &v.mX);

		v *= s;
		v.mY = height;

		u = (v.mX < 0) ? (v.mX * 0.25 + 0.25) : (v.mX * 0.25 + 0.25);
		v = (v.mZ < 0) ? (v.mZ * 0.25 + 0.25) : (v.mZ * 0.25 + 0.25);

		n = v * 0.2f;
		n.mY = -0.6;

		t = o;
		t.mX += v.mX;
		t.mZ += v.mZ;

		index_t idx = freyjaMeshVertexCreate3fv(mesh, v.mVec);
		freyjaMeshVertexNormal3fv(mesh, idx, n.mVec);
		//vertices[i].push_back(idx);

		index_t t = freyjaMeshTexCoordCreate2f(mesh, u, v);
		//texcoords[j].push_back(t);


		t = o + v;
		n.mY = -n.mY;

		idx = freyjaMeshVertexCreate3fv(mesh, v.mVec);
		freyjaMeshVertexNormal3fv(mesh, idx, n.mVec);
		//vertices[i].push_back(idx);

		t = freyjaMeshTexCoordCreate2f(mesh, u, v);
		//texcoords[j].push_back(t);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (uint32 i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (uint32 j = 0; j < sides; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		/* Reuse top vertices for segment-th ring */
		if (i == segments)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices2[j];
				segVert.pushBack(index);

				index = freyjaTexCoordCreate2f(u, v);
				segTex.pushBack(index);
			}

			continue;
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));
			y = height * ((float)i/(float)segments);

			u = 1.0 * ((float)j/(float)count);
			v = 0.5 * ((float)i/(float)segments) + 0.5;

			nx = x * 0.5;
			ny = 0.0;
			nz = z * 0.5;

			index = freyjaVertexCreate3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoordCreate2f(u, v);
			segTex.pushBack(index);
		}
	}



	/* Tube */
	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < count; ++j)
		{
			/* Make the 0th/count edge of rings quad, then the rest in order */
			if (!j)
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index]);
				freyjaPolygonVertex1i(segVert[index]);

				// B
				freyjaPolygonTexCoord1i(segTex[index + count-1]);
				freyjaPolygonVertex1i(segVert[index + count-1]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2 + count-1]);
				freyjaPolygonVertex1i(segVert[index2 + count-1]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2]);
				freyjaPolygonVertex1i(segVert[index2]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
			else
			{
				index = count * i;
				index2 = count * (i + 1);
				freyjaBegin(FREYJA_POLYGON);
				// A
				freyjaPolygonTexCoord1i(segTex[index+j]);
				freyjaPolygonVertex1i(segVert[index+j]);

				// B
				freyjaPolygonTexCoord1i(segTex[index-1+j]);
				freyjaPolygonVertex1i(segVert[index-1+j]);

				// C
				freyjaPolygonTexCoord1i(segTex[index2-1+j]);
				freyjaPolygonVertex1i(segVert[index2-1+j]);

				// D
				freyjaPolygonTexCoord1i(segTex[index2+j]);
				freyjaPolygonVertex1i(segVert[index2+j]);
				freyjaPolygonMaterial1i(0);
				freyjaEnd(); // FREYJA_POLYGON
			}
		}
	}

	return mesh;
#endif
}


void freyjaMeshTesselateTriangles(index_t mesh)
{
	Vector<index_t> purge;
	index_t polygon;
	uint32 polygonCount = freyjaGetMeshPolygonCount(mesh);

	for (uint32 i = 0, vertexCount = 0; i < polygonCount; ++i)
	{
		polygon = i;  

		// You can check the polygon to be sure it's allocated, but
		// unallocated polygons have zero vertices anyway
		vertexCount = freyjaGetMeshPolygonVertexCount(mesh, polygon);

		// For the sake of compatibility only support quads for now
		switch (vertexCount)
		{
		case 0: // Unallocated or empty
		case 1: // Point?
		case 2: // Line?
		case 3: // Already a triangle
			break;

		case 4: // Quad
			{
				index_t a, b, c, d, ta, tb, tc, td;
				index_t material = freyjaGetMeshPolygonMaterial(mesh, polygon);

				// FIXME: Might want to copy flags too!

				/* 1. Get ABCD quad vertices */
				a = freyjaGetMeshPolygonVertexIndex(mesh, polygon, 0);
				b = freyjaGetMeshPolygonVertexIndex(mesh, polygon, 1);
				c = freyjaGetMeshPolygonVertexIndex(mesh, polygon, 2);
				d = freyjaGetMeshPolygonVertexIndex(mesh, polygon, 3);

				if (freyjaGetMeshPolygonTexCoordCount(mesh, polygon))
				{
					ta = freyjaGetMeshPolygonTexCoordIndex(mesh, polygon, 0);
					tb = freyjaGetMeshPolygonTexCoordIndex(mesh, polygon, 1);
					tc = freyjaGetMeshPolygonTexCoordIndex(mesh, polygon, 2);
					td = freyjaGetMeshPolygonTexCoordIndex(mesh, polygon, 3);
				}

				/* 2. Make ABC ACD triangles */
				index_t triangle = freyjaMeshPolygonCreate(mesh);
				freyjaMeshPolygonMaterial(mesh, triangle, material);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, a);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, b);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, c);

				if (freyjaGetMeshPolygonTexCoordCount(mesh, polygon))
				{
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, ta);
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, tb);
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, tc);
				}


				triangle = freyjaMeshPolygonCreate(mesh);
				freyjaMeshPolygonMaterial(mesh, triangle, material);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, a);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, c);
				freyjaMeshPolygonAddVertex1i(mesh, triangle, d);

				if (freyjaGetMeshPolygonTexCoordCount(mesh, polygon))
				{
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, ta);
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, tc);
					freyjaMeshPolygonAddTexCoord1i(mesh, triangle, td);
				}

				/* 3. Prepare to remove ABCD polygon and update references */
				purge.push_back(polygon);
			}
			break;

		default:
			MSTL_MSG("Can not tesselate face[%i] with %i sides.", 
					 polygon, vertexCount);
		}
	}

	// Remove the faces we just tesselated
	uint32 i;
	foreach (purge, i)
	{
		freyjaMeshPolygonDelete(mesh, purge[i]);
	}
}

