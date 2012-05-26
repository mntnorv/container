// container.h
// Part of "container" library by Mantas Norvaisa

#ifndef INCLUDED_CONTAINER_H
#define INCLUDED_CONTAINER_H

//--------------------------------------------------
//------------------Container-Class-----------------
//--------------------------------------------------

template <class T>
class ContainerArray
{
//----------------------------------------
// Element class
//----------------------------------------
public:
	class element
	{
	private:
		T data;
		typename ContainerArray<T>::element * next;
		typename ContainerArray<T>::element * prev;
		bool isnull;
	public:
		element(): data(T()), next(NULL), prev(NULL), isnull(true) { }
		~element() {}

		T& Data() {return data;}
		typename ContainerArray<T>::element * Next () {return next;}
		typename ContainerArray<T>::element * Prev () {return prev;}
		bool IsNull () {return isnull;}

		void Data (T newData) {data = newData;}
		void Next (typename ContainerArray<T>::element * newNext) {next = newNext;}
		void Prev (typename ContainerArray<T>::element * newPrev) {prev = newPrev;}
		void IsNull (bool newIsNull) {isnull = newIsNull;}
	};
//----------------------------------------
// Iterator class
//----------------------------------------
public:
	class Iterator
	{
	private:
		typename ContainerArray<T>::element * i;
	public:
		Iterator ();
		~Iterator () {};

		void setPtr (typename ContainerArray<T>::element * ptr);

		T& Data ();
		bool IsNull ();

		void operator++ (int);
		void operator-- (int);
	};
//----------------------------------------
// Private vars and methods
//----------------------------------------
private:
	typename ContainerArray<T>::element * first;
	typename ContainerArray<T>::element * last;
	int size;

	typename ContainerArray<T>::element * getElement (int i);
	void deleteElement (typename ContainerArray<T>::element * el);
//----------------------------------------
// Public vars and methods
//----------------------------------------
public:
	ContainerArray();
	ContainerArray(ContainerArray<T> & copyFromThis);
	~ContainerArray();

	void InsertFront (T newElement);
	void InsertBack (T newElement);
	void Insert (T newElement, int position);
	void Delete (int i);
	void Swap (int i, int j);
	void Clear ();
	void BubbleSort (bool asc);
	void CopyFrom (ContainerArray<T> & arr);

	int GetSize();
	Iterator GetFirst();
	Iterator GetLast();

	ContainerArray<T> & operator= (ContainerArray<T> & other);
	T& operator[] (int i);
};



//====================================================================================================
// CONTAINER ARRAY
//====================================================================================================



//--------------------------------------------------
//------------------Private-methods-----------------
//--------------------------------------------------

