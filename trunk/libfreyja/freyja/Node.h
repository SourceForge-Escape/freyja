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

class NodeObserver;

class Node : 
		public mstl::ObserverSubject, 
		public XMLSerializer,
		public Renderable
{
public:

	typedef enum {

		tWorld = 1,
		tParent,
		tPivot, 
		//tCenterMass,
		tLocal

	} TransformSpace;

	typedef mstl::list<freyja::Node*>::iterator ChildIterator;
	typedef mstl::avl_tree<mstl::String, freyja::Node*> ChildDictionary;
	typedef mstl::list<freyja::Node*> ChildList;


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
	// Properties
	////////////////////////////////////////////////////////////

	bool IsMuted() const
	{ return mMuted; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Is this node allowing content editing?
	 *
	 ------------------------------------------------------*/

	virtual void Mute() 
	{ mMuted = true; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set this node to not allow content editing.
	 *
	 ------------------------------------------------------*/

	virtual void UnMute() 
	{ mMuted = false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set this node to allow content editing.
	 *
	 ------------------------------------------------------*/

	const char* GetMetadata() const
	{ return mMetadata.c_str(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get metadata for this node.
	 *
	 ------------------------------------------------------*/

	void SetMetadata(const char* metadata) 
	{ mMetadata = metadata; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set metadata for this node.
	 *
	 ------------------------------------------------------*/

	const char* GetName() const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the human readable name of this node.
	 *
	 ------------------------------------------------------*/

	void SetName(const char* name) { mName = name; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set the human readable name of this node.
	 *
	 ------------------------------------------------------*/

	static freyja::Node* Cast( freyja_ptr ptr )
	{ return (freyja::Node*)ptr; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME Add RTTI checking.
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

	virtual freyja::Node* GetParent() const
	{ return mParent; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void SetParent( freyja::Node* parent ) 
	{ 
		mParent = parent; 
		if ( mParent ) mParent->AddChild( this );
	}
	/*------------------------------------------------------
	 * Pre  : 
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

	ChildIterator GetChildIterator() const
	{ return mChildren.begin(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns interator for ChildList.
	 *
	 ------------------------------------------------------*/

	virtual mstl::String GetInfo() const = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return debug / text details of node.
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

	const hel::Vec3& GetScale() const
	{ return mScale; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scale property.
	 *
	 ------------------------------------------------------*/

	freyja_ptr GetUID() const
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

	const void SetPosition( const hel::Vec3& v )
	{ mPosition = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get position property.
	 *
	 ------------------------------------------------------*/

	const void SetOrientation( const hel::Quat& q )
	{ mOrientation = q; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get orientation property.
	 *
	 ------------------------------------------------------*/

	const void SetScale( const hel::Vec3& v )
	{ mScale = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scale property.
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
	 * Post : Scales node (tLocal). 
	 *
	 ------------------------------------------------------*/

	virtual void Translate( const hel::Vec3& v, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Translates node.
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected methods.
	////////////////////////////////////////////////////////////

	virtual void AddChild( freyja::Node* child ) 
	{ 
		child->mParent = this;
		mChildDictionary.insert( mstl::String( child->GetName() ), child );
		mChildren.push_back( child );
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void NotifyOnDelete() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Notify observers that this node is being removed.
	 *
	 ------------------------------------------------------*/

	void Copy( freyja::Node* node ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy member values.
	 *
	 ------------------------------------------------------*/


	//byte mFlags;                            /* State flags. */

	bool mMuted;                              /* Determines if editing is allowed. */

	mstl::String mName;                       /* String name. */

	mstl::String mMetadata;                   /* Optional metadata for any use. */

	hel::Vec3 mPosition;                      /* Offset of this node from parent. */

	hel::Quat mOrientation;                   /* Orientation relative to parent. */

	hel::Vec3 mScale;                         /* Absolute scaling. */

	hel::Mat44 mLocalTransform;               /* Might as well cache local transform. */

	freyja::Node* mParent;                    /* Node parent. */
 
	ChildList mChildren;                      /* Node children. */
	ChildDictionary mChildDictionary;
};


class NodeObserver : 
	public mstl::Observer
{
public:
	NodeObserver() : mstl::Observer() {}
	virtual ~NodeObserver() {}
	virtual void NotifyUpdate( const Node* node ) {}
	virtual void NotifyDelete( const Node* node ) {}
	virtual void NotifyParent( const Node* node ) {}
	virtual void NofityUnparent( const Node* node ) {}
};


inline
Node::Node( ) :
	//mFlags( 0 ),
	mMuted( false ),
	mName( "" ),
	mMetadata( "" ),
	mPosition( 0.0f, 0.0f, 0.0f ),
	mOrientation( ),
	mScale( 1.0f, 1.0f, 1.0f ),
	mLocalTransform( ),
	mParent( NULL ),
	mChildren( )
{ }


inline
Node::Node( const char* name ) :
	//mFlags( 0 ),
	mMuted( false ),
	mName( name ),
	mMetadata( "" ),
	mPosition( 0.0f, 0.0f, 0.0f ),
	mOrientation( ),
	mScale( 1.0f, 1.0f, 1.0f ),
	mLocalTransform( ),
	mParent( NULL ),
	mChildren( )
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

