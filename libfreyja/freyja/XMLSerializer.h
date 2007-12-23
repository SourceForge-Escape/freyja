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

#ifndef GUARD__FREYJA_XMLSERIALIZER_H_
#define GUARD__FREYJA_XMLSERIALIZER_H_


namespace freyja {

class XMLSerializer
{
public:
	XMLSerializer()
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~XMLSerializer()
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	virtual const char* GetType() const = 0; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag string (often just class name). 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetVersion() const = 0; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns tag version (often just serial format version). 
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND

	virtual bool Serialize(TiXmlElement* parent) const = 0;
	/*------------------------------------------------------
	 * Pre  : <parent> is this object's parent in XML tree.
	 * Post : Serializes this object to XML.
	 *
	 ------------------------------------------------------*/

	virtual bool Unserialize(TiXmlElement* node) = 0;
	/*------------------------------------------------------
	 * Pre  : <node> is this object in XML tree.
	 * Post : Unserializes node from XML.
	 *
	 ------------------------------------------------------*/

#endif // TINYXML_FOUND
};

} // namespace freyja


#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>

#   define FREYJA_XMLSERIALIZER_INTERFACE \
    virtual bool Serialize( TiXmlElement* parent ) const; \
	virtual bool Unserialize( TiXmlElement* node ); 
 
#   define FREYJA_XMLSERIALIZER_NODE TiXmlElement*

#else

//#   define FREYJA_XMLSERIALIZER_INTERFACE 
#   define FREYJA_XMLSERIALIZER_NODE void*

#endif // TINYXML_FOUND


#endif // GUARD__FREYJA_XMLSERIALIZER_H_

