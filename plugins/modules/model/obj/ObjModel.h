// A simple OBJ loader
// Terry Hendrix 2006.07.24
//
// Made hastily from my obj load wrapper for freyja
// and my updated mstl STL

#ifndef GUARD__FREYJA_OBJMODEL_H_
#define GUARD__FREYJA_OBJMODEL_H_

#include <ctype.h>

#include <hel/math.h>
#include <hel/Vec3.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

using namespace mstl;
using namespace hel;

class ObjModel
{
public:

	class ObjFace
	{
	public:

		ObjFace() :
			mSmoothingGroup(0),
			mTexture(0),
			mVertexRefs(),
			mTexCoordRefs(),
			mNormalRefs()
		{
		}

		ObjFace(const ObjFace &face) :
			mSmoothingGroup(face.mSmoothingGroup),
			mTexture(face.mTexture),
			mVertexRefs(face.mVertexRefs),
			mTexCoordRefs(face.mTexCoordRefs),
			mNormalRefs(face.mNormalRefs)
		{
		}	

		ObjFace &operator =(const ObjFace &face)
		{
			mSmoothingGroup = face.mSmoothingGroup;
			mTexture = face.mTexture;
			mVertexRefs = face.mVertexRefs;
			mTexCoordRefs = face.mTexCoordRefs;
			mNormalRefs = face.mNormalRefs;
			return *this;
		}
		
		~ObjFace()
		{
		}

		int mSmoothingGroup;
		int mTexture;
		Vector<uint32> mVertexRefs;
		Vector<uint32> mTexCoordRefs;
		Vector<uint32> mNormalRefs;
	};

	class ObjMesh
	{
	public:

		ObjMesh() :
			mFaces(),
			mNormals(),
			mTexCoords(),
			mVertices(),
			mMaterial("noshader.tga"),
			mMaterialIndex(0),
			mHasNormals(false),
			mHasTexCoords(false)
		{
		}

		ObjMesh(const ObjMesh &mesh) :
			mFaces(mesh.mFaces),
			mNormals(mesh.mNormals),
			mTexCoords(mesh.mTexCoords),
			mVertices(mesh.mVertices),
			mMaterial(mesh.mMaterial),
			mMaterialIndex(mesh.mMaterialIndex),
			mHasNormals(mesh.mHasNormals),
			mHasTexCoords(mesh.mHasTexCoords)
		{
		}

		ObjMesh &operator =(const ObjMesh &mesh)
		{
			mFaces = mesh.mFaces;
			mNormals = mesh.mNormals;
			mTexCoords = mesh.mTexCoords;
			mVertices = mesh.mVertices;
			mMaterial = mesh.mMaterial;
			mMaterialIndex = mesh.mMaterialIndex;
			mHasNormals = mesh.mHasNormals;
			mHasTexCoords = mesh.mHasTexCoords;
			
			return *this;
		}

		~ObjMesh()
		{
		}


		Vector<ObjFace> mFaces;

		Vector<Vec3> mNormals;

		Vector<Vec3> mTexCoords;

		Vector<Vec3> mVertices;

		String mMaterial;

		uint32 mMaterialIndex;

		bool mHasNormals;

		bool mHasTexCoords;
	};


	ObjModel() : mMeshes(), mTextures()
	{
	}

