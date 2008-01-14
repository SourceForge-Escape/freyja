/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2003-2008 Mongoose
 * Comments: Unreal Tournament skeletal model plugin.
 *
 *-- History ------------------------------------------------ 
 *
 * 2008.01.13:
 * Mongoose - Created, split from PSKModel.cpp
 *            Improved material support.
 *            
 ================================================================*/

#define LEGACY_EULER_FIX_UP 0

#define LEGACY_TRANSFORM 0

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/freyja.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <hel/Mat44.h>
#include <hel/Vec3.h>
#include <hel/Quat.h>
#include "PSKModel.h"

using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__psk_check(char *filename);
	int freyja_model__psk_import(char *filename);
	int freyja_model__psk_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("psk");
	freyjaPluginDescription1s("UE2 Intermediate (*.psk)");
	freyjaPluginAddExtention1s("*.psk");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
	freyjaPluginArg1f("scale", 0.25f);
}


int import_model(char *filename)
{
	if (!freyja_model__psk_check(filename))
		return freyja_model__psk_import(filename);

	return -1;
}


int freyja_model__psk_check(char *filename)
{
	FILE *f = fopen(filename, "rb");;
	
	if (!f)
	{
		perror(filename);
		return -1;
	}

	char buffer[32];
    fread(buffer, 1, 28, f);
    buffer[8] = 0;

	if (strncmp("ACTRHEAD", buffer, 8) != 0)
	{
		//printf("Not a valid PSK Model\n");
		return -1;
	}																		   

	return 0;
}


int freyja_model__psk_import(char *filename)
{
	/* Check header to verify it's a PSK model */
	if (freyja_model__psk_check(filename) < 0)
		return -1;

	/* Attempt to load model */
	PSKModel psk;

	if (psk.load(filename) < 0)
		return -2;

	/* Load plugin preferences */
	int pluginId = freyjaGetPluginId();

	vec_t scale = 0.25f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);
	
	/* Start a new mesh */
	index_t model = freyjaModelCreate();
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	/* Load materials. */
	Vector<index_t> materialIds;
	for (uint32 i = 0; i < psk.mNumMaterials; ++i)
	{
		String name = psk.mMaterials[i].name;

		index_t materialIndex = freyjaMaterialCreate(); 
		materialIds.push_back( materialIndex );
		freyjaMaterialName( materialIndex, name.c_str() );

		// 'Guess' texture filenames from model path.
		{
			String path = filename;
			const char sep = mstl::SystemIO::GetPathSep( );
			int i = path.find_last_of( sep );
			if ( i > 0 )
			{
				path[i+1] = '\0';
			}

			String tex;
			tex.Set( "%s%s.tga", path.c_str(), name.c_str() );

			if ( !mstl::SystemIO::File::DoesFileExist( tex.c_str() ) )
			{
				tex.Set( "%s%s.png", path.c_str(), name.c_str() );
			}

			index_t tid = freyjaTextureCreateFilename( tex.c_str() );
			freyjaMaterialTexture( materialIndex, tid+2 );  // +2 for 0.9.5 mapping
			freyjaMaterialTextureName( materialIndex, tex.c_str() );
		}
	}

	/* Copy skinned geometry */	
	for (uint32 i = 0, idx; i < psk.mNumVertices; ++i)
	{
		idx = i * 3; 
		// FIXME: Don't cheat around coord conversion once multibone is fixed ;)
		Vec3 v(psk.mVertices[idx], psk.mVertices[idx+2], -psk.mVertices[idx+1]);
		v *= scale;
		/*index_t vertex =*/ freyjaMeshVertexCreate3fv(mesh, v.mVec);
	}

	for (uint32 i = 0; i < psk.mNumWeights; ++i)
	{
		// No index translation needed, since 0.9.5+ doesn't append to meshes.
		index_t vertex = psk.mWeights[i].vertexIndex;  
		freyjaMeshVertexWeight(mesh, vertex, psk.mWeights[i].boneIndex, 
								psk.mWeights[i].weight);
	}

	for (uint32 i = 0; i < psk.mNumFaces; ++i)
	{
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, materialIds[ psk.mFaces[i].material ] );

		// FIXME: This is actually a group bitflag, and some 9.x ABI only support ids.
		freyjaMeshPolygonGroup1u( mesh, face, psk.mFaces[i].smoothingGroups );

		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].x].vertex);
		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].y].vertex);
		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].z].vertex);

		index_t texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].x].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].x].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

		texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].y].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].y].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

		texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].z].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].z].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
	}

	/* Generate geometry normals for lighting. */
	freyjaMeshGenerateVertexNormals(mesh);
	//freyjaMeshNormalFlip(mesh);  // don't flip when mirroring 'Z'.

	/* Construct skeleton and bind pose. */
	index_t skeleton = freyjaSkeletonCreate();
	freyjaModelAddSkeleton(model, skeleton);

	for (uint32 i = 0; i < psk.mNumBones; ++i)
	{
		hel::Vec3 t( psk.mBones[i].restLoc[0],
					 psk.mBones[i].restLoc[2],
					 -psk.mBones[i].restLoc[1] );
		t *= scale;

		// FIXME: Incorrect, but the same result as the last implemenation.
		hel::Quat q( psk.mBones[i].restDir[3],
		             psk.mBones[i].restDir[0],
		             psk.mBones[i].restDir[2],
		             -psk.mBones[i].restDir[1] );		

		/* Create a new bone for the skeleton. */
		index_t bone = freyjaBoneCreate(skeleton);
		freyjaBoneFlags(bone, 0x0);
		freyjaBoneName(bone, psk.mBones[i].name);
		freyjaBoneTranslate3f( bone, t.mX, t.mY, t.mZ );
		freyjaBoneRotateQuat4f( bone, q.mW, q.mX, q.mY, q.mZ );
		freyjaBoneParent(bone, psk.mBones[i].parentIndex);

		/* Setup its children. */
		for (uint32 j = 0; j < psk.mNumBones; ++j)
		{
			if ( psk.mBones[j].parentIndex == i && i != j )
			{
				freyjaBoneAddChild(bone, j);
			}
		}
	}
			
	freyjaSkeletonUpdateBones(skeleton);

	return 0;
}


