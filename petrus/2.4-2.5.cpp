#include <iostream>
#include <list>

using namespace std;

class NullIteratorException : public exception
{
private:
	string str;
public:
	NullIteratorException(string mes) : str(mes) {}

	virtual const char* what() const
	{
		return str.c_str();
	}
};



template<class KeyTy, class ValTy>
class Node
{
protected:
	KeyTy key;
	ValTy value;

	Node* left;
	Node* right;
	Node* parent;

	int height;
public:
	virtual void setValue(ValTy d) { value = d; }
	virtual ValTy getValue() { return value; }

	virtual void setKey(KeyTy d) { key = d; }
	virtual KeyTy getKey() { return key; }

	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	Node<KeyTy, ValTy>(KeyTy k, ValTy v)
	{
		value = v;
		key = k;
		left = right = parent = NULL;
		height = 1;
	}

	Node<KeyTy, ValTy>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		height = 1;
	}

	virtual void print()
	{
		cout << *this;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	template<class KeyTy, class ValTy> friend ostream& operator<< (ostream& stream, Node<KeyTy, ValTy>& N);
};

template<class KeyTy, class ValTy>
ostream& operator<< (ostream& stream, Node<KeyTy, ValTy>& N)
{
	stream << "\nKey: " << N.getKey() << "; Value: " << N.getValue() << "; Height: " << N.getHeight();
	return stream;
}

template<class KeyTy, class ValTy>
void print(Node<KeyTy, ValTy>* N) { cout << "\nKey: " << N->getKey() << "; Value: " << N->getValue() << "; Height: " << N->getHeight(); }



template<typename KeyTy, typename ValTy>
class TreeIterator : public std::iterator<std::input_iterator_tag, ValTy>
{
private:
	Node<KeyTy, ValTy>* ptr;

	Node<KeyTy, ValTy>* root;

	Node<KeyTy, ValTy>* tMax;
	
	Node<KeyTy, ValTy>* tMin;


	Node<KeyTy, ValTy>* getRoot()
	{
		Node<KeyTy, ValTy>* localPtr = ptr;
		while (localPtr->getParent() != NULL)
			localPtr = localPtr->getParent();
		return localPtr;
	}

	Node<KeyTy, ValTy>* getMax()
	{
		Node<KeyTy, ValTy>* localPtr = root;
		while (localPtr->getRight() != NULL)
			localPtr = localPtr->getRight();
		return localPtr;
	}

	Node<KeyTy, ValTy>* getMin()
	{
		Node<KeyTy, ValTy>* localPtr = root;
		while (localPtr->getLeft() != NULL)
			localPtr = localPtr->getLeft();
		return localPtr;
	}

public:
	TreeIterator() 
	{
		ptr = NULL;
	}
	
	TreeIterator(Node<KeyTy, ValTy>* p)
	{
		ptr = p;
		if (ptr)
		{
			root = getRoot();
			tMax = getMax();
			tMin = getMin();
		}

	}
	
	TreeIterator(const TreeIterator& it)
	{
		ptr = it.ptr;
		if (ptr)
		{
			root = it.root;
			tMax = it.tMax;
			tMin = it.tMin;
		}

	}

	TreeIterator& operator=(const TreeIterator& it)
	{
		ptr = it.ptr;
		root = it.root;
		tMax = it.tMax;
		tMin = it.tMin;

		return *this;
	}
	
	TreeIterator& operator=(Node<KeyTy, ValTy>* p) { ptr = p; return *this; }

	bool operator!=(TreeIterator const& other) const { return ptr != other.ptr; }
	
	bool operator==(TreeIterator const& other) const { return ptr == other.ptr; }

	Node<KeyTy, ValTy>& operator*()
	{
		if (ptr == NULL)
			throw NullIteratorException("Error: Iterator is empty");
		return *ptr;
	}

	TreeIterator& operator++()
	{
		//if (ptr == NULL)
			//throw NullIteratorException("Error: Iterator is empty");

		if (ptr == tMax)
		{
			ptr = NULL;
			return *this;
		}

		if (ptr != NULL)
		{
			if (ptr->getRight() != NULL)
			{
				Node<KeyTy, ValTy>* min = ptr->getRight();
				while (min->getLeft() != NULL)
					min = min->getLeft();
				ptr = min;
			}
			else
			{
				Node<KeyTy, ValTy>* current = ptr->getParent();
				if (current == NULL)
					return *this;
				KeyTy value = ptr->getKey();
				while (current->getKey() < value && current->getParent() != NULL)
					current = current->getParent();
				if (current->getKey() >= value)
					ptr = current;
			}
			return *this;
		}

	}

