/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and EggPlugin.
 *
 *             This common interface makes it possible to share ABI 
 *             compatible binary C/C++ plugins between FreyjaPlugin and
 *             EggPlugin.  ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#include <mstl/Vector.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>
#include <hel/BoundingVolume.h>

#include "FreyjaFileWriter.h"
#include "EggPlugin.h" // temp for testing
#include "FreyjaPluginABI.h"


long freyjaCheckModel(char *filename)
{
	FreyjaFileReader r;
	freyja_file_header_t header;


	if (!r.openFile(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.littleEndianMode();

	/* Read header */
	r.readCharString(8, header.magic);
	header.version = r.readLong();
	header.flags = r.readLong();
	header.reserved = r.readLong();
	r.readCharString(64, header.comment);	
	r.closeFile();

	if (!strncmp(header.magic, FREYJA_FILE_MAGIC, 7))
	{
		return 0;
	}

	return -1;
}


long freyjaLoadModel(char *filename)
{
	FreyjaFileReader r;
	freyja_file_header_t header;
	Vector<long> vertices, texcoords;
	freyja_file_chunk_t chunk;
	long offset, i, j, n, flags, index, count;
	vec4_t wxyz;
	vec3_t xyz;
	vec2_t uv;
	char buffer[64];


	if (freyjaCheckModel((char *)filename) != 0)
		return -1;

	if (!r.openFile(filename))
		return -1;

	/* Data is LITTLE_ENDIAN */
	r.littleEndianMode();

	/* Read header */
	r.readCharString(8, header.magic);
	header.version = r.readLong();
	header.flags = r.readLong();
	header.reserved = r.readLong();
	r.readCharString(64, header.comment);	

	if (strncmp(header.magic, FREYJA_FILE_MAGIC, 7))
	{
		return -1;
	}

	while (!r.endOfFile())
	{
		chunk.type = r.readLong();
		chunk.size = r.readLong();
		chunk.flags = r.readLong();
		chunk.version = r.readLong();
		offset = r.getFileOffset() + chunk.size;

		switch (chunk.type)
		{
		case FREYJA_CHUNK_BONE:
			printf("BONE\n");
			memset(buffer, 0, 64);
			r.readCharString(64, buffer);

			freyjaBegin(FREYJA_BONE);
			index = freyjaGetCurrent(FREYJA_BONE);
			freyjaBoneFlags1i(index, 0x0);
			freyjaBoneParent1i(index, r.readLong());
			freyjaBoneName1s(index, buffer);
		
			for (j = 0; j < 3; ++j)
				xyz[j] = r.readFloat32();

			freyjaBoneTranslate3fv(index, xyz);

			for (j = 0; j < 4; ++j)
				wxyz[j] = r.readFloat32();

			freyjaBoneRotateQuatWXYZ4fv(index, wxyz);
			break;


		case FREYJA_CHUNK_VERTICES:
			printf("VERT\n");
			count = r.readLong();

			for (i = 0; i < count; ++i)
			{
				for (j = 0; j < 3; ++j)
					xyz[j] = r.readFloat32();
				index = freyjaVertex3fv(xyz);

				for (j = 0; j < 3; ++j)
					xyz[j] = r.readFloat32();
				freyjaVertexNormal3fv(index, xyz);

				for (j = 0; j < 2; ++j)
					uv[j] = r.readFloat32();
				freyjaVertexTexCoord2fv(index, uv);

				vertices.pushBack(index);
			}
			break;


		case FREYJA_CHUNK_TEXCOORDS:
			printf("COOR\n");
			count = r.readLong();

			for (i = 0; i < count; ++i)
			{
				for (j = 0; j < 2; ++j)
					uv[j] = r.readFloat32();

				index = freyjaTexCoord2fv(uv);
				texcoords.pushBack(index);
			}
			break;


		case FREYJA_CHUNK_POLYGONS:
			printf("POLY\n");
			count = r.readLong();

			for (i = 0; i < count; ++i)
			{
				freyjaBegin(FREYJA_POLYGON);

				freyjaPolygonMaterial1i(r.readLong());
				flags = r.readLong();
				n = r.readLong();

				for (j = 0; j < n; ++j)
				{
					index = r.readLong();
					freyjaPolygonVertex1i(index);//vertices[index]);
				}

				if (flags & fPolygon_PolyMapped)
				{
					n = r.readLong();

					for (j = 0; j < n; ++j)
					{
						index = r.readLong();
						freyjaPolygonTexCoord1i(index);//vertices[index]);
					}
				}
				else if  (flags & fPolygon_ColorMapped)
				{
					n = r.readLong() * 2;

					for (j = 0; j < n; ++j)
					{
						index = r.readLong();
						freyjaPolygonTexCoord1i(index);//vertices[index]);
					}
				}

				freyjaEnd();
			}
			break;


		case FREYJA_CHUNK_MESH:
			flags = r.readLong();

			count = r.readLong();
			for (j = 0; j < count; ++j)
				;//freyjaMeshPolygon1u(r.readLong());

			count = r.readLong();
			for (j = 0; j < count; ++j)
				;//freyjaMeshGroup1u(r.readLong());
			break;


		default:
			continue;
		}

		r.setFileOffset(offset);
	}

	r.closeFile();

	return 0;
}


long freyjaSaveModel(char *filename)
{
	Vector<long> vertices, texcoords;
	FreyjaFileWriter w;
	freyja_file_header_t header;
	freyja_file_chunk_t chunk;
	vec3_t xyz;
	vec4_t wxyz;
	char buffer[64];
	long i, j, n, index, idx;
	long count, flags, material;
	vec2_t uv;


	if (!w.openFile(filename))
		return -1;

	memset(header.magic, 0, 8);
	memset(header.comment, 0, 64);
	strncpy(header.magic, FREYJA_FILE_MAGIC, 7);
	header.version = FREYJA_FILE_VERSION;
	header.flags = 0x0;
	header.reserved = 0x0;
	strcpy(header.comment, "Freyja 3d: http://icculus.org/freyja");

	/* Data is LITTLE_ENDIAN */
	w.littleEndianMode();

	/* Write header */
	w.writeCharString(8, header.magic);
	w.writeLong(header.version);
	w.writeLong(header.flags);
	w.writeLong(header.reserved);
	w.writeCharString(64, header.comment);


	/* Write chunks... */


	/* Bones */
	if (freyjaGetCount(FREYJA_BONE))
	{
		freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);
		index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);
		count = freyjaGetCount(FREYJA_BONE);

		for (i = 0; i < count; ++i)
		{
			memset(buffer, 0, 64);
			freyjaGetBoneName(index, 64, buffer);
			idx = freyjaGetBoneParent(index);

			chunk.type = FREYJA_CHUNK_BONE;
			chunk.size = 64 + 4 + 12 + 16;
			chunk.flags = 0x0;
			chunk.version = 1;

			w.writeLong(chunk.type);
			w.writeLong(chunk.size);
			w.writeLong(chunk.flags);
			w.writeLong(chunk.version);
			w.writeCharString(64, buffer);
			w.writeLong(idx);

			freyjaGetBoneTranslation3fv(index, xyz);

			for (j = 0; j < 3; ++j)
				w.writeFloat32(xyz[j]);

			freyjaGetBoneRotationWXYZ4fv(index, wxyz);

			for (j = 0; j < 4; ++j)
				w.writeFloat32(wxyz[j]);

			index = freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
		}
	}


	/* Vertices */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	index = freyjaGetCurrent(FREYJA_VERTEX);
	count = freyjaGetCount(FREYJA_VERTEX);

	chunk.type = FREYJA_CHUNK_VERTICES;
	chunk.size = 4 + count * (12 + 12 + 8);
	chunk.flags = 0x0;
	chunk.version = 1;

	w.writeLong(chunk.type);
	w.writeLong(chunk.size);
	w.writeLong(chunk.flags);
	w.writeLong(chunk.version);

	w.writeLong(count);

	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3fv(xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexNormal3fv(xyz);
		for (j = 0; j < 3; ++j)
			w.writeFloat32(xyz[j]);

		freyjaGetVertexTexCoord2fv(uv);
		for (j = 0; j < 2; ++j)
			w.writeFloat32(uv[j]);

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
		vertices.pushBack(index);

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}


	/* Vertex Frames */
	// FIXME


	/* Vertex Weights */
	// FIXME
	

	/* Texcoords */
	freyjaIterator(FREYJA_TEXCOORD, FREYJA_LIST_RESET);
	index = freyjaGetCurrent(FREYJA_TEXCOORD);
	count = freyjaGetCount(FREYJA_TEXCOORD);

	chunk.type = FREYJA_CHUNK_TEXCOORDS;
	chunk.size = 4 + count * 8;
	chunk.flags = 0x0;
	chunk.version = 1;

	w.writeLong(chunk.type);
	w.writeLong(chunk.size);
	w.writeLong(chunk.flags);
	w.writeLong(chunk.version);

	w.writeLong(count);

	for (i = 0; i < count; ++i)
	{
		freyjaGetTexCoord2fv(index, uv);
		for (j = 0; j < 2; ++j)
			w.writeFloat32(uv[j]);

		index = freyjaIterator(FREYJA_TEXCOORD, FREYJA_LIST_CURRENT);
		texcoords.pushBack(index);

		freyjaIterator(FREYJA_TEXCOORD, FREYJA_LIST_NEXT);
	}


	/* Polygons */
	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
	index = freyjaGetCurrent(FREYJA_POLYGON);
	n = freyjaGetCount(FREYJA_POLYGON);

	chunk.type = FREYJA_CHUNK_POLYGONS;
	chunk.size = 4;
	chunk.flags = 0x0;
	chunk.version = 1;


	for (i = 0; i < n; ++i)
	{
		count = freyjaGetPolygonEdgeCount(index);
		chunk.size += 12;
		chunk.size += count * 4;

		if (flags & fPolygon_PolyMapped)
		{
			chunk.size += count * 4;
		}
		else if  (flags & fPolygon_ColorMapped)
		{
			chunk.size += count * 8;
		}

		index = freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	w.writeLong(chunk.type);
	w.writeLong(chunk.size);
	w.writeLong(chunk.flags);
	w.writeLong(chunk.version);

	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
	index = freyjaGetCurrent(FREYJA_POLYGON);
	n = freyjaGetCount(FREYJA_POLYGON);

	w.writeLong(n);

	for (i = 0; i < n; ++i)
	{
		material = freyjaGetPolygonMaterial(index);
		flags = freyjaGetPolygonFlags(index);
		count = freyjaGetPolygonEdgeCount(index);

		w.writeLong(material);
		w.writeLong(flags);
		w.writeLong(count);

		for (j = 0; j < count; ++j)
		{
			idx = freyjaGetPolygonVertexIndex(index, j);
			w.writeLong(vertices[idx]);
		}

		if (flags & fPolygon_PolyMapped)
		{
			w.writeLong(count);

			for (j = 0; j < count; ++j)
			{
				idx = freyjaGetPolygonTexCoordIndex(index, j);
				w.writeLong(texcoords[idx]);
			}
		}
		else if  (flags & fPolygon_ColorMapped)
		{
			w.writeLong(count);

			for (j = 0; j < count*2; ++j)
			{
				idx = freyjaGetPolygonTexCoordIndex(index, j);
				w.writeLong(texcoords[idx]);
			}
		}

		index = freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}


	/* Materials */


	/* Vertex groups */


	/* Meshes */
	freyjaIterator(FREYJA_MESH, FREYJA_LIST_RESET);
	index = freyjaGetCurrent(FREYJA_MESH);
	n = freyjaGetCount(FREYJA_MESH);

	for (i = 0; i < n; ++i)
	{
		if (freyjaGetMeshPolygonCount(index) < 1)
			continue;

		count = (4 + freyjaGetMeshPolygonCount(index) * 4 +
				 freyjaGetMeshVertexGroupCount(index) * 4);

		chunk.type = FREYJA_CHUNK_MESH;
		chunk.size = count;
		chunk.flags = 0x0;
		chunk.version = 1;

		w.writeLong(chunk.type);
		w.writeLong(chunk.size);
		w.writeLong(chunk.flags);
		w.writeLong(chunk.version);

		w.writeLong(0); // FIXME: Flags

		count = freyjaGetMeshPolygonCount(index);
		w.writeLong(count);
		for (j = 0; j < count; ++j)
			w.writeLong(freyjaGetMeshPolygonIndex(index, j));

		count = freyjaGetMeshVertexGroupCount(index);
		w.writeLong(count);
		for (j = 0; j < count; ++j)
			w.writeLong(freyjaGetMeshVertexGroupIndex(index, j));
		
		index = freyjaIterator(FREYJA_MESH, FREYJA_LIST_NEXT);
	}


	/* Metadata */


	/* That wasn't so bad, was it? */
	w.closeFile();

	return 0;
}


// TODO parent plugin class

/* UVMap generation utils */
void freyjaGenerateUVMapsFromMesh(long meshIndex)
{
#ifdef FIXME
	egg_texel_t *t = EggPlugin::mEggPlugin->getTexel(freyjaGetCurrent(FREYJA_VERTEX));
	Vector<long> seen;
	egg_polygon_t *poly;
	egg_texel_t *texel = _egg->getTexel(texCoordIndex);
	egg_texel_t *tex;
	Vector3d u, trans, min;
	long i, j, k;


	if (texel)
	{
		for (i = texel->ref.begin(); i < (int)texel->ref.end(); ++i)
		{
			poly = _egg->getPolygon(texel->ref[i]);

			if (poly)
			{
				for (j = poly->texel.begin(); j < (int)poly->texel.end(); ++j)
				{
					tex = _egg->getTexel(poly->texel[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->texel[j])
						{
							tex = 0x0;
							break;
						}
					}

					if (!tex) 
						continue;

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < trans.mVec[0])
						trans.mVec[0] = u.mVec[0];

					if (u.mVec[1] < trans.mVec[1])
						trans.mVec[1] = u.mVec[1];

					if (x) tex->st[0] = -tex->st[0];
					if (y) tex->st[1] = -tex->st[1];

					u = Vector3d(tex->st[0], tex->st[1], 0);

					if (u.mVec[0] < min.mVec[0])
						min.mVec[0] = u.mVec[0];

					if (u.mVec[1] < min.mVec[1])
						min.mVec[1] = u.mVec[1];

					seen.pushBack(poly->texel[j]);
				}
			}
		}

		u = trans;
		trans = trans - min;
		//trans += Vector3d(u.mVec[0]/2, u.mVec[1]/2, 0);

		for (k = seen.begin(); k < (int)seen.end(); ++k)
		{
			tex = _egg->getTexel(seen[k]);

			if (tex)
			{
				u = Vector3d(tex->st[0], tex->st[1], 0) + trans;

				tex->st[0] = u.mVec[0];
				tex->st[1] = u.mVec[1];
			}
		}
	}
	else
	{
		egg_vertex_t *vertex = _egg->getVertex(getCurrentVertex());
		egg_vertex_t *vert;

		for (i = vertex->ref.begin(); i < (int)vertex->ref.end(); ++i)
		{
			poly = _egg->getPolygon(vertex->ref[i]);

			if (poly)
			{
				for (j = poly->vertex.begin(); j < (int)poly->vertex.end(); ++j)
				{
					vert = _egg->getVertex(poly->vertex[j]);

					for (k = seen.begin(); k < (int)seen.end(); ++k)
					{
						if (seen[k] == (int)poly->vertex[j])
						{
							vert = 0x0;
							break;
						}
					}

					if (!vert) 
						continue;

					if (x) vert->uv[0] = -vert->uv[0];
					if (y) vert->uv[1] = -vert->uv[1];
					seen.pushBack(poly->vertex[j]);
				}
			}
		}
	}

#endif
}


