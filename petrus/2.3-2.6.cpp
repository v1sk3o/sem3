#include <iostream>
#include <string>
#include <queue>

using namespace std;

class IteratorException : public exception
{
private:
	string str;
public:
	IteratorException(string mes) : str(mes) {}

	virtual const char* what() const
	{
		return str.c_str();
	}
};



template<typename T>
void print_queue(T& q)
{
	while (!q.empty()) 
	{
		cout << q.top() << " ";
		q.pop();
	}
	cout << endl;
}


template <class T>
class Node
{
private:
	T value;
public:
	T getValue() { return value; }
	
	void setValue(T v) { value = v; }
	
	int operator<(Node N)
	{
		return (value < N.getValue());
	}
	
	int operator>(Node N)
	{
		return (value > N.getValue());
	}
	
	void print()
	{
		cout << value;
	}
};

template <class T>
void print(Node<T>* N)
{
	cout << N->getValue();
}


template <class T>
class Heap
{
private:
	Node<T>* arr;
	int len;
	int size;

public:
	int getCapacity() { return size; }
	
	int getCount() { return len; }
	
	Node<T>& operator[](int index)
	{
		if (index < 0 || index >= len)
			throw IteratorException("Index is out of range!");
		return arr[index];
	}
	
	Heap<T>(int MemorySize = 100)
	{
		arr = new Node<T>[MemorySize];
		len = 0;
		size = MemorySize;
	}
	
	void Swap(int index1, int index2)
	{
		if (index1 < 0 || index2 < 0 || index1 >= len || index2 >= len)
			throw IteratorException("Index is out of range!");
		Node<T> temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}
	
	void Copy(Node<T>* dest, Node<T>* source)
	{
		dest->setValue(source->getValue());
	}
	
	Node<T>* GetLeftChild(int index)
	{
		if (index < 0 || index * 2 >= len)
			throw IteratorException("Index is out of range!");
		return &arr[index * 2 + 1];
	}
	
	Node<T>* GetRightChild(int index)
	{
		if (index < 0 || index * 2 >= len)
			throw IteratorException("Index is out of range!");
		return &arr[index * 2 + 2];
	}
	
	Node<T>* GetParent(int index)
	{
		if (index <= 0 || index >= len)
			throw IteratorException("Index is out of range!");
		if (index % 2 == 0)
			return &arr[index / 2 - 1];
		return &arr[index / 2];
	}
	
	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			throw IteratorException("Index is out of range!");
		return index * 2 + 1;
	}
	
	int GetRightChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
			throw IteratorException("Index is out of range!");
		return index * 2 + 2;
	}
	
	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
			throw IteratorException("Index is out of range!");
		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}
	
	void SiftUp(int index = -1)
	{
		if (index == -1) 
			index = len - 1;

		int parent_index = 0; 
		if (index % 2 == 0)
			parent_index = index / 2 - 1;
		else
			parent_index = index / 2;
		
		if (parent_index < 0) 
			return;

		if (arr[index] > arr[parent_index])
		{
			Swap(index, parent_index);
			SiftUp(parent_index);
		}
	}
	
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}
	
	void push(Node<T>* N)
	{
		if (len < size)
		{
			Copy(&arr[len], N);
			len++;
			SiftUp();
		}
	}
	
	void Heapify(int index = 0)
	{
		if (index < 0 || index >= len) 
			return;

		int left_index = index * 2 + 1;
		if (left_index >= len) 
			return;

		int right_index = index * 2 + 2;
		int max_index = -1;
		if (right_index >= len)
			max_index = left_index;
		else if (arr[left_index] > arr[right_index])
			max_index = left_index;
		else
			max_index = right_index;

		if (arr[max_index] > arr[index])
		{
			Swap(max_index, index);
			Heapify(max_index);
		}
	}
	
	void Straight(void(*f)(Node<T>*))
	{
		for (int i = 0; i < len; i++)
			f(&arr[i]);
	}
	
	void InOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}
	
	Node<T>* ExtractMax()
	{
		if (len == 0) 
			return NULL;

		Node<T>* res = new Node<T>;
		Copy(res, &arr[0]);
		Swap(0, len - 1);
		len--;
		Heapify();
		return res;
	}
	
	Node<T>* Remove(int index = 0)
	{
		if (len == 0 || index >= len || index < 0) 
			return NULL;

		if (index == 0) 
			ExtractMax();
		else
		{
			Node<T>* res = new Node<T>;
			Copy(res, &arr[index]);
			Swap(index, len - 1);
			len--;

			if ((&arr[index])->getValue() > (GetParent(index))->getValue()) 
				SiftUp(index);
			else if ((&arr[index])->getValue() < (GetParent(index))->getValue()) 
				Heapify(index);

			return res;
		}
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

	bool operator<(Auto& A)
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

	bool operator>(Auto& A)
	{
		if (price > A.price)
			return true;
		else if (price == A.price)
		{
			if (year > A.year)
				return true;
			else if (year == A.year)
			{
				if (mark > A.mark)
					return true;
				else if (mark == A.mark)
				{
					if (mk > A.mk)
						return true;
				}
			}
		}
		return false;
	}

	bool operator>(const Auto& A)
	{
		if (price > A.price)
			return true;
		else if (price == A.price)
		{
			if (year > A.year)
				return true;
			else if (year == A.year)
			{
				if (mark > A.mark)
					return true;
				else if (mark == A.mark)
				{
					if (mk > A.mk)
						return true;
				}
			}
		}
		return false;
	}

	friend bool operator<(const Auto& A, const Auto& B);

	friend ostream& operator<<(ostream& s, const Auto& A);
};