	TreeIterator& operator++(int v)
	{
		return this->operator++();
	}

	TreeIterator& operator--()
	{
		if (ptr == tMin)
		{
			ptr = NULL;
			return *this;
		}

		if (ptr != NULL)
		{
			if (ptr->getLeft() != NULL)
			{
				Node<KeyTy, ValTy>* max = ptr->getLeft();
				while (max->getRight() != NULL)
					max = max->getRight();
				ptr = max;
			}
			else
			{
				Node<KeyTy, ValTy>* current = ptr->getParent();
				if (current == NULL)
					return *this;
				KeyTy value = ptr->getKey();
				while (current->getKey() > value && current->getParent() != NULL)
					current = current->getParent();
				if (current->getKey() <= value)
					ptr = current;
			}
		}
		return *this;

	}

	TreeIterator& operator--(int v)
	{
		return this->operator--();
	}
};



template<class KeyTy, class ValTy>
class Tree
{
protected:
	Node<KeyTy, ValTy>* root;

	virtual void fixHeight(Node<KeyTy, ValTy>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	virtual Node<KeyTy, ValTy>* push_R(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		if (N == NULL) return NULL;

		if (root == NULL)
		{
			root = N;
			return N;
		}

		if (Current->getKey() > N->getKey())
		{
			if (Current->getLeft() != NULL)
				push_R(N, Current->getLeft());
			else
			{
				Current->setLeft(N);
				N->setParent(Current);
			}
		}
		if (Current->getKey() < N->getKey())
		{
			if (Current->getRight() != NULL)
				push_R(N, Current->getRight());
			else
				Current->setRight(N);
			N->setParent(Current);
		}
		if (Current->getKey() == N->getKey());

		if (Current != NULL)
			fixHeight(Current);
		return N;
	}

	virtual Node<KeyTy, ValTy>* Remove_R(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		if (N == NULL) return NULL;

		if (root == NULL)
			return NULL;
		if (Current->getKey() > N->getKey())
		{
			if (Current->getLeft() != NULL)
				Remove_R(N, Current->getLeft());
			else
				return NULL;
		}
		if (Current->getKey() < N->getKey())
		{
			if (Current->getRight() != NULL)
				Remove_R(N, Current->getRight());
			else
				return NULL;
		}
		if (Current->getKey() == N->getKey()) {
			if (Current->getLeft() == NULL && Current->getRight() == NULL)
			{
				if (Current == root)
					root = NULL;
				else
				{
					Node<KeyTy, ValTy>* parent = Current->getParent();
					if (parent->getKey() < Current->getKey())
						parent->setRight(NULL);
					if (parent->getKey() > Current->getKey())
						parent->setLeft(NULL);
					Current->setParent(NULL);
				}
			}
			if (Current->getLeft() != NULL && Current->getRight() == NULL)
			{
				if (Current == root)
				{
					Node<KeyTy, ValTy>* baby = Current->getLeft();
					root = baby;
					baby->setParent(NULL);
					Current->setParent(NULL);
					Current->setLeft(NULL);
				}
				else
				{
					Node<KeyTy, ValTy>* parent = Current->getParent();
					Node<KeyTy, ValTy>* baby = Current->getLeft();
					if (parent->getKey() < Current->getKey())
						parent->setRight(baby);
					if (parent->getKey() > Current->getKey())
						parent->setLeft(baby);
					baby->setParent(parent);
					Current->setParent(NULL);
					Current->setLeft(NULL);
				}
			}
			if (Current->getLeft() == NULL && Current->getRight() != NULL)
			{
				if (Current == root)
				{
					Node<KeyTy, ValTy>* baby = Current->getRight();
					root = baby;
					baby->setParent(NULL);
					Current->setParent(NULL);
					Current->setRight(NULL);
				}
				else
				{
					Node<KeyTy, ValTy>* parent = Current->getParent();
					Node<KeyTy, ValTy>* baby = Current->getRight();
					if (parent->getKey() < Current->getKey())
						parent->setRight(baby);
					if (parent->getKey() > Current->getKey())
						parent->setLeft(baby);
					baby->setParent(parent);
					Current->setParent(NULL);
					Current->setRight(NULL);
				}
			}
			if (Current->getLeft() != NULL && Current->getRight() != NULL)
			{
				if (Current == root)
				{
					Node<KeyTy, ValTy>* baby_R = Current->getRight();
					Node<KeyTy, ValTy>* baby_L = Current->getLeft();
					Node<KeyTy, ValTy>* min = Min(baby_R);
					Node<KeyTy, ValTy>* parent_min = min->getParent();
					parent_min->setLeft(NULL);
					Current->setParent(NULL);
					Current->setRight(NULL);
					Current->setLeft(NULL);
					root = min;
					min->setParent(NULL);
					if (baby_R != min)
						min->setRight(baby_R);
					min->setLeft(baby_L);
					baby_L->setParent(min);
					baby_R->setParent(min);
				}
				else
				{
					Node<KeyTy, ValTy>* parent = Current->getParent();
					Node<KeyTy, ValTy>* baby_R = Current->getRight();
					Node<KeyTy, ValTy>* baby_L = Current->getLeft();
					Node<KeyTy, ValTy>* min = Min(baby_R);
					Node<KeyTy, ValTy>* parent_min = min->getParent();
					if (parent->getKey() < Current->getKey())
						parent->setRight(min);
					else
						parent->setLeft(min);
					parent_min->setLeft(NULL);
					Current->setParent(NULL);
					Current->setRight(NULL);
					Current->setLeft(NULL);
					min->setParent(parent);
					if (baby_R != min)
						min->setRight(baby_R);
					min->setLeft(baby_L);
					baby_L->setParent(min);
					baby_R->setParent(min);
				}
			}
		}
		return N;
	}

	virtual Node<KeyTy, ValTy>* Find_RKey(KeyTy key, Node<KeyTy, ValTy>* Current)
	{
		if (Current == NULL) return NULL;

		if (Current->getKey() == key) return Current;

		if (Current->getKey() > key) return Find_RKey(key, Current->getLeft());

		if (Current->getKey() < key) return Find_RKey(key, Current->getRight());
	}

	virtual Node<KeyTy, ValTy>* Find_RValue(ValTy value, Node<KeyTy, ValTy>* Current)
	{
		if (Current == NULL) return NULL;
		TreeIterator<KeyTy, ValTy> it(Tree::Min());
		for (; it != NULL; it++)
		{
			if ((*it).getValue() == value)
				return &(*it);
		}
	}

public:
	Tree<KeyTy, ValTy>() { root = NULL; }

	virtual Node<KeyTy, ValTy>* getRoot() { return root; }

	virtual Node<KeyTy, ValTy>* push(Node<KeyTy, ValTy>* N)
	{
		return push_R(N, root);
	}

	virtual Node<KeyTy, ValTy>* push(KeyTy k, ValTy v)
	{
		Node<KeyTy, ValTy>* N = new Node<KeyTy, ValTy>;
		N->setKey(k);
		N->setValue(v);
		return push(N);
	}

	virtual Node<KeyTy, ValTy>* Remove(Node<KeyTy, ValTy>* N)
	{
		return Remove_R(N, root);
	}

	virtual Node<KeyTy, ValTy>* Remove(KeyTy k, ValTy v)
	{
		Node<KeyTy, ValTy>* N = new Node<KeyTy, ValTy>;
		N->setKey(k);
		N->setValue(v);
		return Remove(N);
	}

	virtual Node<KeyTy, ValTy>* Min(Node<KeyTy, ValTy>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();

		return Current;
	}

	virtual Node<KeyTy, ValTy>* Max(Node<KeyTy, ValTy>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getRight() != NULL)
			Current = Current->getRight();

		return Current;
	}

