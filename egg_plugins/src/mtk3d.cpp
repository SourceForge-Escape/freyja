/*===============================================================
 * Project: Mongoose Tool Kit
 * Author : Terry 'Mongoose' Hendrix
 * Website: http://www.westga.edu/~stu7440/
 * Email  : stu7440@westga.edu
 * Object : 
 * Comment: 3d functions.  This is pretty slow.
 * 
 *-- History -----------------------------------------------
 *
 * 2000-03-25:
 * Mongoose - Recreated from GooseEgg prototyping
 *
 * 1999-06-14:
 * Mongoose - Created
 ==============================================================*/

#include <math.h>
#include "mtk3d.h"


/*
double mtk3d_norm3d(double u[3])
{
  return(sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]));
}

double mtk3d_norm2d(double u[2])
{
  return(sqrt(u[0]*u[0]+u[1]*u[1]));
}
*/

void mtkQuaternionToEuler(vec_t x, vec_t y, vec_t z, vec_t w,
			  vec_t *heading, vec_t *bank, vec_t *attitude)
{
  double sqw = w*w;
  double sqx = x*x;
  double sqy = y*y;
  double sqz = z*z;

  *heading = atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw));
  *bank = atan2(2.0 * (y*z + x*w), (-sqx - sqy + sqz + sqw));
  *attitude = asin(-2.0 * (x*z - y*w));
}

void mtkEulerToQuaternion(vec_t roll, vec_t pitch, vec_t yaw, quaternion_t q)
{
  float cr, cp, cy, sr, sp, sy, cpcy, spsy;


  cr = cos(roll/2.0);
  cp = cos(pitch/2.0);
  cy = cos(yaw/2.0);
  
  sr = sin(roll/2.0);
  sp = sin(pitch/2.0);
  sy = sin(yaw/2.0);

  cpcy = cp * cy;
  spsy = sp * sy;

  q.w = cr * cpcy + sr * spsy;
  q.x = sr * cpcy - cr *spsy;
  q.y = cr * sp + sr * cp * sy;
  q.z = cr * cp * sy - sr * sp * cy;
}


void mtkVectorMatrixMult(double v[4], matrix_t m, double result[4])
{
  int i, j;


  for (i = 0; i < 4; ++i)
  {
    result[i] = 0;
    
    for (j = 0; j < 4; ++j)
      result[i] += m[i][j] * v[j];
  }
}

unit_t mtkDist2d(unit_t x, unit_t y, unit_t x2, unit_t y2)
{
   return (sqrt( ((x2 - x)*(x2 - x)) + ((y2 - y)*(y2 - y)) ));
}

unit_t mtkDist3d(unit_t x, unit_t y, unit_t z,
		 unit_t x2, unit_t y2, unit_t z2)
{
   return (sqrt( ((x2 - x)*(x2 - x))+((y2 - y)*(y2 - y))+((z2 - z)*(z2 - z))));
}

unit_t mtkDegToRad(unit_t degrees)
{
   return ((degrees / 180.0) * MTK_PI);
}

void mtkMatrixMult(matrix_t m1, matrix_t m2, matrix_t mp)
{
   int i,j;


   for(i = 0; i < 4; ++i)
      for(j = 0 ; j < 4; ++j)
         mp[i][j] = m1[i][0] * m2[0][j] +
                    m1[i][1] * m2[1][j] +
                    m1[i][2] * m2[2][j] +
                    m1[i][3] * m2[3][j];
}

void mtkMatrixCopy(matrix_t source, matrix_t dest)
{
   int i,j;


   for(i = 0; i < 4; ++i)
      for(j = 0 ; j < 4; ++j)
	 dest[i][j] = source[i][j];
}

void mtkMatrixTransform(matrix_t t, point_t p)
{
   unit_t m1[4];


   m1[0] = p[0];
   m1[1] = p[1];
   m1[2] = p[2];
   m1[3] = 1;

   p[0] = t[0][0]*m1[0] + t[1][0]*m1[1] + t[2][0]*m1[2] + t[3][0]*m1[3];
   p[1] = t[0][1]*m1[0] + t[1][1]*m1[1] + t[2][1]*m1[2] + t[3][1]*m1[3];
   p[2] = t[0][2]*m1[0] + t[1][2]*m1[1] + t[2][2]*m1[2] + t[3][2]*m1[3];
}

void mtkMatrixIdentity(matrix_t matrix)
{
   matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0;
   matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0;
   matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0;
   matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1;
}

