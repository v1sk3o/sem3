#include <iostream>
#include <list>

using namespace std;

class Fraction
{
protected:
	int num, den;
public:
	Fraction()
	{
		num = 0;
		den = 1;
	}
	Fraction(int numf)
	{
		num = numf;
		den = 1;
	}
	Fraction(int numf, int denf)
	{
		num = numf;
		den = denf;
	}

	~Fraction() {}

	int getNum()
	{
		return num;
	}

	bool operator> (const Fraction& f)
	{
		return ((double)num / (double)den) > ((double)f.num / (double)f.den);
	}

	friend ostream& operator<< (ostream& os, const Fraction& f);
};

ostream& operator<< (ostream& os, const Fraction& f)
{
	return os << f.num << '/' << f.den;
}

bool simple(Fraction f)
{
	int count = 0;
	for (int i = 1; i <= f.getNum(); i++)
	{
		if (f.getNum() % i == 0)
			count++;
	}
	return count == 2;
}

template<typename T>
list<T> filter(list<T> source, bool (*ptr_f)(T))
{
	list<T> res;
	auto it = source.begin();
	while (it != source.end())
	{
		if (ptr_f(*it))
			res.push_back(*it);
		it++;
	}
	return res;
}

template<typename T>
void push(list<T>& source, T f)
{
	auto it = source.begin();
	while (it != source.end())
	{
		if (*it > f)
			break;
		it++;
	}
	source.insert(it, f);
}

template<typename T>
T pop(list<T>& source)
{
	auto it = source.begin();
	T res = *it;
	source.erase(it);
	return res;
}

template<typename T>
void print(list<T>& source)
{
	auto it = source.begin();
	while (it != source.end())
	{
		cout << *it++ << ' ';
	}
}



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


int main()
{
	list<Fraction> f_list;
	for (int i = 1; i <= 10; i++)
	{
		Fraction A(i, i + 1);
		push(f_list, A);
	}
	cout << "Fraction list: " << '\n';
	print(f_list);

	Fraction B(4, 7);
	Fraction C(7, 4);
	Fraction D(4, 9);
	push(f_list, B);
	push(f_list, C);
	push(f_list, D);
	cout << "\n\nList after pushing (4/7, 7/4, 4/9): " << '\n';
	print(f_list);

	Fraction del = pop(f_list);
	cout << "\n\nList after popping:" << '\n';
	print(f_list);
	cout << "\n\nPopped element: " << del;

	list<Fraction> filtered;
	bool (*pred)(Fraction);
	pred = simple;
	cout << "\n\nFiltered list:" << '\n';
	filtered = filter(f_list, pred);
	print(filtered);

	Auto A1("X5", "green", "Audi", 123456, 5, 2017, 200000);
	Auto A2("Creta", "black", "Hyundai", 654321, 5, 2011, 200000);
	Auto A3("Creta", "grey", "Hyundai", 654321, 5, 2013, 200000);
	Auto A4("Granta", "red", "Lada", 897623, 6, 2010, 300000);
	list<Auto> cars;
	push(cars, A1);
	push(cars, A2);
	push(cars, A3);
	push(cars, A4);
	cout << "\n\nCar list:";
	print(cars);

	Auto A5("CR", "white", "Honda", 673549, 5, 2013, 200000);
	push(cars, A5);
	cout << "\n\nList after pushing (CR Honda): ";
	print(cars);

	Auto AP = pop(cars);
	cout << "\n\nList after popping:";
	print(cars);
	cout << "\n\nPopped element: " << AP << endl;
}
