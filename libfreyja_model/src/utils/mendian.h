/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja, MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the endian utils.
 *
 *           See file COPYING for license details.
 *
 *-- History ---------------------------------------------------------- 
 *
 * See the source file's header.
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_ENDIAN_H
#define GUARD__MONGOOSE_ENDIAN_H

#include <stdio.h>


/* Disable C++ name mangling */
extern "C" {

	size_t eRead(void *ptr, size_t size, size_t nmemb, FILE *stream);
	size_t eRead_Small_uint32(void *ptr, FILE *stream);
	size_t eRead_Small_int32(void *ptr, FILE *stream);
	size_t eRead_Small_float(void *ptr, FILE *stream);

	size_t eWrite_Small_uint32(void *ptr, FILE *stream);
	size_t eWrite_Small_int32(void *ptr, FILE *stream);
	size_t eWrite_Small_float(void *ptr, FILE *stream);
}

#endif
