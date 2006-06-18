
#include "grntype.h"
#include "grntextchunk.h"

using namespace std;

TextChunk::TextChunk()
{
}

TextChunk::~TextChunk()
{
}

void TextChunk::load(  cGrannyStream * file,dword offset,dword baseOffset,dword peers)
{
	dword oldPos = file->tellg();
	file->seekg( offset + baseOffset );
	numEntries = file->readDword();
	file->readDword();	//unknown

	for( unsigned int i = 0; i < numEntries ; ++i )
	{
		char entry[512] = { 0 };
		unsigned short j = 0;

		do
		{
			file->get( entry[j] );
		}
		while( entry[j++] != 0 );

		entries.push_back( std::string( entry ) );
	}

	file->seekg( oldPos );
}

// Find String ID by Name
dword TextChunk::findString( std::string name )
{
	dword i;
	for( i = 0; i < numEntries; ++i )
		if( name == entries[ i ] )
			return i;

	return 0;
}

string TextChunk::findID( dword id )
{
	if( id >= entries.size() )
		return "";

	return entries[id];
}
