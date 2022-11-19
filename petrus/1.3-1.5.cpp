#include <list>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;


class Exception : public exception
{
protected:
	char* str;
public:
	Exception()
	{
		str = NULL;
	}
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class IteratorErrorException : public Exception
{
public:
	IteratorErrorException(char* s) : Exception(s) { }
	IteratorErrorException(const char* s) : Exception(s) { }
	IteratorErrorException(const IteratorErrorException& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	virtual void print()
	{
		cout << "IteratorErrorException: " << str << "; " << '\n';
	}
};


template <class T>
class Element
{
private:
	Element* next;
	Element* prev;
	T field;
public:
	Element(T value = 0, Element<T>* next_ptr = NULL, Element<T>* prev_ptr = NULL)
	{
		field = value;
		next = next_ptr;
		prev = prev_ptr;
	}

	bool operator> (Element<T>& el)
	{
		if (this->getValue() > el.getValue())
			return false;
		return true;
	}

	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }
	
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }
	
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T> friend ostream& operator<< (ostream& ustream, Element<T>& obj);
};

template<class T>
ostream& operator<< (ostream& ustream, Element<T>& obj)
{
	ustream << obj.field;
	return ustream;
}


template <class T>
class LinkedListParent
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int num;
public:
	LinkedListParent()
	{
		head = NULL;
		tail = NULL;
		num = 0;
	}

	LinkedListParent(const LinkedListParent<T>& obj)
	{
		head = NULL;
		tail = NULL;
		num = 0;

		Element<T>* cur = new Element<T>(*(obj.head));
		for (int i = 0; i < obj.num; i++)
		{
			Element<T>* newElem = new Element<T>((obj[i])->getValue());

			if (i == 0)
				head = tail = newElem;
			else
			{
				tail->setNext(newElem);
				newElem->setPrevious(tail);
				tail = newElem;
			}
			num++;
		}
	}
	
	virtual ~LinkedListParent()
	{
		if (head == tail)
		{
			delete head;
		}
		else if (head != NULL)
		{
			Element<T>* current = head;
			Element<T>* temp = head->getNext();
			for (; current->getNext() != tail; current = temp, temp = temp->getNext())
				delete current;
		}
		head = NULL;
		tail = NULL;
		num = 0;
	}

	virtual int Number() { return num; }

	virtual Element<T>* getBegin() { return head; }
	virtual Element<T>* getEnd() { return tail; }
	
	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(T value) = 0;

	virtual Element<T>* operator[](int i) const
	{
		if (i < 0 || i > num) 
			return NULL;
		int k = 0;
		Element<T>* cur = head;
		for (k = 0; k < i; k++)
			cur = cur->getNext();
		return cur;
	}

	template<class T> friend ostream& operator<< (ostream& ustream, LinkedListParent<T>& obj);
	template<class T> friend istream& operator>> (istream& ustream, LinkedListParent<T>& obj);
};

template<class T>
ostream& operator<< (ostream& ustream, LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << current->getValue() << " ";
		return ustream;
	}
	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << current->getValue() << "\n";
	return ustream;
}

template<class T>
istream& operator>> (istream& ustream, LinkedListParent<T>& obj)
{
	int len;
	double v = 0;
	ustream >> len;
	for (int i = 0; i < len; i++)
	{
		ustream >> v;
		obj.push(v);
	}
	return ustream;
}


template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
private:
	Element<ValueType>* ptr;
