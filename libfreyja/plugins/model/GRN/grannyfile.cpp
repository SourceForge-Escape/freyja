/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "grntype.h"
#include "grannyfile.h"
#include <iostream>

using namespace std;

// Utility Function
void calculateBoneRests( Bone *bone );

cGrannyFile::cGrannyFile()
{
#ifdef OLD
	texture = 0;
#endif
	m_initialized = false;
}

cGrannyFile::~cGrannyFile()
{
#ifdef OLD
	// Destroy the Texture we're binding
	delete texture;
#endif
}

int cGrannyFile::load( std::string filename, std::string basepath )
{
    std::fstream * file = new fstream( filename.c_str(), ios::in|ios::binary );	

	if( !file->is_open() ) {
		printf("Error: File not found: %s\n", filename.c_str());
		return -1;
	}
	

	//cout << "Loading file " + basepath + filename << endl;

	stream = new cGrannyStream(file, filename);
	file->close();
	delete file;

	stream->seekg( 0x40 );	// skip header (Could be FileType magic)

	dword chunk=stream->readDword();
	switch (chunk)
	{
	case 0xCA5E0000:
		//printf("Loading main chunk\n");
		mainChunk();
		break;
	default:
		hex( cerr );
		//cerr << "Unknown main chunk: " << chunk << endl;
		return -1;
	}

	if( getTextureName() != "" )
		loadTexture( basepath.c_str() );

	Bones &bones = getBones();
	if( bones.bones.size() > 0 )
	{
		calculateBoneRests( bones.bones[0] );
	}

	delete stream;
	stream = NULL;
	m_initialized = true;

	return 0;
}

void cGrannyFile::addTime( float t )
{
}

string cGrannyFile::getTextureName()
{
	return object.getTextureName();
}

Meshes &cGrannyFile::getMeshes()
{
	return object.getMeshes();
}

Bones &cGrannyFile::getBones()
{
	return object.getBones();
}

BoneTies &cGrannyFile::getTies()
{
	return object.getTies();
}

Textures &cGrannyFile::getTextures()
{
	return object.getTextures();
}

BoneAnim &cGrannyFile::getBoneAnim( dword id )
{
	return object.getBoneAnim( id );
}

Animations &cGrannyFile::getAnimations()
{
	return object.getAnimations();
}

dword cGrannyFile::findString(string str)
{
	return object.findString(str);
}

dword cGrannyFile::getValue(dword obj,dword key)
{
	return object.getValue(obj,key);
}
dword cGrannyFile::findValue(dword key,dword value)
{
	return object.findValue(key,value);
}

std::string cGrannyFile::findID(dword id)
{
	return object.findID(id);
}

void cGrannyFile::mainChunk()
{
	dword children = stream->readDword();
	
	for( int i = 0; i < 6; ++i )
		stream->readDword(); // CRC?

	for( dword child = 0; child < children; ++child )
	{
		// Chunk Header
		dword chunk = stream->readDword();

		switch( chunk )
		{
			// Final Chunk (End-of-File?)
		case 0xCA5E0101:
			final.load( stream );
			break;

			// Copyright Chunk
		case 0xCA5E0102:
			copyright.load( stream );
			break;

			// Object Chunk
		case 0xCA5E0103:
			object.load( stream );
			break;
		default:
			hex(cerr);
			cerr<<"Unknown GRN Chunk: "<<chunk<<endl;
			exit(1);
		}
	}
}

bool cGrannyFile::loadTexture( const char *basepath )
{
	std::string filename;
	filename = basepath;
	filename += getTextureName();
	filename += ".tga";

#ifdef OLD
	// Try to load our Texture
	texture = new Texture;
	if( !texture->LoadFromFile( filename.c_str() ) )
	{
		delete texture;
		texture = 0;
		return false;
	}
#endif

	return true;
}

