/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Warcraft 3 MDX plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-09:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja_egg/EggPlugin.h>

#include <freyja_model/mendian.h>
#define fread_int_small eRead_Small_int32
#define fread_u_int_small eRead_Small_uint32
#define fread_float_small eRead_Small_float

extern "C" {

	int freyja_model__mdx_check(char *filename);
	int freyja_model__mdx_import(char *filename);
	int freyja_model__mdx_export(char *filename);
}



int freyja_model__mdx_check(char *filename)
{
	FILE *f;
	char buffer[256];
	unsigned int version, versionMinor;


	f = fopen(filename, "rb");


	if (!f)
	{
		perror(filename);
		return -1;
	}

	fread(buffer, 1, 8, f);
	eRead_Small_uint32(&version, f);
	eRead_Small_uint32(&versionMinor, f);

	fclose(f);

	if (!strncmp("MDLXVERS", buffer))
	{
		if (version == 0x00040000 && versionMinor == 0x03200000)
		{
			printf("Seems to be valid Warcraft III mdx\n");
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

typedef struct {

	unsigned int unknown;
	unsigned char name[124];

} mdx_seqs_header_t;

typedef struct {

	unsigned int unknown;
	unsigned char name[128];

} mdx_seqs_t;


int freyja_model__mdx_import(char *filename)
{
	FILE *f;
	char buffer[256];
	unsigned int version, versionMinor, count;


	printf("freyja_model__mdx_import> Not implemented fully, %s:%i\n", 
			 __FILE__, __LINE__);


	f = fopen(filename, "rb");


	if (!f)
	{
		perror(filename);
		return -1;
	}

	fread(buffer, 1, 8, f);
	eRead_Small_uint32(&version, f);
	eRead_Small_uint32(&versionMinor, f);

	if (!strncmp("MDLXVERS", buffer, 8))
	{
		if (version == 0x00040000 && versionMinor == 0x03200000)
		{
			printf("Seems to be valid Warcraft III mdx\n");
		}
		else
		{
			fclose(f);
			return -1;
		}
	}

	fread(buffer, 1, 4, f);

	if (!strncmp("MODL", buffer, 4))
	{
		printf("MODL [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}


	eRead_Small_uint32(&count, f);
	fread(buffer, 1, count, f);
	buffer[count-1] = 0;
	printf("Model name = '%s'\n", buffer);

	fread(buffer, 1, count, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("SEQS", buffer, 4))
	{
		printf("SEQS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	unsigned int mtlsOffset;
	eRead_Small_uint32(&mtlsOffset, f);
	
	mdx_seqs_header_t seqh;
	fread(&seqh, 1, 130, f);

	
	unsigned int seqsOffset = ftell(f);
	count = (seqsOffset - mtlsOffset) / 132;

	mdx_seqs_t *seqs = new mdx_seqs_t[count];

	for (i = 0; i < count; ++i)
		fread(&seq[i], 1, 132, f);

	eRead_Small_uint32(&matsCount, f);
	fread(buffer, 1, 4, f);
	if (!strncmp("MTLS", buffer, 4))
	{
		printf("MTLS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	unsigned int texsOffset;
	eRead_Small_uint32(&texsOffset, f);

	/*

	eRead_Small_uint32(&matsCount, f);
	eRead_Small_uint32(&matsCount, f);
	eRead_Small_uint32(&matsCount, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("LAYS", buffer, 4))
	{
		printf("LAYS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}
	*/

	fseek(texsOffset, SEEK_CUR, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("TEXS", buffer, 4))
	{
		printf("TEXS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	unsigned int geosOffset;
	eRead_Small_uint32(&geosOffset, f);	

	/*
	  ....
	 */
	
	fseek(geosOffset, SEEK_CUR, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("GEOS", buffer, 4))
	{
		printf("GEOS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);
	eRead_Small_uint32(&count, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("VRTX", buffer, 4))
	{
		printf("VRTX [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);

	float *verts = new float[count*3]

	for (i = 0; i < count*3; ++i)
		fread(verts+i, 4, 1, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("NRMS", buffer, 4))
	{
		printf("NRMS [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);

	float *norms = new float[count*3]

	for (i = 0; i < count*3; ++i)
		fread(norms+i, 4, 1, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("PTYP", buffer, 4))
	{
		printf("PTYP [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);
	eRead_Small_uint32(&count, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("PCNT", buffer, 4))
	{
		printf("PCNT [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);
	eRead_Small_uint32(&count, f);

	fread(buffer, 1, 4, f);
	if (!strncmp("PVTX", buffer, 4))
	{
		printf("PVTX [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	// ...

	fclose(f);
	return -1; // 0;
}


#ifdef NOTES
'MDLXVERS'  '0x00040000' ( 4 ) '0x03200000' ( 800 )

'MODL'  

uint32 stringLen? '0x017400' ( 372 )

char modelName[stringLen?]  @ 0x18

byte unknown[]   @ 0x118

'SEQS'           @ 0x18c  ( 396 bytes offset )
uint32 seqsCount   '0x0b5800' ( 2094 )
{
	char Name[]
} 132 bytes?  ( might use a 2 byte 'QC' mini chunk marker )


'MTLS'


'LAYS'  44 bytes + 4byte marker?

TEXS

GEOS

VRTX

4 btyes 'PTYP'

4 bytes 'PCNT'

uint32  '1'

unit32  '234'

4 bytes 'PVTX'

NDXI

MTGC

MATSC


UVAS


UVBS


VRTX

...

ATCH ( bones? )

KATV[]

PIVT

APRE

EVTS
KEVT
CLID
Collision Sphere01
Collision Sphere02
#endif

int freyja_model__mdx_export(char *filename)
{
	printf("freyja_model__mdx_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}
