/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : MDDC
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Md2
 * Comments: This is the MD2 model module.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 
 *
 *           Based on viewmd2 (c) Michael K Vance, 1999  
 *                                briareos@lokigames.com
 *                                mkv102@psu.edu
 *
 *
 *-- History ---------------------------------------------------------- 
 *
 * See the source file's header.
 ==========================================================================*/


#ifndef __QUAKEFORGE_MONGOOSE_MD2_H
#define __QUAKEFORGE_MONGOOSE_MD2_H


#define MD2_IDALIASHEADER  ( '2' << 24 ) + ( 'P' << 16 ) + ( 'D' << 8 ) + 'I'
#define MD2_ALIAS_VERSION  8
#define MD2_MAX_TRIANGLES  4096
#define MD2_MAX_VERTS      2048
#define MD2_MAX_FRAMES     512
#define MD2_MAX_MD2SKINS   32
#define MD2_MAX_SKINNAME   64


typedef struct BoundingBox_s
{
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;

} BoundingBox_t;

typedef struct TexCoordsIndex_s
{
    short s;
    short t;

} TexCoordsIndex_t;

typedef struct TriangleIndex_s
{
    short v[3];
    short st[3];

} TriangleIndex_t;

typedef struct VertexIndex_s
{
    float x;
    float y;
    float z;
    unsigned char normal;

} VertexIndex_t;

typedef struct  AliasFrame_s
{
    float scale[3];
    float translate[3];
    char name[16];
    VertexIndex_t* verts;

} AliasFrame_t;

typedef struct MD2_s
{
   int              ident;
   int              version;
   int              numXYZ;
   int              numTris;
   int              numFrames;
   TexCoordsIndex_t *texCoords;
   TriangleIndex_t  *tris;
   AliasFrame_t     *frames;
   int              *commands;
   int              frameSize;
   int              numSkins;
   int              skinWidth;
   int              skinHeight;
   int              numST;
   int              numGLCmds;
   int              ofsSkins;
   int              ofsST;
   int              ofsTris;
   int              ofsFrames;
   int              ofsGLCmds;
   int              ofsEnd;

} MD2_t;


class Md2
{
private:

    MD2_t _header;

    char _skins[MD2_MAX_MD2SKINS][MD2_MAX_SKINNAME];
 
    BoundingBox_t _bbox;

    int _valid;

public:

    Md2();

    Md2(char *filename);
 
    ~Md2();

    MD2_t *Header();

    VertexIndex_t *Frame(int n);

	AliasFrame_t *getFrame(unsigned int frame);

	unsigned int getNumFrames();

    void BoundingBox_Update(const float x, const float y, const float z);

    int SkinWidth();

    int SkinHeight();

    char *SkinName(int n);

    int Load(char *filename);

    void TexCoords(short *s, short *t, int tri, int vert);

    double GetRadius();


#ifdef USING_OPENGL
	void render(unsigned int frame, unsigned int texture);
#endif
};

#endif
