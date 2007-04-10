/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
#include <stdio.h>
#include <string.h>
#include <hel/Mat44.h>
#include <hel/Vector3d.h>
#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <mstl/Thread.h>


void testLinkageOnly()
{
	Vector3d u, v;

	u = u + v;
}


int runLibHelTest(int argc, char *argv[])
{
	unsigned int i, errs;
	hel::Mat44 a, b, c;
	matrix_t m;



	// Test 3 cases of identity use
	for (errs = 0, i = 0; i < 3; ++i)
	{
		// Fill A matrix with garbage
		m[ 0] = m[ 1] = m[ 2] = m[ 3] = 45.0f;
		m[ 4] = m[ 5] = m[ 6] = m[ 7] = 90.0f;
		m[ 8] = m[ 9] = m[10] = m[11] = 180.0f;
		m[12] = m[13] = m[14] = m[15] = 270.0f;
		a.SetMatrix(m);

		switch (i)
		{
		case 0:
			printf("Set to Identity");
			a.SetIdentity();
			break;
		case 1:
			printf("Identity * Identity");
			c.SetIdentity();
			b.SetIdentity();
			a = c * b; 
			break;
		case 2:
			printf("Identity *= Identity");
			a.SetIdentity();
			b.SetIdentity();
			a = a * b;
			break;
		}

		if (a.IsIdentity())
		{
			printf(" \t[ Passed ]\n");
		}
		else
		{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			mstl::String s(a.ToString().c_str());
			s.Replace('|', '\n');
			printf("%s\n", s.c_str());
		}

		printf("\n");
	}

	/* 2003.06.18, Mongoose - These tests are weak and 
		only spot check some of the matrix */

#if 0


	/* Cheap translation test */
	a.SetIdentity();
	printf("I -> Translate (10, 20, 30)\n");
	a.Translate(10, 20, 30);

#ifdef COLUMN_ORDER
	unsigned char i0  = 0, i1  = 4, i2  =  8, i3  = 12;
	unsigned char i4  = 1, i5  = 5, i6  =  9, i7  = 13; 
	unsigned char i8  = 2, i9  = 6, i10 = 10, i11 = 14;
	unsigned char i12 = 3, i13 = 7, i14 = 11, i15 = 15;
#else
	unsigned char i0  =  0, i1  =  1, i2  =  2, i3  =  3;
	unsigned char i4  =  4, i5  =  5, i6  =  6, i7  =  7;
	unsigned char i8  =  8, i9  =  9, i10 = 10, i11 = 11;
	unsigned char i12 = 12, i13 = 13, i14 = 14, i15 = 15;
#endif

	if (a.mMatrix[i12] != 10 || 
		 a.mMatrix[i13] != 20 || 
		 a.mMatrix[i14] != 30)
	{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();		
	}

	/* Cheap X rotation test */
	a.SetIdentity();
	printf("I -> Rotate (90 degrees, 0, 0)\n");
	a.Rotate(90*0.01745329251994329f, 0, 0);

	if (a.mMatrix[i0] != 1 || a.mMatrix[i15] != 1 || 
		 a.mMatrix[i9] != -1 || a.mMatrix[i6] != 1)
	{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();		
	}

	/* Cheap Y rotation test */
	a.setIdentity();
	printf("I -> Rotate (0, 90 degrees, 0)\n");
	a.Rotate(0, 90*0.01745329251994329f, 0);

	if (a.mMatrix[i8] != 1 || a.mMatrix[i2] != -1 ||
		 a.mMatrix[i15] != 1)
	{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();		
	}

	/* Cheap Z rotation test */
	a.SetIdentity();
	printf("I -> Rotate (0, 0, 90 degrees)\n");
	a.Rotate(0, 0, 90*0.01745329251994329f);

	if (a.mMatrix[i4] != -1 || a.mMatrix[i15] != 1 || 
		 a.mMatrix[i1] != 1 || a.mMatrix[i10] != 1)
	{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();		
	}

	printf("\n%i errors\n", errs);
	printf("\n");

	printf("Prescision test...\n");
	printf("I ->\n");
	a.SetIdentity();
	printf(" -> Rotate (0, 0, 90 degrees)\n");
	a.Rotate(0, 0, 90*0.01745329251994329f);
	printf(" -> Translate (10, 20, 30)\n");
	a.Translate(10, 20, 30);
	printf(" -> scale (10, 10, 10)\n");
	a.Scale(10, 10, 10);
	a.print();

	printf("\n");

	printf(" -> scale (0.1, 0.1, 0.1)\n");
	a.Scale(0.1, 0.1, 0.1);
	printf(" -> Translate (-10, -20, -30)\n");
	a.Translate(-10, -20, -30);
	printf(" -> Rotate (0, 0, -90 degrees)\n");
	a.Rotate(0, 0, -90*0.01745329251994329f);
	a.print();	

#endif

	printf("\n%i errors\n", errs);

	return errs;
}

