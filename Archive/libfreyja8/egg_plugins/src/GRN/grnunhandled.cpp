#include "grntype.h"
#include "grnunhandled.h"

Unhandled::Unhandled()
{
}

Unhandled::~Unhandled()
{
}

void Unhandled::load( cGrannyStream * file, dword uoffset, dword baseOffset, dword peers )
{
	union {
		dword d;
		float f;
	} df;

	for (dword i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		load(file,offset,baseOffset,children);
		i+=children+1;
	}
}


