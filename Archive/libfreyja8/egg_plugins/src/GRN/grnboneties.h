#ifndef __GRNBONETIES_H__
#define __GRNBONETIES_H__

#include "GrannyStream.h"
#include <vector>

class BoneTie
{
	public:
		BoneTie();
		~BoneTie();
		dword bone;
		float unknown[7];
};

class BoneTies
{
public:
  BoneTies();
  ~BoneTies();
  void load( cGrannyStream * file, dword offset, dword baseOffset, dword children );
	dword findID(dword id);
	dword findBone(dword bone);
	std::vector<dword>boneObjPtrs;
	std::vector<BoneTie *>boneties;
	std::vector<dword>boneObjects;
private:
	dword objectOffset;
	dword endObjOffset;
};

#endif