typedef struct unit_test_s {

	int a;
} unit_test_t;

void mat44_unit_test()//unit_test_t &test)
{
	hel::Mat44 m;
	m.Scale(2, 2, 2);
	m.Translate(2, 4, 6);

	printf("\nmat44_unit_test()\n");

	Vec3 v(1, 1, 1);
	printf("v = <%f, %f, %f>\n", v.X, v.Y, v.Z);
	printf("v = <%f, %f, %f> by array access\n", v[0], v[1], v[2]);

	printf("v = T * v\n");
	v = m * v;
	
	printf("v = <%f, %f, %f>\n", v.X, v.Y, v.Z);
	printf("v = <%f, %f, %f> by array access\n", v[0], v[1], v[2]);

	mstl::String s = m.ToString();
	s.Replace('|', '\n');
	printf("T = \n%s\n", s.c_str());
}

void mat44_rot_timing_test()
{
	printf("\nmat44_rot_timing_test()\n");

	const unsigned int trial = 64, inner = 500000;

	mstl::AccumulationStopWatch w;
	mstl::Vector<mstl::AccumulationStopWatch> f1;
	mstl::Vector<mstl::AccumulationStopWatch> f2;

	printf("\nStarting test...\n");
	w.Start();

	{
		hel::Mat44 m;
		mstl::AccumulationStopWatch ac;
		
		for (unsigned int j = trial; j > 0; --j)
		{
			for (unsigned int i = inner; i > 0; --i)
			{ 
				ac.Start();
				m.SetRotation(15.0f, 30.0f, 45.0f);
				ac.Stop();		
			}

			f1.push_back(ac);
			ac.Reset();
			
			for (unsigned int i = inner; i > 0; --i)
			{ 
				ac.Start();
				m.SetRotation2(15.0f, 30.0f, 45.0f);
				ac.Stop();
			}
			
			f2.push_back(ac);
			ac.Reset();
		}
	}

	w.Stop();

	printf("\nTotal test time: %lfs\n", w.GetElapse());
	printf("Generating report...\n");

	double t1 = 0.0, t2 = 0.0;

	for (unsigned int i = 0; i < trial; ++i)
	{
		t1 += f1[i].GetElapse();
		t2 += f2[i].GetElapse();
		printf("%i. f1 = %lfs \t f2 = %lfs\n", 
			   i, f1[i].GetElapse(), f2[i].GetElapse());
	}

	printf("\nTotal test time: %lfs\n", w.GetElapse());
	printf("\nTrials = %u, Inner loops = %u\n", trial, inner);
	printf("\nTotals: %sf1 = %lfs \t %sf2 = %lfs\n", 
		   (t1 < t2) ? "*" : "", t1,
		   (t2 < t1) ? "*" : "", t2);
}

typedef struct {
	mstl::Vector<mstl::AccumulationStopWatch> *f;
	unsigned int trial, inner;

} mat44_rot_timing_thread_arg_t;