	virtual Node<KeyTy, ValTy>* Find_K(KeyTy key)
	{
		return Find_RKey(key, root);
	}

	virtual Node<KeyTy, ValTy>* Find_V(ValTy value)
	{
		return Find_RValue(value, root);
	}

	virtual void PreOrder(Node<KeyTy, ValTy>* N, void (*f)(Node<KeyTy, ValTy>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	virtual void InOrder(Node<KeyTy, ValTy>* N, void (*f)(Node<KeyTy, ValTy>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<KeyTy, ValTy>* N, void (*f)(Node<KeyTy, ValTy>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}
};



template<class KeyTy, class ValTy>
class IteratedTree : public Tree<KeyTy, ValTy>
{
public:
	IteratedTree<KeyTy, ValTy>() : Tree<KeyTy, ValTy>() {}
	TreeIterator<KeyTy, ValTy> begin() { TreeIterator<KeyTy, ValTy> b = Tree<KeyTy, ValTy>::Min();  return b; }
	TreeIterator<KeyTy, ValTy> rbegin() { return NULL; }
	TreeIterator<KeyTy, ValTy> end() { return NULL; }
	TreeIterator<KeyTy, ValTy> rend() { TreeIterator<KeyTy, ValTy> b = Tree<KeyTy, ValTy>::Max(); return b; }
};



template<class KeyTy, class ValTy>
class AVLTree : public IteratedTree<KeyTy, ValTy>
{
protected:
	int bfactor(Node<KeyTy, ValTy>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	virtual void fixHeight(Node<KeyTy, ValTy>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	Node<KeyTy, ValTy>* RotateRight(Node<KeyTy, ValTy>* p)
	{
		Node<KeyTy, ValTy>* q = p->getLeft();
		if (p == Tree<KeyTy, ValTy>::root)
			Tree<KeyTy, ValTy>::root = q;
		else
		{
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}
		p->setLeft(q->getRight());
		q->setRight(p);
		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL)
			p->getLeft()->setParent(p);
		fixHeight(p);
		fixHeight(q);
		return q;
	}
	
	Node<KeyTy, ValTy>* RotateLeft(Node<KeyTy, ValTy>* q)
	{
		Node<KeyTy, ValTy>* p = q->getRight();
		if (q == Tree<KeyTy, ValTy>::root)
			Tree<KeyTy, ValTy>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}
		q->setRight(p->getLeft());
		p->setLeft(q);
		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL)
			q->getRight()->setParent(q);
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	Node<KeyTy, ValTy>* Balance(Node<KeyTy, ValTy>* p)
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0)
			{
				p->setRight(RotateRight(p->getRight()));
				p->getRight()->setParent(p);
			}
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0)
			{
				p->setLeft(RotateLeft(p->getLeft()));
				p->getLeft()->setParent(p);
			}
			return RotateRight(p);
		}

		return p;
	}

