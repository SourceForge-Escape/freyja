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

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>

#include "VertexArray.h"
#include "freyja.h"

namespace freyja {

VertexArray::VertexArray( const uint32 count ) :
	mArray(),
	mGaps()
{
	mArray.resize( count );
}


VertexArray::VertexArray() :
	mArray(),
	mGaps()
{
}


VertexArray::VertexArray( const VertexArray& array ) :
	mArray( array.mArray ),
	mGaps( array.mGaps )
{
}


VertexArray::~VertexArray()
{
}


bool VertexArray::Serialize( TiXmlElement* parent )
{
	
	// NOTE: You lose 'gaps' this way  =/

	/* No data to be written. */
	if ( !array.size() )
	{
		return true;
	}

	TiXmlElement *buffer = new TiXmlElement( GetType() );
	buffer->SetAttribute("reserve", array.size() );

	for (uint32 i = 0, n = array.size()/3; i < n; ++i)
	{
		unsigned int idx = i * 3;

		TiXmlElement *element = new TiXmlElement("vec3");
		element->SetAttribute("id", idx/3 );
		element->SetDoubleAttribute("x", array[idx] );
		element->SetDoubleAttribute("y", array[idx+1] );
		element->SetDoubleAttribute("z", array[idx+2] );
		buffer->LinkEndChild(element);
	}

	container->LinkEndChild(buffer);
	return true;
}


bool VertexArray::Unserialize( TiXmlElement* node )
{
	// Have to read which gap type above this level

	if ( !node )
		return false;

	{
		/* Set array reserve size. */
		int reserve;
		node->QueryIntAttribute( "reserve", &reserve );

		if ( reserve < 0 ) 
			reserve = 16;

		array.reserve( reserve );
	}
	
	TiXmlElement* child = node->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();
		
		// FIXME: should check id's in future, in case of hand edited files.
		if (s == "vec3")
		{
			child->QueryIntAttribute("id", &attr);
			
			float x, y, z;
			x = y = z = 0.0f;
			child->QueryFloatAttribute("x", &x);
			child->QueryFloatAttribute("y", &y);
			child->QueryFloatAttribute("z", &z);

			//array[id*3] = x;
			//array[id*3+1] = y;
			//array[id*3+2] = z;
			
			array.push_back(x);
			array.push_back(y);
			array.push_back(z);
		}
	}

	return true;
}


	void Clamp1f( const vec_t min, const vec_t max )
	{
		vec_t* array = mArray.get_array();
		for ( uint32 i = 0, n = v.size(); i < n; ++i )
		{
			vec_t r = array[i];
			if (r < min)
				array[i] = min;
			else if (r > max)
				array[i] = max;
		}
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
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
	{
		vec_t* array = v.get_array();
		for ( uint32 i = 0, n = v.size(); i < n; i += 3 )
		{
			mat.Multiply3fv(array + i);
		}
	}
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











	const vec_t& GetArray() { return mArray.get_array(); }

	uint32 Size() { return mArray.size() }
	

private:

	mstl::Vector<vec_t> mArray;     /* Vertex array. */

	mstl::stack<uint32> mGaps;      /* Track any gaps in array usage. */
};



} // End namespace freyja

#endif // GUARD__FREYJA_VERTEXARRAY_H_
