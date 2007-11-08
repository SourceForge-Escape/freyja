/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * Comments: This is the BVH plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2007.10.22:
 * Mongoose - Created, based on specs from:
 *            http://www.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/PluginABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/ModelABI.h>
#include <freyja/MeshABI.h>
#include <freyja/BoneABI.h>
#include <freyja/MaterialABI.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <hel/Vec3.h>


using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__bvh_check(char *filename);
	int freyja_model__bvh_import(char *filename);
	int freyja_model__bvh_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("bvh");
	freyjaPluginDescription1s("Biovision Hierarchy (*.bvh)");
	freyjaPluginAddExtention1s("*.bvh");
	freyjaPluginImport1i(FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_SKELETON);
	freyjaPluginArg1f("scale", 0.4f);
}


int import_model(char *filename)
{
	
	if ( !freyja_model__bvh_check(filename) )
		return freyja_model__bvh_import(filename);

	return -1;
}


int freyja_model__bvh_check(char *filename)
{
	SystemIO::FileReader r;

	if ( !r.Open(filename) )
	{
		return -1;
	}

	char buf[12];
	r.ReadString(10, buf);
	r.Close();

	if ( !strncmp(buf, "HIERARCHY", 9) )
		return 0;

	return -2;
}


bool bvh_import_heierarchy(SystemIO::BufferedTextFileReader& r, 
                           index_t skeleton, index_t parent)
{	
	// Parse joints / possible multiple roots.
	long offset = r.GetOffset();
	if ( r.ParseMatchingSymbol("ROOT") )
	{
		skeleton = freyjaSkeletonCreate();
	}
	else
	{
		r.SetOffset( offset );

		if ( !r.ParseMatchingSymbol("JOINT") )
		{
			r.SetOffset( offset );
			return false;
		}
	}

	index_t bone = freyjaBoneCreate(skeleton);
	freyjaBoneParent(bone, parent);
	freyjaSkeletonAddBone(skeleton, bone); 

	// Name of joint / root
	freyjaBoneName( bone, r.ParseSymbol() );

	if ( !r.ParseMatchingSymbol("{") )
		return false;

	// OFFSET
	{
		r.ParseMatchingSymbol("OFFSET");
		float x = r.ParseFloat();
		float y = r.ParseFloat();
		float z = r.ParseFloat();

		freyjaBoneTranslate3f( bone, x, y, z );
	}

	// CHANNELS
	const char* channels = r.ParseSymbol();
	unsigned int count = r.ParseInteger();
	
	for (unsigned int i = 0; i < count; ++i)
	{
		mstl::String flag = r.ParseSymbol();
		// FIXME channels are currently unused, since the feature was removed.
		//if ( flag == "Xrotation" )
		//	freyjaBoneChannel_XRot( 1 );
	}

	// Parse children
	while ( bvh_import_heierarchy(r, skeleton, bone) )
	{
	}

	// "End Site"
	offset = r.GetOffset();
	if ( r.ParseMatchingSymbol("End") && r.ParseMatchingSymbol("Site") )
	{
		if ( !r.ParseMatchingSymbol("{") )
			return false;

		r.ParseMatchingSymbol("OFFSET");
		float x = r.ParseFloat();
		float y = r.ParseFloat();
		float z = r.ParseFloat();

		if ( !r.ParseMatchingSymbol("}") )
			return false;

		//freyjaSkeletonUpdateBones( skeleton );
	}
	else
	{
		r.SetOffset(offset);
	}

	if ( !r.ParseMatchingSymbol("}") )
		return false;

	return true;
}


int freyja_model__bvh_import(char* filename)
{
	if ( freyja_model__bvh_check(filename) )
		return -1;

	SystemIO::BufferedTextFileReader r;

	if ( !r.Open(filename) )
	{
		MSTL_MSG("Failed to open file '%s'", filename);
		return -2;
	}

	int pluginId = freyjaGetPluginId();
	vec_t scale = 0.4f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);

	if ( !r.ParseMatchingSymbol("HIERARCHY") )
		return -3;

	// Parse joints / possible multiple roots.
	index_t skeleton = INDEX_INVALID, parent = INDEX_INVALID;
	bvh_import_heierarchy(r, skeleton, parent);
	
	
	// Start anim / mocap

