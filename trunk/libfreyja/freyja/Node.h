/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Node
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the scene graph node interface.
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_NODE_H_
#define GUARD__FREYJA_NODE_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <mstl/String.h>
#include <mstl/list.h>
#include <mstl/avl_tree.h>
#include <mstl/Observer.h>

#include "Renderable.h"
#include "XMLSerializer.h"
#include "freyja.h"

namespace freyja {

class Node;
class NodeObserver;
class Scene;

typedef mstl::list<freyja::Node*> NodeList;
typedef mstl::list<freyja::Node*>::iterator NodeIterator;
typedef mstl::avl_tree<mstl::String, freyja::Node*> NodeDictionary;
typedef mstl::avl_tree<mstl::String, NodeList*> NodeListDictionary;

typedef enum {	
	tLocal = 1,
	tParent,
	tWorld

} TransformSpace;


class Node : 
		public mstl::ObserverSubject, 
		public XMLSerializer
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Node();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Node( const char* name );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	virtual ~Node();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public methods.
	////////////////////////////////////////////////////////////

	static freyja::Node* Cast( freyja_ptr ptr )
	{ return (freyja::Node*)ptr; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME Add RTTI checking.
	 *
	 ------------------------------------------------------*/

	virtual XMLSerializerNode CreateXMLSerializerNode( ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Creates serializer node for this object.
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* Duplicate() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy of this node sans children.
	 *
	 ------------------------------------------------------*/

	virtual void DuplicateChildren( freyja::Node* parent, bool recurse ) = 0;
	/*------------------------------------------------------
	 * Pre  : <parent>'s children will be duplicated and added to this node.     
	 *        <recurse> if you want to copy the entire subtree.
	 *
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* GetChild( const uint16 id ) const; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns child with id or NULL.
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* GetChild( const char* name ) const; 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns child with name or NULL.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetChildCount() const
	{ return mChildren.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets the number of children in ChildList.
	 *
	 ------------------------------------------------------*/

	NodeIterator GetChildIterator() const
	{ return mChildren.begin(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns interator for ChildList.
	 *
	 ------------------------------------------------------*/

	hel::Quat GetDerivedOrientation( ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get orientation derived from root of hierarchy.
	 *
	 ------------------------------------------------------*/

	hel::Vec3 GetDerivedPosition( ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get position derived from root of hierarchy.
	 *
	 ------------------------------------------------------*/

	virtual mstl::String GetInfo() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return debug / text details of node.
	 *
	 ------------------------------------------------------*/

	const char* GetMetadata() const
	{ return mMetadata.c_str(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get metadata for this node.
	 *
	 ------------------------------------------------------*/

	bool GetMute( ) const
	{ return mMuted; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Is this node allowing content editing?
	 *        IsMuted() was sexier...
	 *
	 ------------------------------------------------------*/

	const char* GetName( ) const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the human readable name of this node.
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* GetParent( ) const
	{ return mParent; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get parent of this node, which can be NULL.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetPosition() const
	{ return mPosition; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get position property.
	 *
	 ------------------------------------------------------*/

	const hel::Quat& GetOrientation() const
	{ return mOrientation; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get orientation property.
	 *
	 ------------------------------------------------------*/

	RenderableIterator GetRenderableIterator( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scale property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetScale() const
	{ return mScale; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scale property.
	 *
	 ------------------------------------------------------*/

	const freyja::Scene* GetScene( )
	{ return mScene; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scene property.
	 *
	 ------------------------------------------------------*/

	freyja_ptr ToPtr() const
	{ return (freyja_ptr)this; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this pointer as void* for C ABI
	 *        and for Unique Identifer use.
	 *
	 *        Basically, this lets other languages reference
	 *        nodes by address rather than a contrived id system.
	 *
	 ------------------------------------------------------*/

	virtual void RemoveChild( freyja::Node* child );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes child from ChildList.
	 *
	 ------------------------------------------------------*/

	virtual void RemoveChildren( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes all children from ChildList.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Properties
	////////////////////////////////////////////////////////////

	void SetMetadata( const char* metadata ) 
	{ mMetadata = metadata; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set metadata for this node.
	 *
	 ------------------------------------------------------*/

	virtual void SetMute( bool mute ) 
	{ mMuted = mute; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set this node to [not] allow content editing.
	 *
	 ------------------------------------------------------*/

	void SetName( const char* name ) 
	{ mName = name; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set the human readable name of this node.
	 *
	 ------------------------------------------------------*/

	virtual void SetParent( freyja::Node* parent );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : (Re)parent this node to another or NULL.
	 *
	 ------------------------------------------------------*/

	const void SetPosition( const hel::Vec3& v )
	{ mPosition = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set position property.
	 *
	 ------------------------------------------------------*/

	const void SetOrientation( const hel::Quat& q )
	{ mOrientation = q; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set orientation property.
	 *
	 ------------------------------------------------------*/

	const void SetScale( const hel::Vec3& v )
	{ mScale = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set scale property.
	 *
	 ------------------------------------------------------*/

	void SetScene( freyja::Scene* scene )
	{ 
		mScene = scene; 
		NotifyOnSceneChange( ); 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set scene property.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Transforms
	////////////////////////////////////////////////////////////

	virtual void Pitch( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 * Post : X-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	virtual void Roll( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 * Post : Z-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	virtual void Yaw( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 * Post : Y-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	virtual void Rotate( const vec_t angle, const hel::Vec3& axis, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 * Post : Abitrary axis rotation ( Axis angles ).
	 *
	 ------------------------------------------------------*/

	virtual void Scale( const hel::Vec3& scale );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Scales node in tLocal space. 
	 *
	 ------------------------------------------------------*/

	virtual void Translate( const hel::Vec3& v, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates node in given space.
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected methods.
	////////////////////////////////////////////////////////////

	virtual void AddChild( freyja::Node* child );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds child to this node's child list/dict, and
	 *        removes child from any other parent.
	 *
	 ------------------------------------------------------*/

	virtual void NotifyOnDelete( ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Notify observers that this node is being removed.
	 *
	 ------------------------------------------------------*/

	virtual void NotifyOnParentChange( ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Notify observers that this node is being reparented.
	 *
	 ------------------------------------------------------*/

	virtual void NotifyOnSceneChange( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void CopyNodeMembers( freyja::Node* node ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy member values.
	 *
	 ------------------------------------------------------*/

	bool mMuted;                              /* Determines if editing is allowed. */

	mstl::String mName;                       /* String name. */

	mstl::String mMetadata;                   /* Optional metadata for any use. */

	hel::Vec3 mPosition;                      /* Offset of this node from parent. */

	hel::Quat mOrientation;                   /* Orientation relative to parent. */

	hel::Vec3 mScale;                         /* Absolute scaling. */

	hel::Mat44 mLocalTransform;               /* Might as well cache local transform. */

	freyja::Scene* mScene;                    /* Scene that contains this node (can be NULL). */

	freyja::Node* mParent;                    /* Node parent. */
 
	NodeList mChildren;                       /* Node children. */
	NodeDictionary mChildDictionary;

	RenderableList mRenderables;              /* Renderables based on this node. */
};


class NodeObserver : 
	public mstl::Observer
{
public:
	NodeObserver() : mstl::Observer() {}
	virtual ~NodeObserver() {}
	virtual void NotifyUpdate( const Node* node ) {}
	virtual void NotifyDelete( const Node* node ) {}
	virtual void NotifyParentChange( const Node* node ) {}
};


inline
Node::Node( ) :
	mMuted( false ),
	mName( "" ),
	mMetadata( "" ),
	mPosition( 0.0f, 0.0f, 0.0f ),
	mOrientation( ),
	mScale( 1.0f, 1.0f, 1.0f ),
	mLocalTransform( ),
	mScene( NULL ),
	mParent( NULL ),
	mChildren( ),
	mRenderables( )
{ }


inline
Node::Node( const char* name ) :
	mMuted( false ),
	mName( name ),
	mMetadata( "" ),
	mPosition( 0.0f, 0.0f, 0.0f ),
	mOrientation( ),
	mScale( 1.0f, 1.0f, 1.0f ),
	mLocalTransform( ),
	mScene( NULL ),
	mParent( NULL ),
	mChildren( ),
	mRenderables( )
{ }


inline
Node::~Node( )
{ }


} // namespace freyja

#   define FREYJA_NODE_INTERFACE \
	virtual freyja::Node* Duplicate() const; \
	virtual void DuplicateChildren( freyja::Node* parent, bool recurse ); \
	virtual mstl::String GetInfo() const;


#endif // GUARD__FREYJA_NODE_H_

