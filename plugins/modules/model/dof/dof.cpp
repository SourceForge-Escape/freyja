/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * Comments: This is the DOF plug-in module
 *
 *           See file COPYING for license details.
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2007.04.19:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USING_FREYJA_MEMORY 1

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include <mstl/Vector.h>
#include <hel/Vec3.h>

#define Print freyjaPrintMessage 
#define new freyjaNew
#define delete freyjaDelete

using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__dof_check(char *filename);
	int freyja_model__dof_import(char *filename);
	int freyja_model__dof_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("dof");
	freyjaPluginDescription1s("dof model (*.dof)");
	freyjaPluginAddExtention1s("*.dof");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
	freyjaPluginArg1f("scale", 12.5f);
	//freyjaPluginArg1i("import_mesh", 1);
	//freyjaPluginArg1i("export_mesh", 0);
}


int import_model(char *filename)
{
	if (!freyja_model__dof_check(filename))
		return freyja_model__dof_import(filename);

	return -1;
}


int freyja_model__dof_check(char *filename)
{
	SystemIO::FileReader r;

	if (r.Open(filename))
	{
		char magic[5];
		r.ReadString(4, magic);
		mstl::String symbol(magic);
		r.Close();

		if (symbol == "DOF1")
			return 0;
	}

	return -1;
}

typedef struct {

	int count;
	int *vstart; //(start in vertex[] for the first burst vertex)
	int *vcount; //(number of floats in vertex[] to use for this burst)
	int *matid; //(material selection for this burst)
	int *edges; //(should always be 3 for all bursts) -- triangles vert per poly

} dof_burst_t;


