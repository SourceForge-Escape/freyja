/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * Project: mstl
 * Author : Terry 'Mongoose' Hendrix II
 * Website: http://icculus.org/~mongoose/
 * Email  : mongooseichiban@gmail.com
 * Object : list<T>
 * Comment: 
 *
 *-- History -----------------------------------------------
 *
 * 2007.06.27:
 * Mongoose - Created, very basic doubly linked list.
 ==========================================================================*/

#ifndef GUARD__MSTL_LIST_H
#define GUARD__MSTL_LIST_H

#include <stdio.h>
#include <stdlib.h>

namespace mstl 
{
	template <typename T> 
	class listnode
	{
	public:
		listnode(const T& data) 
			: mData(data), mNext(NULL), mPrev(NULL)
		{}
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		T &GetData() 
		{ return mData; }
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void SetData(const T& data) 
		{ mData = data; }
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		listnode<T> *GetNext() 
		{ return mNext; }
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void SetNext(listnode<T> *node)  
		{ mNext = node; } 
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		listnode<T> *GetPrev() 
		{ return mPrev; }
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void SetPrev(listnode<T> *node)  
		{ mPrev = node; } 
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

	protected:
		listnode<T>(const listnode<T> &node);

		listnode<T> &operator =(const listnode<T> &node);

		T mData;

		listnode<T> *mNext;
		
		listnode<T> *mPrev;
	};


	template <typename T> 
	class list 
	{ 
	public: 

		class iterator
		{
		public:
			iterator(listnode<T> *node) 
				: mCursor(node) 
			{}
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : 
			 *
			 --------------------------------------------------------*/

			~iterator() 
			{}
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : 
			 *
			 --------------------------------------------------------*/

			const T& operator*()
			{ return mCursor ? mCursor->GetData() : list<T>::mDefault; }
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : 
			 *
			 --------------------------------------------------------*/

			bool operator!=(const iterator &it)
			{ return ( mCursor != it.mCursor ); } 
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : 
			 *
			 --------------------------------------------------------*/
			
			iterator operator++(int i)
			{ 
				listnode<T> *prev = mCursor;
				mCursor = mCursor ? mCursor->GetNext() : NULL; 
				return iterator( prev ); 
			}
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : Postfix next
			 *
			 --------------------------------------------------------*/
			
			iterator& operator++()
			{ 
				mCursor = mCursor ? mCursor->GetNext() : NULL; 
				return *this; 
			}
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : Prefix next
			 *
			 --------------------------------------------------------*/

			iterator end()
			{ return iterator( NULL ); }
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : 
			 *
			 --------------------------------------------------------*/

			listnode<T>* _get_cursor( )
			{ return mCursor; }
			/*--------------------------------------------------------
			 * Pre  : 
			 * Post : Used internally by list<>.
			 *
			 --------------------------------------------------------*/


		protected:
			listnode<T>* mCursor;
		};
		
 
		list() : mHead(NULL), mTail(NULL) 
		{}
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		list(const list &container);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		~list() 
		{ clear(); }
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		const T& back() const;
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		iterator begin();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void clear();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		bool empty();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		iterator end();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void erase(iterator pos);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		const T& front() const;
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		iterator insert(iterator pos, const T& data);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : Insert data before pos, return an iterator to
		 *        the new element inserted.
		 *
		 --------------------------------------------------------*/

		void pop_back();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void pop_front();
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void push_back(const T& data);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void push_front(const T& data);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		void remove(const T& data);
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

		unsigned int size( );
		/*--------------------------------------------------------
		 * Pre  : 
		 * Post : 
		 *
		 --------------------------------------------------------*/

	protected: 

		static T mDefault;          /* Default value for data. */
		
		listnode<T> *mHead;         /* Head of the list */
		
		listnode<T> *mTail;         /* Tail of the list */

		unsigned int mSize;         /* Element count. */
	}; 



