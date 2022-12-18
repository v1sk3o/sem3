#include <iostream>
#include <iomanip>  
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

class Edge
{
public:
	int from, to, weight;
	Edge(int from = -1, int to = -1, int weight = 0) : from(from), to(to), weight(weight) {}
	Edge(const Edge& E)
	{
		from = E.from;
		to = E.to;
		weight = E.weight;
	}
	int operator<(Edge& E) 
	{
		return (weight < E.weight); 
	}
	friend ostream& operator<<(ostream& s, Edge& e);
};

ostream& operator<<(ostream& s, Edge& e)
{
	s << "From: " << e.from << ", To: " << e.to << ", Weight: " << e.weight << ";";
	return s;
}


vector<vector<int>> MinSpanTree(vector<vector<int>> mat1)
{
	vector<vector<int>> res(mat1.size()); 
	int mst_weight = 0;
	vector<int> used(mat1.size(), 0);
	vector<Edge> edges;
	vector<Edge> tree_edges;
	for (int i = 0; i < res.size(); i++)
		res[i].resize(mat1.size());

	edges.push_back(Edge(0, 0, 0));

	while (!edges.empty())
	{
		int min_ind = 0;
		int w = edges[0].weight;
		for (int i = 1; i < edges.size(); i++)
		{
			if (edges[i].weight < w)
			{
				min_ind = i;
				w = edges[i].weight;
			}
		}
		int to = edges[min_ind].to;
		int from = edges[min_ind].from;
		edges.erase(edges.begin() + min_ind);
		if (used[to]) 
			continue;
		used[to] = 1;
		mst_weight += w;
		tree_edges.push_back(Edge(from, to, w));
		for (int i = 0; i < mat1.size(); i++)
		{
			if (mat1[to][i] > 0) 
				edges.push_back(Edge(to, i, mat1[to][i]));
		}
	}

	cout << "Minimum spanning tree weight: " << mst_weight;

	for (int i = 1; i < tree_edges.size(); i++)
	{
		cout << "\nEdge: " << i << ", " << tree_edges[i];
		res[tree_edges[i].from][tree_edges[i].to] = tree_edges[i].weight;
	}
	return res;
}

vector<int> DFS(vector<vector<int>>& v, int first)
{
	stack<int> s;
	s.push(first);
	vector<bool> visited(v.size(), false);
	vector<int> res;
	int curr = first;
	bool flag = false;

	while (!s.empty())
	{
		flag = false;
		if (!visited[curr])
		{
			visited[curr] = true;
			res.push_back(curr);
		}

		for (int i = 0; i < v.size(); i++)
		{
			if (v[curr][i] != 0 && !visited[i])
			{
				s.push(curr);
				curr = i;
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			curr = s.top();
			s.pop();
		}
	}
	return res;
}

vector<int> BFS(vector<vector<int>>& edges, int first)
{
	queue<int> q;
	q.push(first);
	vector<int> dist(edges.size(), 100000);
	dist[first] = 0;
	int step = 0;

	while (!q.empty())
	{
		int ver = q.front();
		q.pop();

		for (int i = 0; i < edges.size(); i++)
		{
			if ((dist[ver] + edges[ver][i]) < dist[i] && edges[ver][i] > 0)
			{
				q.push(i);
				dist[i] = dist[ver] + edges[ver][i];
			}
		}
		step++;
	}
	return dist;
}

vector<int> VPower(vector<vector<int>>& v)
{
	queue<int> q;
	q.push(0);
	vector<int> counter(v.size(), 0);
	counter[0] = v[0][0];
	vector<bool> visited(v.size(), false);
	
	while (!q.empty())
	{
		int ver = q.front();
		q.pop();

		for (int i = 0; i < v.size(); i++)
		{
			if (v[ver][i] != 0 && !visited[ver])
			{
				q.push(i);
				counter[ver]++;
			}
		}
		visited[ver] = true;
	}
	return counter;
}

double MeanVPower(vector<vector<int>>& v)
{
	double sum = 0;
	vector<int> vers = VPower(v);
	for (int i = 0; i < vers.size(); i++)
		sum += vers[i];
	return sum / v.size();
}



int main()
{
	vector<vector<int> > mat1 =
	{ 
	{ 0, 8, 0, 1, 4, 1, 4, 9, 3, 9 },
	{ 8, 0, 5, 1, 4, 7, 9, 0, 2, 0 },
	{ 0, 5, 0, 9, 6, 0, 5, 6, 0, 6 },
	{ 1, 1, 9, 0, 0, 8, 5, 1, 1, 0 },
	{ 4, 4, 6, 0, 0, 3, 9, 0, 6, 5 },
	{ 1, 7, 0, 8, 3, 0, 1, 1, 4, 7 },
	{ 4, 9, 5, 5, 9, 1, 0, 2, 1, 8 },
	{ 9, 0, 6, 1, 0, 1, 2, 0, 7, 8 },
	{ 3, 2, 0, 1, 6, 4, 1, 7, 0, 7 },
	{ 9, 0, 6, 0, 5, 7, 8, 8, 7, 0 }
	};

	vector<vector<int>> tree = MinSpanTree(mat1);
	cout << endl;

	cout << "\nDFS:\n";
	vector<int> DFS1; 
	DFS1 = DFS(mat1, 0);
	for (int i = 0; i < DFS1.size(); i++)
		cout << DFS1[i] << " ";
	cout << endl;

	cout << "\nDFS (minimum spanning tree):\n";
	DFS1 = DFS(tree, 0);
	for (int i = 0; i < DFS1.size(); i++)
		cout << DFS1[i] << " ";
	cout << endl;

	cout << "\nMinimal paths (from 2):\n";
	vector<int> paths = BFS(mat1, 2);
	for (int i = 0; i < paths.size(); i++)
		cout << "To: " << i << ", Dist: " << paths[i] << ";\n";

	cout << "\nVertex power:\n";
	vector<int> pow = VPower(mat1);
	for (int i = 0; i < pow.size(); i++)
		cout << pow[i] << " ";

	cout << "\n\nMean vertex power: \n" << MeanVPower(mat1) << endl;
}