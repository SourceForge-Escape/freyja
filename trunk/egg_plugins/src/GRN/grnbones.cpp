#include "grntype.h"
#include "grnbones.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

Bone::Bone()
{
}

Bone::~Bone()
{
}

Bones::Bones()
{
}

Bones::~Bones()
{
}

void Bones::load(cGrannyStream * file,dword boneOffset,dword baseOffset,dword peers)
{
//    assert(file);
	int x;
	union {
		dword d;
		float f;
	} fd;
	dword oldPos;
	Bone *bone;
	for (int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0505:	//skeleton
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0506:	//bone
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				bone=new Bone();
				bone->parent=file->readDword();
				bone->id=bones.size();
				for (x=0;x<3;x++)
				{
					fd.d=file->readDword();
					bone->translate.points[x]=fd.f;
				};
				for (x=0;x<4;x++)
				{
					fd.d=file->readDword();
					bone->quaternion.points[x]=fd.f;
				};
				// there's 9 floats left.. we don't know what they are
				bones.push_back(bone);
				if (bone->parent!=bone->id) //endless loop otherwise
				  bones[bone->parent]->children.push_back(bone);
				file->seekg(oldPos);
				break;
			case 0xCA5E0508:	//bonelist
				load(file,offset,baseOffset,children);
				break;

			default:
				hex(cerr);
				cerr<<"Unknown Bones chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}