	////////////////////////////////////////////////////////////
	// Method implementations. 
	////////////////////////////////////////////////////////////

	template <typename T> 
	list<T>::list(const list<T> &container)
	{
		listnode<T> *cur = container.mHead;//->GetNext();

		while( cur )
		{
			push_back( cur->GetData() );
			cur = cur->GetNext();
		}
	}


	template <typename T> 
	const T& list<T>::back() const
	{
		return mTail ? mTail->GetData() : mDefault;  
	}


	template <typename T> 
	class list<T>::iterator list<T>::begin()
	{
		return iterator( mHead );  
	}


	template <typename T> 
	void list<T>::clear()
	{
		while (mHead)
		{
			pop_front();
		}

		mHead = mTail = NULL;
	} 


	template <typename T> 
	bool list<T>::empty()
	{
		return ( mHead );
	}


	template <typename T> 
	class list<T>::iterator list<T>::end()
	{
		return iterator( NULL ); //mTail ? mTail->GetNext() : NULL );  
	}


	template <typename T> 
	const T& list<T>::front() const
	{
		return mHead ? mHead->GetData() : mDefault;  
	}


	template <typename T> 
	void list<T>::erase( iterator pos )
	{
		listnode<T> *cur = pos._get_cursor();

		if ( cur )
		{
			if ( cur->GetNext() )
			{
				cur->GetNext()->SetPrev( cur->GetPrev() );
			}

			if ( cur->GetNext() )
			{
				cur->GetPrev()->SetNext( cur->GetNext() );
			}

			if ( cur == mHead )
			{
				mHead = cur->GetNext();
			}

			if ( cur == mTail )
			{
				mTail = cur->GetPrev();
			}

			--mSize;
		} 	
	}


	template <typename T> 
	class list<T>::iterator list<T>::insert(iterator pos, const T& data)
	{
		listnode<T> *cur = pos._get_cursor();

		if ( !cur )
		{
			return iterator(NULL);
		} 

		listnode<T> *node = new listnode<T>(data);
		node->SetNext( cur->GetNext() );
		cur->SetNext( node );

		if ( node->GetNext() )
		{
			node->GetNext()->SetPrev( node );
		}

		++mSize;
	   
		return iterator( node );  
	}


	template <typename T> 
	void list<T>::pop_back()
	{
		if (mTail)
		{
			listnode<T> *node = mTail->GetPrev();

			if (mHead == mTail)
			{
				mHead = node;
			}

			mTail = node;

			--mSize;
		}
	} 


	template <typename T> 
	void list<T>::pop_front()
	{
		if (mHead)
		{
			listnode<T> *node = mHead->GetNext();

			if (mHead == mTail)
			{
				mTail = node;
			}

			delete mHead; // ??

			mHead = node;
			--mSize;
		}
	} 


	template <typename T> 
	void list<T>::push_back(const T& data)
	{
		listnode<T>* node = new listnode<T>(data);

		if ( !mTail )
		{
			mHead = mTail = node;
		}    
		else
		{
			mTail->SetNext( node );
			node->SetPrev( mTail );
			mTail = node;
		}

		++mSize;
	}


	template <typename T> 
	void list<T>::push_front(const T& data)
	{
		listnode<T>* node = new listnode<T>(data);

		if ( !mHead )
		{
			mHead = mTail = node;
		}    
		else
		{
			mHead->SetPrev( node );
			node->SetNext( mHead );
			mHead = node;
		}

		++mSize;
	}


	template <typename T> 
	void list<T>::remove( const T& data )
	{
		listnode<T>* cur = mHead;

		while( cur )
		{
			if ( cur->GetData() == data )
			{
				erase( iterator( cur ) );
			}
			else
			{
				cur = cur->GetNext();
			}
		}
	}


	template <typename T> 
	unsigned int list<T>::size( )
	{
		return mSize;
	}


} // namespace mstl


#endif // GUARD__MSTL_LIST_H
