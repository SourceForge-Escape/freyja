/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FloatArray
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the FloatArray class.
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

#include "FloatArray.h"
#include "freyja.h"

using namespace freyja;


FloatArray::FloatArray( const uint32 count, const uint16 width ) :
	mArray(),
	mWidth( width )
{
	mArray.SetFlag( mstl::Vector<vec_t>::fNonClass );
	mArray.resize( count * width );
}


FloatArray::FloatArray( ) :
	mArray(),
	mWidth( 3 )
{
	mArray.SetFlag( mstl::Vector<vec_t>::fNonClass );
}


FloatArray::FloatArray( const FloatArray& array ) :
	mArray( array.mArray ),
	mWidth( array.mWidth )
{
}


FloatArray::~FloatArray()
{
}


bool FloatArray::Serialize( TiXmlElement* parent ) const
{
	
	// NOTE: You lose 'gaps' this way  =/

	/* No data to be written. */
	if ( !mArray.size() )
	{
		return true;
	}

	TiXmlElement *buffer = new TiXmlElement( GetType() );
	buffer->SetAttribute("reserve", mArray.size() );

	if ( mWidth == 3 )
	{
		for (uint32 i = 0, n = mArray.size()/3; i < n; ++i)
		{
			const unsigned int idx = i * 3;
			
			TiXmlElement* element = new TiXmlElement("vec3");
			//element->SetAttribute("id", i );
			element->SetDoubleAttribute("x", mArray[idx] );
			element->SetDoubleAttribute("y", mArray[idx+1] );
			element->SetDoubleAttribute("z", mArray[idx+2] );
			buffer->LinkEndChild(element);
		}
	}
	else
	{
		for (uint32 i = 0, n = mArray.size(); i < n; ++i)
		{
			TiXmlElement* element = new TiXmlElement("vec");
			element->SetDoubleAttribute("x", mArray[i] );
			buffer->LinkEndChild(element);
		}
	}

	parent->LinkEndChild(buffer);
	return true;
}


bool FloatArray::Unserialize( TiXmlElement* node )
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

		mArray.reserve( reserve );
	}
	
	TiXmlElement* child = node->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		mstl::String s = child->Value();
		
		// FIXME: should check id's in future, in case of hand edited files.
		if (s == "vec3")
		{
			//int attr;
			//child->QueryIntAttribute("id", &attr);
			
			float x, y, z;
			x = y = z = 0.0f;
			child->QueryFloatAttribute("x", &x);
			child->QueryFloatAttribute("y", &y);
			child->QueryFloatAttribute("z", &z);

			//array[id*3] = x;
			//array[id*3+1] = y;
			//array[id*3+2] = z;
			
			mArray.push_back(x);
			mArray.push_back(y);
			mArray.push_back(z);
		}
		else
		{
			float x = 0.0f;
			child->QueryFloatAttribute("x", &x);
			mArray.push_back(x);
		}
	}

	return true;
}


void FloatArray::Clamp1f( const vec_t min, const vec_t max )
{
	vec_t* array = mArray.get_array();
	for ( uint32 i = 0, n = mArray.size(); i < n; ++i )
	{
		vec_t r = array[i];
		if (r < min)
			array[i] = min;
		else if (r > max)
			array[i] = max;
	}
}
