
#include <hel/Vec3.h>
#include <hel/Ray.h>

#include <stdio.h>


using namespace hel;

int run_test(int argc, char *argv[])
{
	Vec3 origin(0, 5, 0);
	Vec3 dir(0, -1, 0);
	Ray ray(origin, dir);

	{
		Vec3 o = ray.GetOrigin();
		if ( o.mX != origin.mX || o.mY != origin.mY || o.mZ != origin.mZ )
		{
			printf("*** Ray construction failed for origin member.\n");
			printf("origin? %f %f %f\n", o.mX, o.mY, o.mZ);
			return -1;
		}
	}

	{
		Vec3 d = ray.GetDir();
		if ( d.mX != dir.mX || d.mY != dir.mY || d.mZ != dir.mZ )
		{
			printf("*** Ray construction failed for dir member.\n");
			printf("dir? %f %f %f\n", d.mX, d.mY, d.mZ);
			return -1;
		}

	}

	Vec3 a(0.5, 0.0, 0.5);
	Vec3 b(-0.5, 0, 0.5);
	Vec3 c(0, 0, -0.5);
	
	Vec3 tuv(0,0,0); // init



	{
		/* Find vectors for two edges sharing vertex <a>. */
		Vec3 edge1 = b - a;
		Vec3 edge2 = c - a;
			
		/* Begin calculating determinant - also used to calculate U parameter. */
		Vec3 pvec;
		Vec3::Cross(dir, edge2, pvec);

		/* If determinant is near zero, ray lies in plane of triangle. */
		vec_t det = Vec3::Dot(edge1, pvec);

		printf(">>> det = %f\n", det);

		vec_t inv_det = 1.0f / det;

		/* Calculate distance from <a> to ray origin. */
		Vec3 tvec = origin - a;

		/* Calculate U parameter and test bounds. */
		vec_t u = Vec3::Dot(tvec, pvec) * inv_det;

		printf(">>> U = %f\n", u);

		/* Prepare to test V parameter. */
		Vec3 qvec;
		Vec3::Cross(tvec, edge1, qvec);

		/* Calculate V parameter and test bounds. */
		vec_t v = Vec3::Dot(dir, qvec) * inv_det;

		printf(">>> V = %f\n", v);
	}

	printf("*** IntersectTriangle Test ***\n");

	if ( ray.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec) )
	{
		printf("*** Hit: t = %f; uv = %f, %f\n", tuv.mX, tuv.mY, tuv.mZ);
		return 0;
	}
	else
	{
		printf("*** Fail, trace missed.\n");
		printf("\tt = %f; uv = %f, %f\n", tuv.mX, tuv.mY, tuv.mZ);
	}
	
	printf("*** IntersectTriangle Vec3 Test ***\n");

	if ( ray.IntersectTriangle(a, b, c, tuv) )
	{
		printf("*** Hit: t = %f; uv = %f, %f\n", tuv.mX, tuv.mY, tuv.mZ);
		return 0;
	}
	else
	{
		printf("*** Fail, trace missed.\n");
		printf("\tt = %f; uv = %f, %f\n", tuv.mX, tuv.mY, tuv.mZ);
	}

	return -1;
}


int main(int argc, char *argv[])
{
	printf("[Ray unit test]\n");
	return run_test(argc, argv);
}