void mtkMatrixScale(matrix_t matrix, unit_t sx, unit_t sy, unit_t sz)
{
   matrix_t tmp, smatrix;


   smatrix[0][0] = sx;smatrix[0][1] = 0; smatrix[0][2] = 0; smatrix[0][3] = 0;
   smatrix[1][0] = 0; smatrix[1][1] = sy;smatrix[1][2] = 0; smatrix[1][3] = 0;
   smatrix[2][0] = 0; smatrix[2][1] = 0; smatrix[2][2] = sz;smatrix[2][3] = 0;
   smatrix[3][0] = 0; smatrix[3][1] = 0; smatrix[3][2] = 0; smatrix[3][3] = 1;

   mtkMatrixMult(matrix, smatrix, tmp);
   mtkMatrixCopy(tmp, matrix);
}

void mtkMatrixRotate(matrix_t matrix, unit_t ax, unit_t ay, unit_t az)
{
   matrix_t xmat, ymat, zmat, tmp, tmp2;


   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
   xmat[1][0]=0;        xmat[1][1]=cos(ax);  xmat[1][2]=sin(ax);  xmat[1][3]=0;
   xmat[2][0]=0;        xmat[2][1]=-sin(ax); xmat[2][2]=cos(ax);  xmat[2][3]=0;
   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;        xmat[3][3]=1;

   ymat[0][0]=cos(ay);  ymat[0][1]=0;        ymat[0][2]=-sin(ay); ymat[0][3]=0;
   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
   ymat[2][0]=sin(ay);  ymat[2][1]=0;        ymat[2][2]=cos(ay);  ymat[2][3]=0;
   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

   zmat[0][0]=cos(az);  zmat[0][1]=sin(az);  zmat[0][2]=0;        zmat[0][3]=0;
   zmat[1][0]=-sin(az); zmat[1][1]=cos(az);  zmat[1][2]=0;        zmat[1][3]=0;
   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;

   mtkMatrixMult(matrix, ymat, tmp);
   mtkMatrixMult(tmp, xmat, tmp2);
   mtkMatrixMult(tmp2, zmat, matrix);
}

void mtkMatrixTranslate(matrix_t matrix, unit_t tx, unit_t ty, unit_t tz)
{
   matrix_t tmat, tmp;

   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; tmat[3][3]=1;


   mtkMatrixMult(matrix, tmat, tmp);
   mtkMatrixCopy(tmp, matrix);
}

////////////////////////////////

void mtkQuaternionCopy(quaternion_t source, quaternion_t dest)
{
  dest.x = source.x;
  dest.y = source.y;
  dest.z = source.z;
  dest.w = source.w;
}


void mtkQuaternionToMatrix(quaternion_t q, matrix_t m)
{
  float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

  
  x2 = q.x + q.x;  y2 = q.y + q.y;  z2 = q.z + q.z;
  xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
  yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
  wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;
  
  m[0][0] = 1.0 - (yy + zz);
  m[0][1] = xy - wz;
  m[0][2] = xz + wy;
  m[0][3] = 0.0;
  
  m[1][0] = xy + wz;
  m[1][1] = 1.0 - (xx + zz);
  m[1][2] = yz - wx;
  m[1][3] = 0.0;
  
  m[2][0] = xz - wy;
  m[2][1] = yz + wx;
  m[2][2] = 1.0 - (xx + yy);
  m[2][3] = 0.0;
  
  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}


void mtkQuaternionIdentity(quaternion_t q)
{
  q.w = 1.0;
  q.x = 0.0;
  q.y = 0.0;
  q.z = 0.0;
}


void mtkQuaternionSet(quaternion_t q, float angle, float x, float y, float z)
{
  float temp, dist;

  // Normalize
  temp = x*x + y*y + z*z;

  dist = (float)(1.0 / sqrt(temp));

  x *= dist;
  y *= dist;
  z *= dist;

  q.x = x;
  q.y = y;
  q.z = z;

  q.w = (float)cos(angle / 2.0f);
}


void mtkQuaternionMult(quaternion_t a, quaternion_t b, quaternion_t result)
{
  result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
  result.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
  result.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
  result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

  // Quaternion needs to be a unit quaternion
  mtkQuaternionNormalize(result);  
}


void mtkQuaternionNormalize(quaternion_t q)
{
  float dist, square;


  square = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	
  if (square > 0.0)
    dist = (float)(1.0 / sqrt(square));
  else 
    dist = 1;

  q.x *= dist;
  q.y *= dist;
  q.z *= dist;
  q.w *= dist; 
}

