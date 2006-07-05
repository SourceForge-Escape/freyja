/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Bone
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the skeletal animation subsystem
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#include "Bone.h"
#include "BoneABI.h"

using namespace freyja;

Vector<Bone *> Bone::mGobalPool; 


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Bone::Bone() :
	mFlags(0),
	mSkeleton(INDEX_INVALID),
	mParent(INDEX_INVALID),
	mChildren(),
	mRotation(),
	mTranslation(0.0f, 0.0f, 0.0f),
	mBoneToWorld(),
	mUID(INDEX_INVALID)
{
	mName[0] = '\0';
}


Bone::~Bone()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

index_t Bone::getUID()
{
	return mUID;
}


uint32 Bone::getCount() 
{
	return mGobalPool.size();
} 


Bone *Bone::getBone(index_t uid)
{
	if (uid < mGobalPool.size())
		return mGobalPool[uid];

	return 0x0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Bone::addChild(index_t child)
{
	mChildren.pushBack(child);
}


index_t Bone::addToPool()
{
	uint32 i, count;
	bool found = false;


	if (mUID == INDEX_INVALID)
	{

		/* Setup UID and class container reference */
		mUID = count = mGobalPool.size();

		for (i = 0; i < count; ++i)
		{
			if (mGobalPool[i] == 0x0)
			{
				mUID = i;
				mGobalPool.assign(mUID, this);

				found = true;
			}	
		}

		if (!found)
		{
			mGobalPool.pushBack(this);
		}
	}

	return mUID;
}


void Bone::removeChild(index_t child)
{
	Vector<index_t> children;
	uint32 i, n;

	// Yeah, it's crap -- but it's the only real way to repack w/o errors
	// and adjusting bounds -- also this isn't time sensitive
	for (i = 0, n = mChildren.size(); i < n; ++i)
	{
		if (mChildren[i] != child)
		{
			children.pushBack(mChildren[i]);
		}
	}

	mChildren.clear();

	for (i = 0, n = children.size(); i < n; ++i)
	{
		mChildren.pushBack(children[i]);
	}
}


void Bone::removeFromPool()
{
	if (mUID < mGobalPool.size())
		mGobalPool.assign(mUID, 0x0);

	mUID = INDEX_INVALID;
}


void Bone::resetPool()
{
	mGobalPool.clear();
}


void Bone::setName(const char *name)
{
	uint32 i;

	if (name != 0x0 && name[0] != 0)
	{
		for (i = 0; i < 62 && name[i] != 0; ++i)
		{
			mName[i] = name[i];
		}

		mName[i] = 0;
		
		mName[63] = 0;
	}
}


void Bone::updateBoneToWorld()
{
	Bone *b;
	Matrix m(mRotation);
	uint32 i, n;

	for (i = 0, n = mChildren.size(); i < n; ++i)
	{
		b = getBone(mChildren[i]);

		if (b)
		{
			b->mBoneToWorld.setIdentity();
			b->mBoneToWorld = m; // m.rotate(rxyz); 
			b->mBoneToWorld.translate(mTranslation.mVec);

			vec3_t o = {0,0,0}, t;
			b->mBoneToWorld.multiply3v(o, t);
			
			freyjaPrintMessage("! %i=>%i. %f, %f, %f -> %f, %f, %f",
								mUID, mChildren[i], o[0], o[1], o[2], t[0], t[1], t[2]);

			b->updateBoneToWorld();
		}
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_BONE
int runBoneUnitTest(int argc, char *argv[])
{
	Bone bone;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Bone class test]\n");

	return runBoneUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// C ABI
////////////////////////////////////////////////////////////

index_t gFreyjaCurrentBone = INDEX_INVALID;

void freyjaBonePoolClear()
{
	gFreyjaCurrentBone = INDEX_INVALID;
	Bone::resetPool();
}


index_t freyjaGetCurrentBone()
{
	return gFreyjaCurrentBone;
}


void freyjaCurrentBone(index_t boneIndex)
{
	if (freyjaIsBoneAllocated(boneIndex))
		gFreyjaCurrentBone = boneIndex;
}


uint32 freyjaGetBoneCount()
{
	return Bone::getCount();
}


byte freyjaIsBoneAllocated(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return 1;
	}

	return 0;
}


index_t freyjaBoneCreate(index_t skeletonIndex)
{
	Bone *b = new Bone();

	b->mSkeleton = skeletonIndex;
	b->addToPool();
	return b->getUID();
}


void freyjaBoneDelete(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->removeFromPool();
		delete b;
	}
}


void freyjaBoneFlags(index_t boneIndex, byte flags)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->mFlags = flags;
}


