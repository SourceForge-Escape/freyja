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

#ifndef GUARD__FREYJA_QUERYABI_H_
#define GUARD__FREYJA_QUERYABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// QUERY
	///////////////////////////////////////////////////////////////////////

	typedef struct {

		const char *type;
		const char *symbol;
		ptr_t ptr;

	} freyja_query_t;

	typedef void (*FreyjaQueryCallback)(unsigned int size, freyja_query_t *array);


	void freyja_query_set_callback( FreyjaQueryCallback func );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the interactive query handler for user input
	 *        for scripts/macros/plugins and such.
	 *
	 ------------------------------------------------------*/

	index_t freyjaQueryBegin();
	/*------------------------------------------------------
	 * Pre  : Flushes any previous request if not End'ed already.
	 *
	 * Post : Used to buffer query requests all at once.
	 *
	 ------------------------------------------------------*/

	void freyjaQueryEnd(index_t query);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Does the query and frees buffered requests.
	 *
	 ------------------------------------------------------*/

	void freyjaQueryInteger(index_t query, const char *symbol, int32 *i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns integer value or leaves unaltered.
	 *
	 ------------------------------------------------------*/

	void freyjaQueryFloat(index_t query, const char *symbol, vec_t *r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns float value or leaves unaltered.
	 *
	 ------------------------------------------------------*/

	void freyjaQueryString(index_t query, const char *symbol, const char **s);
	/*------------------------------------------------------
	 * Pre  : <s> should not be an allocated string.
	 *
	 * Post : Returns pointer to string, which is not 
	 *        to be deallocated by the user.  
	 *
	 *        Can return NULL.
	 *
	 ------------------------------------------------------*/


} // extern "C"


#endif // GUARD__FREYJA_QUERYABI_H_
