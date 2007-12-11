/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Md5
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Doom3 model plugin
 *
 *-- History ------------------------------------------------- 
 *
 * 2004.08.13:
 * Mongoose - Created
 ==========================================================================*/

#include "Md5.h"

////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <hel/Quat.h>
#include <hel/Mat44.h>

#include <freyja/freyja.h>
#include <freyja/BoneABI.h>
#include <freyja/MeshABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>

#include <mstl/Vector.h>

using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__md5_check(char *filename);
	int freyja_model__md5_import(char *filename);
	int freyja_model__md5_export(char *filename);
	int freyja_model__md5anim_import(char *filename);
	int freyja_model__md5anim_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("md5");
	freyjaPluginDescription1s("Doom3/QuakeWars (*.md5mesh)");
	freyjaPluginAddExtention1s("*.md5mesh");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
}


int import_model(char *filename)
{
	if (!freyja_model__md5_check(filename))
		return freyja_model__md5_import(filename);

	return -1;
}


int freyja_model__md5_check(char *filename)
{
	Md5Model md5;

	if (md5.IsMd5Model(filename) == true)
		return 0; /* 0 is no error */

	return -1;
}


int freyja_model__md5_import(char *filename)
{
	/* Handle embedded md5anim requests. */
	{
		String s = filename;

		int pos = s.find_last_of('.');

		if (pos != String::npos)
		{
			String ext = s.substr( pos );

			if (ext == "md5anim")
			{
				return freyja_model__md5anim_import(filename);
			}
		}
	}


	const vec_t scale = 0.7f;
	Md5Model md5;

	if (md5.LoadModel(filename) == false)
		return -1;

	// NOTE: Freyja 0.9.5 ABI doesn't need array remapping like 0.9.1 ABI.

	/* Start a new model */
	index_t modelIdx = freyjaModelCreate();

	/* Load each mesh into freyja. */
	for (unsigned int i = 0; i < md5.mNumMeshes; ++i)
	{
		Md5Mesh &mesh = md5.mMeshes[i];

		index_t meshIdx = freyjaMeshCreate();
		freyjaModelAddMesh(modelIdx, meshIdx);
		freyjaMeshName1s( meshIdx, mesh.name.c_str() );
		
		index_t materialIndex = freyjaMaterialCreate();
		freyjaMaterialName(materialIndex, mesh.shader.c_str());
		freyjaMaterialTexture(materialIndex, 0);
		freyjaMaterialTextureName(materialIndex, mesh.shader.c_str());

#if 0		
		freyjaMaterialAmbient(materialIndex, ambient);
		freyjaMaterialDiffuse(materialIndex, diffuse);
		freyjaMaterialSpecular(materialIndex, specular);
		freyjaMaterialEmissive(materialIndex, emissive);
		freyjaMaterialShininess(materialIndex, shininess);
		freyjaMaterialTransparency(materialIndex, transparency);
#endif

		// Freyja 0.9.5 wants a non-deformed mesh as input, since
		// it will later deform the mesh as needed depending on mode.
		for (unsigned int j = 0; j < mesh.numverts; ++j)
		{
			Md5Vertex &vertex = mesh.verts[j];
			Vec3 p(0.0f, 0.0f, 0.0f);

			for (int k = 0; k < vertex.numbones; ++k)
			{
				Md5Weight &weight = mesh.weights[ vertex.weight + k ];
				Md5Joint &joint = md5.mJoints[ weight.joint ];

				p += ( joint.rotate.Rotate( weight.pos ) + 
					   joint.translate ) * weight.weight;

				freyjaMeshVertexWeight(meshIdx, j, weight.joint, weight.weight);
			}

			// Was X Z Y in 0.9.1, it will be X Y Z if not translated here.
			p = Vec3(p.mX, p.mZ, -p.mY);
			p *= scale;

			freyjaMeshVertexCreate3fv(meshIdx, p.mVec);

			{
				Vec3 uvw(vertex.uv[0], vertex.uv[1], 0.0f);
				freyjaMeshVertexTexCoord3fv(meshIdx, j, uvw.mVec);
			}
		}

		for (unsigned int j = 0; j < mesh.numtriangles; ++j)
		{
			Md5Triangle &face = mesh.triangles[j];

			/* Start a new polygon */
			index_t faceIdx = freyjaMeshPolygonCreate(meshIdx);

			// Use mesh.shader material cache id instead?
			freyjaMeshPolygonMaterial(meshIdx, faceIdx, i);
			
			freyjaMeshPolygonAddVertex1i( meshIdx, faceIdx, face.vertex[0] );
			freyjaMeshPolygonAddVertex1i( meshIdx, faceIdx, face.vertex[1] );
			freyjaMeshPolygonAddVertex1i( meshIdx, faceIdx, face.vertex[2] );

			// If you want polymapped texcoords...
			//index_t texcoord = freyjaMeshTexCoordCreate2f(meshIdx, u, v);
			//freyjaMeshPolygonAddTexCoord1i(meshIdx, faceIdx, texcoord);
		}

		/* Generate normals to be pretty */
		freyjaMeshGenerateVertexNormals(meshIdx);
		freyjaMeshNormalFlip(meshIdx);
	}


	/* Create a freyja skeleton */
	index_t skeletonIdx = freyjaSkeletonCreate();
	freyjaModelAddSkeleton(modelIdx, skeletonIdx);
	Vec3 transforms[md5.mNumJoints];
	Vec3 origin;


	for (unsigned int j = 0; j < md5.mNumJoints; ++j)
	{
		transforms[j].Zero();
	}

	/* Mongoose 2004.12.21, 
	 * Md5 stores absolute bone pos -- make them offsets from parent. */
	for (unsigned int j = 0; j < md5.mNumJoints; ++j)
	{
		Vec3 p = md5.mJoints[j].translate;
		int idx = j;

		while (idx > -1)
		{
			p -= transforms[idx];
			idx = md5.mJoints[idx].parent;
		}

		transforms[j] = p;
	}

		
	/* Convert joints. */

	for (unsigned int j = 0; j < md5.mNumJoints; ++j)
	{
		/* Create new freyja bone. */
		index_t boneIdx = freyjaBoneCreate(skeletonIdx);
		Md5Joint &joint = md5.mJoints[ j ];

		// This will nop if the allocation of bone happens after skeleton
		freyjaSkeletonAddBone(skeletonIdx, boneIdx);
		freyjaBoneParent(boneIdx, joint.parent);
		freyjaBoneName(boneIdx, joint.name.c_str() );

		/* Local translation cache. */
		Vec3 p = transforms[j];
		p *= scale;

		// FIXME: I haven't factored in local rotations yet.
#if 0
		int idx = joint.parent;
		Quat q;
		Mat44 mat, mat2;

		p = Vec3();

		while (idx > -1)
		{
			//q = q * md5.mJoints[idx].rotate;

			mat2 = md5.mJoints[idx].rotate;

			Vec3 t = transforms[idx] * scale;
			mat2.Translate(t.mX, t.mZ, -t.mY);

			mat = mat * mat2;

			idx = md5.mJoints[idx].parent;
		}

		mat.Invert();
		//mat2 = joint.rotate;
		//mat = mat * mat2;

		p = mat * p;

		//q = mat.ToQuat(); 
		//q.Norm();
		//q = q.GetInverse();
		//p = q.Rotate(p);

		q = joint.rotate;

		/* Upload transform. */
		freyjaBoneTranslate3f(boneIdx, p.mX, p.mZ, -p.mY);
		freyjaBoneRotateQuat4f(boneIdx, q.mW, q.mX, q.mY, q.mZ);
#else
		freyjaBoneTranslate3f(boneIdx, p.mX, p.mZ, -p.mY);
#endif

		/* Parenting */

		for (unsigned int j2 = 0; j2 < md5.mNumJoints; ++j2)
		{
			if (md5.mJoints[j2].parent == (int)j)
			{ 
				freyjaBoneAddChild(boneIdx, j2);
			}
		}
	}

	freyjaPrintMessage("The MD5Model plugin is currently incomplete.");

	return 0;
}


