/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "MDLModel.h"


DWORD	CurrentTexture	= 1;
vec3_t	TransformVertices[MAXVERTICES];	// Transformed vertices
vec3_t	LightValues[MAXVERTICES];		// Light surface normals
vec3_t	*TransformVertPtr;
vec3_t	*LightValuesPtr;
vec3_t	LightVector;					// Light vector in model reference frame
vec3_t	BoneLightVector[MAXBONES];		// Light vectors in bone ref frames
long	AmbientLightColor;				// Ambient world light
float	ShadeLight;						// Direct world light
vec3_t	LightColor;
float	BoneTransforms[MAXBONES][3][4];	// Bone transformation matrix


bool TMDLModel::Init(char *Filename)
{ 
	char	TextureName[256];
	char	SeqGroupName[256];

	mTextureCountHack = 0;
	mTextureBufferHack = NULL;

	Header = LoadModel(Filename);

	if (Header == NULL)
	{
		return false;
	}

	if (0 == Header->NumTextures)
	{
		strcpy(TextureName, Filename);
		strcpy(&TextureName[strlen(TextureName) - 4], "T.mdl");

		TextureHeader	= LoadModel(TextureName);
	}
	else
	{
		TextureHeader	= Header;
	}


	if (Header->NumSeqGroups > 1)
	{
		for (long Loop = 1; Loop < Header->NumSeqGroups; Loop++)
		{
			strcpy(SeqGroupName, Filename);
			sprintf(&SeqGroupName[strlen(SeqGroupName) - 4], "%02ld.mdl", Loop);

			AnimationHeader[Loop]	= LoadDemandSequences(SeqGroupName);
		}
	}

	return true;
}

void TMDLModel::DrawModel()
{
	TransformVertPtr	= &TransformVertices[0];
	LightValuesPtr		= &LightValues[0];

	if (0 == Header->NumBodyParts)
	{		
		return;
	}

	glPushMatrix();

    glTranslatef(Origin[0], Origin[1], Origin[2]);

    glRotatef(Rotation[1], 0, 0, 1);
    glRotatef(Rotation[0], 0, 1, 0);
    glRotatef(Rotation[2], 1, 0, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SetUpBones();
	SetupLighting();
	
	for (long Loop = 0; Loop < Header->NumBodyParts; Loop++) 
	{
		SetupModel(Loop);
		DrawPoints();
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPopMatrix();
}

void TMDLModel::AdvanceFrame(float Time)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	if (Time > 0.1f)
	{
		Time	= 0.1f;
	}

	CurrentFrame	+= Time * SeqDescription->Timing;

	if (SeqDescription->NumFrames <= 1)
	{
		CurrentFrame	= 0;
	}
	
	else
	{
		CurrentFrame	-= (long)(CurrentFrame / (SeqDescription->NumFrames - 1)) * 
			(SeqDescription->NumFrames - 1);
	}
}

void TMDLModel::ExtractBoundBox(float *Minimums, float *Maximums)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset);
	
	Minimums[0]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[0];
	Minimums[1]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[1];
	Minimums[2]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[2];

	Maximums[0]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[0];
	Maximums[1]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[1];
	Maximums[2]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[2];
}

long TMDLModel::SetSequence(long Sequence)
{
	if (Sequence > Header->NumSequences)
	{
		Sequence	= 0;
	}

	else if (Sequence < 0)
	{
		Sequence	= Header->NumSequences - 1;
	}

	CurrentSequence	= Sequence;
	CurrentFrame	= 0;

	return	CurrentSequence;
}

long TMDLModel::GetSequenceCount()
{
	return Header->NumSequences - 1;
}

long TMDLModel::GetSequence()
{
	return	CurrentSequence;
}

void TMDLModel::GetSequenceInfo(float *FrameRate, float *GroundSpeed)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	if (SeqDescription->NumFrames > 1)
	{
		*FrameRate		= 256 * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
		*GroundSpeed	= (float)sqrt(SeqDescription->LinearMovement[0] * 
			SeqDescription->LinearMovement[0] + SeqDescription->LinearMovement[1] * 
			SeqDescription->LinearMovement[1] + SeqDescription->LinearMovement[2] * 
			SeqDescription->LinearMovement[2]);

		*GroundSpeed	= *GroundSpeed * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
	}
	
	else
	{
		*FrameRate		= 256.0f;
		*GroundSpeed	= 0.0f;
	}
}

