/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Object  : 
 * Comments: 
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created, Abstracted from old libfreyja xml wrapper.
 ================================================================*/

#ifndef GUARD__MSTL_GZFILE_H_
#define GUARD__MSTL_GZFILE_H_

#include <mstl/String.h>
#include <zlib.h>

namespace mstl {

bool GzFileRead( const char* filename, char*& buffer, const unsigned int& size );

bool GzFileWrite( const char* filename, const char* buffer, const unsigned int size );


inline
bool GzFileWrite( const char* filename, const char* buffer, const unsigned int size )
{
	gzFile f = gzopen( filename, "wb" );

	if ( f )
	{
		bool ret = true;
		if ( gzwrite(f, buffer, size) < (int)size )
		{
			//freyjaPrintError("gzwrite('%s') failed.", filename);
			ret = false;
		}
		 
		gzclose(f);
		return ret;
	}

	return false;
}


inline
bool GzFileRead( const char* filename, char*& buffer, const unsigned int& size )
{
	gzFile f = gzopen(filename, "rb");

	if ( f )
	{
		bool ret = true;
		if ( gzread(f, buffer, size) < (int)size )
		{
			//freyjaPrintError("gzread('%s') failed.", filename);
			ret = false;
		}

		gzclose(f);
		return ret;
	}

	return false;
}

} // namespace mstl 

#endif // GUARD__MSTL_GZFILE_H_