void calculateBoneRests( Bone *bone )
{


	float X,Y,Z,x,y,z,w;
	X=bone->translate.points[0];
	Y=bone->translate.points[1];
	Z=bone->translate.points[2];

	x=bone->quaternion.points[0];
	y=bone->quaternion.points[1];
	z=bone->quaternion.points[2];
	w=bone->quaternion.points[3];

	GrnMatrix matrix;
	float x2=x+x; float y2=y+y; float z2=z+z;
	float xx=x*x2; float xy=x2*y; float xz=x2*z; float xw=x2*w;
	float yy=y*y2; float yz=y2*z; float yw=y2*w;
	float zz=z*z2; float zw=z2*w;

	matrix[0]= 1-yy-zz;
	matrix[1]=   xy+zw;
	matrix[2]=   xz-yw;
	matrix[3]= 0;
	matrix[4]=   xy-zw;
	matrix[5]= 1-xx-zz;
	matrix[6]=   yz+xw;
	matrix[7]= 0;
	matrix[8]=   xz+yw;
	matrix[9]=   yz-xw;
	matrix[10]=1-xx-yy;
	matrix[11]=0;
	matrix[12]=X;
	matrix[13]=Y;
	matrix[14]=Z;
	matrix[15]=1;

#ifdef HAVE_OPENGL
	glMultMatrixf(matrix.matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix.matrix);
#else
//#   warning "Without OpenGL bone rests are not set for now"
	printf("Without OpenGL bone rests are not set for now\n");
#endif

	bone->matrix=matrix;
	bone->matrix.invert();
	bone->curMatrix=matrix;
	bone->curMatrix*=bone->matrix;

	vector<Bone *>::iterator ibone;
	for (ibone=bone->children.begin();
		 ibone!=bone->children.end();ibone++)
	{
#ifdef HAVE_OPENGL
		glPushMatrix();
		calculateBoneRests(*ibone);
		glPopMatrix();
#endif	
	}
}


void cGrannyFile::Render(cGrannyFile *animation, float & curTime )
{
#ifdef HAVE_OPENGL
	if (!initialized())
		return;

	Meshes &meshes = getMeshes();
	//	cGrannyFile *boneGrn = this;
	Bones &bones = getBones();
	Textures &textures = getTextures();
	BoneTies &boneTies = getTies();

	glPushMatrix();
	glLoadIdentity();
	if( animation )
		animation->getSkeleton( bones.bones[0], curTime ); 
	glPopMatrix();

	list<Mesh>::iterator imesh;
	for( imesh = meshes.meshes.begin(); imesh != meshes.meshes.end(); ++imesh )
	{
		vector<Point *> deformed;
		vector<BoneWeight>::iterator iwt;
		dword pnt = 0;
		for( iwt = imesh->weights.begin(); iwt != imesh->weights.end(); ++iwt, ++pnt )
		{
			Point *pt = new Point();
			for( dword wt = 0; wt < iwt->numWeights; ++wt )
			{
				Point post;
				float weight=iwt->weights[wt];
				dword bone=boneTies.boneties[iwt->bones[wt]]->bone;
				post=bones.bones[bone]->curMatrix*imesh->points[pnt];
				pt->points[0]+=(post.points[0]*weight);
				pt->points[1]+=(post.points[1]*weight);
				pt->points[2]+=(post.points[2]*weight); 
			}
			deformed.push_back( pt );
		}

		// we would need to recalculate normals here.. screw that dude.
		// I doubt OSI does (although since I have yet to actually run UO:TD,
		// I don't know for sure).

		dword poly = 0;
		float x,y,z;
		vector< gPolygon >::iterator polygon;
		for( polygon = imesh->polygons.begin(); polygon != imesh->polygons.end(); polygon++, poly++ )
		{
            glBindTexture( GL_TEXTURE_2D, getTexture() );

			glBegin( GL_POLYGON );
			for( int i = 0; i < 3; ++i )
			{
				//if( !animGrn )  // no normals for animations
				if( imesh->normals.size() )
				{
					Point &p = imesh->normals[ polygon->nodes[ i ] ];
					x = p.points[ 0 ];
					y = p.points[ 1 ];
					z = p.points[ 2 ];
					glNormal3f( x, y, z );
				}

				// Do we have texture-map information?
				if( imesh->textureMap.size() )
				{
					Point &p = imesh->textureMap[ textures.polygons[ poly ].nodes[ i + 1 ] ];
					x = p.points[ 0 ];
					y = p.points[ 1 ];
					glTexCoord2f(x,y);
				}

				Point * p = deformed[ polygon->nodes[ i ] ];
				x = p->points[ 0 ];
				y = p->points[ 1 ];
				z = p->points[ 2 ];

				glColor3f( 1.0f, 1.0f, 1.0f );

				//				if( polygon->nodes[i] == seq )
				//					glColor3f( 0.0f, 1.0f, 0.0f ); 

				glVertex3f( x, y, z);
			}
			glEnd();
		} 
		for (dword j = 0; j < deformed.size(); j++) 
			delete deformed[j];
		deformed.clear();
	}
#endif
}

void cGrannyFile::getSkeleton( Bone *bone, float & curTime )
{
}

cGrannyAnimation::cGrannyAnimation()
{
	animBones = NULL;
}

cGrannyAnimation::~cGrannyAnimation()
{
	if (animBones)
		delete animBones;
}