float TMDLModel::SetController(long ControllerIndex, float Value)
{
	tagMDLBoneController	*BoneController	= 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);

	long Loop;

	for (Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (BoneController->Index == ControllerIndex)
		{			
			break;
		}
	}
	
	if (Loop >= Header->NumBoneControllers)
	{
		return	Value;
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start)
		{
			Value	= -Value;
		}
		
		if (BoneController->Start + 359.0f >= BoneController->End)
		{
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180)
			{
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180)
			{				
				Value = Value + 360.0f;
			}
		}
		
		else
		{
			if (Value > 360.0f)
			{
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f)
			{
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0)
	{
		Setting	= 0;
	}
	
	else if (Setting > 255)
	{
		Setting	= 255;
	}

	Controller[ControllerIndex]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetMouth(float Value)
{
	tagMDLBoneController	*BoneController = 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);

	for (long Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (4 == BoneController->Index)
		{
			break;
		}
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start)
		{
			Value = -Value;
		}

		if (BoneController->Start + 359.0f >= BoneController->End)
		{
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180.0f)
			{				
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}
		
		else
		{
			if (Value > 360.0f)
			{
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f)
			{
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(64.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0)
	{
		Setting	= 0;
	}

	if (Setting > 64)
	{
		Setting	= 64;
	}

	MouthPosition	= (BYTE)Setting;

	return	Setting * (1.0f / 64.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetBlending(long Blender, float Value)
{
	tagMDLSeqDescription	*SeqDescription	= 
		(tagMDLSeqDescription *)((BYTE *)Header + Header->SequenceOffset) + CurrentSequence;

	if (0 == SeqDescription->BlendType[Blender])
	{
		return	Value;
	}

	if (SeqDescription->BlendType[Blender] & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (SeqDescription->BlendEnd[Blender] < SeqDescription->BlendStart[Blender])
		{
			Value = -Value;
		}

		if (SeqDescription->BlendStart[Blender] + 359.0f >= SeqDescription->BlendEnd[Blender])
		{
			if (Value > ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) + 180.0f)
			{
				Value = Value - 360.0f;
			}

			if (Value < ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - SeqDescription->BlendStart[Blender]) / 
		(SeqDescription->BlendEnd[Blender] - SeqDescription->BlendStart[Blender]));

	if (Setting < 0)
	{
		Setting	= 0;
	}

	if (Setting > 255)
	{
		Setting = 255;
	}

	Blending[Blender]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (SeqDescription->BlendEnd[Blender] - 
		SeqDescription->BlendStart[Blender]) + SeqDescription->BlendStart[Blender];
}

long TMDLModel::SetBodyGroup(long Group, long Value)
{
	if (Group > Header->NumBodyParts)
	{
		return -1;
	}

	tagMDLBodyPart	*BodyPart	= 
		(tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + Group;

	long	Current	= (CurrentBodyPart / BodyPart->Base) % BodyPart->NumModels;

	if (Value >= BodyPart->NumModels)
	{
		return	Current;
	}

	CurrentBodyPart	= (CurrentBodyPart - (Current * BodyPart->Base) + (Value * BodyPart->Base));

	return	Value;
}

long TMDLModel::SetSkin(long Value)
{
	if (Value < Header->NumSkinFamilies)
	{
		return	CurrentSkin;
	}

	CurrentSkin	= Value;

	return	Value;
}

tagMDLHeader *TMDLModel::LoadModel(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to open %s\n", Filename);
		perror(Filename);

		return NULL;
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	
	
	rewind(Handle);

	Buffer	= new BYTE[Size];
	
	if (NULL == Buffer)
	{
		printf("Unable to allocate memory\n");
		
		//exit(1);
		return NULL;
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	tagMDLHeader	*MDLHeader	= (tagMDLHeader *)Buffer;
	tagMDLTexture	*Texture	= (tagMDLTexture *)(Buffer + MDLHeader->TextureOffset);
	
	if (MDLHeader->TextureOffset != 0)
	{
		for (long Loop = 0; Loop < MDLHeader->NumTextures; Loop++)
		{
			UploadTexture(&Texture[Loop], Buffer + Texture[Loop].Index, Buffer + 
				Texture[Loop].Width * Texture[Loop].Height + Texture[Loop].Index);
		}
	}

	return	(tagMDLHeader *)Buffer;
}

tagMDLSeqHeader *TMDLModel::LoadDemandSequences(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to open %s\n", Filename);
		
		//exit(1);
		return NULL;
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	
	
	rewind(Handle);

	Buffer	= new BYTE[Size];
	
	if (NULL == Buffer)
	{
		printf("Unable to allocate memory\n");
		
		//exit(1);
		return NULL;
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	return	(tagMDLSeqHeader *)Buffer;
}

void TMDLModel::CalcBoneQuaternion(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
	float *Q)
{
	vec3_t			Angle1;
	vec3_t			Angle2;
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{
		if (0 == Anim->Offset[Loop + 3])
		{
			Angle2[Loop]	= Angle1[Loop]	= Bone->Value[Loop + 3];
		}
		
		else
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop + 3]);
			
			long Index	= Frame;
			
			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}
			
			if (AnimValue->Valid > Index)
			{
				Angle1[Loop]	= AnimValue[Index + 1].Value;

				if (AnimValue->Valid > Index + 1)
				{
					Angle2[Loop]	= AnimValue[Index + 2].Value;
				}
				
				else
				{
					if (AnimValue->Total > Index + 1)
					{
						Angle2[Loop]	= Angle1[Loop];
					}

					else
					{						
						Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
					}
				}
			}
			
			else
			{
				Angle1[Loop]	= AnimValue[AnimValue->Valid].Value;
				
				if (AnimValue->Total > Index + 1)
				{
					Angle2[Loop]	= Angle1[Loop];
				}
				
				else
				{
					Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
				}
			}
			
			Angle1[Loop]	= Bone->Value[Loop + 3] + Angle1[Loop] * Bone->Scale[Loop + 3];
			Angle2[Loop]	= Bone->Value[Loop + 3] + Angle2[Loop] * Bone->Scale[Loop + 3];
		}
	}

	if (!VectorCompare(Angle1, Angle2))
	{
		vec4_t	Q1;
		vec4_t	Q2;
		
		AngleQuaternion(Angle1, Q1);
		AngleQuaternion(Angle2, Q2);
		QuaternionSlerp(Q1, Q2, Value, Q);
	}
	
	else
	{
		AngleQuaternion(Angle1, Q);
	}
}

void TMDLModel::CalcBonePosition(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
	float *Pos)
{
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{
		Pos[Loop]	= Bone->Value[Loop];
		
		if (Anim->Offset[Loop] != 0)
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop]);
			
			long	Index	= Frame;
			
			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}
			
			if (AnimValue->Valid > Index)
			{
				if (AnimValue->Valid > Index + 1)
				{
					Pos[Loop]	+= (AnimValue[Index + 1].Value * (1.0f - Value) + Value * 
						AnimValue[Index + 2].Value) * Bone->Scale[Loop];
				}
				
				else
				{
					Pos[Loop]	+= AnimValue[Index + 1].Value * Bone->Scale[Loop];
				}
			}
			
			else
			{
				if (AnimValue->Total <= Index + 1)
				{
					Pos[Loop]	+= (AnimValue[AnimValue->Valid].Value * (1.0f - Value) + Value * 
						AnimValue[AnimValue->Valid + 2].Value) * Bone->Scale[Loop];
				}
				
				else
				{
					Pos[Loop]	+= AnimValue[AnimValue->Valid].Value * Bone->Scale[Loop];
				}
			}
		}
	}
}

