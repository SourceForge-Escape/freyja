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

#include "freyja.h"
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
	Bone *b = FREYJA_NEW Bone();

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
		FREYJA_DELETE b;
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

	// NOTE: This is setup to allow setting parents that don't exist yet
	//       currently to retain 0.9.3 and older plugin compatibility
	if (b && boneIndex != parentIndex)
	{
		b->mParent = parentIndex;

		// Doing AddChild elsewhere for compatibility reasons, but
		// I think it should be done here by 0.9.5 release
#if 0
		Bone *p = Bone::GetBone(parentIndex);

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
		//MARK_MSGF("! bone %i -> %i parent", childIndex, boneIndex);
		b->AddChild(childIndex);
		b->UpdateBindPose();
	}
}


void freyjaBoneTranslate3f(index_t boneIndex, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(x, y, z);
		b->UpdateBindPose();
	}
}


void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vector3d(xyz[0], xyz[1], xyz[2]);
		b->UpdateBindPose();
	}
}


void freyjaBoneRotateEuler3f(index_t boneIndex, vec_t p, vec_t h, vec_t r)
{
	vec3_t phr = {p, h, r};
	freyjaBoneRotateEuler3fv(boneIndex, phr);
}


void freyjaBoneRotateEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation.setByEulerAngles(phr);
		b->UpdateBindPose();
	}
}


void freyjaBoneRotateQuat4f(index_t boneIndex,
							vec_t w, vec_t x, vec_t y, vec_t z)
{
#if 0
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(w, x, y, z);
		b->UpdateBindPose();
	}
#else
	vec4_t wxyz = {w,x,y,z};
	freyjaBoneRotateQuat4fv(boneIndex, wxyz);
#endif
}


void freyjaBoneRotateQuat4fv(index_t boneIndex, vec4_t wxyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mRotation = Quaternion(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
		b->UpdateBindPose();
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

		//MARK_MSGF(" ! get %u %f %f %f", boneIndex, phr[0], phr[1], phr[2]);
	}
}



void freyjaGetBoneWorldPos3fv(index_t boneIndex, vec3_t xyz)
{
#if 1
	index_t parent = freyjaGetBoneParent(boneIndex);
	Vec3 v;

	if (freyjaIsBoneAllocated(parent))
	{
		freyjaGetBoneWorldPos3fv(parent, v.mVec);
	}

	
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		v = b->mRotation.rotate(v);
		v += b->mTranslation;
	}
	
	xyz[0] = v.mVec[0];
	xyz[1] = v.mVec[1];
	xyz[2] = v.mVec[2];

#elif 1
	Bone *b = Bone::GetBone(boneIndex);

	if (!b)
	{
		return;
	}

	Vec3 loc = b->mTranslation;
	Vec3 rot;
	Vec3 v(0,0,0);
	b->mRotation.getEulerAngles(rot.mVec, rot.mVec+1, rot.mVec+2);
	Matrix world;
	//rot *= 57.0f;
	world.rotate(rot.mVec[0], rot.mVec[1], rot.mVec[2]); // R 0 2 1
	world.translate(loc.mVec[0], loc.mVec[1], loc.mVec[2]);
	v = world * v;
	xyz[0] = v.mVec[0];
	xyz[1] = v.mVec[1];
	xyz[2] = v.mVec[2];

	index_t parent = freyjaGetBoneParent(boneIndex);

	if (freyjaIsBoneAllocated(parent))
	{
		vec3_t off;
		freyjaGetBoneWorldPos3fv(parent, off);

		xyz[0] += off[0];
		xyz[1] += off[1];
		xyz[2] += off[2];
	}
#else
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		Vec3 v;
		
		v = b->GetBindPose() * v;
		HEL_VEC3_COPY(v.mVec, xyz);
		MARK_MSGF("%f %f %f", xyz[0], xyz[1], xyz[2]);
	}
#endif
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


void freyjaBoneUpdateBindPose(index_t bone)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->UpdateBindPose();
	}
}


void freyjaBoneGetBindTransform(index_t bone, matrix_t m)
{	
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->mBindPose.getMatrix(m);
	}
}


void freyjaBoneGetBindTransformInverse(index_t bone, matrix_t m)
{	
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->mBindPose.getInvert(m);
	}
}


void freyjaBoneBindTransformVertex(index_t bone, vec3_t p, vec_t w)
{	
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
#if 1 // Local test to factor out API issues
		Matrix m, t;
		Vec3 v(p);
		t.setIdentity();
		t = b->mRotation;
		t.translate(b->mTranslation.mVec);

		Bone *parent = Bone::GetBone(b->GetParent());

		while (parent)
		{
			m.setIdentity();
			m = parent->mRotation;
			m.translate(parent->mTranslation.mVec);

			t = m * t;

			parent = Bone::GetBone(parent->GetParent());
		}

		v = (t * v) * w;
		HEL_VEC3_COPY(v.mVec, p);
#else
		Vec3 v(p);
		v = (b->mBindPose * v) * w;
		HEL_VEC3_COPY(v.mVec, p);
#endif
	}
}


index_t freyjaBoneTrackNew(index_t bone)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		index_t track = b->NewTrack();
		return track;
	}

	return INDEX_INVALID;
}


void freyjaBoneTrackDuration(index_t bone, index_t track, vec_t duration)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);
		t.SetDuration(duration);
	}
}


void freyjaBoneTrackRate(index_t bone, index_t track, vec_t fps)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);
		t.SetRate(fps);
	}
}


index_t freyjaBoneKeyFrameNew(index_t bone, index_t track, vec_t time)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);

		// Rot / Loc now atomix, so instead return index to unallocated key
		//index_t key = t.NewKeyframe(time);
		index_t key = t.GetKeyfameIndex(time);

		return key;
	}

	return INDEX_INVALID;
}


void freyjaBonePosKeyFrame3f(index_t bone, index_t track, index_t key,
							 vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);
		Vec3KeyFrame *k = t.GetLocKeyframe(key);
		
		if (k)
		{
			//k->SetPosition(Vec3(x, y, z));
			k->SetData(Vec3(x, y, z));
		}
	}
}


void freyjaBoneRotKeyFrameEuler3f(index_t bone, index_t track, index_t key,
								  vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);
		Vec3KeyFrame *k = t.GetRotKeyframe(key);
		
		if (k)
		{
			//k->SetEulerRotation(Vec3(x, y, z));
			k->SetData(Vec3(x, y, z));
		}
	}
}


void freyjaBoneRotKeyFrameQuat4f(index_t bone, index_t track, index_t key,
								 vec_t w, vec_t x, vec_t y, vec_t z)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		BoneTrack &t = b->GetTrack(track);
		Vec3KeyFrame *k = t.GetRotKeyframe(key);
		
		if (k)
		{
			Quaternion q;
			q.getEulerAngles(&x, &y, &z);
			//k->SetEulerRotation(Vec3(x, y, z));
			k->SetData(Vec3(x, y, z));
		}
	}
}
