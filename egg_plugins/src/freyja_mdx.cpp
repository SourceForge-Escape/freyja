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
#include <freyja_model/EggPlugin.h>
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


	printf("freyja_model__mdx_check> Not implemented fully, %s:%i\n", 
			 __FILE__, __LINE__);

	f = fopen(filename, "rb");

	if (!f)
	{
		perror(filename);
		return -1;
	}

	fread(&buffer, 1, 8, f);
	eRead_Small_uint32(&version, f);
	eRead_Small_uint32(&versionMinor, f);

	fclose(f);

	if (!strncmp("MDLXVERS", buffer, 8))
	{
		if (version == 0x00040000 && versionMinor == 0x03200000)
		{
			printf("Seems to be valid Warcraft III mdx\n");
			return 0;
		}

		return 0;
	}

	return -1;
}

typedef struct {

	unsigned int unknown;
	unsigned char name[124];

} mdx_seqs_header_t;

typedef struct {

	unsigned int unknown;
	unsigned char name[128];

} mdx_seqs_t;


int readChunkId(FILE *f, char *looking, int size)
{
	char buffer[size+8];

	fread(&buffer, 1, size, f);
	if (!strncmp(looking, buffer, size))
	{
		printf("%s [OK]\n", looking);
		return 0;
	}
	else
	{
		printf("%s [FAILED] @ %lu\n", looking, ftell(f));
	}

	fclose(f);
	return -1;
}

