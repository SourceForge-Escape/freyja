
#include <stdio.h>
#include <string.h>
#include <hel/Matrix.h>
#include <hel/Vector3d.h>


void testLinkageOnly()
{
	Vector3d u, v;

	u = u + v;
}


int runLibHelTest(int argc, char *argv[])
{
	unsigned int i, errs;
	Matrix a, b, c;
	matrix_t m;



	// Test 3 cases of identity use
	for (errs = 0, i = 0; i < 3; ++i)
	{
		// Fill A matrix with garbage
		m[ 0] = m[ 1] = m[ 2] = m[ 3] = 45.0f;
		m[ 4] = m[ 5] = m[ 6] = m[ 7] = 90.0f;
		m[ 8] = m[ 9] = m[10] = m[11] = 180.0f;
		m[12] = m[13] = m[14] = m[15] = 270.0f;
		a.setMatrix(m);

		switch (i)
		{
		case 0:
			printf("Set to Identity");
			a.setIdentity();
			break;
		case 1:
			printf("Identity * Identity");
			c.setIdentity();
			b.setIdentity();
			a = c * b; 
			break;
		case 2:
			printf("Identity *= Identity");
			a.setIdentity();
			b.setIdentity();
			a = a * b;
			break;
		}

		if (a.isIdentity())
		{
			printf(" \t[ Passed ]\n");
		}
		else
		{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();
		}

		printf("\n");
	}

	/* 2003.06.18, Mongoose - These tests are weak and 
		only spot check some of the matrix */


	/* Cheap translation test */
	a.setIdentity();
	printf("I -> Translate (10, 20, 30)\n");
	a.translate(10, 20, 30);

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
	a.setIdentity();
	printf("I -> Rotate (90 degrees, 0, 0)\n");
	a.rotate(90*0.01745329251994329f, 0, 0);

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
	a.rotate(0, 90*0.01745329251994329f, 0);

	if (a.mMatrix[i8] != 1 || a.mMatrix[i2] != -1 ||
		 a.mMatrix[i15] != 1)
	{
			++errs;
			printf(" \t[ Failed ]\a\n"); // beep
			a.print();		
	}

	/* Cheap Z rotation test */
	a.setIdentity();
	printf("I -> Rotate (0, 0, 90 degrees)\n");
	a.rotate(0, 0, 90*0.01745329251994329f);

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
	a.setIdentity();
	printf(" -> Rotate (0, 0, 90 degrees)\n");
	a.rotate(0, 0, 90*0.01745329251994329f);
	printf(" -> Translate (10, 20, 30)\n");
	a.translate(10, 20, 30);
	printf(" -> scale (10, 10, 10)\n");
	a.scale(10, 10, 10);
	a.print();

	printf("\n");

	printf(" -> scale (0.1, 0.1, 0.1)\n");
	a.scale(0.1, 0.1, 0.1);
	printf(" -> Translate (-10, -20, -30)\n");
	a.translate(-10, -20, -30);
	printf(" -> Rotate (0, 0, -90 degrees)\n");
	a.rotate(0, 0, -90*0.01745329251994329f);
	a.print();	

	printf("\n%i errors\n", errs);

	return errs;
}


int main(int argc, char *argv[])
{
	printf("[libhel test]\n");
	runLibHelTest(argc, argv);

	return 0;
}