/* Mesh generation utils */
void freyjaGenerateQuadPlaneMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	long index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 1.0));

	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON


	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateQuadCubeMesh(vec3_t origin, vec_t side)
{
	Vector<long> vertices, texcoords;
	long index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, 0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, 0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + 0.0);
	freyjaVertexNormal3f(index, -0.33, -0.33, -0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index =freyjaVertex3f(origin[0] + side, origin[1] + side, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, 0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + side, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, 0.33, -0.33, 0.33);
	vertices.pushBack(index);

	index = freyjaVertex3f(origin[0] + 0.0, origin[1] + 0.0, origin[2] + side);
	freyjaVertexNormal3f(index, -0.33, -0.33, 0.33);
	vertices.pushBack(index);

	freyjaEnd(); // FREYJA_VERTEX_GROUP 


	/* Generate polymapped texcoords */
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.0, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.25, 0.5));

	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.5));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(0.5, 0.75));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.75));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 1.0));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 1.0));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.0));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));

	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.25));
	texcoords.pushBack(freyjaTexCoord2f(1.0, 0.50));
	texcoords.pushBack(freyjaTexCoord2f(0.75, 0.50));


	/* Now generate mesh */
	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[0]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[1]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[2]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[3]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[4]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[5]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonTexCoord1i(texcoords[6]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[7]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[8]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[9]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[10]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonTexCoord1i(texcoords[11]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[12]);
	freyjaPolygonVertex1i(vertices[0]);
	freyjaPolygonTexCoord1i(texcoords[13]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[14]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[15]);
	freyjaPolygonVertex1i(vertices[3]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[16]);
	freyjaPolygonVertex1i(vertices[1]);
	freyjaPolygonTexCoord1i(texcoords[17]);
	freyjaPolygonVertex1i(vertices[2]);
	freyjaPolygonTexCoord1i(texcoords[18]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[19]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaBegin(FREYJA_POLYGON);
	freyjaPolygonTexCoord1i(texcoords[20]);
	freyjaPolygonVertex1i(vertices[4]);
	freyjaPolygonTexCoord1i(texcoords[21]);
	freyjaPolygonVertex1i(vertices[5]);
	freyjaPolygonTexCoord1i(texcoords[22]);
	freyjaPolygonVertex1i(vertices[6]);
	freyjaPolygonTexCoord1i(texcoords[23]);
	freyjaPolygonVertex1i(vertices[7]);
	freyjaPolygonMaterial1i(0);
	freyjaEnd(); // FREYJA_POLYGON

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateCircleMesh(vec3_t origin, long count)
{
	Vector<long> vertices, texcoords;
	long i, index, center, centerUV;
	vec_t x, z, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.5, 0.5);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		freyjaTexCoord2f(x, z);
		texcoords.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateConeMesh(vec3_t origin, vec_t height, long count)
{
	Vector<long> vertices, texcoords, texcoords2;
	long i, index, point, center, pointUV, centerUV;
	vec_t x, z, u, v, nx, ny, nz;

	
	if (count < 3)
		count = 3;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoord2f(0.75, 0.25);
	point = index;

	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	for (i = 0; i < count; ++i)
	{ 
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
		texcoords2.pushBack(index);
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP 

	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Cone */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateFunctionalSphereMesh(vec3_t origin, vec_t radius, 
										long count, long segments, 
										float(*gen)(long i, long j, long seg))
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	height = radius * 2.0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
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

			r = (*gen)(i, j, segments);
			r *= radius;

			index = freyjaVertex3f(origin[0]+x*r, origin[1]+y*r, origin[2]+z*r);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	/* Tube */
	for (i = 1; i < segments-1; ++i)
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

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateSphereMesh(vec3_t origin, vec_t radius, 
							  long count, long segments)
{
	Vector<long> segVert, segTex, topTex, bottomTex;
	long i, j, index, index2, top, bottom, bt, tt;
	vec_t x, z, y, u, v, nx, ny, nz, height, r;

	(radius < 0) ? radius = -radius : 0;
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	height = radius * 2.0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	for (i = 0; i < segments; ++i)
	{
		y = height * ((float)i/(float)segments);
		v = 0.5 * ((float)i/(float)segments) + 0.5;

		r = sin(helDegToRad(180 * ((float)i/(float)segments)));
		(i > segments/2) ? y = height*(1.0-r)+y*r : y *= r;

		if (i == 0)
		{
			bottom = freyjaVertex3f(origin[0], origin[1]+y, origin[2]);
			freyjaVertexNormal3f(bottom, 0.0, -1.0, 0.0);

			/* Padding, if iterator needs alignment */
			for (j = 0; j < count; ++j)
			{
				segVert.pushBack(-1);
				segTex.pushBack(-1);
			}
			continue;
		}
		else if (i == segments-1)
		{
			top = freyjaVertex3f(origin[0], origin[1]+height, origin[2]);
			freyjaVertexNormal3f(top, 0.0, 1.0, 0.0);
		}

		for (j = 0; j < count; ++j)
		{
			x = cos(helDegToRad(360.0 * ((float)j / (float)count)));
			z = sin(helDegToRad(360.0 * ((float)j / (float)count)));

			u = 1.0 * ((float)j/(float)count);

			nx = x * r;
			ny = (1.0 - r) * (i < segments/2) ? -0.3 : 0.3; // soften the halves
			nz = z * r;

			x *= r * radius;
			z *= r * radius;

			index = freyjaVertex3f(origin[0]+x, origin[1]+y, origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		
		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		index = freyjaTexCoord2f(u, v);
		topTex.pushBack(index);
		index = freyjaTexCoord2f(u+0.5, v);
		bottomTex.pushBack(index);
	}

	bt = freyjaTexCoord2f(0.75, 0.25);
	tt = freyjaTexCoord2f(0.25, 0.25);

	/* Tube */
	for (i = 0; i < segments-1; ++i)
	{
		if (i == 0)
		{
			index = count * 1;

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(bottomTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(bt);
					freyjaPolygonVertex1i(bottom);
					freyjaPolygonTexCoord1i(bottomTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(bottomTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}

			continue;
		}
		else if (i == segments-2)
		{
			index = count * (i + 1);

			for (j = 0; j < count; ++j)
			{
				if (!j)
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[0]);
					freyjaPolygonVertex1i(segVert[index]);
					freyjaPolygonTexCoord1i(topTex[count-1]);
					freyjaPolygonVertex1i(segVert[index+count-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON
				}
				else
				{
					/* Base */
					freyjaBegin(FREYJA_POLYGON);
					freyjaPolygonTexCoord1i(tt);
					freyjaPolygonVertex1i(top);
					freyjaPolygonTexCoord1i(topTex[j]);
					freyjaPolygonVertex1i(segVert[index+j]);
					freyjaPolygonTexCoord1i(topTex[j-1]);
					freyjaPolygonVertex1i(segVert[index+j-1]);
					freyjaPolygonMaterial1i(0);
					freyjaEnd(); // FREYJA_POLYGON					
				}
			}
		}

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

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateCylinderMesh(vec3_t origin, vec_t height, 
								long count, long segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2, point, center, pointUV, centerUV;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */
	index = freyjaVertex3f(origin[0], origin[1] + height, origin[2]);
	freyjaVertexNormal3f(index, 0.0, 1.0, 0.0);
	pointUV = freyjaTexCoord2f(0.75, 0.25);
	point = index;

	index = freyjaVertex3f(origin[0], origin[1], origin[2]);
	freyjaVertexNormal3f(index, 0.0, -1.0, 0.0);
	centerUV = freyjaTexCoord2f(0.25, 0.25);
	center = index;

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertex3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoord2f(u, v);
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

				index = freyjaTexCoord2f(u, v);
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

			index = freyjaVertex3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


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

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{ 
		if (!i)
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[0]);
			freyjaPolygonVertex1i(vertices[0]);
			freyjaPolygonTexCoord1i(texcoords[count-1]);
			freyjaPolygonVertex1i(vertices[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[0]);
			freyjaPolygonVertex1i(vertices2[0]);
			freyjaPolygonTexCoord1i(texcoords2[count-1]);
			freyjaPolygonVertex1i(vertices2[count-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
		else
		{
			/* Base */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(centerUV);
			freyjaPolygonVertex1i(center);
			freyjaPolygonTexCoord1i(texcoords[i]);
			freyjaPolygonVertex1i(vertices[i]);
			freyjaPolygonTexCoord1i(texcoords[i-1]);
			freyjaPolygonVertex1i(vertices[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON

			/* Top */
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonTexCoord1i(pointUV);
			freyjaPolygonVertex1i(point);
			freyjaPolygonTexCoord1i(texcoords2[i]);
			freyjaPolygonVertex1i(vertices2[i]);
			freyjaPolygonTexCoord1i(texcoords2[i-1]);
			freyjaPolygonVertex1i(vertices2[i-1]);
			freyjaPolygonMaterial1i(0);
			freyjaEnd(); // FREYJA_POLYGON
		}
	}

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


void freyjaGenerateTubeMesh(vec3_t origin, vec_t height, 
							long count, long segments)
{
	Vector<long> vertices, vertices2, texcoords, texcoords2, segVert, segTex;
	long i, j, index, index2;
	vec_t x, z, y, u, v, nx, ny, nz;

	
	(segments < 1) ? segments = 1 : 0;
	(count < 3) ? count = 3 : 0;

	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	/* Generate geometery */

	nz = nx = 0.0;
	ny = -1.0;

	/* Bottom and top */
	for (i = 0; i < count; ++i)
	{
		x = cos(helDegToRad(360.0 * ((float)i / (float)count)));
		z = sin(helDegToRad(360.0 * ((float)i / (float)count)));
		y = height;

		u = (x < 0) ? (x * 0.25 + 0.25) : (x * 0.25 + 0.25);
		v = (z < 0) ? (z * 0.25 + 0.25) : (z * 0.25 + 0.25);

		nx = x * 0.2;
		ny = -0.6;
		nz = z * 0.2;

		index = freyjaVertex3f(origin[0] + x, origin[1], origin[2] + z);
		freyjaVertexNormal3f(index, nx, ny, nz);
		vertices.pushBack(index);

		index =freyjaTexCoord2f(u, v);
		texcoords.pushBack(index);

		index = freyjaVertex3f(origin[0] + x, origin[1]+y, origin[2] + z);
		freyjaVertexNormal3f(index, nx, -ny, nz);
		vertices2.pushBack(index);

		index = freyjaTexCoord2f(u+0.5, v);
		texcoords2.pushBack(index);
	}


	/* Tube, doesn't have 0th or height-th ring */
	for (i = 0; i < segments+1; ++i)
	{
		/* Reuse bottom vertices for 0th ring */
		if (!i)
		{
			for (j = 0; j < count; ++j)
			{
				u = 1.0 * ((float)j/(float)count);
				v = 0.5 * ((float)i/(float)segments) + 0.5;

				index = vertices[j];
				segVert.pushBack(index);

				index = freyjaTexCoord2f(u, v);
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

				index = freyjaTexCoord2f(u, v);
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

			index = freyjaVertex3f(origin[0]+x, origin[1]+y , origin[2]+z);
			freyjaVertexNormal3f(index, nx, ny, nz);
			segVert.pushBack(index);

			index = freyjaTexCoord2f(u, v);
			segTex.pushBack(index);
		}
	}

	freyjaEnd(); // FREYJA_VERTEX_GROUP


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

	freyjaEnd(); // FREYJA_MESH

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBox(vec3_t bbox[2])
{
	Vector<unsigned int> *list;
	long i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	count = freyjaGetCount(FREYJA_VERTEX); 

	list = new Vector<unsigned int>();

	if (count == FREYJA_PLUGIN_ERROR)
	{
		return list;
	}


	/* Using egg iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_LIST_CURRENT));
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(vec3_t bbox[2],
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		freyjaGetVertex3fv(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(freyjaIterator(FREYJA_VERTEX, 
												  FREYJA_LIST_CURRENT));
				}
			}
		}
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


void freyjaPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->freyjaPrintMessage(format, &args);
	va_end(args);
}


void freyjaPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->freyjaPrintError(format, &args);
	va_end(args);
}


void freyjaGenerateUVFromXYZ(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		freyjaPrintMessage("freyjaGenerateUVFromXYZ> ERROR: Invalid UV");
		return;
	}

	*u = (xyz[0] > 0) ? xyz[0] : -xyz[0];
	s = 0.025;
  
	while (*u > 1.0)
	{
		*u *= s;
		s *= 0.01;
	}
  
	*v = (xyz[1] > 0) ? xyz[1] : -xyz[1];
	s = 0.025;
  
	while (*v > 1.0)
	{
		*v *= s;
		s *= 0.01;
	}
  
	*u = 1.0 - *u;
	*v = 1.0 - *v;
}


void freyjaGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector <long> ref;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, vertexCount, faceCount;
	long v0, v1, v2, index;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);
	
	freyjaPrintMessage("freyjaGenerateVertexNormals()");

	vertexCount = freyjaGetCount(FREYJA_VERTEX);
	faceCount = freyjaGetCount(FREYJA_POLYGON); 

	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		freyjaGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		freyjaGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		freyjaGetPolygon1u(FREYJA_VERTEX, 2, &v2);

		freyjaPrintMessage("<%d %d %d>", v0, v1, v2);
		freyjaGetVertexByIndex3fv(v0, a.mVec);
		freyjaGetVertexByIndex3fv(v1, b.mVec);
		freyjaGetVertexByIndex3fv(v2, c.mVec);

		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(new Vector3d(normal));

		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

		if (index < 0)
		{
			freyjaPrintError("freyjaGenerateVertexNormals> ERROR bad vertex\n");
			continue;
		}

		normal.zero();

		freyjaGetVertexPolygonRef(ref);

		for (j = ref.begin(); j < ref.end(); ++j)
		{
			normal += *faceNormals[ref[j]];
		}

		normal.normalize();

		freyjaVertexNormal3fv(index,normal.mVec);

		freyjaPrintMessage("%d :: %d faces :: %f %f %f", index,
						   ref.size(),
						   normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
    }

	faceNormals.erase();
	
	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
}


Vector<unsigned int> *freyjaFindVerticesByBoundingVolume(BoundingVolume &vol)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	count = freyjaGetCount(FREYJA_VERTEX); 

	if (count < 1)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);

		freyjaGetVertexByIndex3fv(index, xyz);
		
		if (vol.isVertexInside(xyz))
		{
			list->pushBack(index);
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *freyjaFindVerticesInBox(vec3_t bbox[2],
											  Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count, index;
	vec3_t xyz;


	freyjaCriticalSection(FREYJA_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using freyja iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (FREYJA_PLUGIN_ERROR == freyjaIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		index = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
		freyjaGetVertexByIndex3fv(index, xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(index);
				}
			}
		}

		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return list;
}


void freyjaGetVertexPolygonRef(Vector<long> &polygons)
{
	polygons.clear();

	if (EggPlugin::mEggPlugin)
	{
		egg_vertex_t *v = EggPlugin::mEggPlugin->getVertex(freyjaGetCurrent(FREYJA_VERTEX));
		unsigned int i;

		if (v && !v->ref.empty())
		{
			// if longs ploygons.copy(v->ref);

			for (i = v->ref.begin(); i < v->ref.end(); ++i)
				polygons.pushBack((long)v->ref[i]);
		}
	}
}


////////////////////////////////////////////////////////////////////


void freyjaBegin(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaBegin(type);
}


void freyjaEnd()
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaEnd();
}


long freyjaTexCoord2fv(vec2_t uv)
{
	return freyjaTexCoord2f(uv[0], uv[1]);
}


long freyjaTexCoord2f(vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTexCoord2f(u, v);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(xyz[0], xyz[1], xyz[2]);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexFrame3f(long index, vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_vertex_t *v = EggPlugin::mEggPlugin->getVertex(index);

		if (v)
		{
			vec_t *xyz = new vec3_t;
			xyz[0] = x;
			xyz[1] = y;
			xyz[2] = z;
			v->frameId.pushBack(freyjaGetCurrent(FREYJA_VERTEX_FRAME));
			v->frames.pushBack((vec3_t *)xyz);
		}
	}
}


long freyjaVertex3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaVertex3f(x, y, z);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaVertexWeight(long index, vec_t weight, long bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexWeight(index, weight, bone);
}


void freyjaVertexTexCoord2fv(long vIndex, vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexTexCoord2f(vIndex, uv[0], uv[1]);
}


void freyjaVertexTexCoord2f(long vIndex, vec_t u, vec_t v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexTexCoord2f(vIndex, u, v);
}


void freyjaVertexNormal3fv(long vIndex, vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexNormal3f(vIndex, 
													nxyz[0], nxyz[1], nxyz[2]);
}


void freyjaVertexNormal3f(long vIndex, vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaVertexNormal3f(vIndex, x, y, z);
}


void freyjaPolygonVertex1i(long egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonVertex1i(egg_id);
}


void freyjaPolygonTexCoord1i(long egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonTexCoord1i(egg_id);
}


void freyjaPolygonMaterial1i(long id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaPolygonMaterial1i(id);  
}


unsigned long freyjaGetFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaFlags();

	return 0;
}


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGroupCenter(x, y, z);
}


void freyjaBoneFlags1i(long boneIndex, long flags)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->flag = flags;
	}
}


void freyjaBoneParent1i(long boneIndex, long parentIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->parent = parentIndex;
	}
}


void freyjaBoneName1s(long boneIndex, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		strncpy(bone->name, name, 64);
	}
}


void freyjaBoneAddChild1i(long boneIndex, long childIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		egg_tag_t *child =  EggPlugin::mEggPlugin->getBone(childIndex);
			
		if (child)
			child->parent = bone->id;

		/* If it fails here it's got to be picked up in SKELETON */
		bone->slave.pushBack(childIndex);
	}
}


void freyjaBoneAddMesh1i(long boneIndex, long meshIndex)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		// No check was old policy
		bone->mesh.pushBack(meshIndex);
	}
}


void freyjaBoneTranslate3f(long boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->center[0] = x;
		bone->center[1] = y;
		bone->center[2] = z;
	}
}


void freyjaBoneTranslate3fv(long boneIndex, vec3_t xyz)
{
	freyjaBoneTranslate3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateEulerXYZ3f(long boneIndex, vec_t x, vec_t y, vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		bone->rot[0] = x;
		bone->rot[1] = y;
		bone->rot[2] = z;
	}
}


void freyjaBoneRotateEulerXYZ3fv(long boneIndex, vec3_t xyz)
{
	freyjaBoneRotateEulerXYZ3f(boneIndex, xyz[0], xyz[1], xyz[2]);
}


void freyjaBoneRotateQuatWXYZ4f(long boneIndex,vec_t w,vec_t x,vec_t y,vec_t z)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(boneIndex);

	if (bone)
	{
		vec_t heading, bank, attitude;
		Quaternion q = Quaternion(w, x, y, z);

		q.getEulerAngles(&heading, &bank, &attitude);

		heading *= 57.295779513082323;
		bank *= 57.295779513082323;
		attitude *= 57.295779513082323;

		EggPlugin::mEggPlugin->freyjaBoneRotate(heading, bank, attitude);
	}
}


void freyjaBoneRotateQuatWXYZ4fv(long boneIndex, vec4_t wxyz)
{
	freyjaBoneRotateQuatWXYZ4f(boneIndex, wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
}




void freyjaMeshTreeFrameAddBone(long tag)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshTreeFrameAddBone(tag);
}


int freyjaGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;

	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureFilename(index, 
															   filename);
	return FREYJA_PLUGIN_ERROR;	
}


int freyjaGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetTextureImage(index, w, h, 
														 depth, type, image);
	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureFilename1s(char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreFilename(filename);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaTextureStoreBuffer(unsigned char *image, unsigned int depth,
							  unsigned int width, unsigned int height,
							  freyja_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaTextureStoreBuffer(image, depth,
															width, height,
															type);

	return FREYJA_PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

long freyjaGetCount(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCount(type);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaIterator(freyja_object_t type, long item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaIterator(type, item);

	return FREYJA_PLUGIN_ERROR;
}


void freyjaGetTexCoord2fv(long index, vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetTexCoord(index, uv);
}


void freyjaGetVertexByIndex3fv(long index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexByIndex(index, xyz);
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertex(xyz);
}


void freyjaGetVertexTexCoord2fv(vec2_t uv)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(uv);
}


void freyjaGetVertexNormal3fv(vec3_t nxyz)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetVertexNormal(nxyz);
}

long freyjaGetMeshPolygonCount(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->polygon.end();
	}

	return 0;
}

long freyjaGetMeshVertexGroupCount(long meshIndex)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh)
	{
		return mesh->group.end();
	}

	return 0;
}

long freyjaGetMeshPolygonIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh && element > 0 && element < (int)mesh->polygon.end())
	{
		return mesh->polygon[element];
	}

	return -1;
}

long freyjaGetMeshVertexGroupIndex(long meshIndex, long element)
{
	egg_mesh_t *mesh = EggPlugin::mEggPlugin->getMesh(meshIndex);

	if (mesh && element > 0 && element < (int)mesh->group.end())
	{
		return mesh->group[element];
	}

	return -1;
}


long freyjaGetPolygonVertexIndex(long polygonIndex, long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonVertexIndex(polygonIndex, element);

	return -1;
}


long freyjaGetPolygonTexCoordIndex(long polygonIndex, long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonTexCoordIndex(polygonIndex, element);

	return -1;
}


long freyjaGetPolygonMaterial(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonMaterial(polygonIndex);

	return 0;
}


long freyjaGetPolygonFlags(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonFlags(polygonIndex);

	return 0;
}


long freyjaGetPolygonEdgeCount(long polygonIndex)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->getPolygonEdgeCount(polygonIndex);

	return 0;
}


long freyjaGetPolygon1u(freyja_object_t type, long item, long *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetPolygon3f(freyja_object_t type, long item, vec_t *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetPolygon(type, item, value);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaCriticalSection(freyja_lock_t request)
{
	//freyjaPrintMessage("freyjaCriticalSection> Not implemented, %s:%i\n", 
	//				   __FILE__, __LINE__);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneName(long index, unsigned int size, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


void freyjaBoneParent(long index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(freyjaGetCurrent(FREYJA_BONE));


	if (bone)
	{
		bone->parent = index;
	}	
}


long freyjaGetBoneParent(long index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);


	if (bone)
	{
		return bone->parent;
	}

	return -2;
}


long freyjaGetBoneRotationWXYZ4fv(long index, vec4_t wxyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		Quaternion q = Quaternion(bone->rot[0], bone->rot[1], bone->rot[2]);
		q.getQuaternion4fv(wxyz);

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneRotationXYZ3fv(long index, vec3_t xyz)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);
	
	if (bone)
	{
		xyz[0] = bone->rot[0];
		xyz[1] = bone->rot[1];
		xyz[2] = bone->rot[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneTranslation3fv(long index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_tag_t *bone = EggPlugin::mEggPlugin->getBone(index);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneMeshIndex(long element)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshIndex(element);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetCurrent(freyja_object_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetCurrent(type);

	return FREYJA_PLUGIN_ERROR;
}


long freyjaGetBoneMeshCount()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->freyjaGetBoneMeshCount();

	return FREYJA_PLUGIN_ERROR;
}


void freyjaGetBoneRotate3f(vec_t *x, vec_t *y, vec_t *z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaGetBoneRotate(x, y, z);
}


void freyjaMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->freyjaMeshFlags1u(flags);	
}


//////////////////////////////////////////////////////////////////////
// Plugin import / export nicities
//////////////////////////////////////////////////////////////////////

/* TODO hook up to EggPlugin */

void freyjaPluginBegin()
{
	// ATM this does nothing, just here for reserved use
}


void freyjaPluginFilename1s(char *filename)
{

}


void freyjaPluginDescription1s(char *info_line)
{

}


void freyjaPluginAddExtention1s(char *ext)
{

}


void freyjaPluginImport1i(long flags)
{

}


void freyjaPluginExport1i(long flags)
{

}

void freyjaPluginEnd()
{
	// ATM this does nothing, just here for reserved use
}


long freyjaGetPluginId()
{
	return -1;
}


int freyjaGetPluginArg1f(long pluginId, char *name, float &arg)
{
	return -1;
}


int freyjaGetPluginArg1i(long pluginId, char *name, long &arg)
{
	return -1;
}


int freyjaGetPluginArg1s(long pluginId, char *name, long len, char *arg)
{
	return -1;
}
