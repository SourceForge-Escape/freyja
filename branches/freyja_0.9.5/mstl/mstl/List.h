/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : List
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: mtk Template list
 *
 *           UINT_MAX is an error condition, used in place of -1
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.02.19:
 * Mongoose - Using RBTree and list overlay for faster access
 *
 * 2000-10-26:
 * Mongoose - Created
 ================================================================*/


#ifndef __FREYJA_MONGOOSE_LIST_H_
#define __FREYJA_MONGOOSE_LIST_H_

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "Tree.h"

 
template <class T> class ListNode
{
public:

	ListNode(T data, unsigned int id)
	{
		_data  = data;
		_id = id;
		_next = NULL;
	}

	~ListNode()
	{
	}

	void Id(unsigned int id)
	{
		_id = id;
	}

	unsigned int Id()
	{
		return _id;
	}

	void Data(T data)
	{
		_data = data;
	}

	T Data()
	{
		return _data;
	}

	ListNode<T> *Next()
	{
		return _next;
	}

	void Next(ListNode<T> *next)
	{
		_next = next;
	}

	void Print()
	{
		//printf("(%i, %p)",  _id, _data);
		printf("%i", _id);
	}

private:

	ListNode<T> *_next;
	unsigned int _id;
	T _data;
};


template <class T> class List
{
public:

	List()
	{
		_num_items = 0;
		_head = NULL;
		_current = NULL;
		_last = NULL;
		_cache = NULL;
	}

	~List()
	{
		Clear();
	}

	// NOTE: this only copies data, the containers aren't the same ids
	void Copy(List<T> *list)
	{
		if (!list)
			return;

		for (list->Reset(); list->CurrentExists(); list->Next())
			Add(list->Current());
	}

	void Clear()
	{
		_num_items = 0;
		_last = _cache = NULL;

		while (_head)
		{
			_current = _head;
			_head = _head->Next();
			delete _current;
		}

		_tree.Clear();
	}

	T SearchId(unsigned int key)
	{
		ListNode<T> *current = _head;
		ListNode<T> *next = NULL;


		if (_head)
		{
			if (_cache)
			{
				next = _cache->Next();
			}

			// Mongoose 2002.02.19, Optimize for sequential searches
			if (next && key == next->Id())
			{
				current = next;
			}
			else // Mongoose 2002.02.19, Use search algorithm otherwise
			{
				current = _tree.SearchByKey(key, &_error);
			}
      
			if (current)
			{
				_cache = _current = current;
				return current->Data();
			}
		}
		else
		{      
			printf("List[%i] = NULL\n", key);
		}

		return 0;
	}

	unsigned int SearchKey(T data)
	{
		ListNode<T> *current = _head;
		ListNode<T> *last = NULL;


		if (_cache)
		{
			// Mongoose: 2001-01-31, hhmmm... fixed?
			if (data == _cache->Data())
				return _cache->Id();
		}

		while (current)
		{
			// Found
			if (current->Data() == data)
			{
				_cache = current;
				return current->Id();
			}

			last = current;
			current = current->Next();
		}    

		return UINT_MAX;
	}

	T operator [] (unsigned int i)
	{
		return SearchId(i);
	}


	void RemoveId(unsigned int id)
	{
		ListNode<T> *current = _head;
		ListNode<T> *last = NULL;


		_last = _cache = NULL; 

		while (current)
		{
			// Remove
			if (current->Id() == id)
			{
				if (current == _head)
					_head = current->Next();
				else
				{
					last->Next(current->Next());
				}

				if (_current == current)
					_current = NULL;
	
				_tree.RemoveByKey(current->Id());
				delete current;
				_num_items--;

				return;
			}

			last = current;
			current = current->Next();
		}    
	}

	void Remove(T data)
	{
		ListNode<T> *current = _head;
		ListNode<T> *last = NULL;


		_last = _cache = NULL;

		while (current)
		{
			// Remove
			if (current->Data() == data)
			{
				if (current == _head)
					_head = current->Next();
				else
				{
					last->Next(current->Next());
				}
	
				if (_current == current)
					_current = NULL;

				_tree.RemoveByKey(current->Id());
				delete current;
				_num_items--;

				return;
			}

			last = current;
			current = current->Next();
		}    
	}


	void Replace(T data, T data2)
	{
		ListNode<T> *current = _head;
		ListNode<T> *last = NULL;


		_last = _cache = NULL;

		while (current)
		{
			// Replace
			if (current->Data() == data)
			{
				current->Data(data2);
				return;
			}

			last = current;
			current = current->Next();
		}    
	}


	bool Empty()
	{
		return (_head == NULL);
	}


	unsigned int NumItems()
	{
		return _num_items;
	}


	void Print(void (*print_func)(T))
	{
		ListNode<T> *current = _head;

		printf(" [%i] {\n", _num_items);

    
		while (current)
		{
			printf("#%i, ", current->Id());

			if (print_func)
				(*print_func)(current->Data());

			current = current->Next();

			fflush(stdout);
		}

		printf(" }\n");    
	}


	void Print()
	{
		ListNode<T> *current = _head;

		printf("List %i {\n", _num_items);

    
		while (current)
		{
			//current->Print();
			printf("%i", current->Id());

			current = current->Next();

			if (current)
				printf(", ");

			fflush(stdout);
		}

		printf(" }\n");
	}

	//////////////////////////////

	void start()
	{
		Reset();
	}

	bool forward()
	{
		return CurrentExists();
	}

	bool next()
	{
		return Next();
	}

	T current()
	{
		return Current();
	}

	//////////////////////////////

	void Reset()
	{
		_current = _head;
		_cache = _head;
	}

	bool operator ++ (int dummy)
	{
		return Next();
	}

	bool Next()
	{
		if (_current)
			_current = _current->Next();

		return (_current != NULL);
	}

	unsigned int CurrentId()
	{
		if (!_current)
			return UINT_MAX;
    
		return _current->Id();
	}

	bool CurrentExists()
	{
		return (_current != 0);
	}

	T Current()
	{
		if (_current)
			return _current->Data();
		else
			return 0;
	}

	unsigned int Add(T data)
	{
		ListNode<T> *node;


		node = new ListNode<T>(data, _num_items++);
		return Add(node);
	}

	unsigned int Add(ListNode<T> *node)
	{
		ListNode<T> *current;
		ListNode<T> *last;
		unsigned int i;


		if (_head)
		{
			current = _head;
			last = NULL;
			i = 0;

			//EXP
			if (_last)
			{
				i = _last->Id();
				current = _last;
			}

			while (current)
			{
				// Prepend
				if (current->Id() > i)
				{
					node->Id(i);
					node->Next(current);
	  
					if (current == _head)
						_head = node;
					else if (last)
						last->Next(node);

					_tree.Insert(node->Id(), node);
					return node->Id();
				}

				i++;
				last = current;
				current = current->Next();
			}

			// Append
			last->Next(node);
		}
		else
		{
			_head = node;
		}

		_last = node; //EXP

		_tree.Insert(node->Id(), node);
		return node->Id();
	}

private:

	bool _error;

	unsigned int _num_items;

	Tree<unsigned int, ListNode<T> *> _tree;

	ListNode<T> *_head;

	ListNode<T> *_current;

	ListNode<T> *_last;

	ListNode<T> *_cache;
};

#endif
