#ifndef __GRNANIM_H__
#define __GRNANIM_H__

#include <vector>
#include "GrannyStream.h"

class BoneAnim
{
	public:
		BoneAnim();
		~BoneAnim();
		void load( cGrannyStream * file, dword offset, dword baseOffset, dword children );
		std::vector<float> translateTimeline;
		std::vector<float> quaternionTimeline;
		std::vector<Point> translates;
		std::vector<Point> quaternions;
		float length;
		dword id;
	private:
		dword numTranslates,numQuaternions,numUnknowns;
		std::vector< float > unknownTimeline;
		std::vector< Point > unknowns;
};

class Animations
{
public:
  Animations();
  ~Animations();
  void load( cGrannyStream * file,dword offset,dword baseOffset,dword children);
	BoneAnim &getBoneAnim(dword id);
	std::vector<BoneAnim>bones;
private:
};

#endif
