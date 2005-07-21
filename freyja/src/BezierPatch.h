/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : BezierPatch
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class is the BezierPatch class for Freyja.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_BEZIERPATCH - Builds BezierPatch class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.14:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_BEZIERPATCH_H_
#define GUARD__FREYJA_MONGOOSE_BEZIERPATCH_H_

#include <hel/Vector3d.h>
#include <mstl/Vector.h>
#include <math.h>


class BezierPatch
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	BezierPatch()
	{
		texture = 0;
		displayList = 0;
		x = y = 0;
		
		reset();
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of BezierPatch
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~BezierPatch()
	{
		vertices.erase();
		texcoords.erase();
	}
	/*------------------------------------------------------
	 * Pre  : BezierPatch object is allocated
	 * Post : Deconstructs an object of BezierPatch
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	Vector3d solveBernstein(vec_t u, Vector3d *p)
	{
		Vector3d a, b, c, d, r;

		a = p[0] * pow(u, 3);
		b = p[1] * 3*pow(u, 2)*(1-u);
		c = p[2] * 3*u*pow((1-u), 2);
		d = p[3] * pow((1-u), 3);

		r = (a + b) + (c + d);

		return r;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void generatePolygonMesh(int divs)
	{
		int u = 0, v;
		vec_t py, px, pyold, divsInverse = 1.0f / (vec_t)divs;
		Vector3d temp[4];
		Vector3d *last = new Vector3d[divs+1];  /* Array Of Points To Mark
												   The First Line Of Polygons */

		temp[0] = control[0][3]; // The First Derived Curve (Along X-Axis)
		temp[1] = control[1][3];
		temp[2] = control[2][3];
		temp[3] = control[3][3];

		for (v = 0; v <= divs; ++v)
		{
			// Create The First Line Of Points
			//px = ((vec_t)v)/((vec_t)divs);		// Percent Along Y-Axis
			px = (vec_t)v * divsInverse;		// Percent Along Y-Axis

			/* Use The 4 Points From The Derived Curve To 
			 * Calculate The Points Along That Curve */
			last[v] = solveBernstein(px, temp);
		}

		vertices.erase();
		texcoords.erase();

		for (u = 1; u <= divs; ++u) 
		{
			//py    = ((float)u)/((float)divs);		// Percent Along Y-Axis
			//pyold = ((float)u-1.0f)/((float)divs);	// Percent Along Old Y Axis
			py = (vec_t)u * divsInverse;
			pyold = ((vec_t)u - 1.0f) * divsInverse;

			// Calculate New Bezier Points
			temp[0] = solveBernstein(py, control[0]);
			temp[1] = solveBernstein(py, control[1]);
			temp[2] = solveBernstein(py, control[2]);
			temp[3] = solveBernstein(py, control[3]);

			for (v = 0; v <= divs; ++v)
			{
				//px = ((float)v)/((float)divs);		// Percent Along The X-Axis
				px = (vec_t)v * divsInverse;
				
				vertices.pushBack(new Vector3d(last[v]));
				texcoords.pushBack(new Vector3d(pyold, px, 0));

				last[v] = solveBernstein(px, temp);
				vertices.pushBack(new Vector3d(last[v]));
				texcoords.pushBack(new Vector3d(py, px, 0));
			}
		}

		delete [] last;
	}


	void reset()
	{
		unsigned int i, j;


		control[0][0] = Vector3d(-0.75,	0.50, -0.75);
		control[0][1] = Vector3d(-0.25,	0.00, -0.75);
		control[0][2] = Vector3d( 0.25,	0.00, -0.75);
		control[0][3] = Vector3d( 0.75,	0.50, -0.75);
		control[1][0] = Vector3d(-0.75,	0.75, -0.25);
		control[1][1] = Vector3d(-0.25,	0.50, -0.25);
		control[1][2] = Vector3d( 0.25,	0.50, -0.25);
		control[1][3] = Vector3d( 0.75,	0.75, -0.25);
		control[2][0] = Vector3d(-0.75,	0.00,  0.25);
		control[2][1] = Vector3d(-0.25,	0.50,  0.25);
		control[2][2] = Vector3d( 0.25,	0.50,  0.25);
		control[2][3] = Vector3d( 0.75,	0.00,  0.25);
		control[3][0] = Vector3d(-0.75,	0.50,  0.75);
		control[3][1] = Vector3d(-0.25,	1.00,  0.75);
		control[3][2] = Vector3d( 0.25,	1.00,  0.75);
		control[3][3] = Vector3d( 0.75,	0.50,  0.75);
		
		for (i = 0; i < 4; ++i)
		{
			for(j = 0; j < 4; ++j)
			{
				control[i][j] *= 10.0f;
			}
		}

		vertices.erase();
		texcoords.erase();
	}


	Vector3d control[4][4];

	Vector<Vector3d *> vertices;

	Vector<Vector3d *> texcoords;

	unsigned int texture;

	unsigned int displayList;

	unsigned int x, y;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


};

#endif