void TMDLModel::CalcRotations(vec3_t *Pos, vec4_t *Q, tagMDLSeqDescription *SeqDescription, 
	tagAnimation *Anim, float FrameValue)
{
	long		Frame;
	float		Fractional;

	Frame		= (long)FrameValue;
	Fractional	= (FrameValue - Frame);

	tagMDLBone	*Bone	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);
	
	for (long Loop = 0; Loop < Header->NumBones; Loop++, Bone++, Anim++) 
	{
		CalcBoneQuaternion(Frame, Fractional, Bone, Anim, Q[Loop]);
		CalcBonePosition(Frame, Fractional, Bone, Anim, Pos[Loop]);
	}

	if (SeqDescription->MotionType & TRANSITION_X)
	{
		Pos[SeqDescription->MotionBone][0]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Y)
	{
		Pos[SeqDescription->MotionBone][1]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Z)
	{
		Pos[SeqDescription->MotionBone][2]	= 0.0f;
	}
}

tagAnimation *TMDLModel::GetAnim(tagMDLSeqDescription *SeqDescription)
{
	tagMDLSeqGroup	*pseqgroup	= (tagMDLSeqGroup *)((BYTE *)Header + 
		Header->SeqGroupOffset) + SeqDescription->SeqGroup;

	if (SeqDescription->SeqGroup == 0)
	{
		return	(tagAnimation *)((BYTE *)Header + pseqgroup->Data + 
			SeqDescription->AnimOffset);
	}

	return	(tagAnimation *)((BYTE *)AnimationHeader[SeqDescription->SeqGroup] + 
		SeqDescription->AnimOffset);
}

void TMDLModel::SlerpBones(vec4_t Q1[], vec3_t Pos1[], vec4_t Q2[], vec3_t Pos2[], float Value)
{
	vec4_t	Q3;
	float	Inverse;

	if (Value < 0.0f)
	{
		Value	= 0.0f;
	}
	
	else if (Value > 1.0f)
	{
		Value	= 1.0f;
	}

	Inverse	= 1.0f - Value;

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionSlerp(Q1[Loop], Q2[Loop], Value, Q3);
		Q1[Loop][0]		= Q3[0];
		Q1[Loop][1]		= Q3[1];
		Q1[Loop][2]		= Q3[2];
		Q1[Loop][3]		= Q3[3];
		Pos1[Loop][0]	= Pos1[Loop][0] * Inverse + Pos2[Loop][0] * Value;
		Pos1[Loop][1]	= Pos1[Loop][1] * Inverse + Pos2[Loop][1] * Value;
		Pos1[Loop][2]	= Pos1[Loop][2] * Inverse + Pos2[Loop][2] * Value;
	}
}

void TMDLModel::SetUpBones()
{
	float			BoneMatrix[3][4];
	static vec3_t	Pos1[MAXBONES];
	static vec4_t	Q1[MAXBONES];
	static vec3_t	Pos2[MAXBONES];
	static vec4_t	Q2[MAXBONES];
	static vec3_t	Pos3[MAXBONES];
	static vec4_t	Q3[MAXBONES];
	static vec3_t	Pos4[MAXBONES];
	static vec4_t	Q4[MAXBONES];

	if (CurrentSequence >= Header->NumSequences)
	{
		CurrentSequence	= 0;
	}

	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	tagAnimation	*Anim	= GetAnim(SeqDescription);
	
	CalcRotations(Pos1, Q1, SeqDescription, Anim, CurrentFrame);

	if (SeqDescription->NumBlends > 1)
	{
		float	Value;

		Anim	+= Header->NumBones;
		CalcRotations(Pos2, Q2, SeqDescription, Anim, CurrentFrame);
		
		Value	= Blending[0] / 255.0f;

		SlerpBones(Q1, Pos1, Q2, Pos2, Value);

		if (4 == SeqDescription->NumBlends)
		{
			Anim	+= Header->NumBones;
			CalcRotations(Pos3, Q3, SeqDescription, Anim, CurrentFrame);

			Anim	+= Header->NumBones;
			CalcRotations(Pos4, Q4, SeqDescription, Anim, CurrentFrame);

			Value	= Blending[0] / 255.0f;
			SlerpBones(Q3, Pos3, Q4, Pos4, Value);

			Value	= Blending[1] / 255.0f;
			SlerpBones(Q1, Pos1, Q3, Pos3, Value);
		}
	}

	tagMDLBone	*Bones	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionMatrix(Q1[Loop], BoneMatrix);

		BoneMatrix[0][3]	= Pos1[Loop][0];
		BoneMatrix[1][3]	= Pos1[Loop][1];
		BoneMatrix[2][3]	= Pos1[Loop][2];

		if (-1 == Bones[Loop].Parent)
		{
			memcpy(BoneTransforms[Loop], BoneMatrix, sizeof(float) * 12);
		} 
		
		else
		{
			R_ConcatTransforms(BoneTransforms[Bones[Loop].Parent], BoneMatrix, 
				BoneTransforms[Loop]);
		}
	}
}

