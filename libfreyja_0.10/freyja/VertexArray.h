/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : VertexArray
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the VertexArray class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.10:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_VERTEXARRAY_H_
#define GUARD__FREYJA_VERTEXARRAY_H_

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>

#include "XMLSerializer.h"
#include "freyja.h"

namespace freyja {

class VertexArray : 
		public XMLSerializer
{
public:

	VertexArray( const uint32 count );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	VertexArray( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	VertexArray( const VertexArray& array );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~VertexArray();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual const char* GetType() { return "VertexArray"; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag string (often just class name). 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetVersion() { return 1; } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns serial format version. 
	 *
	 ------------------------------------------------------*/

	bool Serialize( TiXmlElement* parent );
	/*------------------------------------------------------
	 * Pre  : <parent> is this object's parent in XML tree.
	 * Post : Serializes this object to XML.
	 *
	 ------------------------------------------------------*/

	bool Unserialize( TiXmlElement* node );
	/*------------------------------------------------------
	 * Pre  : <node> is this object in XML tree.
	 * Post : Unserializes node from XML.
	 *
	 ------------------------------------------------------*/

	void Clamp1f( const vec_t min, const vec_t max );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Clamps all real values to min / max.
	 *        
	 ------------------------------------------------------*/

	uint32 AppendElement( const vec_t* array, const uint32 size = 3 )
	{
		const uint32 offset = mArray.size();
		for ( uint32 i = 0; i < size; ++i )
		{
			mArray.push_back( array[i] );
		}

		return offset; 
		// element index doesn't make sense here (( mArray.end() / size ) - 1);
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Appends array of <size> to this array.
	 *        Returns offset of appended array.
	 *        
	 ------------------------------------------------------*/

	void GetElement( uint32 idx, const vec_t* array, const uint32 size = 3 );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Copies <size> elements into <array> from offset <idx>.
	 *        
	 ------------------------------------------------------*/

	void SetElement( uint32 idx, const vec_t* array, const uint32 size = 3 );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Copies <size> elements from <array> to offset <idx>.
	 *        
	 ------------------------------------------------------*/

	uint32 NewElement3fv( vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Fills in array gaps or appends with new value.
	 *        
	 ------------------------------------------------------*/

	index_t Append3fv( vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void Addition3fv( const vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 * NOTE : If this is a sparse array ( most of these are ) you operate
	 *        on more than actually used data, however it doesn't use the class
	 *        index system ( so it's likely still faster in general case )
	 *        
	 ------------------------------------------------------*/

	void Transform3fv( const hel::Mat44& mat )
	{ mat.MultiplyVertexArray( mArray.size(), mArray.get_array() );	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void GetTripleVec(Vector<vec_t> &v, index_t tIndex, vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void SetTripleVec(Vector<vec_t> &v, index_t tIndex, const vec3_t xyz);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	const vec_t* GetArray() { return mArray.get_array(); }

	uint32 Size() { return mArray.size(); }
	

private:

	mstl::Vector<vec_t> mArray;     /* Vertex array. */

	mstl::stack<uint32> mGaps;      /* Track any gaps in array usage. */
};



} // End namespace freyja

#endif // GUARD__FREYJA_VERTEXARRAY_H_
