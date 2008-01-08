/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : XMLSerializerInterface
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the XML serialization interface.
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created, broke out into it's own interface since 
 *            freyja is going to vtable hell anyway now I'm allowing inheritance.
 ==========================================================================*/

#ifndef GUARD__FREYJA_XMLSERIALIZERINTERFACE_H_
#define GUARD__FREYJA_XMLSERIALIZERINTERFACE_H_

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

namespace freyja {

class XMLSerializerInterface
{
public:
	virtual const char* GetType()  = 0; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag string (often just class name). 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetVersion() = 0; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag version (often just serial format version). 
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND

	bool Serialize(TiXmlElement* parent) = 0;
	/*------------------------------------------------------
	 * Pre  : <parent> is this object's parent in XML tree.
	 * Post : Serializes this object to XML.
	 *
	 ------------------------------------------------------*/

	bool Unserialize(TiXmlElement* node) = 0;
	/*------------------------------------------------------
	 * Pre  : <node> is this object in XML tree.
	 * Post : Unserializes node from XML.
	 *
	 ------------------------------------------------------*/

#endif
};

} // namespace freyja

#endif // GUARD__FREYJA_XMLSERIALIZERINTERFACE_H_