void TMDLModel::Lighting(float *lv, long Bone, long Flags, vec3_t Normal)
{
	float 	Illumination;
	float	LightCosine;

	Illumination	= (float)AmbientLightColor;

	if (Flags & LIGHT_FLATSHADE)
	{
		Illumination	+= ShadeLight * 0.8f;
	} 
	
	else 
	{
		float	Value;

		LightCosine	= DotProduct (Normal, BoneLightVector[Bone]);

		if (LightCosine > 1.0f)
		{			
			LightCosine = 1.0f;
		}

		Illumination	+= ShadeLight;

		Value	= 1.5f;
		
		if (Value <= 1.0f)
		{
			Value	= 1.0f;
		}

		LightCosine	= (LightCosine + (Value - 1.0f)) / Value; 
		
		if (LightCosine > 0.0f) 
		{
			Illumination	-= ShadeLight * LightCosine; 
		}
		
		if (Illumination <= 0)
		{
			Illumination	= 0;
		}
	}

	if (Illumination > 255) 
	{
		Illumination	= 255;
	}

	*lv	= Illumination / 255.0f;
}

void TMDLModel::SetupLighting()
{
	AmbientLightColor	= 32;
	ShadeLight			= 192;

	LightVector[0]		= 0.0f;
	LightVector[1]		= 0.0f;
	LightVector[2]		= -1.0f;

	LightColor[0]		= 1.0f;
	LightColor[1]		= 1.0f;
	LightColor[2]		= 1.0f;

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		VectorIRotate(LightVector, BoneTransforms[Loop], BoneLightVector[Loop]);
	}
}

void TMDLModel::SetupModel(long BodyPart)
{
	if (BodyPart > Header->NumBodyParts)
	{
		BodyPart	= 0;
	}

	tagMDLBodyPart   *BodyPartPtr	= (tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + 
		BodyPart;

	long	Index	= CurrentBodyPart / BodyPartPtr->Base;
	Index			= Index % BodyPartPtr->NumModels;

	Model	= (tagMDLModel *)((BYTE *)Header + BodyPartPtr->ModelOffset) + Index;
}

void TMDLModel::DrawPoints()
{
	float			*LightValue;

	BYTE			*BoneVertexPtr	= ((BYTE *)Header + Model->VertexInfoOffset);
	BYTE			*BoneNormalPtr	= ((BYTE *)Header + Model->NormalInfoOffset);
	tagMDLTexture	*Texture		= (tagMDLTexture *)((BYTE *)TextureHeader + 
		TextureHeader->TextureOffset);

	tagMDLMesh		*MeshPtr		= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset);
	vec3_t			*Vertices		= (vec3_t *)((BYTE *)Header + Model->VertexOffset);
	vec3_t			*Normals		= (vec3_t *)((BYTE *)Header + Model->NormalOffset);
	short			*SkinReference	= (short *)((BYTE *)TextureHeader + TextureHeader->SkinOffset);
	
	if (CurrentSkin != 0 && CurrentSkin < TextureHeader->NumSkinFamilies)
	{
		SkinReference	+= (CurrentSkin * TextureHeader->NumSkinReferences);
	}

	long Loop;

	for (Loop = 0; Loop < Model->NumVertices; Loop++)
	{
		VectorTransform(Vertices[Loop], BoneTransforms[BoneVertexPtr[Loop]], 
			TransformVertPtr[Loop]);
	}

	LightValue	= (float *)LightValuesPtr;
	
	for (long OuterLoop = 0; OuterLoop < Model->NumMesh; OuterLoop++) 
	{
		long	Flags	= Texture[SkinReference[MeshPtr[OuterLoop].SkinReference]].Flags;
		
		for (long InnerLoop = 0; InnerLoop < MeshPtr[OuterLoop].NumNormals; InnerLoop++, 
			LightValue += 3, Normals++, BoneNormalPtr++)
		{
			float	Temp;

			Lighting(&Temp, *BoneNormalPtr, Flags, (float *)Normals);

			LightValue[0]	= Temp * LightColor[0];
			LightValue[1]	= Temp * LightColor[1];
			LightValue[2]	= Temp * LightColor[2];
		}
	}

	glCullFace(GL_FRONT);

	for (Loop = 0; Loop < Model->NumMesh; Loop++) 
	{
		MeshPtr				= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset) + Loop;
		short	*Triangles	= (short *)((BYTE *)Header + MeshPtr->TriangleOffset);

		float	U			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Width;
		float	V			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Height;

		glBindTexture(GL_TEXTURE_2D, Texture[SkinReference[MeshPtr->SkinReference]].Index);

		long	NumTriangles;

		while ((NumTriangles = *(Triangles++)))
		{
			if (NumTriangles < 0)
			{
				glBegin(GL_TRIANGLE_FAN);
				NumTriangles	= -NumTriangles;
			}
				
			else
			{
				glBegin(GL_TRIANGLE_STRIP);
			}

			for (; NumTriangles > 0; NumTriangles--, Triangles += 4)
			{
				glTexCoord2f(Triangles[2] * U, Triangles[3] * V);
					
				LightValue	= LightValuesPtr[Triangles[1]];
				glColor4f(LightValue[0], LightValue[1], LightValue[2], 1.0f);

				float	*Vertex	= TransformVertPtr[Triangles[0]];
				glVertex3f(Vertex[0], Vertex[1], Vertex[2]);
			}
			
			glEnd();
		}
	}
}

