/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the endian utils.
 *
 *           See file COPYING for license details.
 *
 *           Used some of my inline XOR decrypting dRead functions
 *
 *           The SWAP_ and FIX_ macros by Paul Bourke in 1991
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2000-03-17:
 * Mongoose: Created
 ==========================================================================*/

#include "mendian.h"

#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX_SHORT(x) (*(unsigned short *)&(x) = SWAP_2(*(unsigned short *)&(x)))
#define FIX_INT(x)   (*(unsigned int *)&(x)   = SWAP_4(*(unsigned int *)&(x)))
#define FIX_FLOAT(x) FIX_INT(x)



size_t eRead(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fread(ptr, size, nmemb, stream);
}


size_t eRead_Small_uint32(void *ptr, FILE *stream)
{
	size_t sz = fread(ptr, 4, 1, stream);

#ifdef BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return sz;
}


size_t eRead_Small_int32(void *ptr, FILE *stream)
{
	size_t sz = fread(ptr, 4, 1, stream);

#ifdef BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return sz;
}


size_t eRead_Small_float(void *ptr, FILE *stream)
{
	size_t sz = fread(ptr, 4, 1, stream);

#ifdef BIG_ENDIAN
	FIX_FLOAT(*ptr)
#endif
	return sz;
}