template <class T>
typename ContainerArray<T>::element * ContainerArray<T>::getElement (int i)
{
	typename ContainerArray<T>::element * ptr;

	if (i <= size/2)
	{
		ptr = first->Next();

		for (int j = 0; j < i && (!ptr->IsNull()); j++)
			ptr = ptr->Next();
	}
	else
	{
		ptr = last->Prev();

		for (int j = size - 1; j > i && (!ptr->IsNull()); j--)
			ptr = ptr->Prev();
	}

	return ptr;
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::deleteElement (typename ContainerArray<T>::element * el)
{
	if (!el->IsNull())
	{
		el->Prev()->Next(el->Next());
		el->Next()->Prev(el->Prev());

		delete el;
		
		size--;
	}
}

//--------------------------------------------------
//--------------------Constructors------------------
//--------------------------------------------------

template <class T>
ContainerArray<T>::ContainerArray()
{
	first = new typename ContainerArray<T>::element;
	last = new typename ContainerArray<T>::element;

	first->Next (last);
	last->Prev (first);

	size = 0;
}

//--------------------------------------------------

template <class T>
ContainerArray<T>::ContainerArray(ContainerArray<T> & copyFromThis)
{
	CopyFrom (copyFromThis);
}

//--------------------------------------------------
//---------------------Destructor-------------------
//--------------------------------------------------

template <class T>
ContainerArray<T>::~ContainerArray()
{
	typename ContainerArray<T>::element * ptr = first;

	while (ptr != NULL)
	{
		typename ContainerArray<T>::element * old = ptr;
		ptr = ptr->Next();

		delete old;
	}
}

//--------------------------------------------------
//---------------------Insert-----------------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::InsertFront (T newElement)
{
	typename ContainerArray<T>::element * elem = new typename ContainerArray<T>::element;

	elem->Prev (first);
	elem->Next (first->Next());
	elem->Next()->Prev (elem);
	elem->Prev()->Next (elem);

	elem->Data (newElement);
	elem->IsNull (false);

	size++;
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::InsertBack (T newElement)
{
	typename ContainerArray<T>::element * elem = new typename ContainerArray<T>::element;

	elem->Next (last);
	elem->Prev (last->Prev());
	elem->Next()->Prev (elem);
	elem->Prev()->Next (elem);

	elem->Data (newElement);
	elem->IsNull (false);

	size++;
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::Insert (T newElement, int position)
{
	if (position < size && position > -1)
	{
		typename ContainerArray<T>::element * next = getElement (position);
		typename ContainerArray<T>::element * prev = next->prev;
		typename ContainerArray<T>::element * elem = new typename ContainerArray<T>::element;

		elem->Data() = newElement;

		prev->Next(elem);
		next->Prev(elem);
		elem->Next(next);
		elem->Prev(prev);

		size++;
	}
}

//--------------------------------------------------
//---------------Delete, Swap, Clear----------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::Delete (int i)
{
	typename ContainerArray<T>::element * ptr = getElement(i);

	deleteElement (ptr);
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::Swap (int i, int j)
{
	typename ContainerArray<T>::element * iptr = getElement(i);
	typename ContainerArray<T>::element * jptr = getElement(j);

	if ((!iptr->IsNull()) && (!jptr->IsNull()))
	{
		T temp = iptr->Data();
		iptr->Data(jptr->Data());
		jptr->Data(temp);
	}
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::Clear()
{
	typename ContainerArray<T>::element * ptr = first->Next();

	while (!ptr->IsNull())
	{
		typename ContainerArray<T>::element * old = ptr;
		ptr = ptr->Next();

		delete old;
	}

	first->Next (last);
	last->Prev (first);

	size = 0;
}

//--------------------------------------------------
//----------------------Sort------------------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::BubbleSort (bool asc)
{
	if (size > 1)
	{
		bool swapped;
		bool swap;

		do
		{
			swapped = false;

			for (typename ContainerArray<T>::element * ptr = first->Next(); !ptr->Next()->IsNull(); ptr = ptr->Next())
			{
				swap = false;

				if (asc && ptr->Data() > ptr->Next()->Data())
					swap = true;

				if ((!asc) && ptr->Next()->Data() > ptr->Data())
					swap = true;

				if (swap)
				{
					T temp = ptr->Data();
					ptr->Data (ptr->Next()->Data());
					ptr->Next()->Data (temp);
					swapped = true;
				}
			}
		}
		while (swapped == true);
	}
}

//--------------------------------------------------
//----------------------Copy------------------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::CopyFrom (ContainerArray<T> & arr)
{
	Clear();

	for (ContainerArray<T>::Iterator i = arr.GetFirst(); !i.IsNull(); i++)
	{
		InsertBack (i.Data());
	}
}

//--------------------------------------------------
//-------------------"GET"-Methods------------------
//--------------------------------------------------

template <class T>
int ContainerArray<T>::GetSize()
{
	return size;
}

//--------------------------------------------------

template <class T>
typename ContainerArray<T>::Iterator ContainerArray<T>::GetFirst()
{
	Iterator fi;
	fi.setPtr (first->Next());
	return fi;
}

//--------------------------------------------------

template <class T>
typename ContainerArray<T>::Iterator ContainerArray<T>::GetLast()
{
	Iterator li;
	li.setPtr (last->Prev());
	return li;
}

//--------------------------------------------------
//---------------------Operators--------------------
//--------------------------------------------------

template <class T>
ContainerArray<T> & ContainerArray<T>::operator= (ContainerArray<T> & other)
{
	if (this != &other)
	{
		CopyFrom (other);
	}

	return *this;
}

//--------------------------------------------------

template <class T>
T& ContainerArray<T>::operator[] (int i)
{
	return getElement(i)->Data();
}



//====================================================================================================
// ITERATOR
//====================================================================================================



//--------------------------------------------------
//--------------------Constructor-------------------
//--------------------------------------------------

template <class T>
ContainerArray<T>::Iterator::Iterator ()
{
	i = NULL;
}

//--------------------------------------------------
//---------------------Functions--------------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::Iterator::setPtr (typename ContainerArray<T>::element * ptr)
{
	i = ptr;
}

//--------------------------------------------------

template <class T>
T& ContainerArray<T>::Iterator::Data ()
{
	return i->Data();
}

//--------------------------------------------------

template <class T>
bool ContainerArray<T>::Iterator::IsNull ()
{
	if (i == NULL)
		return true;
	else
		return i->IsNull();
}

//--------------------------------------------------
//---------------------Operators--------------------
//--------------------------------------------------

template <class T>
void ContainerArray<T>::Iterator::operator++ (int)
{
	if (i != NULL && !i->IsNull())
	{
		i = i->Next();
	}
}

//--------------------------------------------------

template <class T>
void ContainerArray<T>::Iterator::operator-- (int)
{
	if (i != NULL && !i->IsNull())
	{
		i = i->Prev();
	}
}

//--------------------------------------------------

#endif