public:
	ListIterator() { ptr = NULL; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	bool operator!=(ListIterator const& other) const 
	{
		return ptr != other.ptr;
	}
	bool operator==(ListIterator const& other) const 
	{
		return ptr == other.ptr;
	}

	Element<ValueType>& operator*()
	{
		if (ptr == NULL)
			throw IteratorErrorException("The iterator cannot be dereferenced.");
		return *ptr;
	}

	ListIterator& operator++() 
	{
		if (ptr == NULL)
			throw IteratorErrorException("The iterator is empty.");
		ptr = ptr->getNext();
		return *this;
	}
	ListIterator& operator++(int v) 
	{
		if (ptr == NULL)
			throw IteratorErrorException("The iterator is empty.");
		ptr = ptr->getNext();
		return *this;
	}

	ListIterator& operator--() 
	{
		if (ptr == NULL)
			throw IteratorErrorException("The iterator is empty.");
		ptr = ptr->getPrevious();
		return *this;
	}
	ListIterator& operator--(int v) 
	{
		if (ptr == NULL)
			throw IteratorErrorException("The iterator is empty.");
		ptr = ptr->getPrevious();
		return *this;
	}

	ListIterator& operator=(const ListIterator& it) 
	{
		ptr = it.ptr; 
		return *this;
	}
	ListIterator& operator=(Element<ValueType>* p) 
	{
		ptr = p;
		return *this;
	}

	void setNext(Element<ValueType>* elem)
	{
		if (ptr->getNext() != NULL)
			ptr->setNext(elem);
	}
	void setPrev(Element<ValueType>* elem)
	{
		if (ptr->getPrevious() != NULL)
			ptr->setPrevious(elem);
	}
	void setNext(ListIterator& it)
	{
		if (it != NULL)
		{
			Element<ValueType>* elem = new Element<ValueType>(*it);
			ptr->setNext(elem);
		}
	}
	void setPrev(ListIterator& it)
	{
		if (it != NULL)
		{
			Element<ValueType>* elem = new Element<ValueType>(*it);
			ptr->setPrevious(elem);
		}
	}

	template<typename ValueType1>
	friend Element< ValueType1>* getPtr(ListIterator<ValueType1> value);
};

template<typename ValueType1>
Element<ValueType1>* getPtr(ListIterator<ValueType1> value)
{
	return value.ptr;
}


template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { }
	virtual ~IteratedLinkedList() { }

	ListIterator<T> begin()
	{
		ListIterator<T> it = LinkedListParent<T>::head;
		if (it == NULL)
			throw IteratorErrorException("Error: Iterator is empty!");
		return it;
	}
	ListIterator<T> end()
	{
		ListIterator<T> it = LinkedListParent<T>::tail;
		if (it == NULL)
			throw IteratorErrorException("Error: Iterator is empty!");
		return it;
	}

	virtual Element<T>* push(T value)
	{
		LinkedListParent<T>::num++; 
		Element<T>* newElem = new Element<T>(value); 
		if (LinkedListParent<T>::head == NULL) 
			LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
		else 
		{
			LinkedListParent<T>::tail->setNext(newElem); 
			newElem->setPrevious(LinkedListParent<T>::tail); 
			LinkedListParent<T>::tail = newElem; 
		}
		return LinkedListParent<T>::tail; 
	}
	virtual Element<T>* pop() 
	{
		LinkedListParent<T>::num--; 
		Element<T>* res = LinkedListParent<T>::head; 
		if (LinkedListParent<T>::head == NULL) 
			return NULL;
		if (LinkedListParent<T>::head == LinkedListParent<T>::tail) 
		{
			LinkedListParent<T>::head = NULL;
			LinkedListParent<T>::tail = NULL;
			return res; 
		}
		LinkedListParent<T>::head = LinkedListParent<T>::head->getNext(); 
		res->setNext(NULL); 
		LinkedListParent<T>::head->setPrevious(NULL);
		return res;
	}

	virtual void print()
	{
		if (LinkedListParent<T>::head == NULL)
		{
			cout << "Error: List is empty!" << endl;
			return;
		}
		ListIterator<T> iterator = begin();
		while (iterator != end())
		{
			cout << *iterator << ", ";
			iterator++;
		}
		cout << *iterator << endl;
	}
};

template <class T>
class Stack : public IteratedLinkedList<T>
{
public:
	Stack() : IteratedLinkedList<T>() { }
	virtual ~Stack() { }
};

template <class T>
class orderStack : public Stack<T>
{
public:
	orderStack() : Stack<T>() { }
	virtual ~orderStack() { }

	virtual Element<T>* push(T value)
	{
		LinkedListParent<T>::num++;
		Element<T>* newElem = new Element<T>(value);
		if (LinkedListParent<T>::head == NULL)
			LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
		else if (*(LinkedListParent<T>::head) > *newElem)
		{
			LinkedListParent<T>::head->setPrevious(newElem);
			newElem->setNext(LinkedListParent<T>::head);
			newElem->setPrevious(NULL);
			LinkedListParent<T>::head = newElem;
		}
		else {
			ListIterator<T> it = IteratedLinkedList<T>::begin();
			while (it != IteratedLinkedList<T>::end())
			{
				if (*it > *newElem)
					break;
				it++;
			}
			if (it == LinkedListParent<T>::tail)
			{
				LinkedListParent<T>::tail->setNext(newElem);
				newElem->setPrevious(LinkedListParent<T>::tail);
				LinkedListParent<T>::tail = LinkedListParent<T>::tail->getNext();
			}
			else
			{
				Element<T>* temp = getPtr(it)->getPrevious();
				Element<T>* newElem = new Element<T>(value);

				temp->setNext(newElem);
				getPtr(it)->setPrevious(newElem);

				newElem->setPrevious(temp);
				newElem->setNext(getPtr(it));
			}
		}
		return newElem;
	}

};