	virtual ~ObjModel()
	{
	}

	
	virtual bool Load(const char *filename)
	{
		SystemIO::TextFileReader r;
		SystemIO::SetLocaleNumericToC();
		const char *symbol;
		vec_t x, y, z, u, v;
		uint32 mesh = 0, face = 0, smoothinggroup = 0, texture = 0;
		String map;


		if (!r.Open(filename))
		{
			return false;
		}

		SystemIO::Print("Loading '%s'...\n", filename);

		// Start with a single empty mesh
		mMeshes.push_back(ObjMesh());

		while ((symbol = r.ParseSymbol()) && !r.IsEndOfFile())
		{
			if (!strncmp(symbol, "#", 1))
			{
				r.FindNextChar('\n');
			}
			else if (!strcmp(symbol, "mtllib"))
			{
				// Filename of the materials/etc
				symbol = r.ParseSymbol();
				SystemIO::Print("Material list '%s' is not used\n", symbol);
			}
			else if (!strcmp(symbol, "usemtl"))
			{
				// Filename of the material/texture
				symbol = r.ParseSymbol();
				//SystemIO::Print("Material '%s' is not used\n", symbol);
				mMeshes[mesh].mMaterial = String(symbol);
			}
			else if (!strcmp(symbol, "usemap"))
			{
				// Filename of the material/texture
				symbol = r.ParseSymbol();
				map = String(symbol);

				uint32 i;
				bool found = false;
				foreach (mTextures, i)
				{
					if (mTextures[i] == map)
					{
						texture = i;
						found = true;
						break;
					}
				}

				if (!found)
				{
					texture = mTextures.size();
					mTextures.push_back(map);
				}
			}
			else if (!strncmp(symbol, "mg", 2))
			{
				SystemIO::Print("Merging Groups are not supported\n");
			}
			else if (!strncmp(symbol, "vp", 2))
			{
				SystemIO::Print("Parameter space vertices are not supported\n");
			}
			else if (!strcmp(symbol, "vt"))
			{
				u = r.ParseFloat();
				v = r.ParseFloat();

				mMeshes[mesh].mHasTexCoords = true;
				mMeshes[mesh].mTexCoords.push_back(Vec3(u, v, 0.0f));
			}
			else if (!strcmp(symbol, "vn"))
			{
				x = r.ParseFloat();
				y = r.ParseFloat();
				z = r.ParseFloat();

				mMeshes[mesh].mHasNormals = true;
				mMeshes[mesh].mNormals.push_back(Vec3(x, y, z));
			}
			else if (!strcmp(symbol, "v"))
			{
				x = r.ParseFloat();
				y = r.ParseFloat();
				z = r.ParseFloat();

				mMeshes[mesh].mVertices.push_back(Vec3(x, y, z));
				Vec3 v(mMeshes[mesh].mVertices[mMeshes[mesh].mVertices.size()-1]); 
				//SystemIO::Print("v %f %f %f\n", v.mVec[0], v.mVec[1], v.mVec[2]);
			}
			else if (!strcmp(symbol, "o"))
			{
				// Name of the new group, but not used here
				symbol = r.ParseSymbol();
				SystemIO::Print("Object name '%s' is not used\n", symbol);
			}
			else if (!strcmp(symbol, "g"))
			{
				// Name of the new group, but not used here
				symbol = r.ParseSymbol();
				SystemIO::Print("Group name '%s' is not used\n", symbol);
			}
			else if (!strcmp(symbol, "s"))
			{
				int idx = r.ParseInteger();
				smoothinggroup = idx;
				//SystemIO::Print("Smoothing Group %i is not used\n", idx);
			}
			else if (!strcmp(symbol, "f")) // f 1/1/1 2/2/2 3/3/3
			{
				ObjFace f;				

				// FIXME: Add state machine parser to handle more quirks
				// like 'f 1//1' better

				// NOTE: OBJ indices start at 1 not 0
				for (int i = 0, count = 3; i < count; ++i)
				{
					// vertices
					uint32 idx = r.ParseInteger() - 1;
					//SystemIO::Print("** %i\n", idx);
					f.mVertexRefs.push_back(idx);

					f.mSmoothingGroup = smoothinggroup;
					f.mTexture = texture;

					// texcoords
					if (mMeshes[mesh].mHasTexCoords)
					{
						//r.FindNextChar('/');
						r.NextChar(); // '/' no whitespace
						idx = r.ParseInteger() - 1;
						f.mTexCoordRefs.push_back(idx);
					}
					else
					{
						// No texcoords
					}

					// Polymapped normals
					if (mMeshes[mesh].mHasNormals) 
					{
						//r.FindNextChar('/');
						r.NextChar(); // '/' no whitespace
						idx = r.ParseInteger();
						f.mNormalRefs.push_back(idx);
					}

					// Handle quad faces and polygons ( loop invariant trick )
					uint32 off = r.GetOffset();
					symbol = r.ParseSymbol();
					r.SetOffset(off);
					
					if (i == 2 && symbol && isdigit(symbol[0]))
						++count;
				}

				/*SystemIO::Print("f %i %i %i...\n", 
					f.mVertexRefs[0],
					f.mVertexRefs[1],
					f.mVertexRefs[2]
					);
				*/

				mMeshes[mesh].mFaces.push_back(f);
				//mMeshes[mesh].mFaces[face].mMaterialIndex = 0;
				++face;
			}
		}

		return true;
	}


	Vector<ObjMesh> mMeshes;

	Vector<String> mTextures;
};



#endif // GUARD__FREYJA_OBJMODEL_H_