	virtual Node<KeyTy, ValTy>* push_R(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		Node<KeyTy, ValTy>* pushedNode = Tree<KeyTy, ValTy>::push_R(N, Current);
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}
	
	virtual Node<KeyTy, ValTy>* Remove_R(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		Node<KeyTy, ValTy>* RemoveNode = Tree<KeyTy, ValTy>::Remove_R(N, Current);
		if (Current != NULL)
			return Balance(Current);
		return RemoveNode;
	}

public:
	AVLTree<KeyTy, ValTy>() : IteratedTree<KeyTy, ValTy>() {}

	virtual Node<KeyTy, ValTy>* push(Node<KeyTy, ValTy>* N)
	{
		return push_R(N, Tree<KeyTy, ValTy>::root);
	}

	virtual Node<KeyTy, ValTy>* push(KeyTy k, ValTy v)
	{
		Node<KeyTy, ValTy>* N = new Node<KeyTy, ValTy>;
		N->setKey(k);
		N->setValue(v);
		return push(N);
	}

	virtual Node<KeyTy, ValTy>* Remove(KeyTy k, ValTy v)
	{
		Node<KeyTy, ValTy>* N = new Node<KeyTy, ValTy>;
		N->setKey(k);
		N->setValue(v);
		return Remove(N);
	}
	
	virtual Node<KeyTy, ValTy>* Remove(Node<KeyTy, ValTy>* N)
	{
		return Remove_R(N, Tree<KeyTy, ValTy>::root);
	}
};

template<class KeyTy, class ValTy>
ostream& operator<< (ostream& stream, AVLTree<KeyTy, ValTy>& N)
{
	TreeIterator<KeyTy, ValTy> it = N.begin();
	while (it != N.end())
	{
		stream << *it << " ";
		it++;
	}
	return stream;
}



template<class KeyTy, class ValTy>
class AVLTreeMulti : public AVLTree<KeyTy, ValTy>
{
public:
	AVLTreeMulti<KeyTy, ValTy>() : AVLTree<KeyTy, ValTy>() {}

	list<ValTy> operator[](KeyTy k)
	{
		list<ValTy> res;
		for (auto it = this->begin(); it != this->end(); it++)
		{
			if ((*it).getKey() == k)
				res.push_back((*it).getValue());
		}
		return res;
	}

protected:
	virtual Node<KeyTy, ValTy>* push_R(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		Node<KeyTy, ValTy>* pushedNode = push_MultiR(N, Current);
		if (Current != NULL)
			return this->Balance(Current);
		return pushedNode;
	}

	virtual Node<KeyTy, ValTy>* push_MultiR(Node<KeyTy, ValTy>* N, Node<KeyTy, ValTy>* Current)
	{
		if (N == NULL)
			return NULL;

		if (this->root == NULL)
		{
			this->root = N;
			return N;
		}

		if (Current->getKey() > N->getKey())
		{
			if (Current->getLeft() != NULL)
				push_R(N, Current->getLeft());
			else
			{
				Current->setLeft(N);
				N->setParent(Current);
			}
		}
		if (Current->getKey() < N->getKey())
		{
			if (Current->getRight() != NULL)
				push_R(N, Current->getRight());
			else
			{
				Current->setRight(N);
				N->setParent(Current);
			}
		}
		if (Current->getKey() == N->getKey())
		{
			if (Current->getRight() != NULL)
				push_R(N, Current->getRight());
			else
			{
				Current->setRight(N);
				N->setParent(Current);
			}
		}
		if (Current != NULL)
			this->fixHeight(Current);
		return N;
	}
};



