/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: KeyFrame class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/


#include "KeyFrame.h"

using namespace mstl;
using namespace freyja;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

uint32 KeyFrame::GetSerializedSizeBase()
{
	return 1 + 4 + 4 + mMetaData.length();
}


bool KeyFrame::SerializeBase(SystemIO::FileWriter &w)
{
	w.WriteByte(mFlags);
	w.WriteFloat32(mTime);
	w.WriteInt32U( mMetaData.length() );

	if ( mMetaData.length() )
	{
		w.WriteString( mMetaData.length(), mMetaData.c_str() );
	}
	
	return true;
}


bool KeyFrame::UnserializeBase(SystemIO::FileReader &r)
{
	mFlags = r.ReadByte();
	mTime = r.ReadFloat32();

	{
		uint32 len = r.ReadInt32U(); 
		char s[len+1];
		r.ReadString(len, s);
		s[len] = 0;
		mMetaData = s;
	}

	return true;
}


bool KeyFrame::SerializeBase(SystemIO::TextFileWriter &w)
{
	w.Print(" %u", mFlags);
	w.Print(" %f", mTime);
	w.Print(" \"%f\"", mMetaData.c_str() );

	return true;
}


bool KeyFrame::UnserializeBase(SystemIO::TextFileReader &r)
{
	mFlags = r.ParseInteger();
	mTime = r.ParseFloat();
	mMetaData = r.ParseStringLiteral();

	return true;
}


#if TINYXML_FOUND
bool KeyFrame::SerializeBase(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	keyframe->SetAttribute("flags", mFlags);
	keyframe->SetDoubleAttribute("time", mTime);
		
	// Metadata is NOT written out as CDATA to avoid parsing it.
	if ( mMetaData.c_str() )
	{
		TiXmlElement *metadata = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( mMetaData.c_str() );
		//text->SetCDATA(true);
		metadata->LinkEndChild(text);
		keyframe->LinkEndChild(metadata);
	}

	return true;
}


bool KeyFrame::UnserializeBase(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	{ 
		int attr = 0;
		keyframe->QueryIntAttribute("flags", &attr);
		mFlags = attr;
	}

	keyframe->QueryFloatAttribute("time", &mTime);

	TiXmlElement *child = keyframe->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();
		
		if (s == "metadata")
		{
			mMetaData = child->GetText();
		}
	}

	return true;
}
#endif


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_KEYFRAME
int runKeyFrameUnitTest(int argc, char *argv[])
{
	KeyFrame test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[KeyFrame class test]\n");

	return runKeyFrameUnitTest(argc, argv);
}
#endif

