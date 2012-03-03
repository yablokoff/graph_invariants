#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct Edge{
	
	unsigned int l,r;
	
	Edge( unsigned int l, unsigned int r){
		this->l = l;
		this->r = r;
	}
	
	bool operator==(const Edge& cmp){
		if (l==cmp.l && r==cmp.r)
			return true;
		else
			return false;
	}
};

struct VertexOrder{

	unsigned int num, order;

	VertexOrder( unsigned int num, unsigned int order){
		this->num = num;
		this->order = order;
	}
	
	inline bool operator==(const VertexOrder& cmp){
		if (num==cmp.num && order==cmp.order)
			return true;
		else
			return false;
	}

	inline bool operator<(const VertexOrder& cmp){
		return order>cmp.order;
	}
};

class Graph
{
private:
	unsigned int vertexCount;
	unsigned int edgesCount;
	bool** adjacencyMatrix;
	vector<int> vertexColors;
	vector<VertexOrder> vertexOrder;
	int _lenPathBetween(unsigned int fromNode, unsigned int toNode);
	int _lenPathBetween(unsigned int, unsigned int, vector<Edge>, int);
	bool _full_connectivity(set<unsigned int>, set<unsigned int>);
	int _extend_compsub(set<unsigned int>, set<unsigned int>, set<unsigned int>, unsigned int);
	vector<unsigned int> _adjacentFor( unsigned int node );
	bool _all_colored();
	bool _adjacent_color(unsigned int, unsigned int);
	void _color_the_same(unsigned int);
public:
	Graph(unsigned int edgesCount, unsigned int vertexCount);
	Graph(const Graph&);
	void addEdge(unsigned int i, unsigned int j);
	void removeEdge(unsigned int i, unsigned int j);
	bool isEdge(unsigned int i, unsigned int j);
	void calculateOrder();
	~Graph();
	unsigned int order();
	unsigned int size();
	unsigned int clique();
	unsigned int diameter();
	unsigned int girth();
	unsigned int colorNumber();
};