int freyja_model__dof_import(char *filename)
{
	SystemIO::FileReader r;

	int pluginId = freyjaGetPluginId();

	vec_t scale;
	freyjaGetPluginArg1f(pluginId, "scale", &scale);

	if (freyja_model__dof_check(filename) < 0 || !r.Open(filename))
	{
		return -1;
	}

	index_t model = freyjaModelCreate();
	
	char chunkId[5];
	int32 size, trisCount, vertCount, uvCount, normCount, bCount, colorCount, material;
	short *tris = NULL;
	float *verts = NULL;
	float *uvs = NULL;
	float *norms = NULL;
	float *colors = NULL;
	dof_burst_t bursts;
	bursts.count = 0;
	index_t mesh;

	while ((r.ReadString(4, chunkId)) && !r.IsEndOfFile())
	{
		chunkId[4] = 0;
		mstl::String symbol(chunkId);

		if (symbol == "DOF1")
		{
			size = r.ReadInt32();
			Print("DOF is %i bytes\n", size);
		}
		else if (symbol == "EDOF")
		{
			Print("End of DOF\n");
			break; // End of DOF model
		}
		else if (symbol == "MEND")
		{
			Print("End of MAT\n");
		}
		else if (symbol == "MATS")
		{
			int chunkSz = r.ReadInt32();
			Print("%s %ibytes\n", symbol.c_str(), chunkSz);
			r.Skip(chunkSz); // skip for now
		}
		else if (symbol == "MAT0")
		{
			int chunkSz = r.ReadInt32();
			Print("%s %ibytes\n", symbol.c_str(), chunkSz);
			r.Skip(chunkSz); // skip for now
		}
		else if (symbol == "GEOB")
		{
			int chunkSz = r.ReadInt32();
			Print("%s %ibytes\n", symbol.c_str(), chunkSz);
			//r.Skip(chunkSz); // skip for now
		}
		else if (symbol == "GOB1")
		{
			int chunkSz = r.ReadInt32();
			Print("%s %ibytes\n", symbol.c_str(), chunkSz);

			// Start a new mesh		
			mesh = freyjaMeshCreate();
			freyjaModelAddMesh(model, mesh);
		}
		// Sub GOB1
		else if (symbol == "GHDR")
		{
			int chunkSz = r.ReadInt32();
			r.ReadInt32(); // unused flags ( no defined behavior )
			r.ReadInt32(); // unused paintflags ( no defined behavior )
			material = r.ReadInt32();
			Print("%s %ibytes, mat = %i\n", symbol.c_str(), chunkSz, material);
			//r.Skip(chunkSz); // skip for now
		}
		else if (symbol == "INDI")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now

			trisCount = r.ReadInt32();
			tris = new short[trisCount];
			r.ReadBuffer(trisCount*2, (unsigned char*)tris);
			//r.ReadInt16Array(trisCount, tris);
			Print("%s %ibytes, %i tris\n", symbol.c_str(), chunkSz, trisCount);
		}
		else if (symbol == "VERT")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now

			vertCount = r.ReadInt32();
			verts = new float[vertCount*3];
			r.ReadFloat32Array(vertCount*3, verts);
			Print("%s %ibytes, %i verts\n", symbol.c_str(), chunkSz, vertCount);

			for (uint32 i = 0, n = vertCount, idx; i < n; ++i)
			{
				idx = i * 3;
				Vec3 v(verts[idx], verts[idx+1], verts[idx+2]);
				v *= scale;
				freyjaMeshVertexCreate3fv(mesh, v.mVec);
			}
		}
		else if (symbol == "TVER")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now

			uvCount = r.ReadInt32();
			uvs = new float[uvCount*2];
			r.ReadFloat32Array(uvCount*2, uvs);
			Print("%s %ibytes, %i uvs\n", symbol.c_str(), chunkSz, uvCount);

			for (uint32 i = 0, n = uvCount, idx; i < n; ++i)
			{
				idx = i * 2;
				//freyjaMeshTexCoordCreate2f(..); // texcoords already created
				freyjaMeshTexCoord2fv(mesh, i, uvs+idx);
			}
		}
		else if (symbol == "NORM")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now

			normCount = r.ReadInt32();
			norms = new float[normCount*3];
			r.ReadFloat32Array(normCount*3, norms);

			Print("%s %ibytes, %i norms\n", symbol.c_str(), chunkSz, normCount);
		}
		else if (symbol == "VCOL")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now

			colorCount = r.ReadInt32();
			colors = new float[colorCount*3];
			r.ReadFloat32Array(colorCount*3, colors);
			Print("%s %ibytes\n", symbol.c_str(), chunkSz);
		}
		else if (symbol == "BRST")
		{
			int chunkSz = r.ReadInt32();
			//r.Skip(chunkSz); continue; // skip for now
			bursts.count = r.ReadInt32();
			bursts.vstart = new int[bursts.count];
			r.ReadInt32Array(bursts.count, bursts.vstart);
			bursts.vcount = new int[bursts.count];
			r.ReadInt32Array(bursts.count, bursts.vcount);
			bursts.matid = new int[bursts.count];
			r.ReadInt32Array(bursts.count, bursts.matid);
			bursts.edges = new int[bursts.count];
			r.ReadInt32Array(bursts.count, bursts.edges);

			Print("%s %ibytes, bursts = %i\n", symbol.c_str(), chunkSz, bursts.count);
		}
		else if (symbol == "GEND")
		{
			Print("End of GOB\n");

			for (uint32 i = 0, n = trisCount/3; i < n; ++i)
			{
				index_t face = freyjaMeshPolygonCreate(mesh);
				freyjaMeshPolygonMaterial(mesh, face, material);
				freyjaMeshPolygonGroup1u(mesh, face, material);  

				for (uint32 j = 0; j < 3; ++j)
				{
					index_t vertex = tris[i*3+j];
					freyjaMeshPolygonAddVertex1i(mesh, face, vertex);

					if (norms)
					{
						freyjaMeshVertexNormal3fv(mesh, vertex, norms+vertex);
					}

					if (uvs) 
					{
						freyjaMeshPolygonAddTexCoord1i(mesh, face, vertex);
					}
				}
			}

			// FIXME: metadata support for the unused stuff might be nice

			bursts.count = 0;
			if (bursts.vstart) delete [] bursts.vstart; bursts.vstart = NULL;
			if (bursts.vcount) delete [] bursts.vcount; bursts.vcount = NULL;
			if (bursts.matid) delete [] bursts.matid; bursts.matid = NULL;
			if (bursts.edges) delete [] bursts.edges; bursts.edges = NULL;

			if (tris) delete [] tris; tris = NULL;
			if (verts) delete [] verts; verts = NULL;
			if (uvs) delete [] uvs; uvs = NULL;
			if (norms) delete [] norms; norms = NULL;
			if (colors) delete [] colors; colors = NULL;
		}


	}

	r.Close();

	return 0;
}


int freyja_model__dof_export(char *filename)
{
	return -1;
}


