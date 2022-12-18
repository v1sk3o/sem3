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

#define INFINITY 10000

void FillVectorWith(vector<int>& v, int value, int size = 0)
{
	if (size != 0)
	{
		v.resize(0);
		for (int i = 0; i < size; i++)
			v.push_back(value);
	}
	for (int i = 0; i < v.size(); i++)
		v[i] = value;
}

void FillMatrixWith(vector<vector<int>>& matrix, int value, int width = 0, int height = 0)
{
	if (width != 0 && height != 0)
	{
		matrix.resize(0);
		int i, j;
		for (i = 0; i < height; i++)
		{
			vector<int> v1;
			for (j = 0; j < width; j++)
				v1.push_back(value);
			matrix.push_back(v1);
		}
	}
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
			matrix[i][j] = value;
	}
}

int FindPath(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) 
{
	int i;
	int CurVertex;
	vector<int> Flow;
	FillVectorWith(Flow, 0, vertices);
	vector<int> Link; 
	FillVectorWith(Link, -1, vertices);
	Flow[source] = INFINITY;
	queue<int> q; 
	q.push(source);
	
	while (!q.empty())
	{
		CurVertex = q.front();
		q.pop();
		if (CurVertex == target) 
			break;
		for (int i = 0; i < vertices; i++)
		{
			if (c[CurVertex][i] - f[CurVertex][i] > 0 && Flow[i] == 0)
			{
				Link[i] = CurVertex;
				Flow[i] = (Flow[CurVertex] < c[CurVertex][i] - f[CurVertex][i]) ? Flow[CurVertex] : c[CurVertex][i] - f[CurVertex][i];
				q.push(i);
			}
		}
	}

	if (Link[target] == -1)
		return 0;
	CurVertex = target;
	while (CurVertex != source)
	{
		f[Link[CurVertex]][CurVertex] += Flow[target];
		CurVertex = Link[CurVertex];
	}
	return Flow[target];
}

int MaxFlow(vector<vector<int>>& f, vector<vector<int>>& c, int source, int target, int vertices)
{
	FillMatrixWith(f, 0);
	int MaxFlow = 0;
	int AddFlow;
	do
	{
		AddFlow = FindPath(f, c, source, target, vertices);
		MaxFlow += AddFlow;
	} while (AddFlow > 0);
	return MaxFlow;
}

bool hamilton(vector<vector<int>>& mat, vector <bool>& visited, vector <int>& path, int curr)
{
	path.push_back(curr);
	if (path.size() == mat.size())
	{
		if (mat[path[0]][path.size() - 1] > 0)
			return true;
		else
		{
			path.pop_back();
			return false;
		}
	}
	visited[curr] = true;
	for (int i = 0; i < mat.size(); i++)
		if (mat[curr][i] > 0 && !visited[i])
			if (hamilton(mat, visited, path, i))
				return true;
	visited[curr] = false;
	path.pop_back();
	return false;
}



int main()
{
	vector<vector<int>> mat1 =
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

	int source = 0;
	int target = mat1[0].size() - 1;
	int vertices = mat1[0].size();
	vector<vector<int>> f;
	FillMatrixWith(f, 0, vertices, vertices);
	cout << "Maximum flow: " << MaxFlow(f, mat1, source, target, vertices);
	
	cout << "\nHamilton path:\n";
	vector<bool> visited(mat1[0].size(), 0);
	vector<int> path;
	if (hamilton(mat1, visited, path, 0))
		for (int i = 0; i < path.size(); i++)
			cout << path[i] << " ";
	else
		cout << "There is no hamilton path.";
}
