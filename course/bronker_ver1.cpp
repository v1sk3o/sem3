#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;

class Graph
{
protected:
	set<int> nodes;
	map<int, set<int>> adjacentList;

	void bronKerbosch(set<int> R, set<int> P, set<int> X, vector<set<int>>& cliques)
	{
		if (P.size() == 0 && X.size() == 0)
		{
			bool occup = false;
			for (int i = 0; i < cliques.size(); ++i)
			{
				if (cliques[i].size() == R.size())
				{
					bool areSame = true;
					set<int>::iterator it1, it2;
					for (it1 = cliques[i].begin(), it2 = R.begin(); it1 != cliques[i].end() && it2 != R.end(); it1++, it2++)
					{
						if ((*it1) != (*it2))
						{
							areSame = false;
							break;
						}
					}

					if (areSame)
					{
						occup = true;
						break;
					}
				}
			}

			if (!occup)
				cliques.push_back(set<int>(R));
		}

		set<int>::iterator it = P.begin();
		while (it != P.end())
		{
			int u = *it;
			it++;

			set<int> N = getNeighbours(u);
			set<int> R2 = set<int>(R);
			set<int> X2;
			set<int> P2;

			P.erase(u);
			R2.insert(u);
			set_intersection(P.begin(), P.end(), N.begin(), N.end(), inserter(P2, P2.begin()));
			set_intersection(X.begin(), X.end(), N.begin(), N.end(), inserter(X2, X2.begin()));
			bronKerbosch(R2, P2, X2, cliques);

			set<int> P3;
			set<int> singleton = { u };
			set_difference(P2.begin(), P2.end(), singleton.begin(), singleton.end(), inserter(P3, P3.begin()));
			X.insert(u);
		}
	}

public:
	Graph() { }

	Graph(int size)
	{
		for (int i = 0; i < size; i++)
		{
			nodes.insert(i);
			adjacentList.insert(make_pair(i, set<int>()));
		}
	}

	~Graph() { }

	void addNode(int index)
	{
		if (nodes.find(index) != nodes.end())
			return;
		nodes.insert(index);
		adjacentList.insert(make_pair(index, set<int>()));
	}

	void addEdge(int node1, int node2)
	{
		if (nodes.find(node1) == nodes.end() || nodes.find(node2) == nodes.end())
			return;


		if (adjacentList.find(node1) == adjacentList.end())
			adjacentList.insert(make_pair(node1, set<int>()));
		else 
			adjacentList.find(node1)->second.insert(node2);


		if (adjacentList.find(node2) == adjacentList.end())
			adjacentList.insert(make_pair(node2, set<int>()));
		else
			adjacentList.find(node2)->second.insert(node1);
	}

	void delNode(int index)
	{
		if (nodes.find(index) == nodes.end())
			return;
		nodes.erase(index);
		adjacentList.erase(index);
	}

	void delEdge(int node1, int node2)
	{
		if (nodes.find(node1) == nodes.end() || nodes.find(node2) == nodes.end())
			return;

		if (adjacentList.find(node1) != adjacentList.end())
			adjacentList[node1].erase(node2);


		if (adjacentList.find(node2) != adjacentList.end())
			adjacentList[node2].erase(node1);
	}


	bool areAdjacent(int node1, int node2)
	{
		return  adjacentList[node1].find(node2) != adjacentList[node1].end() ? true : false;
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

	vector<set<int>> allCliques()
	{
		vector<set<int>> cliques;
		set<int> R;
		set<int> P = set<int>(nodes);
		set<int> X;
		bronKerbosch(R, P, X, cliques);
		return cliques;
	}

	void print()
	{
		for (set<int>::iterator it = nodes.begin(); it != nodes.end(); it++)
		{
			cout << *it << " --> ";
			for (set<int>::iterator it2 = adjacentList[(*it)].begin(); it2 != adjacentList[(*it)].end(); it2++)
				cout << *it2 << " ";
			cout << endl;
		}
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
		for (set<int>::iterator it2 = obj.adjacentList[(*it)].begin(); it2 != obj.adjacentList[(*it)].end(); it2++)
			ustream << *it2 << " ";
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
		for (int i = 0; i < nSize, it1!=obj.nodes.end(); i++, it1++)
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
}


int main()
{
	Graph g;
	g.addNode(0);
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);
	g.addNode(4);
	g.addNode(10);


	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(2, 3);
	g.addEdge(2, 10);
	g.addEdge(3, 4);
	g.addEdge(10, 4);
	g.addEdge(3, 10);
	g.addEdge(0, 10);
	g.addEdge(0, 3);
	g.addEdge(2, 4);

	cout << "Your graph vertexes and their neighbours:\n";
	cout << g;

	cout << "\nAll cliques:\n";
	vector<set<int>> c = g.allCliques();
	for (int i = 0; i < c.size(); i++)
	{
		cout << "{ ";
		for (auto x : c[i])
			cout << x << " ";
		cout << "} ";
	}

	cout << endl << endl << "Saving into file 'test.txt' and reading from there:";
	ofstream file("test.txt");
	file << g;
	file.close();

	Graph f;
	ifstream file1("test.txt");
	file1 >> f;
	file1.close();
	cout << endl << f;
}
