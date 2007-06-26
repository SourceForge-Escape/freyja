/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: This is the interactive query ABI.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.24:
 * Mongoose - Created
 ==========================================================================*/

#include <mstl/String.h>
#include <mstl/Vector.h>

#include "QueryABI.h"

using mstl::String;
using mstl::Vector;


namespace freyja {

class Query
{
 public:

	Query() : mSymbols(), mPointers() {}

	void Append(const char *symbol, const char *type, ptr_t ptr) 
	{
		String s = symbol;  // "symbol@type"
		s += "@";
		s += type;
		mSymbols.push_back( s );
		mPointers.push_back( ptr );	
	}

	void Execute() 
	{ 
		if ( mExecute && mSymbols.size() ) 
		{
			// Make this C exportable...
			freyja_query_t *q = new freyja_query_t[ mSymbols.size() ];

			uint32 i;
			foreach (mSymbols, i)
			{
				String s = mSymbols[i];

				int off = s.find_last_of('@');
				
				if (off > 0)
				{
					s[off] = 0;
				}

				q[i].type = (off < 0) ? "?" : ( s.c_str() + off + 1 );
				q[i].symbol = s.c_str();
				q[i].ptr = mPointers[i];
			}

			(*mExecute)(mSymbols.size(), q); 

			delete [] q;
		}
	}

	static FreyjaQueryCallback mExecute;
	Vector<String> mSymbols;
	Vector<ptr_t> mPointers;
};

} // namespace freyja

using freyja::Query;

FreyjaQueryCallback Query::mExecute = NULL;

// We only allow one query to exist at a time to avoid interaction!
Query *gQuery = NULL;


Query *freyjaGetQuery(index_t id)
{
	return gQuery;
}


void freyjaQuerySetCallback(FreyjaQueryCallback func)
{
	Query::mExecute = func;
}


index_t freyjaQueryBegin()
{
	if (gQuery)
		delete gQuery;

	gQuery = new Query();
	return 1;
}


void freyjaQueryEnd(index_t query)
{
	Query *q = freyjaGetQuery(query);

	if (!q)
		return;

	// Execute query
	q->Execute();

	delete q;
}


void freyjaQueryInteger(index_t query, const char *symbol, int32 *i)
{
	Query *q = freyjaGetQuery(query);

	if (!q)
		return;

	q->Append(symbol, "int", (ptr_t)i);
}


void freyjaQueryFloat(index_t query, const char *symbol, vec_t *r)
{
	Query *q = freyjaGetQuery(query);

	if (!q)
		return;

	q->Append(symbol, "float", (ptr_t)r);
}


void freyjaQueryString(index_t query, const char *symbol, const char **s)
{
	Query *q = freyjaGetQuery(query);
	*s = NULL;

	if (!q)
		return;

	q->Append(symbol, "char *", (ptr_t)s);
}



