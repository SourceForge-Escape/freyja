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

  StackNode(T data)
  {
    mData  = data;
    mPrev = NULL;
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

  StackNode<T> *mPrev;

  T mData;
};



template <class Type> class stack
{
public:

	stack()
	{
		mCount = 0;
		mTop = NULL;
	}


	~stack()
	{
		while (mTop)
		{
			pop();
		}
	}


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


	void push(Type data)
	{
		StackNode<Type> *node;

		node = new StackNode<Type>(data);
		node->Prev(mTop);

		mTop = node;
		++mCount;
	}


	bool empty()
	{
		return (mCount > 0);
	}
 

	unsigned int size()
	{
		return mCount;
	}


 private:

	unsigned int mCount;

	StackNode<Type> *mTop;

	Type mError;
};

};

#endif




