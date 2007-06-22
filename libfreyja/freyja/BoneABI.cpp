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
#include "SkeletonABI.h"
#include "BoneABI.h"

using namespace freyja;
using namespace hel;


////////////////////////////////////////////////////////////
// C++ ABI for Bone
////////////////////////////////////////////////////////////

Bone *freyjaGetBoneClass(index_t boneUID)
{
	return Bone::GetBone(boneUID);
}


bool freyjaBoneSaveChunkTextJA(SystemIO::TextFileWriter &w, index_t bone)
{
	freyjaPrintMessage("> Writing out bone %i...", bone);
	Bone *b = freyjaGetBoneClass(bone);
	return b ? b->Serialize(w) : false;
}


bool freyjaBoneLoadChunkTextJA(SystemIO::TextFileReader &r)
{
	index_t bone = freyjaBoneCreate(0); // FIXME: This just assumes one skeleton
	freyjaPrintMessage("> Reading in bone %i...", bone);
	Bone *b = freyjaGetBoneClass(bone);
	return b ? b->Serialize(r) : false;
}


int32 freyjaBoneSaveChunkJA(SystemIO::FileReader &r, index_t bone)
{
	//FIXME
	return 0;
}


int32 freyjaBoneLoadChunkJA(SystemIO::FileReader &r, freyja_file_chunk_t &chunk)
{
	//FIXME
	return 0;
}


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
	return ( Bone::GetBone(boneIndex) ) ? 1 : 0;
}


index_t freyjaBoneCreate(index_t skeletonIndex)
{
	Bone *b = FREYJA_NEW Bone();
	b->mSkeleton = skeletonIndex;
	b->AddToPool();

	freyjaSkeletonAddBone(skeletonIndex, b->GetUID());

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

		// Doing 'AddChild()' elsewhere for compatibility reasons.
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
	{
		b->SetName(name);
	}
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
		b->mTranslation = Vec3(x, y, z);
		b->UpdateBindPose();
	}
}


void freyjaBoneTranslate3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		b->mTranslation = Vec3(xyz[0], xyz[1], xyz[2]);
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
		b->mRotation.SetByEulerAngles(phr);
		b->mRotation.Norm(); // 2007.06.20
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
		b->mRotation = Quat(w, x, y, z);
		b->mRotation.Norm(); // 2007.06.20
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
		b->mRotation = Quat(wxyz);
		b->mRotation.Norm(); // 2007.06.20
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
	Bone *b = Bone::GetBone(boneIndex);
	name[0] = '\0';

	if (b && name != NULL)
	{
		if (size > 64)
			size = 64;

		for (uint32 i = 0; i < size; ++i)
		{
			name[i] = b->mName[i];
		}

		name[size-1] = '\0';
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
		b->mRotation.GetQuat(wxyz);
	}
}


void freyjaGetBoneRotationEuler3fv(index_t boneIndex, vec3_t phr)
{
	Bone *b = Bone::GetBone(boneIndex);
	Quat q;// = Quaternion(phr[0], phr[1], phr[2]); // P H R -> R P Y

	if (b)
	{
		q = b->mRotation;
		//freyjaPrintError("FIXME %p, %s:%i", b, __FILE__, __LINE__);
		q.GetEulerAngles(phr);  // P H R -> H B A
		//vec_t tmp;
		//tmp = phr[0];
		//phr[0] = phr[2];
		//phr[2] = tmp;

		//MARK_MSGF(" ! get %u %f %f %f", boneIndex, phr[0], phr[1], phr[2]);
	}
}


const vec_t *freyjaGetBoneBindPose16fv(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);

	// Mongoose, 2007.06.12: Removed some kind of old test code...

	return b ? b->mBindPose.mMatrix : Mat44::mIdentity;
}


const vec_t *freyjaGetBoneLocalBindTransform16fv(index_t boneIndex)
{
	Bone *b = Bone::GetBone(boneIndex);
	return b ? b->mLocalTransform.mMatrix : Mat44::mIdentity;
}


void freyjaGetBoneWorldPos3fv(index_t boneIndex, vec3_t xyz)
{
// Enable to use bind for a world test ( for testing models with no anim )
#if 0 
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		Vec3 v;
		v = b->GetBindPose() * v;
		helCopyVec3(v.mVec, xyz);
	}
#else
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		Vec3 v; // origin

		// Cached last time / track pair update used here.
		v = b->mTrack.mWorld * v;
		helCopyVec3(v.mVec, xyz);
	}	
#endif
}


