// A simple OBJ loader
// Terry Hendrix 2006.07.24
//
// Made hastily from my obj load wrapper for freyja
// and my updated mstl STL

#include <ctype.h>

#include <hel/math.h>
#include <hel/Vector3d.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include "Texture.h"

using namespace mstl;

class OpenGLObjModel : public ObjModel
{
public:

	OpenGLObjModel() : 
		ObjModel(),
		mPos(0.0f, 0.0f, 0.0f),
		mOrn(0.0f, 0.0f, 0.0f),
		mScale(1.0f, 1.0f, 1.0f)
	{
	}

	virtual ~OpenGLObjModel()
	{
	}


	virtual bool Load(const char *filename)
	{
		bool loaded = ObjModel::Load(filename);

#if 0
		if (loaded)
		{
			// TODO: Load materials requested and setup mesh texture ids
			int i = mMeshes.begin();
			int count = mMeshes.end();
			for (; i < count; ++i)
			{
				// Init to missing texture
				mMeshes[i].mMaterialIndex = 2;//Texture::mMissingId;

				if (!mMeshes[i].mMaterial.Empty())
				{
					const char *mat = mMeshes[i].mMaterial.GetCString();
					SystemIO::Print("FIXME: Material %s not loaded\n", mat);
					//mMeshes[i].mMaterialIndex = TextureManager::Load(mat);
				}
				else
				{
					SystemIO::Print("Assigning missing texture %i\n", Texture::mMissingId);
				}
			}
		}
#endif
		
		return loaded;
	}


	void Draw()
	{
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);

		glTranslatef(mPos.mVec[0], mPos.mVec[1], mPos.mVec[2]);
		// TODO Rotation
		glScalef(mScale.mVec[0], mScale.mVec[1], mScale.mVec[2]);

		int i = mMeshes.begin();
		int iCount = mMeshes.end();
		for (; i < iCount; ++i)
		{
			glColor3fv(gOpaqueWhite);
			glBindTexture(GL_TEXTURE_2D, mMeshes[i].mMaterialIndex);

			int j = mMeshes[i].mFaces.begin();
			int jCount = mMeshes[i].mFaces.end();

			for (; j < jCount; ++j)
			{
				int k = mMeshes[i].mFaces[j].mVertexRefs.begin();
				int kCount = mMeshes[i].mFaces[j].mVertexRefs.end();

				switch (kCount)
				{
				case 4:
					glBegin(GL_QUADS);
					break;

				default:
					glBegin(GL_TRIANGLE_STRIP);
				}

				for (; k < kCount; ++k)
				{
					if (mMeshes[i].mHasTexCoords)
					{
						uint32 t = mMeshes[i].mFaces[j].mTexCoordRefs[k];		
						glTexCoord3fv(mMeshes[i].mTexCoords[t].mVec);
					}

					if (mMeshes[i].mHasNormals)
					{
						uint32 n = mMeshes[i].mFaces[j].mNormalRefs[k];		
						glNormal3fv(mMeshes[i].mNormals[n].mVec);
					}
					else
					{
						glNormal3f(0,1,0);
					}

					uint32 v = mMeshes[i].mFaces[j].mVertexRefs[k];
					glVertex3fv(mMeshes[i].mVertices[v].mVec);
				}

				glEnd();
			}
			
		}

		glPopMatrix();
	}	

	Vec3 mPos;

	Vec3 mOrn;

	Vec3 mScale;
};