bool TMDLModel::UploadTexture(tagMDLTexture *Texture, BYTE *Data, BYTE *Palette)
{
	long	Row1[256];
	long	Row2[256];
	long	Column1[256];
	long	Column2[256];
	BYTE	*TextureBuffer;
	BYTE	*BufferPtr;

	long OutWidth, OutHeight;

	for (OutWidth = 1; OutWidth < Texture->Width; OutWidth <<= 1);

	if (OutWidth > 256)
	{
		OutWidth = 256;
	}

	for (OutHeight = 1; OutHeight < Texture->Height; OutHeight <<= 1);

	if (OutHeight > 256)
	{
		OutHeight = 256;
	}

	TextureBuffer	= new BYTE[OutWidth * OutHeight * 4];

	if (NULL == TextureBuffer)
	{
		printf("Unable to allocate memory.\n");

		//exit(1);
		return false;
	}

	BufferPtr	= TextureBuffer;

	long Loop;

	for (Loop = 0; Loop < OutWidth; Loop++)
	{
		Column1[Loop]	= (long)((Loop + 0.25f) * (Texture->Width / (float)OutWidth));
		Column2[Loop]	= (long)((Loop + 0.75f) * (Texture->Width / (float)OutWidth));
	}

	for (Loop = 0; Loop < OutHeight; Loop++)
	{
		Row1[Loop]	= (long)((Loop + 0.25f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;
		
		Row2[Loop]	= (long)((Loop + 0.75f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;
	}

	for (long OuterLoop = 0;  OuterLoop < OutHeight; OuterLoop++)
	{
		for (long InnerLoop = 0; InnerLoop < OutWidth; InnerLoop++, BufferPtr += 4)
		{
			BYTE	*Pixel1	= &Palette[Data[Row1[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel2	= &Palette[Data[Row1[OuterLoop] + Column2[InnerLoop]] * 3];
			BYTE	*Pixel3	= &Palette[Data[Row2[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel4	= &Palette[Data[Row2[OuterLoop] + Column2[InnerLoop]] * 3];

			BufferPtr[0]	= (Pixel1[0] + Pixel2[0] + Pixel3[0] + Pixel4[0]) >> 2;
			BufferPtr[1]	= (Pixel1[1] + Pixel2[1] + Pixel3[1] + Pixel4[1]) >> 2;
			BufferPtr[2]	= (Pixel1[2] + Pixel2[2] + Pixel3[2] + Pixel4[2]) >> 2;
			BufferPtr[3]	= 0xFF;
		}
	}

	glBindTexture(GL_TEXTURE_2D, CurrentTexture);		
	glTexImage2D(GL_TEXTURE_2D, 0, 3, OutWidth, OutHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		TextureBuffer);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture->Index	= CurrentTexture;

	CurrentTexture++;

	//mTextureCountHack = 1;
	//mTextureBufferHack = TextureBuffer;

	delete [] TextureBuffer;

	return true;
}


#ifdef FREYJA_MODEL_PLUGINS

#include <freyja8/EggPlugin.h>


extern "C" {

	int freyja_model__halflife_check(char *filename);
	int freyja_model__halflife_import(char *filename);
}

int freyja_model__halflife_check(char *filename)
{
	FILE *f;
	char header[32];


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("mdl_check> fopen failed");
		return -1;
	}

	fread(&header, 32, 1, f);      
	fclose(f);

	header[4] = 0;

	// HalfLife
	if (strcmp(header, "IDST") == 0)
	{
		return 0;
	}
	return -2;
}


int freyja_model__halflife_import(char *filename)
{
	TMDLModel mdl;
	//unsigned int i, b, f, m, vert;
	//float u, v, w, h;
	//short s, t;


	if (!mdl.Init(filename))
	{
		return -1;
	}

	mdl.SetSequence(0);

	printf("Processing HalfLife textures: ");

#ifdef FIXME
	for (i = 10; i < mdl.mTextureCountHack; ++i)
	{
		if (mdl.mTexture[i].pixmap &&
			mdl.mTexture[i].w > 0 && mdl.mTexture[i].h > 0)
		{
			eggTextureStoreBuffer(mdl.mTexture[i].pixmap, 3, 
								  mdl.mTexture[i].w, mdl.mTexture[i].h,
								  EGG_COLORMODE_RGB);
		}
	}
#endif

#ifdef FIXME
	for (b = 0; b < hl.mBodyPartCount; ++b)
	{
		eggBegin(FREYJA_MESH);
		eggBegin(FREYJA_GROUP);	
		trans.Clear();

		for (i = 0; i < hl.mBodyParts[b].vertexCount; ++i)
		{
			vert = eggVertexStore3f(hl.mBodyParts[b].vertices[i*3][0],
									hl.mBodyParts[b].vertices[i*3][1], 
									hl.mBodyParts[b].vertices[i*3][2]);
			
			// Mongoose 2002.02.09, Generates id translator list
			trans.Add(i, vert);	
		}

		eggEnd(); // FREYJA_GROUP

		for (m = 0; m < hl.mBodyParts[b].meshCount; ++m)
		{
			for (f = 0; f < hl.mBodyParts[b].meshes[m].faceCount; ++f)
			{
				// Start a new polygon
				eggBegin(FREYJA_POLYGON);

				// Store vertices by true id, using translation table
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[0].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[1].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[2].vertindex]);


				/* Generate, Store, and link UVs to polygon */
				if (hl.mImages) // should only be null while fixing this plugin
				{
					w = (float)hl.mImages[hl.mBodyParts[b].meshes[m].material].w;
					h = (float)hl.mImages[hl.mBodyParts[b].meshes[m].material].h;

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[0].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[0].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[1].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[1].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[2].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[2].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					eggTexture1i(hl.mBodyParts[b].meshes[m].material);

					eggEnd(); // FREYJA_POLYGON
				}
			}
		}

		eggEnd(); // FREYJA_MESH
	}

	return 0;
#endif

	return -1;
}

#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MDLMODEL

TMDLModel gModel;
int gAction = 0;
float gUp = 0.0f;


#ifdef HAVE_OPENGL
#   include "../SDLUnitTest.h"

SDLUnitTest ut;


void gameFrame()
{
}

void handleKeyReleaseEvent(unsigned int button, unsigned int mod)
{
}

void handleEventCode(unsigned int event)
{
	switch (event)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;
	}

	handleKeyPressEvent(event, 0);
}

void drawModel()
{
	static float lastTime = 0.0f;
	float time;


	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
	glTranslatef(0.0f, 0.0f, gUp);
	glScalef(3.0f, 3.0f, 3.0f);

	gModel.SetBlending(1, 1.0f);

	time = getTicks() * 0.001f;
	gModel.AdvanceFrame(time - lastTime);
	gModel.DrawModel();
	lastTime = time;

	glPopMatrix();
}

void displayFrame()
{
	static float lastTime = 0.0f;
	float time;

	glClearColor(0.3, 0.3, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0,    0.0, -256.0, 
			  0.0,    8.0,    0.0, 
			  0.0,    1.0,    0.0);

	time = getTicks() * 0.1f;

	if (time - lastTime > 5.0f)
	{
		lastTime = time;

		if (ut.mFlags & SDLUnitTest::fAutoRotate)
			++ut.mYaw;
	}

	/* Start solid color rendering of grid and axis lines */
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	/* Draw light symbol */
	glPushMatrix();
	glTranslatef(ut.mLightPos[0], ut.mLightPos[1], ut.mLightPos[2]);
	glDrawLight(5.0);
	glPopMatrix();

	/* Draw transformed origin axis and grid/floor */
	glPushMatrix();
	glTranslatef(0.0f, -128.0f, 512.0f);
	glRotatef(ut.mPitch, 1, 0, 0);
	glRotatef(ut.mYaw,   0, 1, 0);
	glDrawAxis3f(30.0, 26.25, 3.75);
	glDrawGrid(500.0, 50.0);
	glPopMatrix();

	/* End solid color rendering, restore old rendering flags */
	glPopAttrib();

	/* Draw model */
	glEnable(GL_TEXTURE_2D);

	if (ut.mFlags & SDLUnitTest::fLighting)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, ut.mLightPos);
		glEnable(GL_LIGHTING);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, -120.0f, 512.0f);
	glRotatef(ut.mPitch, 1, 0, 0);
	glRotatef(ut.mYaw,   0, 1, 0);
	glScalef(ut.mScale, ut.mScale, ut.mScale);
	// glDrawAxis3f(30.0, 26.25, 3.75);
	drawModel();
	glPopMatrix();

	/* Render 2d text and masks now */
#ifdef HAVE_SDL_TTF
	if (ut.mFlags & SDLUnitTest::fWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnterMode2d(ut.getWidth(), ut.getHeight());
	ut.drawWidgets();
	glExitMode2d();

	if (ut.mFlags & SDLUnitTest::fWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glFlush();
	ut.swapBuffersGL();
}


void initScene(int argc, char *argv[])
{
	/* OpenGL setup */
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT); 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	/* Texture setup */
	ut.mTexture.reset();
	ut.mTexture.setFlag(Texture::fUseMipmaps);
	ut.mTexture.setMaxTextureCount(64);
	printf("Loading textures: ");

	if (argc > 3 && argv[3] && argv[3][0])
	{
#ifdef FIXME
		gModelRenderer.mFlags |= PSKModelRenderer::fRenderTexture;
		for (unsigned int i = 0; i < gModel.mNumMaterials; ++i)
		{
			char texture[256];
			sprintf(texture, "%s/%s.tga", argv[3], gModel.mMaterials[i].name);
			gModelRenderer.mTextures[i] = ut.mTexture.loadTGA(texture);
			
			if (gModelRenderer.mTextures[i] < 0)
			{
				sprintf(texture, "%s/%s.png", argv[3], 
						gModel.mMaterials[i].name);
				gModelRenderer.mTextures[i] = ut.mTexture.loadPNG(texture);
			}
		}
#endif
	}

	printf(" done\n\n");

#ifdef HAVE_SDL_TTF
	ut.mTexture.loadFontTTF("font.ttf", 32, 126 - 32);
	bufferedPrintf(ut.mMessage, 256, "MDLModel Unit Test");
#endif

	printf("\n");
	printf("----------------------------------\n");
	printf("ESC        - Exit\n");
	printf("F1         - Take screenshot\n");
	printf("F10        - Set window to 640x460\n");
	printf("F11        - Set window to 800x600\n");
	printf("F12        - Set window to 1024x768\n");
	printf("ALT+ENTER  - Toogle fullscreen\n");
	printf("UP/DOWN    - Adjust scene pitch\n");
	printf("RIGHT/LEFT - Adjust scene yaw\n");
	printf("-/=        - Adjust model scaling\n");
	printf(";/'        - Switch PSA frame (EXPERIMENTAL)\n");
	printf("/          - Show PSK frame\n");
	printf("----------------------------------\n");
	printf("1 - Toggle polygon rendering\n");   
	printf("2 - Toggle polygon render debugging\n"); 
	printf("3 - Toggle textured polygons\n");  
	printf("4 - Toggle points\n");  
	printf("7 - Toggle scene rotation\n");
	printf("8 - Toggle alpha blending\n");      
	printf("8 - Toggle OpenGL Lighting\n"); 
	printf("r - Reset mesh to default\n");  
	printf("w - Toggle wireframe rendering\n");
#ifdef INTERACTIVE_BONE_RENDER
	printf("g,h - Select bone Id\n");
	printf("z,x,c, b,n,m - Adjust bone rotatation\n");
#endif
	printf("f - Convert rotation to angles to render correctly\n");
	printf("i,k,j,l - Move light\n");
	printf("----------------------------------\n");
}


void handleKeyPressEvent(unsigned int key, unsigned int mod)
{
	switch (key)
	{
	case SDLK_ESCAPE: /* 0x1B, 27d, ESC */
		exit(0);
		break;
	case SDLK_F1:
		ut.mTexture.glScreenShot("MDLModel.test", 
								ut.getWidth(), ut.getHeight());
		bufferedPrintf(ut.mMessage, 256, "Took screenshot...");
		break;
	case SDLK_F2:
		ut.toggleFullscreen();
		break;
	case SDLK_F9:
		ut.resize(640, 460);
		bufferedPrintf(ut.mMessage, 256, "Switching to 640x480 resolution");
		break;
	case SDLK_F10:
		ut.resize(800, 600);
		bufferedPrintf(ut.mMessage, 256, "Switching to 800x600 resolution");
		break;
	case SDLK_F11:
		ut.resize(1024, 768);
		bufferedPrintf(ut.mMessage, 256, "Switching to 1024x768 resolution");
		break;
	case SDLK_F12:
		ut.resize(1280, 1024);
		bufferedPrintf(ut.mMessage, 256, "Switching to 1280x1024 resolution");
		break;
	case SDLK_RIGHT:
		++ut.mYaw;
		break;
	case SDLK_LEFT:
		--ut.mYaw;
		break;
	case SDLK_UP:
		++ut.mPitch;
		break;
	case SDLK_DOWN:
		--ut.mPitch;
		break;
	case 't':
		ut.setShowCursor(true);
		break;
	case '-':
		if (ut.mScale * 0.15 > 0.000001)
			ut.mScale *= 0.15f;
		break;
	case '=':
		ut.mScale *= 1.15f;
		break;
	case '\'':
		++gUp;
		break;
	case ';':
		--gUp;
		break;
	case '[':
		++gAction;
		if (gAction > gModel.GetSequenceCount())
			gAction = gModel.GetSequenceCount();

		gModel.SetSequence(gAction);
		break;
	case ']':
		--gAction;

		if (gAction < 0)
			gAction = 0;

		gModel.SetSequence(gAction);
		break;
	case '0':
		ut.mFlags ^= SDLUnitTest::fRenderBones;
		break;
	case '1':
		ut.mFlags ^= SDLUnitTest::fRenderFaces;
		break;
	case '2':
		break;
	case '3':
		ut.mFlags ^= SDLUnitTest::fRenderTexture;

		if (ut.mFlags & SDLUnitTest::fRenderTexture)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		break;
	case '4':
		ut.mFlags ^= SDLUnitTest::fRenderPoints;
		break;
	case '7':
		ut.mFlags ^= SDLUnitTest::fAutoRotate;
		break;
	case '8':
		ut.mFlags ^= SDLUnitTest::fAlphaBlending;

		if (ut.mFlags & SDLUnitTest::fAlphaBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glAlphaFunc(GL_GREATER, 0);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		printf("%sabled alpha blending\n",
				(ut.mFlags & SDLUnitTest::fAlphaBlending) ? "En" : "Dis");
		break;
	case '9':
		ut.mFlags ^= SDLUnitTest::fLighting;

		if (ut.mFlags & SDLUnitTest::fLighting)
		{
			static bool firsttime = true;


			glEnable(GL_LIGHTING);

			if (firsttime)
			{
				float specular[] = { 1.0, 1.0, 1.0, 1.0 };
				float ambient[] = { 0.64, 0.64, 0.64, 1.0 };
				float shine[] = { 50.0 };


				// Caustic fx possible with mod amb?
				glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
				glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE,   ambient);
				glMaterialfv(GL_FRONT, GL_SHININESS, shine);
				glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
				glEnable(GL_LIGHT0);

				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

				firsttime = false;
			}
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
		break;
	case 'i':
		ut.mLightPos[1] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'k':
		ut.mLightPos[1] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'j':
		ut.mLightPos[0] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'l':
		ut.mLightPos[0] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'o':
		ut.mLightPos[2] -= 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'u':
		ut.mLightPos[2] += 16.0f;
		glLightfv(GL_LIGHT0,   GL_POSITION,  ut.mLightPos);
		break;
	case 'w':
		ut.mFlags ^= SDLUnitTest::fWireframe;

		if (ut.mFlags & SDLUnitTest::fWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		printf("%sabled wireframe rendering\n", 
				(ut.mFlags & SDLUnitTest::fWireframe) ? "En" : "Dis");
		break;
	}
}

#endif

///////////////////////////////////////////////////


void print_help()
{
	printf("MDLModel.test <options> file.psk [skindir] [file.psa]\n");
	printf("   eg 'PSKModel.test lg model.psk textureDir/ anim.psa'\n");
	printf("\tl - Load model\n");
	printf("\tg - Start GL visual debugger\n");
	printf("\tv - Debug Vertex loader\n");
	printf("\tm - Debug Material loader\n");
	printf("\tf - Debug Face loader\n");
	printf("\tw - Debug Weight loader\n");
	printf("\tb - Debug Bone loader\n");
	printf("\tu - Debug UV loader\n");
	printf("\tp - Debug PSA loader\n");
	printf("\t. - PSA debugger (PSKModel.test . file.psa)\n");
	printf("\t0 - UMOD debugger (PSKModel.test 0 file.utx)\n");
	printf("\t1 - UMOD ascii hex dump  (PSKModel.test 1 file.utx)\n");
	printf("\t2 - UMOD bin obj dump    (PSKModel.test 2 file.utx)\n");
	printf("\t3 - Decrypt pak only     (PSKModel.test 3 file.utx)\n");
	printf("\th - Help\n");
}


int runMDLModelUnitTest(int argc, char *argv[])
{
	bool gl = false;

	ut.mResource.registerSymbolValue("EVENT_SHOWBONES",    (int)'0');
	ut.mResource.registerSymbolValue("EVENT_WIREFRAME",    (int)'w');
	ut.mResource.registerSymbolValue("EVENT_QUIT",         SDLK_ESCAPE);
	ut.mResource.registerSymbolValue("EVENT_SCREENSHOT",   SDLK_F1);
	ut.mResource.registerSymbolValue("EVENT_FULLSCREEN",   SDLK_F2);
	ut.mResource.registerSymbolValue("EVENT_RES640x480",   SDLK_F9);
	ut.mResource.registerSymbolValue("EVENT_RES800x600",   SDLK_F10);
	ut.mResource.registerSymbolValue("EVENT_RES1024x768",  SDLK_F11);
	ut.mResource.registerSymbolValue("EVENT_RES1280x1024", SDLK_F12);
	ut.mResource.evalFile("src/halflife/MDLModelUnitTest.lsp");

	ut.mFlags |= SDLUnitTest::fAutoRotate;
	ut.mYaw = 0.0f;
	ut.mPitch = -20.0f;
	ut.mScale = 1.75f;
	ut.mLightPos[0] = 0.0;
	ut.mLightPos[1] = 512.0;
	ut.mLightPos[2] = 1024.0;
	ut.mLightPos[3] = 0.0;

	if (argc > 2)
	{
		for (int i = 0; argv[1][i] != 0; ++i)
		{
			switch (argv[1][i])
			{
			case 'g':
				gl = true;
				break;
			case 'w':
				//gModel.mFlags |= PSKModel::fDebugWeightLoad;
				break;
			case 'b':
				//gModel.mFlags |= PSKModel::fDebugBoneLoad;
				break;
			case 'v':
				//gModel.mFlags |= PSKModel::fDebugPointLoad;
				break;
			case 'm':
				//gModel.mFlags |= PSKModel::fDebugMattLoad;
				break;
			case 'f':
				//gModel.mFlags |= PSKModel::fDebugFaceLoad;
				break;
			case 'u':
				//gModel.mFlags |= PSKModel::fDebugUVLoad;
				break;
			case 'h':
				print_help();
				break;
			}

			if (gl)
			{
#ifdef HAVE_OPENGL
				ut.initVideo(800, 600, false);
				initScene(argc, argv);
#else
				printf("Build not OpenGL enabled\n");
#endif
			}

			/* Load MDL model */
			gModel.Init(argv[2]);
			gModel.SetSequence(0);
			ut.mFlags |= SDLUnitTest::fRenderFaces;

			if (gl)
			{
#ifdef HAVE_OPENGL
				ut.runGame();
#endif
			}
		}
	}
	else
	{
		printf("%s:\n", argv[0]);
		print_help();
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[MDLModel class test]\n");
	runMDLModelUnitTest(argc, argv);

	return 0;
}
#endif