void freyjaGetBoneTranslation3fv(index_t boneIndex, vec3_t xyz)
{
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		helCopyVec3(b->mTranslation.mVec, xyz);
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


void freyjaBoneTransform3fv(index_t bone, 
							freyja_transform_action_t action, vec3_t v)
{
	freyjaBoneTransform(bone, action, v[0], v[1], v[2]);
}


void freyjaBoneInverseTransform3fv(index_t bone, 
								   freyja_transform_action_t action, 
								   vec3_t v)
{
	hel::Mat44 m;
	vec3_t xyz;

	switch (action)
	{
	case fTranslate:
		freyjaGetBoneTranslation3fv(bone, xyz);
		xyz[0] -= v[0];
		xyz[1] -= v[1];
		xyz[2] -= v[2];
		freyjaBoneTranslate3fv(bone, xyz);
		break;

	case fRotate:
		freyjaGetBoneRotationEuler3fv(bone, xyz);

		xyz[0] = helDegToRad(v[0] - helRadToDeg(xyz[0]));
		xyz[1] = helDegToRad(v[1] - helRadToDeg(xyz[1]));
		xyz[2] = helDegToRad(v[2] - helRadToDeg(xyz[2]));

		freyjaBoneRotateEuler3fv(bone, xyz);
		break;

	case fScale:
		freyjaGetBoneTranslation3fv(bone, xyz);
		xyz[0] /= v[0];
		xyz[1] /= v[1];
		xyz[2] /= v[2];
		freyjaBoneTranslate3fv(bone, xyz);
		break;

	case fScaleAboutOrigin:
		break;

	case fRotateAboutOrigin:
		break;

	default:
		MSTL_MSG("%s(..., %s, ...) not supported\n", 
				 __func__,
				 freyjaActionToString(action));
	}
}


void freyjaBoneTransform(index_t boneIndex, 
                         freyja_transform_action_t action, 
                         vec_t x, vec_t y, vec_t z)
{
	hel::Mat44 m;
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

		xyz[0] = helDegToRad(x + helRadToDeg(xyz[0]));
		xyz[1] = helDegToRad(y + helRadToDeg(xyz[1]));
		xyz[2] = helDegToRad(z + helRadToDeg(xyz[2]));

		freyjaBoneRotateEuler3fv(boneIndex, xyz);
		break;

	case fScale:
		freyjaGetBoneTranslation3fv(boneIndex, xyz);
		xyz[0] *= x;
		xyz[1] *= y;
		xyz[2] *= z;
		freyjaBoneTranslate3fv(boneIndex, xyz);
		break;

	case fScaleAboutOrigin:
		break;

	case fRotateAboutOrigin:
		break;

	default:
		MSTL_MSG("%s(..., %s, ...) not supported\n", 
				 __func__,
				 freyjaActionToString(action));
	}
}


void freyjaBoneUpdateBindPose(index_t bone)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->UpdateBindPose();
		b->UpdateBindPoseForChildren();

		// Starting in 0.9.5.8 release, enforce a basis world pose
		b->UpdateWorldPose(0, 0.0f);
		b->UpdateWorldPoseForChildren(0, 0.0f);
	}
}


void freyjaBoneGetBindTransform(index_t bone, matrix_t m)
{	
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->mBindPose.GetMatrix(m);
	}
}


void freyjaBoneGetBindTransformInverse(index_t bone, matrix_t m)
{	
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		b->mBindPose.GetInverseMatrix(m);
	}
}


void freyjaBoneBindTransformVertex(index_t boneIndex, vec3_t p, vec_t w)
{	
	Bone *b = Bone::GetBone(boneIndex);

	if (b)
	{
		Vec3 v;
		v = b->GetBindPose() * v;
		v *= w;
		helCopyVec3(v.mVec, p);
	}
}


///////////////////////////////////////////////////////////////////

uint32 freyjaGetBoneTrackCount(index_t bone)
{
	Bone *b = Bone::GetBone(bone);

	if ( b )
	{
		return b->GetTrackCount();
	}

	return 0;
}


uint32 freyjaGetBonePosKeyframeCount(index_t bone, index_t track)
{
	Bone *b = Bone::GetBone(bone);

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetLocKeyframeCount();
	}

	return 0;
}
	

vec_t freyjaGetBonePosKeyframeTime(index_t bone, index_t track, index_t key)
{
	Bone *b = Bone::GetBone(bone);

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetLocKeyframeTime(key);
	}

	return 0.0f;
}


void freyjaGetBonePosKeyframe3fv(index_t bone, index_t track, index_t key,
								 vec3_t xyz)
{
	Bone *b = Bone::GetBone(bone);
	xyz[0] = xyz[1] = xyz[2] = 0.0f;

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetLocKeyframe(key, xyz);
	}
}


uint32 freyjaGetBoneRotKeyframeCount(index_t bone, index_t track)
{
	Bone *b = Bone::GetBone(bone);

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetRotKeyframeCount();
	}

	return 0;
}


vec_t freyjaGetBoneRotKeyframeTime(index_t bone, index_t track, index_t key)
{
	Bone *b = Bone::GetBone(bone);

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetRotKeyframeTime(key);
	}

	return 0.0f;
}


void freyjaGetBoneRotKeyframeEuler3fv(index_t bone, index_t track, index_t key,
								 vec3_t xyz)
{
	Bone *b = Bone::GetBone(bone);
	xyz[0] = xyz[1] = xyz[2] = 0.0f;

	if ( b && track < b->GetTrackCount())
	{
		BoneTrack &t = b->GetTrack(track);
		return t.GetRotKeyframe(key, xyz);
	}
}


///////////////////////////////////////////////////////////////

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
			Quat q;
			q.GetEulerAngles(x, y, z);
			//k->SetEulerRotation(Vec3(x, y, z));
			k->SetData(Vec3(x, y, z));
		}
	}
}
