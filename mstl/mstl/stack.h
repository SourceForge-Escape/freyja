/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * Project: Freyja
 * Author : Terry 'Mongoose' Hendrix II
 * Website: http://www.westga.edu/~stu7440/
 * Email  : stu7440@westga.edu
 * Object : Stack
 * Comment: 
 *
 *-- History -----------------------------------------------
 *
 * 2006.06.18:
 * Mongoose - Merged back in comments from a long time ago hah
 *
 * 2004.04.15:
 * Mongoose - No more leading underscores, style changes
 *
 * 2001.05.12:
 * Mongoose - Created, from my old Stack template class
 ==========================================================================*/

#ifndef GUARD__MONGOOSE_MSTL_STACK_H
#define GUARD__MONGOOSE_MSTL_STACK_H

#include <stdio.h>
#include <stdlib.h>

namespace mstl {

template <class T> class StackNode
{
 public:

	StackNode(T data) :
		mPrev(NULL),
		mData(data)
	{
	}


	~StackNode()
	{
	}

	void Data(T data)
	{
		mData = data;
	}

	T Data()
	{
		return mData;
	}

	StackNode<T> *Prev()
	{
		return mPrev;
	}

	void Prev(StackNode<T> *prev)
	{
		mPrev = prev;
	}

private:

	StackNode<T>(const StackNode<T> &node);

	StackNode<T> &operator =(const StackNode<T> &node);

	
	StackNode<T> *mPrev;
	
	T mData;
};



template <class Type> class stack
{
public:

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : Nothing
    * Pre      : None
    * Post     : Constructs an Stack
    -----------------------------------------*/
	stack() :
		mCount(0),
		mTop(NULL),
		mError(false)
	{
	}

	stack(const stack<Type> &s) :
		mCount(0),
		mTop(NULL),
		mError(false)
	{
		StackNode<Type> *cur = s.mTop;
		stack<Type> tmp; // haha

		while (cur)
		{
			Type data = cur->Data();
			cur = cur->Prev();
			tmp.push(data);
		}

		while ( !tmp.empty() )
		{
			push(tmp.pop());
		}
	}


	stack<Type> operator=(const stack<Type> &s) 
	{
		mCount = 0;
		mTop = NULL;
		mError = false;

		StackNode<Type> *cur = mTop;
		stack<Type> tmp; // haha

		while (cur)
		{
			Type data = cur->Data();
			cur = cur->Prev();
			tmp.push(data);
		}

		while ( !tmp.empty() )
		{
			push(tmp.pop());
		}

		return *this;
	}


   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : Nothing
    * Pre      : None
    * Post     : Deconstructs an Stack
    -----------------------------------------*/
	~stack()
	{
		while (mTop)
		{
			pop();
		}
	}

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : value of top stack element
    * Pre      : stack isn't empty
    * Post     : Returns top of stack's value
    -----------------------------------------*/
	Type peek()
	{
		if (mTop)
		{
			return mTop->Data();
		}
		else
		{
			fprintf(stderr, "Stack<T>::Peek> NULL mTop!\n");     
			return (Type)0x0;
		}
	}

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : value of top stack element
    * Pre      : stack isn't empty
    * Post     : Removes top of stack and 
    *            returns it's value
    -----------------------------------------*/
	Type pop()
	{
		StackNode<Type> *rm;

		if (mTop)
		{
			rm = mTop;

			Type data = mTop->Data();
			mTop = mTop->Prev();

			delete rm;
			--mCount;

			return data;
		}
		else
		{
			fprintf(stderr, "Stack<T>::Pop> NULL mTop!\n");
			return (Type)0x0;
		}
	}

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : Value to push on top of stack
    * Returns  : Nothing
    * Pre      : Stack isn't full
    * Post     : Stack has new top, with value i
    -----------------------------------------*/
	void push(Type data)
	{
		StackNode<Type> *node = new StackNode<Type>(data);
		node->Prev(mTop);

		mTop = node;
		++mCount;
	}

   /*-----------------------------------------
    * Created  : 990831 by Terry Hendrix II
    * Modified : NA
    * Inputs   : None
    * Returns  : true if stack is empty, else
    *            returns false
    * Pre      : None
    * Post     : No side effect
    -----------------------------------------*/
	bool empty()
	{
		return (mCount == 0);
	}
 

	unsigned int size()
	{
		return mCount;
	}


 private:

	unsigned int mCount;

	StackNode<Type> *mTop;

	bool mError;
};

} /* namespace mstl */

#endif




