#ifndef __GRN_H__
#define __GRN_H__

#include <string>
#include "grnitemlist.h"

#ifdef OLD
#   include "renderer/Texture.h"
#endif

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#endif

class cGrannyFile
{
public:
	cGrannyFile();
	virtual ~cGrannyFile();
	int load( std::string filename, std::string basepath );
	void addTime( float t );
	virtual void getSkeleton( Bone *bone, float & curTime );
	std::string getTextureName();
	Meshes &getMeshes();
	Bones &getBones();
	Textures &getTextures();
	BoneAnim &getBoneAnim(dword id);
	BoneTies &getTies();
	Animations &getAnimations();
	dword findString( std::string str );
	dword getValue(dword obj,dword key);
	std::string findID(dword id);
	dword findValue(dword key,dword value);
	void Render(cGrannyFile *animation,  float & curTime );

	bool loadTexture( const char *basepath );

	// Returns the GL Texture Identifier
	unsigned int getTexture() const
	{
#ifdef OLD
		if( texture )
			return texture->GetGLTex();
		else
			return 0;
#else
		return 0;
#endif
	}

	bool initialized () {
		return m_initialized;
	}

private:
	void mainChunk();
	ItemList copyright,object,final;
	cGrannyStream *stream;
#ifdef OLD
	Texture *texture; // Our main Texture
#endif
	bool m_initialized;
};

class cGrannyAnimation : public cGrannyFile
{
public:
	cGrannyAnimation();
	virtual ~cGrannyAnimation();
	void Assign (cGrannyFile * model);
	dword * GetAnimBones(void);
	void getSkeleton( Bone *bone, float & curTime );
private:
	dword *animBones;

};

#endif