void freyjaBoneParent(index_t boneIndex, index_t parentIndex)
{
	Bone *b = Bone::getBone(boneIndex);
	Bone *p = Bone::getBone(parentIndex);

	if (b && boneIndex != parentIndex)
	{
		b->mParent = parentIndex;

		if (p)
		{
			//p->addChild(boneIndex);
			//p->updateBoneToWorld();
		}
	}
}


void freyjaBoneName(index_t boneIndex, const char *name)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
		b->setName(name);
}


void freyjaBoneRemoveChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b && boneIndex != childIndex)
		b->removeChild(childIndex);
}


void freyjaBoneAddChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b && boneIndex != childIndex)
	{
		freyjaPrintMessage("! bone %i -> %i parent", childIndex, boneIndex);
		b->addChild(childIndex);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(x, y, z);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(xyz[0], xyz[1], xyz[2]);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateEuler3f(index_t boneIndex, vec_t p, vec_t h, vec_t r)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		freyjaPrintMessage(" ! set %f %f %f", p, h, r);
		b->mRotation = Quaternion(p, h, r);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		freyjaPrintMessage(" ! set %f %f %f", phr[0], phr[1], phr[2]);
		b->mRotation.setByEulerAngles(phr);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateQuat4f(index_t boneIndex,
							vec_t w, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(w, x, y, z);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
		//b->updateBoneToWorld();
	}
}


const char *freyjaGetBoneNameString(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mName;
	}

	return 0x0;
}


void freyjaGetBoneName(index_t boneIndex, uint32 size, char *name)
{
	uint32 i, n = 64;
	Bone *b = Bone::getBone(boneIndex);

	if (b && name != 0x0 && size >= 64)
	{
		for (i = 0; i < n; ++i)
		{
			name[i] = b->mName[i];
		}
	}	
}


index_t freyjaGetBoneParent(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mParent;
	}

	return INDEX_INVALID;
}


void freyjaGetBoneRotationQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		b->mRotation.getQuaternion4fv(wxyz);
	}
}


void freyjaGetBoneRotationEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::getBone(boneIndex);
	Quaternion q;// = Quaternion(phr[0], phr[1], phr[2]); // P H R -> R P Y

	if (b)
	{
		q = b->mRotation;
		//freyjaPrintError("FIXME %p, %s:%i", b, __FILE__, __LINE__);
		q.getEulerAngles(phr);  // P H R -> H B A
		//vec_t tmp;
		//tmp = phr[0];
		//phr[0] = phr[2];
		//phr[2] = tmp;
		freyjaPrintMessage(" ! get %f %f %f", phr[0], phr[1], phr[2]);
	}
}


void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		HEL_VEC3_COPY(b->mTranslation.mVec, xyz);
	}
}


index_t freyjaGetBoneSkeletonIndex(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mSkeleton;
	}

	return INDEX_INVALID;
}


index_t freyjaGetBoneChild(index_t boneIndex, uint32 element)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b && element < b->mChildren.size())
	{
		return b->mChildren[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetBoneChildCount(index_t boneIndex)
{
	Bone *b = Bone::getBone(boneIndex);

	if (b)
	{
		return b->mChildren.size();
	}

	return 0;
}

#include <hel/Matrix.h>

void freyjaBoneTransform(index_t boneIndex, 
                         freyja_transform_action_t action, 
                         vec_t x, vec_t y, vec_t z)
{
	Matrix m;
	vec3_t xyz;


	switch (action)
	{
	case fTranslate:
		freyjaGetBoneTranslation3fv(boneIndex, xyz);
		xyz[0] += x;
		xyz[1] += y;
		xyz[2] += z;
		freyjaBoneTranslate3fv(boneIndex, xyz);
		break;

	case fRotate:
		freyjaGetBoneRotationEuler3fv(boneIndex, xyz);

		xyz[0] = HEL_DEG_TO_RAD(x + HEL_RAD_TO_DEG(xyz[0]));
		xyz[1] = HEL_DEG_TO_RAD(y + HEL_RAD_TO_DEG(xyz[1]));
		xyz[2] = HEL_DEG_TO_RAD(z + HEL_RAD_TO_DEG(xyz[2]));

		freyjaBoneRotateEuler3fv(boneIndex, xyz);
		break;

	case fScale:
		freyjaPrintError("FIXME %s:%i", __FILE__, __LINE__);
		break;

	case fScaleAboutPoint:
		freyjaPrintError("FIXME %s:%i", __FILE__, __LINE__);
		break;

	case fRotateAboutPoint:
		freyjaPrintError("FIXME %s:%i", __FILE__, __LINE__);
		break;
	}
}




