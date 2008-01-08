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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_NODE - Builds class as a console unit test. 
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
#include <mstl/Vector.h>
#include <mstl/String.h>
#include <mstl/Observer.h>

#include "XMLSerializerInterface.h"
#include "freyja.h"

namespace freyja {

class Node : public ObserverSubject : public XMLSerializerInterface
{
public:

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,   /* This node should be rendered with a highlight.*/
		fSelected    =  2,   /* This node is marked as selected. */
		fHidden      =  4,   /* This node hidden on the client. */
		fRayHit      =  8,   /* This node was marked in a Ray hit test. */
		fReserved16  = 16,   /*  */   
		fReserved32  = 32,   /*  */
		fReserved64  = 64    /*  */    
	} Flag;

	typedef enum {

		tWorld = 1,
		tParent,
		tPivot, //	tCenterMass,
		tLocal

	} TransformSpace;


	class NodeObserver : public ObserverInterface
	{
	public:
		virtual ~NodeObserver() {}
		virtual void NotifyUpdate( const Node* node ) {}
		virtual void NotifyDelete( const Node* node ) {}
		virtual void NotifyParent( const Node* node ) {}
		virtual void NofityUnparent( const Node* node ) {}
	protected:
		NodeObserver() : ObserverInterface() {}
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Node();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Node(const char* name);
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

	void ClearFlag(Node::Flag flag) { mFlags &= ~flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Clears option flag.
	 *
	 ------------------------------------------------------*/

	void SetFlag(Node::Flag flag) { mFlags |= flag; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets option flag.
	 *
	 ------------------------------------------------------*/

	const byte& GetFlags() { return mFlags; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option flag bitmap.
	 *
	 ------------------------------------------------------*/

	bool IsMuted() { return mMuted; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Is this node allowing content editing?
	 *
	 ------------------------------------------------------*/

	virtual void Mute() { mMuted = true; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set this node to not allow content editing.
	 *
	 ------------------------------------------------------*/

	virtual void UnMute() { mMuted = false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set this node to allow content editing.
	 *
	 ------------------------------------------------------*/

	const char* GetMetadata() { return mMetadata.c_str(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get metadata for this node.
	 *
	 ------------------------------------------------------*/

	void SetMetadata(const char* metadata) { mMetadata = metadata; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set metadata for this node.
	 *
	 ------------------------------------------------------*/

	const char* GetName() { return mName.c_str(); }
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

	virtual freyja::Node* Duplicate();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copy of this node sans children.
	 *
	 ------------------------------------------------------*/

	virtual void DuplicateChildren( freyja::Node* parent, bool recurse );
	/*------------------------------------------------------
	 * Pre  : <parent>'s children will be duplicated and added to this node.     
	 *        <recurse> if you want to copy the entire subtree.
	 *
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* GetParent() 
	{ return mParent; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual freyja::Node* GetChild(uint32 child_index ) 
	{ return ( child_index<mChildren.size() ) ? mChildren[child_index] : NULL; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetChildCount()
	{ return mChildren.size(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	// GetChildIterator();

	virtual RemoveChild( freyja::Node* child );

	virtual RemoveChild( uint32 child_index );

	virtual RemoveChildren( );


	const hel::Vec3& GetPosition() const { return mPosition; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get position property.
	 *
	 ------------------------------------------------------*/

	const hel::Quat& GetOrientation() const { return mOrientation; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get orientation property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetScale() const { return mScale; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get scale property.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetWorldPosition() const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	const hel::Quat& GetWorldOrientation() const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	// No more Euler arrays to avoid confusion!  

	virtual void Pitch( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 *
	 * Post : X-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	virtual void Roll( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 *
	 * Post : Z-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	virtual void Yaw( const vec_t angle, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 *
	 * Post : Y-axis rotation ( Euler angles componet ).
	 *
	 ------------------------------------------------------*/

	//void RotateAboutPoint(const hel::Vec3& point, const hel::Vec3& v);
	virtual void Rotate( const vec_t angle, const hel::Vec3& axis, TransformSpace about = tParent );
	/*------------------------------------------------------
	 * Pre  : <angle> is in radians.
	 *
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

	virtual void UpdateParentOnDelete() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void UpdateChildrenOnDelete() = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	byte mFlags;                              /* State flags. */

	bool mMuted;                              /* Determines if editing is allowed. */

	mstl::String mName;                       /* String name. */

	mstl::String mMetadata;                   /* Optional metadata for any use. */

	hel::Vec3 mPosition;                      /* Offset of this node from parent. */

	hel::Quat mOrientation;                   /* Orientation relative to parent. */

	hel::Vec3 mScale;                         /* Absolute scaling. */

	hel::Mat44 mLocalTransform;               /* Might as well cache local transform. */

	freyja::Node* mParent;                    /* Node parent. */

	mstl::Vector<freyja::Node*> mChildren;    /* Node children. */
};

} // namespace freyja

#endif // GUARD__FREYJA_NODE_H_