void cGrannyAnimation::Assign (cGrannyFile * model)
{
	if (!initialized() || !model->initialized())
		return;
	BoneTies &boneTies = getTies();
	BoneTies &boneLodTies = model->getTies();
	Animations &anims = getAnimations();

	vector<BoneAnim>::iterator ibn;
	dword anmNm = findString("__ObjectName");
	dword lodNm = findString("__ObjectName");

	animBones = new dword[100];
	for( int i = 0; i < 100; i++ )
		animBones[i]=0xffffffff;

	dword abone = 0;
	for( ibn = anims.bones.begin(); ibn != anims.bones.end(); ibn++, abone++ )
	{
		dword boneId;
		dword animId = ibn->id;
		dword id = boneTies.boneObjects[ animId - 1 ];
		dword textId = getValue( id, anmNm );
		std::string boneStr = findID( textId );

		/* WTF? This will never execute guys if find() returns 0..n */
		if( (int)boneStr.find( "mesh" ) != -1 || (int)boneStr.find( "master" ) != -1 )
			boneId = abone;
		else
		{
			textId = model->findString( boneStr );
			dword obj=model->findValue( lodNm, textId );
			id = boneLodTies.findID( obj );
			boneId = boneLodTies.findBone( id );
		}

		animBones[ boneId ] = abone;
	}
	
}

dword * cGrannyAnimation::GetAnimBones(void)
{
	return animBones;
}

void cGrannyAnimation::getSkeleton( Bone *bone, float & curTime )
{
	float X,Y,Z,x,y,z,w;
	dword rid,mid;

	if (!animBones)
		return;
	
	
	rid=mid=0;
	if( animBones[ bone->id ] == 0xffffffff )
	{
		return;
	}
	else
	{
		BoneAnim &boneAnim = getBoneAnim( animBones[ bone->id ] );
		vector<float>::iterator movi, roti;

		// Problem here, i suppose that some animations are looped back/forward
		if( curTime > boneAnim.length )
		{
			curTime = 0.0f;
		}

		for( roti = boneAnim.quaternionTimeline.begin();
			 roti != boneAnim.quaternionTimeline.end() && (*roti) < curTime;
			 roti++, rid++ );

		for( movi = boneAnim.translateTimeline.begin();
			 movi != boneAnim.translateTimeline.end() && (*movi) < curTime;
			 movi++, mid++ );

		X = boneAnim.translates[ mid ].points[ 0 ];
		Y = boneAnim.translates[ mid ].points[ 1 ];
		Z = boneAnim.translates[ mid ].points[ 2 ];

		// Interpolate when between Keyframes
		if( curTime != (*movi) )
		{
			float F1 = *movi;
			movi++;
			if (movi!=boneAnim.translateTimeline.end())
			{
				float F2 = *movi;
				float x2 = boneAnim.translates[mid+1].points[0];
				float y2 = boneAnim.translates[mid+1].points[1];
				float z2 = boneAnim.translates[mid+1].points[2];
				float t = ( curTime - F1 ) / ( F2 - F1 );
				x = X + t * ( x2 - X );
				y = Y + t * ( y2 - Y );
				z = Z + t * ( z2 - Z );
				X = x;
				Y = y;
				Z = z;
			}
		}

		x = boneAnim.quaternions[ rid ].points[ 0 ];
		y = boneAnim.quaternions[ rid ].points[ 1 ];
		z = boneAnim.quaternions[ rid ].points[ 2 ];
		w = boneAnim.quaternions[ rid ].points[ 3 ];
	}

	GrnMatrix matrix;
	float x2=x+x; float y2=y+y; float z2=z+z;
	float xx=x*x2; float xy=x2*y; float xz=x2*z; float xw=x2*w;
	float yy=y*y2; float yz=y2*z; float yw=y2*w;
	float zz=z*z2; float zw=z2*w;

	matrix[0] = 1 - yy - zz;
	matrix[1] =   xy + zw;
	matrix[2] =   xz - yw;
	matrix[3] = 0;
	matrix[4] =   xy - zw;
	matrix[5] = 1 - xx - zz;
	matrix[6] =   yz + xw;
	matrix[7] = 0;
	matrix[8] =   xz + yw;
	matrix[9] =   yz - xw;
	matrix[10] = 1 - xx - yy;
	matrix[11] = 0;
	matrix[12] = X;
	matrix[13] = Y;
	matrix[14] = Z;
	matrix[15] = 1;

#ifdef HAVE_OPENGL
	glMultMatrixf( matrix.matrix );
	glGetFloatv( GL_MODELVIEW_MATRIX, matrix.matrix );
	bone->curMatrix = matrix;
	bone->curMatrix *= bone->matrix;

	vector<Bone *>::iterator ibone;
	for( ibone = bone->children.begin(); ibone != bone->children.end(); ++ibone )
	{
		glPushMatrix();
		getSkeleton( *ibone, curTime);
		glPopMatrix();
	}
#endif
}
