/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Observer pattern interface.
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is simple *non generic Observer pattern interface.
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.06:
 * Mongoose - Created, Basically to make all observers consistant and pretty.
 ==========================================================================*/

#ifndef GUARD__MSTL_OBSERVER_H_
#define GUARD__MSTL_OBSERVER_H_

#include <mstl/list.h>


namespace mstl {

class ObserverSubject;

class Observer
{
public:

	virtual ~Observer( ) {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	virtual void Notify( ObserverSubject* subject ) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Notification handler for subject change.
	 *
	 ------------------------------------------------------*/

protected:

	Observer( ) {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/
};


class ObserverSubject
{
public:

	typedef mstl::list<Observer*>::iterator ObserverIterator;

	virtual ~ObserverSubject( ) 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	ObserverIterator GetObserverIterator( ) const
	{ return mObservers.begin(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Iterator.
	 *
	 ------------------------------------------------------*/

	virtual void NotifyObservers( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Notify observers by an update from this subject.
	 *
	 ------------------------------------------------------*/

	virtual void Register( Observer* observer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Register a listener for this subject.
	 *
	 ------------------------------------------------------*/

	virtual void Unregister( Observer* observer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Unregister a listener for this subject.
	 *
	 ------------------------------------------------------*/


protected:

	ObserverSubject( ) : 
		mObservers() 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	mstl::list<Observer*> mObservers;
};


inline
void ObserverSubject::NotifyObservers( )
{
	ObserverIterator it = GetObserverIterator();
	while ( *it )
	{
		(*it)->Notify( this );
		++it;
	}
}


inline
void ObserverSubject::Register( Observer* observer )
{
	mObservers.push_back( observer );
}


inline
void ObserverSubject::Unregister( Observer* observer )
{
	mObservers.remove( observer );
}


} // namespace mstl

#endif // GUARD__MSTL_OBSERVER_H_