bool operator<(const Auto& A, const Auto& B)
{
	if (A.price < B.price)
		return true;
	else if (A.price == B.price)
	{
		if (A.year < B.year)
			return true;
		else if (A.year == B.year)
		{
			if (A.mark < B.mark)
				return true;
			else if (A.mark == B.mark)
			{
				if (A.mk < B.mk)
					return true;
			}
		}
	}
	return false;
}

ostream& operator<<(ostream& s, const Auto& A)
{
	s << "\nName: " << A.name << "; Colour: " << A.colour << "; Mark: " << A.mark << "; Serial Number: " << A.mk << "; Number of doors: " << A.doors << "; Year: " << A.year << "; Price: " << A.price;
	return s;
}



int main()
{
	Auto A1("X5", "green", "Audi", 123456, 5, 2017, 260000);
	Auto A2("Creta", "black", "Hyundai", 654321, 5, 2011, 200000);
	Auto A3("Creta", "grey", "Hyundai", 654321, 5, 2013, 200000);
	Auto A4("Granta", "red", "Lada", 897623, 6, 2010, 100000);
	Auto A5("FX35", "white", "Infiniti", 762362, 5, 2012, 320000);
	Auto A6("CLS", "black", "Mercedes", 191042, 6, 2016, 490000);
	Auto A7("M5", "blue", "BMW", 921233, 6, 2015, 560000);

	priority_queue<Auto> q;
	q.push(A1);
	q.push(A2);
	q.push(A3);
	q.push(A4);
	q.push(A5);
	q.push(A6);
	q.push(A7);

	Heap<Auto> h;
	h.push(A1);
	h.push(A2);
	h.push(A3);
	h.push(A4);
	h.push(A5);
	h.push(A6);
	h.push(A7);

	cout << "Auto priority queue:";
	print_queue(q);

	cout << "\nAuto heap:";
	void(*ptr)(Node<Auto>*); 
	ptr = print;
	h.Straight(ptr);
	
	Node<Auto>* rem = h.Remove(2);
	cout << "\n\nRemoved element:";
	rem->print();

	cout << "\n\nHeap after removing element:"; 
	h.Straight(ptr);

	cout << "\n\nElements printed with ExtractMax:";
	while (h.getCount() > 0)
	{
		Node<Auto>* root = h.ExtractMax();
		root->print();
		delete root;
	}

	cout << "\n\nIterator exception:\n";
	try
	{
		Heap<Auto> ex;
		ex.GetLeftChildIndex(0);
	}
	catch (IteratorException e)
	{
		cout << e.what();
	}
}

