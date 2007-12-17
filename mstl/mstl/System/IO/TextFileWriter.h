/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : TextFileWriter
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Comments: Text file writer.  
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_TEXTFILEWRITER_H_
#define GUARD__MSTL_SYSTEMIO_TEXTFILEWRITER_H_

#include "File.h"

namespace mstl {
namespace SystemIO {

class TextFileWriter : public File
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	TextFileWriter() : File() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~TextFileWriter() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	bool Open(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
 
	void Print(const char *s, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void PrintFloat(double d);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void PrintInteger(int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void PrintString(const char *s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

};



////////////////////////////////////////////////////////////
// Public Inline Methods
////////////////////////////////////////////////////////////

inline
bool TextFileWriter::Open(const char *filename) 
{ 
	return File::Open(filename, "wb"); 
}


inline
void TextFileWriter::Print(const char *s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(mFileHandle, s, args);
	va_end(args);
}


inline
void TextFileWriter::PrintFloat(double d)
{
	fprintf(mFileHandle, "%f", d);
}


inline
void TextFileWriter::PrintInteger(int i)
{
	fprintf(mFileHandle, "%i", i);
}


inline
void TextFileWriter::PrintString(const char *s)
{
	fprintf(mFileHandle, "%s", s);
}

} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_TEXTFILEWRITER_H_

