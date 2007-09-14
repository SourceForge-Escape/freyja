
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include <hel/Vec3.h>
#include <hel/Ray.h>

#include <stdio.h>


using namespace hel;

// TODO: Test to validate and roughly compare AABB tests.
bool run_test_aabb_sat(unsigned int count, Vec3& min, Vec3& max, Ray& ray)
{
	Vec3 tuv;
	mstl::AccumulationStopWatch w;
	bool result = false;
	w.Start();

	for ( unsigned int i = 0; i < count; ++i )
	{
		result = ray.IntersectAABB_SAT(min.mVec, max.mVec, tuv.mX);
	}

	w.Stop();
	printf("SAT, %u, %lf, %s, t = %f\n", 
		count, w.GetElapse(), result ? "pass" : "fail", tuv.mX);

	return result;
}


bool run_test_aabb_old(unsigned int count, Vec3& min, Vec3& max, Ray& ray)
{
	Vec3 tuv;
	mstl::AccumulationStopWatch w;
	bool result = false;
	w.Start();

	for ( unsigned int i = 0; i < count; ++i )
	{		
		result = ray.IntersectAABB_Old(min.mVec, max.mVec, tuv.mX);
	}

	w.Stop();
	printf("OLD, %u, %lf, %s, t = %f\n", 
		count, w.GetElapse(), result ? "pass" : "fail", tuv.mX);
	return result;
}


bool run_test_aabb(unsigned int count, Vec3& min, Vec3& max, Ray& ray)
{
	Vec3 tuv;
	mstl::AccumulationStopWatch w;
	bool result = false;
	w.Start();

	for ( unsigned int i = 0; i < count; ++i )
	{
		result = ray.IntersectAABB(min.mVec, max.mVec, tuv.mX);
	}

	w.Stop();
	printf("Default, %u, %lf, %s, t = %f\n", 
		count, w.GetElapse(), result ? "pass" : "fail", tuv.mX);
	
	return result;
}


bool run_test_triangle1(unsigned int count, Vec3& a, Vec3& b, Vec3& c, Ray& ray)
{
	Vec3 tuv;
	mstl::AccumulationStopWatch w;
	bool result = false;
	w.Start();

	for ( unsigned int i = 0; i < count; ++i )
	{
		result = ray.IntersectTriangle(a, b, c, tuv);
	}

	w.Stop();
	printf("#1, %u, %lf, %s, t = %f, uv = ( %f, %f )\n", 
		count, w.GetElapse(), result ? "hit" : "miss", tuv.mX, tuv.mY, tuv.mZ);
	
	return result;
}

bool run_test_triangle2(unsigned int count, Vec3& a, Vec3& b, Vec3& c, Ray& ray)
{
	Vec3 tuv;
	mstl::AccumulationStopWatch w;
	bool result = false;
	w.Start();

	for ( unsigned int i = 0; i < count; ++i )
	{
		result = ray.IntersectTriangle(a.mVec, b.mVec, c.mVec, tuv.mVec);
	}

	w.Stop();
	printf("#2, %u, %lf, %s, t = %f, uv = ( %f, %f )\n", 
		count, w.GetElapse(), result ? "hit" : "miss", tuv.mX, tuv.mY, tuv.mZ);
	
	return result;
}


int run_test(int argc, char *argv[])
{	
	// AABB
	{
		Vec3 min(-10, -10, -10);
		Vec3 max(10, 10 ,10);
		Ray ray( Vec3(0,0,0), Vec3(0,-1,0) );

		printf("--------------------------------------------------------\n");
		run_test_aabb_old(1024, min, max, ray);
		run_test_aabb_old(4096, min, max, ray);
		run_test_aabb_old(10240, min, max, ray);

		run_test_aabb_sat(1024, min, max, ray);
		run_test_aabb_sat(4096, min, max, ray);
		run_test_aabb_sat(10240, min, max, ray);

		run_test_aabb(1024, min, max, ray);
		run_test_aabb(4096, min, max, ray);
		run_test_aabb(10240, min, max, ray);
		printf("--------------------------------------------------------\n");
	}


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

	run_test_triangle1(102400, a, b, c, ray);
	run_test_triangle2(102400, a, b, c, ray);

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