class Auto
{
protected:
	string name, colour, mark;
	int mk, doors, year, price;
public:
	Auto()
	{
		name = "Granta";
		colour = "black";
		mark = "Lada";
		mk = 111111;
		doors = 5;
		year = 2010;
		price = 100000;
	}

	Auto(string n, string c, string m, int k, int d, int y, int p)
	{
		name = n;
		colour = c;
		mark = m;
		mk = k;
		doors = d;
		year = y;
		price = p;
	}

	~Auto() {}

	bool operator>(Auto A)
	{
		if (price < A.price)
			return true;
		else if (price == A.price)
		{
			if (year < A.year)
				return true;
			else if (year == A.year)
			{
				if (mark < A.mark)
					return true;
				else if (mark == A.mark)
				{
					if (mk < A.mk)
						return true;
				}
			}
		}
		return false;
	}

	friend ostream& operator<<(ostream& s, Auto& A);
};

ostream& operator<<(ostream& s, Auto& A)
{
	s << "\nName: " << A.name << "; Colour: " << A.colour << "; Mark: " << A.mark << "; Serial Number: " << A.mk << "; Number of doors: " << A.doors << "; Year: " << A.year << "; Price: " << A.price;
	return s;
}


bool simple(int value)
{
	int count = 0;
	for (int i = 1; i <= value; i++)
	{
		if (value % i == 0)
			count++;
	}
	return count == 2;
}

template<typename T>
Stack<T> filter(Stack<T>& list, bool (*cmd)(T))
{
	Stack<T> res;
	auto i = list.begin();
	while (i != NULL)
	{
		if (cmd((*i).getValue()))
		{
			res.push((*i).getValue());
		}
		i++;
	}
	return res;
}

template<typename T, typename T1, typename T2>
T Ufilter(T1& list, bool (*cmd)(T2))
{
	T1 res;
	auto i = list.begin();
	while (i != NULL)
	{
		if (cmd((*i).getValue()))
			res.push((*i).getValue());
		i++;
	}
	return res;
}



int main()
{
	Stack<int> st;
	for (int i = 1; i < 6; i++)
		st.push(i);

	cout << "Printed basic stack (non ordered with using iterators):\n";
	auto it = st.begin();
	while (it != st.end())
	{
		cout << (*it) << ", ";
		it++;
	}
	cout << (*it);

	cout << "\nPrinted basic stack (non ordered without using iterators):\n";
	st.print();

	cout << "Stack after push(0) (non ordered):\n";
	st.push(0);
	st.print();

	cout << "Stack after pop (non ordered):\n";
	st.pop();
	st.print();

	cout << "Stack after filter (non ordered):\n";
	bool (*cmd)(int);
	cmd = simple;
	Stack<int> res = filter(st, cmd);
	res.print();

	cout << "IteratedLinkedList after ufilter:\n";
	IteratedLinkedList<int> resU = Ufilter<IteratedLinkedList<int>, Stack<int>, int>(st, cmd);
	resU.print();

	cout << endl;
	try
	{
		Stack<int> err;
		err.push(2);
		auto it = err.end();
		it++; 
		cout << *it;
	}
	catch (IteratorErrorException e)
	{
		e.print();
	}

	cout << "\nAuto stack (ordered):";
	orderStack<Auto> stA;
	Auto A1("X5", "green", "Audi", 123456, 5, 2017, 200000);
	Auto A2("Creta", "black", "Hyundai", 654321, 5, 2011, 200000);
	Auto A3("Creta", "grey", "Hyundai", 654321, 5, 2013, 200000);
	Auto A4("Granta", "red", "Lada", 897623, 6, 2010, 300000);
	Auto A5("CR", "white", "Honda", 673549, 5, 2013, 200000);
	stA.push(A1);
	stA.push(A2);
	stA.push(A3);
	stA.push(A4);
	stA.print();

	cout << "\nAuto stack after push(CR) (ordered):";
	stA.push(A5);
	stA.print();

	cout << "\nAuto stack after pop (ordered):";
	stA.pop();
	stA.print();
}