#if 0
MOTION
Frames:    2
Frame Time: 0.033333
 8.03	 35.01	 88.36	-3.41	 14.78	-164.35	 13.09	 40.30	-24.60	 7.88	 43.80	 0.00	-3.61	-41.45	 5.82	 10.08	 0.00	 10.21	 97.95	-23.53	-2.14	-101.86	-80.77	-98.91	 0.69	 0.03	 0.00	-14.04	 0.00	-10.50	-85.52	-13.72	-102.93	 61.91	-61.18	 65.18	-1.57	 0.69	 0.02	 15.00	 22.78	-5.92	 14.93	 49.99	 6.60	 0.00	-1.14	 0.00	-16.58	-10.51	-3.11	 15.38	 52.66	-21.80	 0.00	-23.95	 0.00	
#endif

	// End anim / mocap

	return 0;
}


int freyja_model__bvh_export(char *filename)
{
	SystemIO::FileWriter w;
	char name[128];
	long modelIndex = 0;    // make plugin option
	long skeletonIndex = 0; // make plugin option
	long i, j, k;
	long meshCount, meshIndex;
	long vertexCount, vertexIndex;
	long polygonCount, polygonIndex, faceVertexCount;//, faceVertex;
	vec3_t xyz;
	//vec2_t uv;

	int pluginId = freyjaGetPluginId();

	vec_t scale = 2.5f;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);
	scale = 1.0f / scale;	

	if (!w.Open(filename))
	{
		return -1;
	}

	w.WriteString(10, "MS3D000000");
	w.WriteLong(4);

	meshCount = freyjaGetModelMeshCount(modelIndex);

	for (vertexCount = 0, polygonCount = 0, i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		vertexCount += freyjaGetMeshVertexCount(meshIndex);
		polygonCount += freyjaGetMeshPolygonCount(meshIndex);
	}

	w.WriteInt16U(vertexCount);

	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		vertexCount = freyjaGetMeshVertexCount(meshIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = j;//freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetMeshVertexPos3fv(meshIndex, vertexIndex, xyz);

			w.WriteInt8U(0); // flags
			w.WriteFloat32(xyz[0]*scale); // x
			w.WriteFloat32(xyz[1]*scale); // y
			w.WriteFloat32(xyz[2]*scale); // z
			w.WriteInt8(-1); // boneId FIXME: ( ms is 1:1 vertex:bone, so for now skip )
			w.WriteInt8U(0); // refCount
		}
	}

	w.WriteInt16U(polygonCount);
	
	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		for (j = 0; j < polygonCount; ++j)
		{
			polygonIndex = j;//freyjaGetMeshPolygonIndex(meshIndex, j);
			faceVertexCount = freyjaGetMeshPolygonVertexCount(meshIndex, polygonIndex);

			faceVertexCount = 3; /* Milkshape only handles triangles,
								  * make user tesselate model to triangles
								  * to avoid 'write' to model state from here
								  * or handle it here, don't fuck with freyja 
								  * object state from export plugins! */

			w.WriteInt16(0); // flags

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				w.WriteInt16(vertexIndex);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				freyjaGetMeshVertexNormal3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[0]);
				w.WriteFloat32(xyz[1]);
				w.WriteFloat32(xyz[2]);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonTexCoordIndex(meshIndex, j, k);
				//vertexIndex = freyjaGetMeshPolygonIndex(meshIndex, j, k);
				freyjaGetMeshVertexTexCoord3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[0]);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonTexCoordIndex(meshIndex, j, k);
				//vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				freyjaGetMeshVertexTexCoord3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[1]);
			}

			w.WriteInt8U(freyjaGetMeshPolygonGroup(meshIndex, j)); // smoothingGroup
			w.WriteInt8U(i);
		}
	}


	w.WriteInt16U(meshCount);

	for (k = 0, i = 0; i < meshCount; ++i)
	{
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		snprintf(name, 32, "mesh%li", i);

		w.WriteInt8U(0); // flags
		w.WriteString(32, name); // name
		w.WriteInt16U(polygonCount);  // tris count

		for (j = 0; j < polygonCount; ++j, ++k)
		{
			w.WriteInt16U(j+k); // tris indeices
		}

		w.WriteInt8(-1); // materialIndex
	}


	uint32 materialCount = freyjaGetMaterialCount();
	w.WriteInt16U(materialCount); // materialCount
 
	for (i = 0; i < materialCount; ++i)
	{
		index_t material = i;
		//freyjaGetMaterialIndex(material, i);
		//snprintf(name, 32, "mat%li", i);
		snprintf(name, 32, freyjaGetMaterialName(material));
		name[31] = '\0';
		w.WriteString(32, name);

		vec4_t color;
		freyjaGetMaterialAmbient(material, color);
		w.WriteFloat32(color[0]); // ambient
		w.WriteFloat32(color[1]);
		w.WriteFloat32(color[2]);
		w.WriteFloat32(color[3]);

		freyjaGetMaterialDiffuse(material, color);
		w.WriteFloat32(color[0]); // diffuse
		w.WriteFloat32(color[1]);
		w.WriteFloat32(color[2]);
		w.WriteFloat32(color[3]);

		freyjaGetMaterialSpecular(material, color);
		w.WriteFloat32(color[0]); // specular
		w.WriteFloat32(color[1]);
		w.WriteFloat32(color[2]);
		w.WriteFloat32(color[3]);

		freyjaGetMaterialEmissive(material, color);
		w.WriteFloat32(color[0]); // emissive
		w.WriteFloat32(color[1]);
		w.WriteFloat32(color[2]);
		w.WriteFloat32(color[3]);
		
		w.WriteFloat32(freyjaGetMaterialShininess(material)); // shininess
		w.WriteFloat32(0); // transparency

		w.WriteInt8(0); // mode

		// FIXME: We should just use basename?
		snprintf(name, 128, freyjaGetMaterialTextureName(material));
		//snprintf(name, 128, "texturemap.png"); // he he he
		name[127] = '\0';
		w.WriteString(128, name); // texturemap

		snprintf(name, 128, "alphamap.png");
		w.WriteString(128, name); // alphamap
	}

	w.WriteFloat32(30.0f); // fAnimationFPS
	w.WriteFloat32(0.0f);  // fCurrentTime
	w.WriteLong(0);        // iTotalFrames

	long boneIndex;
	long boneCount = freyjaGetSkeletonBoneCount(skeletonIndex);

	w.WriteInt16U(boneCount); // boneCount

	for (i = 0; i < boneCount; ++i)
	{
		boneIndex = freyjaGetSkeletonBoneIndex(skeletonIndex, i);

		w.WriteInt8(0); // flags

		if (freyjaGetBoneNameString(boneIndex) == NULL)
		{
			snprintf(name, 32, "bone%li", i);
		}
		else
		{
			freyjaGetBoneName(boneIndex, 32, name);
		}
		w.WriteString(32, name); // this bone's name

		freyjaGetBoneName(freyjaGetBoneParent(boneIndex), 32, name);
		w.WriteString(32, name); // parent name

		freyjaGetBoneRotationEuler3fv(boneIndex, xyz);
		w.WriteFloat32(helDegToRad(xyz[0]));
		w.WriteFloat32(helDegToRad(xyz[1]));
		w.WriteFloat32(helDegToRad(xyz[2]));

		freyjaGetBoneTranslation3fv(boneIndex, xyz);
		w.WriteFloat32(xyz[0]*scale);
		w.WriteFloat32(xyz[1]*scale);
		w.WriteFloat32(xyz[2]*scale);

		index_t track = 0; // Only use 1st track for now, milk uses just one 
		uint32 trackCount = freyjaGetBoneTrackCount(boneIndex);
		uint32 rotKeyCount = 0;
		uint32 posKeyCount = 0;

		if (trackCount)
		{
			rotKeyCount = freyjaGetBoneRotKeyframeCount(boneIndex, track);
			posKeyCount = freyjaGetBonePosKeyframeCount(boneIndex, track);
		}

		w.WriteInt16U(rotKeyCount); // numRotationKeyframes
		w.WriteInt16U(posKeyCount); // numPositionKeyframes

		for (j = 0; j < rotKeyCount; ++j) // numRotationKeyframes
		{
			vec_t t = freyjaGetBoneRotKeyframeTime(boneIndex, track, j);
			freyjaGetBoneRotKeyframeEuler3fv(boneIndex, track, j, xyz);
			w.WriteFloat32(t);  // time
			w.WriteFloat32(xyz[0]);  // x
			w.WriteFloat32(xyz[1]);  // y
			w.WriteFloat32(xyz[2]);  // z
		}

		for (j = 0; j < posKeyCount; ++j) // numPositionKeyframes
		{
			vec_t t = freyjaGetBonePosKeyframeTime(boneIndex, track, j);
			freyjaGetBonePosKeyframe3fv(boneIndex, track, j, xyz);
			w.WriteFloat32(t);  // time
			w.WriteFloat32(xyz[0]);  // x
			w.WriteFloat32(xyz[1]);  // y
			w.WriteFloat32(xyz[2]);  // z
		}
	}

	w.Close();

	return 0;
}
