/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ABI to expose Bone class and utils to C.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Bone.cpp
 ==========================================================================*/

#include "Bone.h"
#include "BoneABI.h"

using namespace freyja;


////////////////////////////////////////////////////////////
// C ABI for Bone
////////////////////////////////////////////////////////////

index_t gFreyjaCurrentBone = INDEX_INVALID;

void freyjaBonePoolClear()
{
	gFreyjaCurrentBone = INDEX_INVALID;
	Bone::ResetPool();
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
	return Bone::GetCount();
}


byte freyjaIsBoneAllocated(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

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
	b->AddToPool();
	return b->GetUID();
}


void freyjaBoneDelete(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->RemoveFromPool();
		delete b;
	}
}


void freyjaBoneFlags(index_t boneIndex, byte flags)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
		b->mFlags = flags;
}


void freyjaBoneParent(index_t boneIndex, index_t parentIndex)
{
	Bone *b = Bone::GetBone(boneIndex);
	Bone *p = Bone::GetBone(parentIndex);

	if (b && boneIndex != parentIndex)
	{
		b->mParent = parentIndex;

		// Doing AddChild elsewhere for compatibility reasons, but
		// I think it should be done here by 0.9.5 release
#if 0
		if (p)
		{
			p->addChild(boneIndex);
			p->updateBoneToWorld();
		}
#endif
	}
}


void freyjaBoneName(index_t boneIndex, const char *name)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
		b->SetName(name);
}


void freyjaBoneRemoveChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b && boneIndex != childIndex)
		b->RemoveChild(childIndex);
}


void freyjaBoneAddChild(index_t boneIndex, index_t childIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b && boneIndex != childIndex)
	{
		freyjaPrintMessage("! bone %i -> %i parent", childIndex, boneIndex);
		b->AddChild(childIndex);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(x, y, z);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(xyz[0], xyz[1], xyz[2]);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateEuler3f(index_t boneIndex, vec_t p, vec_t h, vec_t r)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		MARK_MSGF(" ! set %f %f %f", p, h, r);
		b->mRotation = Quaternion(p, h, r);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		MARK_MSGF(" ! set %f %f %f", phr[0], phr[1], phr[2]);
		b->mRotation.setByEulerAngles(phr);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateQuat4f(index_t boneIndex,
							vec_t w, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(w, x, y, z);
		//b->updateBoneToWorld();
	}
}


void freyjaBoneRotateQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
		//b->updateBoneToWorld();
	}
}


const char *freyjaGetBoneNameString(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		return b->mName;
	}

	return 0x0;
}


void freyjaGetBoneName(index_t boneIndex, uint32 size, char *name)
{
	uint32 i, n = 64;
	Bone *b = Bone::GetBone(boneIndex);

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
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		return b->mParent;
	}

	return INDEX_INVALID;
}


void freyjaGetBoneRotationQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation.getQuaternion4fv(wxyz);
	}
}


void freyjaGetBoneRotationEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::GetBone(boneIndex);
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
		MARK_MSGF(" ! get %f %f %f", phr[0], phr[1], phr[2]);
	}
}


void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		HEL_VEC3_COPY(b->mTranslation.mVec, xyz);
	}
}


index_t freyjaGetBoneSkeletonIndex(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		return b->mSkeleton;
	}

	return INDEX_INVALID;
}


index_t freyjaGetBoneChild(index_t boneIndex, uint32 element)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b && element < b->mChildren.size())
	{
		return b->mChildren[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetBoneChildCount(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

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