class AutoOwner
{
protected:
	string name, surname, regnum, date, technum;
public:
	AutoOwner()
	{
		name = "Ivan";
		surname = "Ivanov";
		regnum = "A789BC77";
		date = "31.12.1989";
		technum = "87UN879999";
	}

	AutoOwner(string n, string s, string r, string d, string t)
	{
		name = n;
		surname = s;
		regnum = r;
		date = d;
		technum = t;
	}

	~AutoOwner() {}

	bool operator==(AutoOwner A)
	{
		return (this->name == A.name && this->surname == A.surname && this->regnum == A.regnum && this->date == A.date && this->technum == A.technum);
	}

	friend ostream& operator<<(const AutoOwner& A, ostream& s);
	friend ostream& operator<<(ostream& s, const AutoOwner& A);
};

ostream& operator<<(const AutoOwner& A, ostream& s)
{
	s << "{ Name: " << A.name << "; Surname: " << A.surname << "; Register Number: " << A.regnum << "; Birthday: " << A.date << "; Tech Pass Number: " << A.technum << " }";
	return s;
}

ostream& operator<<(ostream& s, const AutoOwner& A)
{
	s << "{ Name: " << A.name << "; Surname: " << A.surname << "; Register Number: " << A.regnum << "; Birthday: " << A.date << "; Tech Pass Number: " << A.technum << " }";
	return s;
}



int main()
{
	AVLTree<string, AutoOwner> A1;
	A1.push("A765HG89", { "Ekaterina", "Stepanova", "A765HG89", "13.08.1970", "78UN566347" });
	A1.push("V049UI44", { "Dmitriy", "Litnikov", "V049UI44", "09.04.1975", "99UN365397" });
	A1.push("R129PO09", { "Yan", "Lubek", "R129PO09", "24.11.1989", "65UN886142" });

	cout << "AutoOwners AVLTree:";
	TreeIterator<string, AutoOwner> it1 = A1.begin();
	while (it1 != A1.end())
	{
		cout << *it1 << " ";
		it1++;
	}

	cout << "\n\nAutoOwners AVLTree (reversed):";
	TreeIterator<string, AutoOwner> it2 = A1.rend();
	while (it2 != A1.rbegin())
	{
		cout << *it2 << " ";
		it2--;
	}

	cout << "\n\nFind object (value = { 'Yan', 'Lubek', 'R129PO09', '24.11.1989', '65UN886142' }): ";
	A1.Find_V({ "Yan", "Lubek", "R129PO09", "24.11.1989", "65UN886142" })->print();

	cout << "\n\nFind object (key = 'A765HG89'): ";
	A1.Find_K("A765HG89")->print();

	cout << "\n\nFind minimum:";
	A1.Min()->print();

	cout << "\n\nFind maximum:";
	A1.Max()->print();

	cout << "\n\nAVLTree after pushing ({ 'Ivan', 'Ivanov', 'A789BC77', '31.12.1989', '87UN879999' }):";
	A1.push("A789BC77", { "Ivan", "Ivanov", "A789BC77", "31.12.1989", "87UN879999" });
	cout << A1;

	cout << "\n\nAVLTree after removing ({ 'Ivan', 'Ivanov', 'A789BC77', '31.12.1989', '87UN879999' }):";
	A1.Remove("A789BC77", { "Ivan", "Ivanov", "A789BC77", "31.12.1989", "87UN879999" });
	cout << A1;

	AVLTreeMulti<string, AutoOwner> A2;
	A2.push("A765HG89", { "Ekaterina", "Stepanova", "A765HG89", "13.08.1970", "78UN566347" });
	A2.push("V049UI44", { "Dmitriy", "Litnikov", "V049UI44", "09.04.1975", "99UN365397" });
	A2.push("V049UI44", { "Daniil", "Sitnikov", "V049UI44", "12.09.1986", "19UN378891" });
	A2.push("R129PO09", { "Yan", "Lubek", "R129PO09", "24.11.1989", "65UN886142" });

	cout << "\n\nAutoOwners MultiAVLTree:" << A2;

	cout << "\n\nFind values (['V049UI44']):\n";
	list<AutoOwner> keys = A2["V049UI44"];
	for (auto it = keys.begin(); it != keys.end(); it++)
		cout << *it << endl;
}