int freyja_model__mdx_import(char *filename)
{
	FILE *f;
	char buffer[512];
	unsigned int i, version, versionMinor, count;


	printf("freyja_model__mdx_import> Not implemented fully, %s:%i\n", 
			 __FILE__, __LINE__);


	f = fopen(filename, "rb");

	if (!f)
	{
		perror(filename);
		return -1;
	}

	fread(&buffer, 8, 1, f);
	eRead_Small_uint32(&version, f);
	eRead_Small_uint32(&versionMinor, f);

	if (!strncmp("MDLXVERS", buffer, 8))
	{
		if (version == 0x4 && versionMinor == 0x320)
		{
			printf("Seems to be valid Warcraft III MDX\n");
		}
		else
		{
			printf("0x%x && 0x%x\n", version, versionMinor);
		}
	}
	else
	{
		fclose(f);
		return -1;
	}

	fread(&buffer, 4, 1, f);

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
	fread(&buffer, count, 1, f);
	buffer[count-1] = 0;
	printf("Model name = '%s'\n", buffer);

	fread(&buffer, 4, 1, f);

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
	printf("mtlsOffset = %d\n", mtlsOffset);

#ifdef FIXME	
	mdx_seqs_header_t seqh;
	fread(&seqh, 1, 130, f);
	
	unsigned int seqsOffset = ftell(f);
	count = (seqsOffset - mtlsOffset) / 132;

	mdx_seqs_t *seqs = new mdx_seqs_t[count];

	for (i = 0; i < count; ++i)
		fread(&seqs[i], 1, 132, f);
#else
	fseek(f, mtlsOffset-4, SEEK_CUR);
	printf("Seeking to %li\n", ftell(f));
#endif

	unsigned int matsCount;
	eRead_Small_uint32(&matsCount, f);
	fread(&buffer, 4, 1, f);
	if (!strncmp("MTLS", buffer, 4))
	{
		printf("MTLS [OK]\n");
	}
	else
	{
		printf("MTLS [FAILED] -- '%s' @ %li\n", buffer, ftell(f));	
		fclose(f);
		return -1;
	}

	unsigned int texsOffset;
	eRead_Small_uint32(&texsOffset, f);

	/*

	eRead_Small_uint32(&matsCount, f);
	eRead_Small_uint32(&matsCount, f);
	eRead_Small_uint32(&matsCount, f);

	fread(&buffer, 1, 4, f);
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

	fseek(f,  texsOffset, SEEK_CUR);

	fread(&buffer, 1, 4, f);
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
	
    eggBegin(FREYJA_MESH);

	fseek(f,  geosOffset, SEEK_CUR);

	fread(&buffer, 1, 4, f);
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

	fread(&buffer, 1, 4, f);
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

	float *verts = new float[count*3];

	for (i = 0; i < count*3; ++i)
	{
		fread(verts+i, 4, 1, f);
	}

    eggBegin(FREYJA_GROUP);
	for (i = 0; i < count; ++i)
	{
		eggVertexStore3f(verts[i*3+1], verts[i*3+2], verts[i*3+0]);	
	}
	eggEnd(); // GROUP

	fread(&buffer, 1, 4, f);
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

	float *norms = new float[count*3];

	for (i = 0; i < count*3; ++i)
	{
		fread(norms+i, 4, 1, f);
	}

	for (i = 0; i < count; ++i)
	{
		eggVertexNormalStore3f(i, norms[i*3+1], norms[i*3+2], norms[i*3+0]);
	}
	fread(&buffer, 1, 4, f);
	if (!strncmp("PTYP", buffer, 4))
	{
		printf("PTYP [OK]\n");
	}
	else
	{
		printf("PTYP [FAILED]\n");
		fclose(f);
		return -1;
	}

	unsigned int ptype1, ptype2;
	eRead_Small_uint32(&ptype1, f);

	//unsigned int *types = 0x0;

	if (ptype1 != 4)
	{
		eRead_Small_uint32(&ptype2, f);
	}
	else
	{
		fseek(f, SEEK_CUR, ptype1*4 - 8);

		//types = new unsigned int[ptype1];
		//
		//for (i = 0; i < ptype1; ++i)
		//	eRead_Small_uint32(types+i, f);
	}

	printf("PTYP: %d %d\n", ptype1, ptype2);

	fread(&buffer, 1, 4, f);
	if (!strncmp("PCNT", buffer, 4))
	{
		printf("PCNT [OK]\n");
	}
	else
	{
		printf("PCNT [FAILED] -- @ %li\n", ftell(f));
		fclose(f);
		return -1;
	}

	unsigned int faceCount;
	eRead_Small_uint32(&count, f);

	if (count == 1)
	{
		eRead_Small_uint32(&faceCount, f);
		faceCount /= 3;
	}
	else
	{
		faceCount = count;
		count *= 4;
	}

	printf("%d faces, %d count\n", faceCount, count);

	fread(&buffer, 1, 4, f);
	if (!strncmp("PVTX", buffer, 4))
	{
		printf("PVTX [OK]\n");
	}
	else
	{
		fclose(f);
		return -1;
	}

	eRead_Small_uint32(&count, f);
	printf("count = %d, faceCount = %d\n", count, faceCount);
	unsigned short int *tris = new unsigned short int[count];

	for (i = 0; i < count; ++i)
	{
		fread(tris+i, 2, 1, f);
		//printf("%d %d\n", i, tris[i]);
	}

	for (i = 0; i < faceCount; ++i)
	{
		printf("%d <%d %d %d>\n", i, tris[i*3], tris[i*3+1], tris[i*3+2]);
		eggBegin(FREYJA_POLYGON);
		eggVertex1i(tris[i*3]);
		eggVertex1i(tris[i*3+1]);
		eggVertex1i(tris[i*3+2]);
		eggEnd(); // POLYGON
	}

	eggEnd(); // MESH

	if (readChunkId(f, "GNDX", 4) < 0)
		return -1;

	eRead_Small_uint32(&count, f);
	printf("count = %d\n", count);
	fseek(f, count, SEEK_CUR);

	if (readChunkId(f, "MTGC", 4) < 0)
		return -1;

	eRead_Small_uint32(&count, f);
	printf("count = %d\n", count);
	fseek(f, count*4, SEEK_CUR);

	if (readChunkId(f, "MATS", 4) < 0)
		return -1;

	eRead_Small_uint32(&count, f);
	printf("count = %d\n", count);
	fseek(f, count*8, SEEK_CUR);

	if (readChunkId(f, "UVAS", 4) < 0)
		return -1;

	eRead_Small_uint32(&count, f);
	printf("count = %d\n", count);

	if (readChunkId(f, "UVBS", 4) < 0)
		return -1;

	eRead_Small_uint32(&count, f);
	printf("count = %d\n", count);

	unsigned int *uvI = new unsigned int[count*2];
	float *uv = new float[count*2];

	for (i = 0; i < count; ++i)
	{
		eRead_Small_uint32(uvI+i*2, f);
		eRead_Small_float(uv+i*2, f);
		eRead_Small_uint32(uvI+i*2+1, f);
		eRead_Small_float(uv+i*2+1, f);
	}

	if (readChunkId(f, "VRTX", 4) < 0)
		//return -1;

	//	printf("%lu\n", ftell(f));
	// ...



	fclose(f);
	return 0;
}


#ifdef NOTES
8 bytes 'MDLXVERS'  
4 bytes '0x4' ( 4 ) 
4 bytes '0x320' ( 800 )

4 bytes 'MODL'  

uint32 stringLen? '0x017400' ( 372 )

char modelName[stringLen?]  @ 0x18

byte unknown[]   @ 0x118

4 bytes 'SEQS'           @ 0x18c  ( 396 bytes offset )
uint32 seqsCount   '0x0b5800' ( 2094 )
{
	char Name[]
} 132 bytes?  ( might use a 2 byte 'QC' mini chunk marker )


4 bytes 'MTLS'

4 bytes 'LAYS'  44 bytes + 4byte marker?

4 bytes 'TEXS'

4 bytes 'GEOS'

4 bytes 'VRTX'

4 btyes 'PTYP'

4 bytes 'PCNT'
uint32  '1'
unit32  '234'

4 bytes 'PVTX'
shorts 

'GNDX'

'MTGC'

'MATS'

'UVAS'

'UVBS'

'VRTX'

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
