#include "grntype.h"
#include "grnboneties.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

BoneTie::BoneTie()
{
}

BoneTie::~BoneTie()
{
}

BoneTies::BoneTies()
{
}

BoneTies::~BoneTies()
{
}

void BoneTies::load(cGrannyStream * file,dword boneOffset,dword baseOffset,dword peers)
{
//    assert(file);
	int x,j;
	union {
		dword d;
		float f;
	} fd;
	dword oldPos;
	BoneTie *bonetie;
	for (int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0b00:		//boneobject
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c00:		// boneties container
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c02:		// bone objptrs
				objectOffset=offset+baseOffset;
				break;
			case 0xCA5E0c03:		// bonetie group
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c04:		// bonetie data
				break;
			case 0xCA5E0c05:		// end bone objptrs
				endObjOffset=offset+baseOffset;
				oldPos=file->tellg();
				file->seekg(objectOffset);
				for (j=0;j<(endObjOffset-objectOffset)/4;j++)
				{
					dword tie=file->readDword();
					boneObjPtrs.push_back(tie);
				}
				file->seekg(oldPos);
				break;
			case 0xCA5E0c06:		// bonetie container
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c07:		// bone objptrs container
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c08:		// bone objptr
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c09:		// bonetie list
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0c0a:		// bonetie
				bonetie=new BoneTie();
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				bonetie->bone=file->readDword();
				for (x=0;x<7;x++)
				{
					fd.d=file->readDword();
					bonetie->unknown[x]=fd.f;
				}
				boneties.push_back(bonetie);
				file->seekg(oldPos);
				break;
			case 0xCA5E0f04:	// bone object
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				{
					dword obj=file->readDword();
					boneObjects.push_back(obj);
				}
				file->seekg(oldPos);
				break;
			default:
				hex(cerr);
				cerr<<"Unknown BoneTies chunk: "<<chunk<<endl;
				exit(1);
		}
		i+=children+1;
	}
}

dword BoneTies::findID(dword id)
{
	vector<dword>::iterator iobj;
	dword obj=1;
	for (iobj=boneObjects.begin();iobj!=boneObjects.end() &&
			(*iobj)!=id;iobj++,obj++);
	return obj;
}

dword BoneTies::findBone(dword bone)
{
	vector<dword>::iterator ibone;
	dword bn=0;
	for (ibone=boneObjPtrs.begin();ibone!=boneObjPtrs.end() &&
			(*ibone)!=bone;ibone++,bn++);
	return bn;
}
