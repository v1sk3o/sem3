#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

class Graph
{
protected:
	set<int> nodes;
	map<int, set<int>> adjacentList;

	void bronKerboschWOP(set<int> R, set<int> P, set<int> X, vector<set<int>>& cliques)
	{
		if (P.size() == 0 && X.size() == 0)
		{
			if (R.size() > 2)
				cliques.push_back(set<int>(R));
		}

		set<int>::iterator it = P.begin();
		while (it != P.end())
		{
			int v = *it++;

			set<int> N = getNeighbours(v), R2 = set<int>(R), X2, P2;

			P.erase(v);
			R2.insert(v);
			set_intersection(P.begin(), P.end(), N.begin(), N.end(), inserter(P2, P2.begin()));
			set_intersection(X.begin(), X.end(), N.begin(), N.end(), inserter(X2, X2.begin()));
			bronKerboschWOP(R2, P2, X2, cliques);
			
			P.erase(v);
			X.insert(v);
		}
	}

	void bronKerboschWP(set<int> R, set<int> P, set<int> X, vector<set<int>>& cliques)
	{
		if (P.size() == 0 && X.size() == 0)
		{
			if(R.size() > 2)
				cliques.push_back(set<int>(R));
		}

		int counter = 0, u = 0;
		for (set<int>::iterator it = P.begin(); it != P.end(); it++)
		{
			if (counter < (getNeighbours(*it).size()))
			{
				counter = getNeighbours(*it).size();
				u = *it;
			}
		}

		set<int> Nu = getNeighbours(u), PNu;
		set_difference(P.begin(), P.end(), Nu.begin(), Nu.end(), inserter(PNu, PNu.begin()));

		set<int>::iterator it = PNu.begin();
		while (it != PNu.end())
		{
			int v = *it++;

			set<int> N = getNeighbours(v), R2 = set<int>(R), X2, P2;

			PNu.erase(v);
			R2.insert(v);
			set_intersection(P.begin(), P.end(), N.begin(), N.end(), inserter(P2, P2.begin()));
			set_intersection(X.begin(), X.end(), N.begin(), N.end(), inserter(X2, X2.begin()));
			bronKerboschWP(R2, P2, X2, cliques);
			
			P.erase(v);
			X.insert(v);
		}
	}


public:
	Graph(int size = 0)
	{
		for (int i = 0; i < size; i++)
			addNode(i);
	}

	Graph(const Graph& g)
	{
		nodes = g.nodes;
		adjacentList = g.adjacentList;
	}

	~Graph() { }

	void addNode(int index)
	{
		if (nodes.find(index) == nodes.end())
		{
			nodes.insert(index);
			adjacentList.insert(make_pair(index, set<int>()));
		}
	}

	void addEdge(int node1, int node2)
	{
		if (nodes.find(node1) != nodes.end() && nodes.find(node2) != nodes.end() && node1 != node2)
		{
			if (adjacentList.find(node1) == adjacentList.end())
				adjacentList.insert(make_pair(node1, set<int>()));
			else
				adjacentList.find(node1)->second.insert(node2);

			if (adjacentList.find(node2) == adjacentList.end())
				adjacentList.insert(make_pair(node2, set<int>()));
			else
				adjacentList.find(node2)->second.insert(node1);
		}
	}

	void delNode(int index)
	{
		if (nodes.find(index) != nodes.end())
		{
			nodes.erase(index);
			adjacentList.erase(index);
		}
	}

	void delEdge(int node1, int node2)
	{
		if (nodes.find(node1) != nodes.end() && nodes.find(node2) != nodes.end() && node1 != node2)
		{
			if (adjacentList.find(node1) != adjacentList.end())
				adjacentList[node1].erase(node2);

			if (adjacentList.find(node2) != adjacentList.end())
				adjacentList[node2].erase(node1);
		}
	}


	bool areAdjacent(int node1, int node2)
	{
		return adjacentList[node1].find(node2) != adjacentList[node1].end();
	}

	set<int> getNeighbours(int index)
	{
		return adjacentList[index];
	}

	set<int> getNodes()
	{
		return nodes;
	}

	int getNumOfNodes()
	{
		return nodes.size();
	}

	int getNumOfEdges()
	{
		int count = 0;
		for (int i = 0; i < adjacentList.size(); i++)
			count += adjacentList[i].size();
		return count / 2;
	}

	vector<set<int>> allCliquesWOP()
	{
		vector<set<int>> cliques;
		if (this->nodes.size() > 2)
		{
			set<int> R;
			set<int> P = set<int>(nodes);
			set<int> X;
			bronKerboschWOP(R, P, X, cliques);
		}
		return cliques;
	}