/*   --- DONATED DOF FORMAT NOTES --- 

The DOF file format is used for storing 3D models. Racer v0.3 used DOF0, but has been superseded by DOF1 since 27-12-00. The format now allows easy skipping of chunks that future/previous versions might not know about.

Loosely inspired by the IFF format (but not really taken over), the format contains 3 conceptual levels, all indicated by 4-character identifiers:

Level 1; objects

    * DOF1; marks the header. It is followed by a 32-bit integer, indicating the length of the DOF1 object to come (0x1D1 in the file below).
    * EDOF; marks the end of the DOF

Level 2; object components

    * MATS; indicates a series of materials coming up. Following by an int indicating the chunk length. After that, another integer follows, indicating the number of materials to come.
    * GEOB; indicates a series of geobs, or Geometrical Objects (I use the term Geode for Geometrical Node, loosely based on SGI's Performer's notion of constructing scene graphs). It also, like MATS, is following by 'int chunkLength, noofGeobs'.
    * MAT0; a material, followed by the chunk length (skip this if you don't know about MAT0's).
      A note about strings:
      Strings are stored as; short length; char text[length].
      In the example case below, both MAT0 strings are empty (2x 0x0000).
      Also note that strings can make the alignment of the rest of the file odd (meaning a string of length 11 will align oncoming identifiers on odd (file) addresses, which may be a performance issue if you load the whole file and peek your way through).
    * GOB1; a geob definition. Followed by the length.

Level 3; object component attributes

These you will find inside the MAT0 and GOB1 chunks. Note that each chunk is directly followed by its length, to make sure future extensions can be easily put in and out. This is with the exception of the GEND and MEND chunks (those do not have a length added)!
First a description of MAT0:

    * MHDR; a header:
      qstring name,className;
    * MCOL; a color chunk, containing:
      float ambient[4];
      float diffuse[4];
      float specular[4];
      float emission[4];
      float shininess;
    * MCFL; material creation flags.
      int creationFlags;
      flags&1 -> environment mapped
      Note that the envmap flag is ignored under v0.5.2; there it is the shader that defines what happens (much more flexible).
    * MUVW; some UVW mapping information for texturing:
      float uvwUoffset;
      float uvwVoffset;
      float uvwUtiling;
      float uvwVtiling;
      float uvwAngle;
      float uvwBlur;
      float uvwBlurOffset;
    * MTRA; transparency and blendMode
      float transparency; (unused)
      int blendMode; (0=no blending, 1=BLEND_SRC_ALPHA=source alpha blending, 2=BLEND_CONST=constant color blending (unused))
    * MTEX; textures (names only, )
      int textures;
      qstring mapName[textures];
      NOTE: no full paths allowed, just the filename part. Maps will be searched in the model images/ path (or from an explicitly given path).
    * MSUB; submaterials.
      int submaterials;
      MAT0[submaterials]; (recursive)
      Submaterials were used mainly for Max's Multi/Sub-object materials. However, this gives some problems using indexed primitives, so I don't use it anymore. Try not to use Multi/Sub-materials but use plain materials instead (which is the most general case).
      The MAT0[] contains ofcourse the entire MAT0 specification in itself.
    * MEND; ends a material definition (must be checked for; more material chunks may be added in the future). Note this doesn't have an added length attribute.

Next, a description of a GOB1 (geometrical definition, or geob):

    * GHDR; header
      int flags; (none defined yet)
      int paintFlags; (none defined yet)
      int materialRef; (the material index)
    * INDI; indices (indexed primitives are used in GOB1)
      int indices;
      short index[indices];
      The index[] array contains indices into the vertex, tvertex etc. arrays. This allows vertex data (position, normal, color etc) to be re-used, and is especially useful on T&L enabled hardware, since on that hardware you have a small cache that remembers the last few transformed & lit vertices. So locality of triangles may mean that the triangle vertices are still in the cache and don't need to be transformed & lit again.
    * VERT; vertices (triangle data only)
      int vertices;
      float vertex[vertices*3];
    * TVER; texture coordinates (2D only)
      int tvertices;
      float tvertex[tvertices*2];
    * NORM; normals (optional, although they may not render without them)
      int normals;
      float normal[normals*3];
      Note that the normals array will normally be the same length as that of VERT (since GOB1 uses indexed primitives, so for every vertex, there is 1 texture coordinate pair (tvertex) and 1 normal.
    * VCOL; vertex colors (optional)
      int vertexColors;
      float vcolor[vertexColors*3];
      Note that the vertex color array will normally be the same length as that of VERT (since GOB1 uses indexed primitives, so for every vertex, there is 1 vertex color. Each color is an RGB triplet; each color varies from 0..1.
    * BRST; burst information (bursts of equal-state triangles)
      int bursts;
      int burstStart[bursts]; (start in vertex[] for the first burst vertex)
      int burstCount[bursts]; (number of floats in vertex[] to use for this burst)
      int burstMtlID[bursts]; (material selection for this burst)
      int burstVperP[bursts]; (should always be 3 for all bursts)
      Note that burstStart/burstCount has a bit of a weird counting mechanism. They don't count elements, but rather real floats, so when using glDrawElements(), you would need to specify a start of burstStart[i]/3 and a count of burstCount[i]/3. This is needed to perhaps support non-triangle data (it did before, but using only triangles is much easier on a lot of the algoritmhs involved).
      In the future, perhaps strips will be supported (making a distinction between lists/strips/fans).
      For just about any DOF out there, you can assume bursts=1, burstStart[0]=0. burstVPerP[0]=3 as well.
      The burstMtlID[0] then is the material index of the geode. burstCount[0] is the number of floating points in the vertex array, so it is vertices*3.
      Bursts>1 is used for multi/sub ASE imports, but in practice this never seems to be used (and is as such probably not even supported in Racer's graphics engine).
    * GEND; ends a geob definition (must be checked for; more geob chunks will be added in the future). Note this doesn't have an added length attribute! (Racer v0.5.2 gives a warning if it detects a possible length attribute).

General notes about the file format:

    * It uses little-endian integers (I'm developing on SGI MIPS, which is big-endian, but PC integers have priority)
    * Note you could concatenate multiple DOF1's in one file; it doesn't rely on an end-of-file to end the DOF description.
    * The XYZ system used is that of OpenGL, so X+ is right, Y+ is up and Z+ is out of the screen (and righthanded).


*/



