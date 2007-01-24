/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : UTMesh
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the Unreal mesh classes
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_UTMESH - Builds UTMesh class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.04:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__MIDGARD_MONGOOSE_UTMESH_H_
#define GUARD__MIDGARD_MONGOOSE_UTMESH_H_

#include <stdlib.h>
#include <stdio.h>
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>


class UTMesh2003
{
public:

	static const unsigned int MAX_VERTICES = 20000;
	static const unsigned int MAX_WEDGES = 50000;
	static const unsigned int MAX_FACES = 40000;

	typedef struct {
		float x;
		float y;
		float z;
		
	} l2_vert_t;

	typedef struct {
		short s;
		float u;
		float v;
		
	} l2_wedge_t;

	typedef struct {
		short a;
		short b;
		short c;
		unsigned char mat;
		unsigned char aux;
		unsigned int group;
		
	} l2_face_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	UTMesh2003() : mVertices(NULL), mWedges(NULL), mFaces(NULL)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of UTMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.04: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~UTMesh2003()
	{
		if (mVertices)
			delete [] mVertices;

		if (mWedges)
			delete [] mWedges;

		if (mFaces)
			delete [] mFaces;
	}
	/*------------------------------------------------------
	 * Pre  : UTMesh object is allocated
	 * Post : Deconstructs an object of UTMesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.04: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	// Just dumped the alpha test code into this class wrapper
	// for now.  I'll come back and do symbol replacement to be 
	// pretty later.

	int read_index(mstl::SystemIO::BufferedFileReader &r, unsigned int &bytes);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool search_for_index(mstl::SystemIO::BufferedFileReader &r,
								 unsigned int offset, unsigned int count, 
								 unsigned int val, 
								 unsigned int &first, unsigned int &bytes);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool search_for_indexf(mstl::SystemIO::BufferedFileReader &r,
								  unsigned int offset, unsigned int count, 
								  unsigned int val, 
								  unsigned int &first, unsigned int &bytes) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void search_for_index_setf(mstl::SystemIO::BufferedFileReader &r,
										unsigned int offset, unsigned int count, 
										unsigned int val, 
										mstl::Vector<int> &size, 
										mstl::Vector<unsigned int> &pos) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void read_index_set(mstl::SystemIO::BufferedFileReader &r,
							  mstl::Vector<int> &set,
							  unsigned int offset, unsigned int count) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool test_vertex_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_vert_t &v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool test_wedge_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_wedge_t &w);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool test_face_offset(mstl::SystemIO::BufferedFileReader &r, unsigned long offset, l2_face_t &face);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool generic_mesh_search(mstl::SystemIO::BufferedFileReader &r,
									 unsigned int &vertCount, unsigned int &vertOffset,
									 unsigned int &wedgeCount, unsigned int &wedgeOffset,
									 unsigned int &faceCount, unsigned int &faceOffset);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool read_skeletalmesh(mstl::SystemIO::BufferedFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	l2_vert_t *mVertices;

	l2_wedge_t *mWedges;

	l2_face_t *mFaces;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif
