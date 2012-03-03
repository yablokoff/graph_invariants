#include <iostream>
#include <cstdio>
#include <vector>
#include "Graph.h"
#define GRAPH_FILE "graph.txt"

using namespace std;

Graph compose_graph(){
	int edges_num, verts_num;
	int vert1, vert2;
	freopen(GRAPH_FILE,"r",stdin);
	cin>>edges_num>>verts_num;
	Graph g(edges_num,verts_num);
	for ( int i=0; i<edges_num; i++ ){
		cin>>vert1>>vert2;
		g.addEdge(vert1-1, vert2-1);
	}
	g.calculateOrder();
	return g;
}

int main(){
	Graph g(compose_graph());
	cout<<"========== Invariants of the graph =========="<<"\n";
	cout<<"Size: "<<g.size()<<"\n";
	cout<<"Order: "<<g.order()<<"\n";
	cout<<"Diameter: "<<g.diameter()<<"\n";
	cout<<"Girth: "<<g.girth()<<"\n";
	cout<<"Clique: "<<g.clique()<<"\n";
	cout<<"Chromatic number: "<<g.colorNumber()<<"\n";
}