int freyja_model__psk_import_legacy(char *filename)
{
	/* Check header to verify it's a PSK model */
	if (freyja_model__psk_check(filename) < 0)
		return -1;

	/* Attempt to load model */
	PSKModel psk;

	if (psk.load(filename) < 0)
		return -2;

	/* Load plugin preferences */
	int pluginId = freyjaGetPluginId();

	vec_t scale = 0.25f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);
	
	/* Start a new mesh */
	index_t model = freyjaModelCreate();
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	/* Load materials. */
	Vector<index_t> materialIds;
	for (uint32 i = 0; i < psk.mNumMaterials; ++i)
	{
		String name = psk.mMaterials[i].name;

		index_t materialIndex = freyjaMaterialCreate(); 
		materialIds.push_back( materialIndex );
		freyjaMaterialName( materialIndex, name.c_str() );

		// 'Guess' texture filenames from model path.
		{
			String path = filename;
			const char sep = mstl::SystemIO::GetPathSep( );
			int i = path.find_last_of( sep );
			if ( i > 0 )
			{
				path[i+1] = '\0';
			}

			String tex;
			tex.Set( "%s%s.tga", path.c_str(), name.c_str() );

			if ( !mstl::SystemIO::File::DoesFileExist( tex.c_str() ) )
			{
				tex.Set( "%s%s.png", path.c_str(), name.c_str() );
			}

			index_t tid = freyjaTextureCreateFilename( tex.c_str() );
			freyjaMaterialTexture( materialIndex, tid+2 );  // +2 for 0.9.5 mapping
			freyjaMaterialTextureName( materialIndex, tex.c_str() );
		}
	}

	/* Copy skinned geometry */	
	for (uint32 i = 0, idx; i < psk.mNumVertices; ++i)
	{
		idx = i * 3; 
		// FIXME: Don't cheat around coord conversion once multibone is fixed ;)
		Vec3 v(psk.mVertices[idx], psk.mVertices[idx+2], psk.mVertices[idx+1]);
		v *= scale;

		/*index_t vertex =*/ freyjaMeshVertexCreate3fv(mesh, v.mVec);
	}

	for (uint32 i = 0; i < psk.mNumWeights; ++i)
	{
		// No index translation needed, since 0.9.5+ doesn't append to meshes.
		index_t vertex = psk.mWeights[i].vertexIndex;  
		freyjaMeshVertexWeight(mesh, vertex, psk.mWeights[i].boneIndex, 
								psk.mWeights[i].weight);
	}

	for (uint32 i = 0; i < psk.mNumFaces; ++i)
	{
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, materialIds[ psk.mFaces[i].material ] );

		// FIXME: This is actually a bitflag, so wait for PolygonGroupBitFlag()
		//        to be checked back into the project.
		freyjaMeshPolygonGroup1u( mesh, face, psk.mFaces[i].smoothingGroups );

		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].x].vertex);
		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].y].vertex);
		freyjaMeshPolygonAddVertex1i(mesh, face, psk.mVTXWs[psk.mFaces[i].z].vertex);

		index_t texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].x].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].x].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

		texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].y].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].y].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

		texcoord = freyjaMeshTexCoordCreate2f(mesh, 
							 psk.mVTXWs[psk.mFaces[i].z].uv[0], 
							 psk.mVTXWs[psk.mFaces[i].z].uv[1]);
		freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
	}

	/* Generate geometry normals for lighting. */
	freyjaMeshGenerateVertexNormals(mesh);
	freyjaMeshNormalFlip(mesh);

	/* Construct skeleton and bind pose. */
	index_t skeleton = freyjaSkeletonCreate();
	freyjaModelAddSkeleton(model, skeleton);

	for (uint32 i = 0; i < psk.mNumBones; ++i)
	{
#if LEGACY_TRANSFORM		
		hel::Vec3 t(psk.mBones[i].restLoc[0],
					psk.mBones[i].restLoc[1],
					psk.mBones[i].restLoc[2]);
#else
	hel::Vec3 t(psk.mBones[i].restLoc[0],
				psk.mBones[i].restLoc[2],
				psk.mBones[i].restLoc[1]);
#endif

#if LEGACY_EULER_FIX_UP
		hel::Quat q(psk.mBones[i].restDir[3],
				 psk.mBones[i].restDir[0],
				 psk.mBones[i].restDir[1],
				 psk.mBones[i].restDir[2]);

		hel::Vec3 r;
		q.GetEulerAngles(r.mX, r.mY, r.mZ);

		if (i == 0) 
		{
			hel::Mat44 rot;
			rot.SetRotation(-90.0f, 0.0f, 0.0f);

#if 1 // Bah, many of my test models have junk transforms for Bip
			vec_t tmp = t.mZ;
			r.mY += helDegToRad(90.0f);
			t = rot * t;
			t.mZ = 0.0f;
			t.mY = tmp;
#else
			r.mY += helDegToRad(90.0f);
			t = rot * t;
			t.mZ = -t.mZ;
#endif
		}
#else
#endif
		t *= scale;

		/* Create a new bone for the skeleton. */
		index_t bone = freyjaBoneCreate(skeleton);
		freyjaBoneFlags(bone, 0x0);
		freyjaBoneName(bone, psk.mBones[i].name);
		freyjaBoneTranslate3f(bone, t.mX, t.mY, t.mZ);
#if LEGACY_EULER_FIX_UP
#   if LEGACY_TRANSFORM
		freyjaBoneRotateEuler3f(bone, r.mX, r.mY, r.mZ);
#   else
		freyjaBoneRotateEuler3f(bone, r.mX, r.mZ, r.mY );
#   endif // LEGACY_TRANSFORM
#else
		freyjaBoneRotateQuat4f(bone, psk.mBones[i].restDir[3],
				 psk.mBones[i].restDir[0],
				 psk.mBones[i].restDir[1],
				 psk.mBones[i].restDir[2]);
#endif
		freyjaBoneParent(bone, psk.mBones[i].parentIndex);

		/* Setup its children. */
		for (uint32 j = 0; j < psk.mNumBones; ++j)
		{
			if (psk.mBones[j].parentIndex == i && i != j)
			{
				freyjaBoneAddChild(bone, j);
			}
		}
	}
			
	freyjaSkeletonUpdateBones(skeleton);

	return 0;
}


int freyja_model__psk_export(char *filename)
{
	freyjaPrintError("freyja_model__psk_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);

	return -1;
}
