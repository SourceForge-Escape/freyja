/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Object  : FloatArray
 * Comments: This is the VertexArray class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.10:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_FLOATARRAY_H_
#define GUARD__FREYJA_FLOATARRAY_H_

#include <hel/math.h>
#include <hel/Mat44.h>
#include <mstl/Vector.h>
#include <mstl/stack.h>
#include <mstl/SystemIO.h>

#include "XMLSerializer.h"
#include "freyja.h"

namespace freyja {

class FloatArray : 
		public XMLSerializer
{
public:

	FloatArray( const uint32 count, const uint16 width = 3 );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	FloatArray( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	FloatArray( const FloatArray& array );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~FloatArray();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	void SwapElements( uint32 idx_a, uint32 idx_b, const uint32 size = 3 );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Copies <size> elements from <array> to offset <idx>.
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

	void Get3fv( index_t idx, vec3_t xyz ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	void Set3fv( index_t idx, const vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *        
	 ------------------------------------------------------*/

	const vec_t* GetArray() //const
	{ return mArray.get_array(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns the array.
	 *        
	 ------------------------------------------------------*/

	uint32 GetElementCount() const
	{ return ( GetSize() / GetWidth() ); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns the element count.
	 *        
	 ------------------------------------------------------*/

	uint32 GetSize() const
	{ return mArray.size(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns the array size.
	 *        
	 ------------------------------------------------------*/

	uint16 GetWidth() const
	{ return mWidth; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns the element width.
	 *        
	 ------------------------------------------------------*/

	virtual const char* GetType() const
	{ return "FloatArray"; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag string (often just class name). 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetVersion() const
	{ return 0; } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns serial format version. 
	 *
	 ------------------------------------------------------*/

	bool Serialize( XMLSerializerNode parent ) const;
	/*------------------------------------------------------
	 * Pre  : <parent> is this object's parent in XML tree.
	 * Post : Serializes this object to XML.
	 *
	 ------------------------------------------------------*/

	bool Unserialize( XMLSerializerNode node );
	/*------------------------------------------------------
	 * Pre  : <node> is this object in XML tree.
	 * Post : Unserializes node from XML.
	 *
	 ------------------------------------------------------*/

	
private:

	mstl::Vector<vec_t> mArray;     /* Vertex array. */

	uint16 mWidth;
};



} // End namespace freyja

#endif // GUARD__FREYJA_VERTEXARRAY_H_