void *mat44_rot_timing_thread_helper2(void *arg)
{
	mat44_rot_timing_thread_arg_t &a = (*(mat44_rot_timing_thread_arg_t *)arg);

	printf("\nmat44_rot_timing_thread_helper2(%p, %u, %u)\n",
		   a.f, a.trial, a.inner);

	hel::Mat44 m;
	mstl::AccumulationStopWatch ac, w;

	w.Start();
	for (unsigned int j = a.trial; j > 0; --j)
	{
		for (unsigned int i = a.inner; i > 0; --i)
		{ 
			ac.Start();
			m.SetRotation2(15.0f, 30.0f, 45.0f);
			ac.Stop();		
		}

		a.f->push_back(ac);
		ac.Reset();
	}
	w.Stop();

	printf("Thread2: %lfs\n", w.GetElapse());

	return NULL;
}


void *mat44_rot_timing_thread_helper(void *arg)
{
	mat44_rot_timing_thread_arg_t &a = (*(mat44_rot_timing_thread_arg_t *)arg);

	printf("\nmat44_rot_timing_thread_helper(%p, %u, %u)\n",
		   a.f, a.trial, a.inner);

	hel::Mat44 m;
	mstl::AccumulationStopWatch ac, w;

	w.Start();
	for (unsigned int j = a.trial; j > 0; --j)
	{
		for (unsigned int i = a.inner; i > 0; --i)
		{ 
			ac.Start();
			m.SetRotation(15.0f, 30.0f, 45.0f);
			ac.Stop();		
		}

		a.f->push_back(ac);
		ac.Reset();
	}
	w.Stop();

	printf("Thread1: %lfs\n", w.GetElapse());

	return NULL;
}


void mat44_rot_timing_thread_test()
{
	printf("\nmat44_rot_timing_thread_test()\n");

	const unsigned int trial = 64, inner = 500000;

	mstl::AccumulationStopWatch w;
	mstl::Vector<mstl::AccumulationStopWatch> f1;
	mstl::Vector<mstl::AccumulationStopWatch> f2;
	mstl::Thread r1, r2;
	mat44_rot_timing_thread_arg_t arg1, arg2;

	arg1.f = &f1;
	arg2.f = &f2;
	arg1.trial = arg2.trial = trial;
	arg1.inner = arg2.inner = inner;

	printf("\nStarting test...\n");
	w.Start();

	r1.Create(mat44_rot_timing_thread_helper, (void *)&arg1);
	r2.Create(mat44_rot_timing_thread_helper2, (void *)&arg2);

	r1.Join();
	r2.Join();

	w.Stop();

	printf("\nTotal test time: %lfs\n", w.GetElapse());
	printf("Generating report...\n");

	double t1 = 0.0, t2 = 0.0;

	for (unsigned int i = 0; i < trial; ++i)
	{
		t1 += f1[i].GetElapse();
		t2 += f2[i].GetElapse();
		printf("%i. f1 = %lfs \t f2 = %lfs\n", 
			   i, f1[i].GetElapse(), f2[i].GetElapse());
	}

	printf("\nTotal test time: %lfs\n", w.GetElapse());
	printf("\nTrials = %u, Inner loops = %u\n", trial, inner);
	printf("\nTotals: %sf1 = %lfs \t %sf2 = %lfs\n", 
		   (t1 < t2) ? "*" : "", t1,
		   (t2 < t1) ? "*" : "", t2);
}


void mat44_rot_timing_test_old()
{
	printf("\nmat44_rot_timing_test() - is it better to do partial assignment or memcpy()?  ( trival timing test test )\n");

	const unsigned int trial = 100, inner = 25000;

	mstl::Vector<mstl::AccumulationStopWatch> wAC;

	{
		matrix_t m;
		mstl::AccumulationStopWatch ac;
		
		for (unsigned int j = trial; j > 0; --j)
		{
			for (unsigned int i = inner; i > 0; --i)
			{ 
				ac.Start();
				memcpy(m, hel::Mat44::mIdentity, sizeof(matrix_t));
				ac.Stop();		
			}
			
			wAC.push_back(ac);
			ac.Reset();
		}
	}

	mstl::Vector<mstl::AccumulationStopWatch> wAC2;

	{
		matrix_t m;
		mstl::AccumulationStopWatch ac2;

		for (unsigned int j = trial; j > 0; --j)
		{
			for (unsigned int i = inner; i > 0; --i)
			{ 
				ac2.Start();
				m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0;
				m[15] = 1;
				ac2.Stop();
			}
			
			wAC2.push_back(ac2);
			ac2.Reset();
		}
	}


	double t1 = 0.0, t2 = 0.0;

	for (unsigned int i = trial; i > 0; --i)
	{
		t1 += wAC[i].GetElapse();
		t2 += wAC2[i].GetElapse();
		printf("%i. wAssignment = %lfs \t| wMemcpy = %lfs\n", 
			   (trial - i), wAC2[i].GetElapse(), wAC[i].GetElapse());
	}

	printf("\ntrials = %u, inner loops = %u\n", trial, inner);
	printf("\nwAssignment = %lfs \t| wMemcpy = %lfs\n", t2, t1);
}