	vector<set<int>> allCliquesWP()
	{
		vector<set<int>> cliques;
		if (this->nodes.size() > 2)
		{
			set<int> R;
			set<int> P = set<int>(nodes);
			set<int> X;
			bronKerboschWP(R, P, X, cliques);
		}
		return cliques;
	}

	Graph operator=(const Graph g)
	{
		this->nodes = g.nodes;
		this->adjacentList = g.adjacentList;
		return *this;
	}

	friend ostream& operator<< (ostream& ustream, Graph& obj);

	friend istream& operator>> (istream& ustream, Graph& obj);
};

ostream& operator<< (ostream& ustream, Graph& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.nodes.size() << endl;
		for (set<int>::iterator it = obj.nodes.begin(); it != obj.nodes.end(); it++)
			ustream << *it << " ";
		ustream << endl;
		for (set<int>::iterator it = obj.nodes.begin(); it != obj.nodes.end(); it++)
		{
			ustream << obj.adjacentList[(*it)].size() << " ";
			for (set<int>::iterator it2 = obj.adjacentList[(*it)].begin(); it2 != obj.adjacentList[(*it)].end(); it2++)
				ustream << *it2 << " ";
			ustream << endl;
		}
		return ustream;
	}

	for (set<int>::iterator it = obj.nodes.begin(); it != obj.nodes.end(); it++)
	{
		ustream << *it << " --> ";
		if (obj.adjacentList[(*it)].size() != 0)
		{
			for (set<int>::iterator it2 = obj.adjacentList[(*it)].begin(); it2 != obj.adjacentList[(*it)].end(); it2++)
				ustream << *it2 << " ";
		}
		else
			ustream << "None ";
		ustream << endl;
	}
	return ustream;
}

istream& operator>> (istream& ustream, Graph& obj)
{
	if (typeid(ustream).name() == typeid(ifstream).name())
	{
		int nSize;
		ustream >> nSize;
		for (int i = 0; i < nSize; i++)
		{
			int n;
			ustream >> n;
			obj.addNode(n);
		}

		set<int>::iterator it1 = obj.nodes.begin();
		for (int i = 0; i < nSize && it1!=obj.nodes.end(); i++, it1++)
		{
			int num;
			ustream >> num;

			for (int j = 0; j < num; j++)
			{
				int n;
				ustream >> n;
				obj.addEdge((*it1), n);
			}
		}
		return ustream;
	}

	for (set<int>::iterator it = obj.nodes.begin(); it != obj.nodes.end(); it++)
	{
		cout << *it << ": ";
		int nSize;
		ustream >> nSize;

		if (nSize > obj.nodes.size() - 1)
			nSize = obj.nodes.size() - 1;

		for (int i = 0; i < nSize; i++)
		{
			int n;
			ustream >> n;

			obj.addEdge((*it), n);
		}
	}
}



int main()
{
	cout << "Enter number of vertexes in your graph: ";
	int size;
	cin >> size;
	while (size <= 0)
	{
		cout << "Please enter number > 0. Try again!\n";
		cin >> size;
	}

	Graph g(size);
	cout << "\nEnter number of neighbours and each neighbour for each vertex: \nExample: <vertex_number>: <number_of_its_neighbours> <nieghbour1> <neighbour2> ...\n";
	cin >> g;
	cout << "\nYour graph vertexes and their neighbours: \n";
	cout << g;
	cout << "\nWhat variant of Bron-Kerbosch algorithm would you like to choose to find all cliques from your graph?\n1 - Without pivoting / 2 - With pivoting\n";
	char c = ' ';
	vector<set<int>> v;
	while (c != '1' && c != '2')
	{
		cin >> c;
		if (c == '1')
			v = g.allCliquesWOP();
		else if (c == '2')
			v = g.allCliquesWP();
		else
			cout << "There is no such variant! Please try again.\n";
	}

	if (v.size() != 0)
	{
		cout << "\nAll cliques from your graph: ";
		for (int i = 0; i < v.size(); i++)
		{
			cout << "{ ";
			for (auto x : v[i])
				cout << x << " ";
			cout << "} ";
		}
	}
	else
		cout << "\nYour graph does not have any cliques.\n";

	cout << "\n\nEnter new file name to save your graph into: ";
	string s;
	cin >> s;
	ofstream savefile(s + ".txt");
	savefile << g;
	savefile.close();
	cout << "Graph was successfully saved to " + s + ".txt!\n";

	cout << "\nTrying to open your saved file...\n";
	ifstream openfile(s + ".txt");
	if (openfile.is_open())
	{
		Graph g2;
		openfile >> g2;
		cout << "Graph from saved file:\n" << g2;
	}
	else
		cout << "File " + s + ".txt was corrupted.";
}