// FIXME: Change this to use selected only later!
//        Those artists and their silly stand-ins.  ;)
int freyja_model__md5_export(char *filename)
{
	// FIXME: Until this is stable we don't allow overwrites 
	//        to save the user's ass.
	if ( SystemIO::File::DoesFileExist(filename) )
	{
		String s = filename;
		if ( s != "/home/mongoose/Desktop/test.md5mesh" )
		{
			FREYJA_INFOMSG(false, "Md5Mesh export is EXPERIMENTAL\nWon't overwrite of '%s'.\n", filename);
			return -1;
		}
	}


	/* Handle embedded md5anim requests. */
	{
		String s = filename;

		int pos = s.find_last_of('.');

		if (pos != String::npos)
		{
			String ext = s.substr( pos );

			if (ext == "md5anim")
			{
				return freyja_model__md5anim_export(filename);
			}
		}
	}



	const vec_t scale = 1.0f / 0.7f;
	Md5Model md5;

	// FIXME: This assumes 0.9.5, which only allows 1 model, 1 skeleton 
	index_t modelIndex = 0;
	index_t skelIndex = 0;


	/* Convert joints. */
	unsigned int boneCount = freyjaGetSkeletonBoneCount(skelIndex);
	md5.mJoints = new Md5Joint[ boneCount ];
	md5.mNumJoints = boneCount;

	for (unsigned int i = 0; i < boneCount; ++i)
	{
		freyjaPrintMessage("Converting joint %i...", i);

		Md5Joint &joint = md5.mJoints[i];

		unsigned int boneIdx = freyjaGetSkeletonBoneIndex(modelIndex, i);

		joint.name = freyjaGetBoneNameString(boneIdx);
		index_t parent = freyjaGetBoneParent(boneIdx);
		joint.parent = (parent == INDEX_INVALID) ? -1 : parent; 
	
		// Md5Joints are in absolute id object coordinates.
		Vec3 p;
		freyjaBoneBindTransformVertex(boneIdx, p.mVec, 1.0f);
		joint.translate = Vec3(p.mX, -p.mZ, p.mY); // Vec3(p.mX, p.mZ, -p.mY);
		joint.translate *= scale;

		vec4_t wxyz;
		freyjaGetBoneRotationQuat4fv(boneIdx, wxyz);
		joint.rotate.mW = wxyz[0];
		joint.rotate.mX = wxyz[1];
		joint.rotate.mY = wxyz[2];
		joint.rotate.mZ = wxyz[3];
	}


	/* Convert meshes. */
	unsigned int meshCount = freyjaGetModelMeshCount(modelIndex);
	md5.mNumMeshes = meshCount;
	md5.mMeshes = new Md5Mesh[ meshCount ];

	for (unsigned int i = 0; i < meshCount; ++i)
	{
		freyjaPrintMessage("Converting mesh %i...", i);

		Md5Mesh &mesh = md5.mMeshes[i];

		unsigned int meshIdx = freyjaGetModelMeshIndex(modelIndex, i);
		unsigned int vertexCount = freyjaGetMeshVertexCount(meshIdx);
		unsigned int faceCount = freyjaGetMeshPolygonCount(meshIdx);
		unsigned int weightCount = freyjaGetMeshWeightCount(meshIdx);

		mesh.name = freyjaGetMeshNameString(meshIdx);
		mesh.shader = "/FIXME/none/";
		mesh.shader += mesh.name;
		mesh.numverts = vertexCount;
		mesh.verts = new Md5Vertex[vertexCount];
		mesh.numtriangles = faceCount;
		mesh.triangles = new Md5Triangle[faceCount];
		mesh.numweights = weightCount;
		mesh.weights = new Md5Weight[weightCount];

		// Faces
		for (unsigned int j = 0; j < faceCount; ++j)
		{
			Md5Triangle &tri = mesh.triangles[j];
#if 0
			unsigned int count = 
			freyjaGetMeshPolygonVertexCount(meshIdx, j);
			FREYJA_ASSERTMSG(count > 3, "Did you remember to tesselate?");
#endif

			for (unsigned int k = 0; k < 3; ++k)
			{
				tri.vertex[k] = 
				freyjaGetMeshPolygonVertexIndex(meshIdx, j, k);
			}
		}

		// Vertices
		for (unsigned int j = 0, wcount = 0; j < vertexCount; ++j)
		{
			Md5Vertex &vertex = mesh.verts[j];
			
			Vec3 p;
			freyjaGetMeshVertexTexCoord3fv(meshIdx, j, p.mVec);
			vertex.uv[0] = p.mX;
			vertex.uv[1] = p.mY;


			freyjaGetMeshVertexPos3fv(meshIdx, j, p.mVec);
			p = Vec3(p.mX, -p.mZ, p.mY); // Vec3(p.mX, p.mZ, -p.mY);
			p *= scale;
			//verts.push_back( p );

			vertex.numbones = freyjaGetMeshVertexWeightCount(meshIdx, j);
			vertex.weight = freyjaGetMeshVertexWeightIndex(meshIdx, j, 0);
			
			// This is done to ensure proper ordering... and avoid vector.
			for (unsigned int k = 0, kn = vertex.numbones;
				 k < kn; ++k, ++wcount)
			{
				Md5Weight &w = mesh.weights[wcount];

				index_t wIdx = freyjaGetMeshVertexWeightIndex(meshIdx, j, k);

				index_t vertIdx, boneIdx;
				vec_t weightv;
			
				freyjaGetMeshWeight(meshIdx, wIdx, &vertIdx, &boneIdx, &weightv);
			
				w.joint = boneIdx;
				w.weight = weightv;

				// FIXME: when we start importing rotation for bind
				//        (if even needed) this has to change.

				// FIXME: these might need reweighting
				//freyjaBoneBindTransformVertex(boneIdx, u.mVec, 1.0f);
				Md5Joint &joint = md5.mJoints[ w.joint ];
#if 0
				Mat44 m = joint.rotate;
				m.Translate(joint.translate * w.weight);

				w.pos = Vec3();

				w.pos = m * p;
				w.pos *= w.weight;

				//( p * w.weight ) - joint.translate; // no rotation
				//q.Rotate( p / w.weight - joint.translate );
#else				
				Quat q = joint.rotate.GetInverse();

				w.pos =  
				//p - joint.translate * w.weight;
				p - q.Rotate( (joint.translate * w.weight) );
				//q.Rotate( p * w.weight ) - joint.translate;

				// v.pos += ( joint.rotate.Rotate( w.pos ) + joint.pos ) * w.weight
#endif
			}
		}
	}

	freyjaPrintError("md5.so export is EXPERIMENTAL.");

	if (md5.SaveModel(filename) == false)
		return -1;

	return 0;
}


int freyja_model__md5anim_import(char *filename)
{
	return -1;
}


int freyja_model__md5anim_export(char *filename)
{
	return -1;
}

#endif // FREYJA_PLUGINS