void math_test()
{
	// T(I) = I
	matrix_t m = { 1, 0, 0, 0, 
				   0, 1, 0, 0, 
				   0, 0, 1, 0, 
				   0, 0, 0, 1 };

	// translate
	m[12] = 2;
	m[13] = 4;
	m[14] = 6;

	// scale
	m[0]  = 2;
	m[5]  = 2;
	m[10] = 2;


	vec3_t v = { 1, 1, 1 };
	vec3_t result;

	helVectorMatrixMult3v(m, v, result);

	printf("<%f %f %f>\n", result[0], result[1], result[2]);
}



void print_vec_t(vec_t v)
{
	printf("%.3f ", v);
}

void print_vec_array(uint32 offset, uint32 size, vec_t *v)
{
	for ( uint32 i = 0, j = offset; i < size; ++i, j += 3 )
	{
		if ( i % 3 == 0 )
			printf("\n");

		printf("%u. <%.3f %.3f %.3f> ", i, v[j], v[j+1], v[j+2]);
	}

	printf("\n");
}


void mat44_threaded_vlist_test()
{
	printf("\nmat44_threaded_vlist_test()\n");

	// Setup matrix for test transform correctness
	hel::Mat44 m;
	m.Translate(4, 8, 16);

	const uint32 count = 
	//16;
	//100000;
	1000000;
	//100000000;

	double t;

	{
		mstl::Vector<vec_t> v(count*3);
		v.assign_all(0.0f);
		mstl::AccumulationStopWatch w;

		w.Start();
		m.MultiplyVertexArray(count, v.get_array());
		w.Stop();
		t = w.GetElapse();

		if (count <= 16)
		{
			printf("Non-threaded implmenetation\n");
			//v.print(print_vec_t);
			print_vec_array(0, count, v.get_array());
		}
	}

	double timing[5];
	const uint32 threads = 5;

	for (uint32 i = 0, j = 1; i < threads; ++i, j *= 2)
	{
		printf("\nThreads: %u\n", j);

		// Recreated every time to avoid false results
		mstl::Vector<vec_t> v(count*3);
		v.assign_all(0.0f);
		mstl::AccumulationStopWatch w;


		w.Start();
		m.MultiplyVertexArrayThreaded(j, count, v.get_array());
		w.Stop();
		timing[i] = w.GetElapse();

		if (count <= 16)
		{
			//v.print(print_vec_t);
			print_vec_array(0, count, v.get_array());
		}
	}


	// Report results
	printf("\nProcessing %i vertices...\n", count);
	printf("No  Threads:\t %lfs\n", t);

	for (uint32 i = 0, j = 1; i < threads; ++i, j *= 2)
	{
		printf("(%i) Threads:\t %lfs\n", j, timing[i]);
	}
}

void mat44_invert_test()
{
	hel::Mat44 m, n;

	m.Print();
	m.Invert();
	m.Print();

	printf("\n");
	m.Translate(-4, -8, -12);
	m.Print();
	m.Invert();
	m.Print();

	// Compare invert and reversed operations to check result
}


void helMatrixUnitTest()
{
	//math_test();

	//mat44_unit_test();

	//mat44_rot_timing_test();

	//mat44_rot_timing_thread_test();

	//mat44_threaded_vlist_test();

	mat44_invert_test();
}


int main(int argc, char *argv[])
{
	printf("[libhel test]\n");
	//runLibHelTest(argc, argv);

	helMatrixUnitTest();

	return 0;
}
