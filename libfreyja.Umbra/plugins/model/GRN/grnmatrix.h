#ifndef __GRNMATRIX_H__
#define __GRNMATRIX_H__

#include "grntype.h"

class GrnMatrix
{
public:
  GrnMatrix();
  GrnMatrix(float *m);
  ~GrnMatrix();
	void operator*=(const GrnMatrix &m);
	Point operator*(const Point &v);
	void operator=(const GrnMatrix &m);
	float &operator[](int i);
	void invert();
	float matrix[16];
};

#endif
