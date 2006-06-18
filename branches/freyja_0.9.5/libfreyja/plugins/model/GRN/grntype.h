#ifndef __GRNTYPE_H__
#define __GRNTYPE_H__

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;


class Point
{
	public:
		Point() {for (int i=0;i<4;i++) points[i]=0.0f;}
		~Point() {}
		float points[4];
};

class gPolygon
{
	public:
		gPolygon() {}
		~gPolygon() {}
		dword nodes[6];
};

#endif
